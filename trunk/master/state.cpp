// Emu module
#include "app.h"
#include "zlib.h"

char StateName[256]="";
static FILE *sf=NULL; // State file
static gzFile gf=NULL; // State file (compressed)
int AutoLoadSave=0;
int VideoReadOnly=0;
int SaveSlot=0;

char *MakeAutoName(int Battery, int Slot)
{
  char *Name=NULL; int Len=0,Extra=0;
  if (EmuTitle==NULL) return NULL;
  Len=strlen(EmuTitle);

  if (Battery) Extra=11; // saves,\ and .sav 0
  else         Extra=18; // saves,\ and _Auto.x.dgz 0

  Name=(char *)malloc(Len+Extra);
  if (Name==NULL) return NULL;
#if 0
  memcpy(Name,"saves\\",6);
  memcpy(Name+6,EmuTitle,Len);
  if (Battery) memcpy(Name+6+Len,".sav",5);
  else         memcpy(Name+6+Len,"_auto.dgz",10);
#else
  if (Battery) snprintf(Name, Len+Extra, "saves\\%s.sav", EmuTitle);
  else         snprintf(Name, Len+Extra, "saves\\%s_auto.%d.dgz", EmuTitle, Slot);
#endif
  return Name;
}

static int StateSeekcb(long int offset, int origin)
{
       if (gf!=NULL) return gzseek(gf,offset,origin);
  else if (sf!=NULL) return fseek(sf,offset,origin);
  return -1;
}

static long int StateTellcb()
{
       if (gf!=NULL) return gztell(gf);
  else if (sf!=NULL) return ftell(sf);
  return -1;
}

static int StateLoadAcb(struct MastArea *pma)
{
       if (gf!=NULL) return gzread(gf,pma->Data,pma->Len);
  else if (sf!=NULL) return fread(pma->Data,1,pma->Len,sf);
  return 0;
}

int StateLoad(int Meka)
{
  int Ret=0;
  if (StateName[0]==0) return 1;
  
  if (Meka==0) gf=gzopen(StateName,"rb");
  else         sf=fopen (StateName,"rb");
  if (gf==NULL && sf==NULL) return 1;

  // Scan state
  MastAcb=StateLoadAcb;
  MastSeekcb=StateSeekcb;
  MastTellcb=StateTellcb;
  if (Meka) Ret=MastAreaMeka();
  else {
    Ret=MvidPreLoadState(VideoReadOnly);
    if (Ret==0) {
      Ret=MastAreaDega();
      if (Ret==0) {
        MvidPostLoadState(VideoReadOnly);
        MdrawRefresh();
      }
    }
  }
  MastTellcb=MastTellcbNull;
  MastSeekcb=MastSeekcbNull;
  MastAcb=MastAcbNull;

  if (sf!=NULL) fclose(sf); sf=NULL;
  if (gf!=NULL) gzclose(gf); gf=NULL;

  return Ret;
}

// ------------------------------------------------------------

static int StateSaveAcb(struct MastArea *pma)
{
       if (gf!=NULL) return gzwrite(gf,pma->Data,pma->Len);
  else if (sf!=NULL) return fwrite(pma->Data,1,pma->Len,sf);
  return 0;
}

int StateSave(int Meka)
{
  int Ret=0;
  if (StateName[0]==0) return 1;

  if (Meka==0) gf=gzopen(StateName,"wb");
  else         sf=fopen (StateName,"wb");
  if (gf==NULL && sf==NULL) return 1;

  // Scan state
  MastAcb=StateSaveAcb;
  MastSeekcb=StateSeekcb;
  MastTellcb=StateTellcb;
  if (Meka) Ret=MastAreaMeka();
  else {
    Ret=MastAreaDega();
    if (Ret==0) MvidPostSaveState();
  }
  MastTellcb=MastTellcbNull;
  MastSeekcb=MastSeekcbNull;
  MastAcb=MastAcbNull;

  if (sf!=NULL) fclose(sf); sf=NULL;
  if (gf!=NULL) gzclose(gf); gf=NULL;
  return Ret;
}

// ------------------------------------------------------------
static FILE *bf=NULL; // Battery file
static int bs=0; // 0=Battery load 1=Battery save 2=Check for something
static int BattCont=0; // 1 if the battery contains something

static int BatteryAcb(struct MastArea *pma)
{
  unsigned char *pd,*pe;
  if (bs==0) { return fread (pma->Data,1,pma->Len,bf); }
  if (bs==1) { return fwrite(pma->Data,1,pma->Len,bf); }

  // Search for non-zero memory in the battery
  if (BattCont) return 0; // Already found something

  pd=(unsigned char *)pma->Data; pe=pd+pma->Len;
  if (pd>=pe) return 0;
  do { if (pd[0]) { BattCont=1; return 0; }  pd++; } while (pd<pe);
  return 0;
}

// Save battery ram
int BatterySave()
{
  char *Name=NULL;
  // Get name of save ram
  Name=MakeAutoName(1, SaveSlot); if (Name==NULL) return 1;

  bf=fopen(Name,"rb"); // See if there is anything already saved
  if (bf!=NULL) { fclose(bf); } // Yes
  else
  {
    // We haven't saved before. See if we have a blank battery
    BattCont=0;
    MastAcb=BatteryAcb; bs=2; MastAreaBattery(); bs=0; MastAcb=MastAcbNull; 
    if (BattCont==0) { free(Name); return 0; } // Blank battery: don't make a file
  }

  CreateDirectory("saves",NULL); // Make sure there is a directory
  bf=fopen(Name,"wb");
  free(Name);
  if (bf==NULL) return 1;
  // Save battery
  MastAcb=BatteryAcb; bs=1; MastAreaBattery(); bs=0; MastAcb=MastAcbNull;
  fclose(bf); bf=NULL;
  return 0;
}

// Load battery ram
int BatteryLoad()
{
  char *Name=NULL;
  // Get name of save ram
  Name=MakeAutoName(1, SaveSlot); if (Name==NULL) return 1;
  bf=fopen(Name,"rb");
  free(Name);
  if (bf==NULL) return 1;
  MastAcb=BatteryAcb; bs=0; MastAreaBattery(); MastAcb=MastAcbNull;
  fclose(bf); bf=NULL;
  return 0;
}

int StateAutoState(int Save, int Slot)
{
  char *Name=NULL;
  int Ret = 0;
  // Load/Save state

  // Make the name of the auto save file
  Name=MakeAutoName(0, Slot); if (Name==NULL) return 1;
  strncpy(StateName,Name,255);
  free(Name);

  if (Save)
  {
    CreateDirectory("saves",NULL); // Make sure there is a directory
    Ret = StateSave(0);
  }
  else
  {
    Ret = StateLoad(0);
  }

  memset(StateName,0,sizeof(StateName));
  return Ret;
}

// Load/Save battery ram and state
int StateAuto(int Save)
{
  if (AutoLoadSave) StateAutoState(Save, SaveSlot);
  if (Save) BatterySave(); else BatteryLoad();
  return 0;
}
// ------------------------------------------------------------
