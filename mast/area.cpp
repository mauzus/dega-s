// Mast - state area module
#include "mastint.h"

int MastAcbNull (struct MastArea *pba) { (void)pba; return 0; }
int (*MastAcb) (struct MastArea *pma)=MastAcbNull; // Area callback

int MastSeekcbNull(long int offset, int origin) { (void)offset; (void)origin; return 0; }
int (*MastSeekcb)(long int offset, int origin)=MastSeekcbNull;

long int MastTellcbNull() { return 0; }
long int (*MastTellcb)()=MastTellcbNull;

// Scan the information stored in non-volatile memory
int MastAreaBattery()
{
  struct MastArea ma;
  memset(&ma,0,sizeof(ma));
  if (pMastb==NULL) return 1;

  ma.Data=pMastb->Sram;
  ma.Len=sizeof(pMastb->Sram);
  MastAcb(&ma);
  return 0;
}

int MastAreaSramIncluded()
{
  struct MastArea ma;
  int FileVer=MastVer;

  long int original=MastTellcb();
  MastSeekcb(0x0004, SEEK_SET);	// magic number
  memset(&ma,0,sizeof(ma));
  ma.Data=&FileVer;ma.Len=sizeof(FileVer);MastAcb(&ma); // 0x0008: Frame count
  MastSeekcb(original, SEEK_SET);
  return (FileVer&MAST_VERSION_MASK) >= 0x10 ? 1: 0;
}

int MastAreaMvidFrameCount()
{
  struct MastArea ma;
  int frameDataLen=0;

  long int original=MastTellcb();
  MastSeekcb(0x0008, SEEK_SET);	// magic number
  memset(&ma,0,sizeof(ma));
  ma.Data=&frameDataLen;ma.Len=sizeof(frameDataLen);MastAcb(&ma); // 0x0008: Frame count
  MastSeekcb(original, SEEK_SET);

  return frameDataLen;
}

int MastAreaMvidOffset()
{
  return 0x6200 + (MastAreaSramIncluded() ? 0x4000 : 0) + sizeof(MastInput);
}

// Scan the information stored in volatile memory
// Dega state style
int MastAreaDega()
{
  if (pMastb==NULL) return 1;
  struct MastArea ma;
  unsigned char Blank[sizeof(pMastb->Sram)];
  unsigned int FileVer=0;
  unsigned int MastFlags=0, FileFlags=0;

  memset(&ma,0,sizeof(ma));
  memset(&Blank,0,sizeof(Blank));

  MastSeekcb(0x0000, SEEK_SET);	// magic number

  // 0x0000 File ID
  {
    char Id[5]="Dega";
    ma.Data=Id; ma.Len=4; MastAcb(&ma);
    if (memcmp(Id,"Dega",4)!=0) return 1; // Check ID is okay
  }

  FileVer=MastVer;
  ma.Data=&FileVer; ma.Len=sizeof(FileVer); MastAcb(&ma); // 0x0004: Version number
  if ((MastVer&MAST_CORE_MASK) != (FileVer&MAST_CORE_MASK)) return 1; // verify that the same Z80 core was used

  ma.Data=&frameCount; ma.Len=sizeof(frameCount); MastAcb(&ma); // 0x0008: Frame count

  if (MastEx&MX_SRAM)
    MastFlags |= SFLAG_SRAM;
  FileFlags=MastFlags;
  ma.Data=&FileFlags; ma.Len=sizeof(FileFlags); MastAcb(&ma); // 0x000c: State flags

#ifdef EMU_DOZE
  ma.Data=&Doze;    ma.Len=sizeof(Doze);    MastAcb(&ma); // 0x0010: Z80 registers
  ma.Data=Blank;    ma.Len=0x30-0x1e;       MastAcb(&ma); // reserved
#elif defined(EMU_Z80JB)
  ma.Data=&Z80;     ma.Len=0x25;            MastAcb(&ma); // 0x0010: Z80 registers
  ma.Data=Blank;    ma.Len=0x30-0x25;       MastAcb(&ma); // reserved
#endif

  ma.Data=&Masta;   ma.Len=sizeof(Masta);   MastAcb(&ma); // 0x0040: Masta
  ma.Data=Blank;    ma.Len=0x40-0x2e;       MastAcb(&ma); // reserved

  // 0x0080: pMastb
  ma.Data=pMastb->Ram; ma.Len=sizeof(*pMastb)-0x4000; // Exclude sram
  MastAcb(&ma);

  // 0x6104
  ma.Data=Blank;    ma.Len=0x100-0x04;      MastAcb(&ma); // reserved

  // 0x6200
  if ((FileVer&MAST_VERSION_MASK) >= 0x10)
  {
    // + 0x4000
    if ((MastEx&MX_SRAM) && (FileFlags&SFLAG_SRAM)) {
      ma.Data=pMastb->Sram;
      ma.Len=sizeof(pMastb->Sram);
      MastAcb(&ma);
    }
    else {
      MastSeekcb(sizeof(pMastb->Sram), SEEK_CUR);
    }
  }

  // + 0x2
  ma.Len = sizeof(MastInput);
  ma.Data = MastInput;
  MastAcb(&ma);

  // Update banks, colors and sound
  MastMapPage0(); MastMapPage1(); MastMapPage2();
  MdrawCramChangeAll();
  MsndRefresh();
  return 0;
}

// Meka style (S00)
int MastAreaMeka()
{
  struct MastArea ma;

  if (pMastb==NULL) return 1;

  memset(&ma,0,sizeof(ma));

  // Blank unsaved parts of state
  Masta.v.Stat=0;
  Masta.v.Mode=0;
  Masta.Irq=0;
  memset(&Masta.p,0,sizeof(Masta.p)); // PSG not saved
  pMastb->Out3F=0;
  pMastb->ThreeD=0;
  pMastb->FmSel=0;
  pMastb->FmDetect=0;
  memset(&(pMastb->FmReg),0,sizeof(pMastb->FmReg));
#ifdef EMU_DOZE
  Doze.ir=0;
#endif
  // Scan state

  // 0000
  {
    char Id[5]="MEKA";
    ma.Data=Id; ma.Len=4; MastAcb(&ma);
    if (memcmp(Id,"MEKA",4)!=0) return 1; // Check ID is okay
  }

  // 0004
  {
    unsigned char Unknown[3]={0x1a,0x07,0x00};
    ma.Data=Unknown; ma.Len=sizeof(Unknown); MastAcb(&ma);
  }
  // 0007

  ma.Len=2;
#ifdef EMU_DOZE
#define SC(regname) ma.Data=&(Doze.regname); MastAcb(&ma);
#elif defined(EMU_Z80JB)
#define SC(regname) ma.Data=&(Z80.regname.w.l); MastAcb(&ma);
#endif
  SC(af)  SC(bc)  SC(de)  SC(hl)  SC(ix) SC(iy) SC(pc) SC(sp)
  SC(af2) SC(bc2) SC(de2) SC(hl2)
#undef SC

  // 001f
  {
    unsigned char Int=0;
    // compress Interrupt state into three bits
#ifdef EMU_DOZE
    if (Doze.iff) Int=1;  Int|=Doze.im<<1;
#elif defined(EMU_Z80JB)
    if (Z80.iff1) Int=1;  Int|=Z80.im<<1;
#endif
    
    Int&=7; ma.Data=&Int; ma.Len=1; MastAcb(&ma); Int&=7;

    // deompress interrupt state
#ifdef EMU_DOZE
    Doze.iff=0; if (Int&1) Doze.iff=0x0101;
    Doze.im=(unsigned char)(Int>>1);
#elif defined(EMU_Z80JB)
    Z80.iff1=Z80.iff2=0; if (Int&1) Z80.iff1=Z80.iff2=1;
    Z80.im=(unsigned char)(Int>>1);
#endif
  }

  // 0020
  {
    unsigned char Unknown[0x1b]=
    {0x00,0x00,0x00,0xe4, 0x00,0x00,0x00,0xfe, 0xff,0xff,0xff,0x00, 0x00,0x00,0x00,0x38,
     0x00,0x00,0x00,0xff, 0xff,0x00,0x00,0x00, 0x00,0x00,0x00};
    ma.Data=Unknown; ma.Len=sizeof(Unknown); MastAcb(&ma);
  }


  // 003b VDP Registers
  Masta.v.Reg[6]&=7;
  ma.Data=Masta.v.Reg; ma.Len=0x10; MastAcb(&ma);
  Masta.v.Reg[6]&=7;

  // 004b
  {
    unsigned char Unknown[2]={0x00,0xa0};
    ma.Data=Unknown; ma.Len=sizeof(Unknown); MastAcb(&ma);
  }

  // 004d Vram write address
  ma.Data=&Masta.v.Addr; ma.Len=2; MastAcb(&ma);

  // 004f Waiting for high byte
  ma.Data=&Masta.v.Wait; ma.Len=1; MastAcb(&ma);

  // 0050 Low byte written to port bf
  ma.Data=&Masta.v.Low;  ma.Len=1; MastAcb(&ma);

  // 0051 Last byte written to port be
  {
    unsigned char DontCare=0;
    ma.Data=&DontCare; ma.Len=1; MastAcb(&ma);
  }

  // 0052
  {
    unsigned char Unknown[13]=
    {0x00,0x00,0x00,0x00, 0x00,0x06,0x00,0x00, 0x00,0x01,0x00,0x01, 0x00};
    ma.Data=Unknown; ma.Len=sizeof(Unknown); MastAcb(&ma);
  }

  // 005f
  ma.Data=Masta.Bank+0; ma.Len=1; MastAcb(&ma);
  {
    unsigned char Unknown[3]={0,0,7};
    ma.Data=Unknown; ma.Len=sizeof(Unknown); MastAcb(&ma);
  }

  // 0063
  ma.Data=Masta.Bank+1; ma.Len=3; MastAcb(&ma);
  {
    unsigned char Unknown=0;
    ma.Data=&Unknown; ma.Len=1; MastAcb(&ma);
  }

  // 0067
  ma.Data=pMastb->Ram; ma.Len=0x2000; MastAcb(&ma);
  // 2067
  ma.Data=pMastb->VRam; ma.Len=0x4000; MastAcb(&ma);
  // 6067
  ma.Data=pMastb->CRam; ma.Len=0x0040; MastAcb(&ma);

  // 60a7-60cb unknown and EOF
  {
    unsigned char End[0x24];
    memset(End,0,sizeof(End));
    End[0x21]='E'; End[0x22]='O'; End[0x23]='F';
    ma.Data=End; ma.Len=sizeof(End); MastAcb(&ma);
  }

  // Update banks, colors and sound
  MastMapPage0(); MastMapPage1(); MastMapPage2();
  MdrawCramChangeAll();
  MsndRefresh();
  return 0;
}
