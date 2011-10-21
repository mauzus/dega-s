// Config file module
#include "app.h"

static char *Config="dega-s.ini";

static char *LabelCheck(char *s,char *Label)
{
#define SKIP_WS(s) for (;;) { if (*s!=' ' && *s!='\t') break;  s++; } // skip whitespace
  int Len;
  if (s==NULL) return NULL;  if (Label==NULL) return NULL;
  Len=strlen(Label);
  SKIP_WS(s) // skip whitespace

  if (strncmp(s,Label,Len)!=0) return NULL; // doesn't match
  return s+Len;
}

// Read in the config file for the whole application
int ConfLoad()
{
  char Line[256];
  FILE *h=NULL;
  h=fopen(Config,"rt");
  if (h==NULL) return 1;

  // Go through each line of the config file
  for (;;)
  {
    int i, Len=0;
    if (fgets(Line,sizeof(Line),h)==NULL) break; // End of config file

    Len=strlen(Line);
    // Get rid of the linefeed at the end
    if (Line[Len-1]==10) { Line[Len-1]=0; Len--; }

#define VAR(x) { char *Value; Value=LabelCheck(Line,#x); \
    if (Value!=NULL) x=strtol(Value,NULL,0); }
#define STR(x) { char *Value; Value=LabelCheck(Line,#x " "); \
    if (Value!=NULL) strcpy(x,Value); }

    STR(RomFolder)

    VAR(SetupPal)
    VAR(MastEx)
    VAR(TryOverlay)
    VAR(StartInFullscreen)
    VAR(StatusMode)
    VAR(MdrawOsdOptions)
    VAR(MenuHideOnLoad)

    VAR(DSoundSamRate)
    VAR(DpsgEnhance)

    VAR(UseJoystick)

    STR(StateFolder)
    VAR(AutoLoadSave)
    VAR(VideoReadOnly)
    VAR(SizeMultiplier)

    for (i = 0; i < KMAPCOUNT; i++)
    {
      char buf[20], *value;
      snprintf(buf, sizeof(buf), "Key%dMapping", i);
      value = LabelCheck(Line, buf);
      if (value!=NULL) KeyMappings[0][i] = strtol(value,&value,0);
      if (value!=NULL) {
        int modifiers = strtol(value,NULL,0);
        for (int j = 1; j < KMODIFIERCOUNT; ++j) {
          KeyMappings[j][i] = (modifiers >> j) & 1;
        }
      }
    }

    //Ram Watch Settings
    VAR(AutoRWLoad);
    VAR(RWSaveWindowPos);
    VAR(ramw_x);
    VAR(ramw_y);

    STR(rw_recent_files[0]);
    STR(rw_recent_files[1]);
    STR(rw_recent_files[2]);
    STR(rw_recent_files[3]);
    STR(rw_recent_files[4]);

#undef STR
#undef VAR
  }

  fclose(h);
  return 0;
}

// Write out the config file for the whole application
int ConfSave()
{
  int i;
  FILE *h=NULL;
  h=fopen(Config,"wt");
  if (h==NULL) return 1;

  // Write title
  fprintf (h,"// %s Config File\n",AppName(MastVer));

#define VAR(x) fprintf (h,#x " %d\n",x);
#define STR(x) fprintf (h,#x " %s\n",x);

    fprintf (h,"\n// File\n");
    STR(RomFolder)

    fprintf (h,"\n// Setup\n");
    VAR(SetupPal)
    VAR(MastEx)
    VAR(TryOverlay)
    VAR(StartInFullscreen)
    VAR(StatusMode)
    VAR(MdrawOsdOptions)
    VAR(MenuHideOnLoad)

    fprintf (h,"\n// Sound\n");
    VAR(DSoundSamRate)
    VAR(DpsgEnhance)

    fprintf (h,"\n// Input\n");
    VAR(UseJoystick)

    fprintf (h,"\n// State\n");
    STR(StateFolder)
    VAR(AutoLoadSave)
    VAR(VideoReadOnly)
    if (SizeMultiplier==0) SizeMultiplier=1;
    VAR(SizeMultiplier)

    fprintf (h, "\n// Keys\n");
    for (i = 0; i < KMAPCOUNT; i++)
    {
      int modifiers = 0;
      for (int j = 1; j < KMODIFIERCOUNT; ++j) {
        modifiers |= (KeyMappings[j][i] << j);
      }
      fprintf( h, "Key%dMapping %d %d\n", i, KeyMappings[0][i], modifiers);
    }

    //Ram Watch Settings
    fprintf (h, "\n// RAM Watch\n");
    VAR(AutoRWLoad);
    VAR(RWSaveWindowPos);
    VAR(ramw_x);
    VAR(ramw_y);

    STR(rw_recent_files[0]);
    STR(rw_recent_files[1]);
    STR(rw_recent_files[2]);
    STR(rw_recent_files[3]);
    STR(rw_recent_files[4]);

#undef STR
#undef VAR

  fclose(h);
  return 0;
}
