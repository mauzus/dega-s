#ifndef _APP_H
#define _APP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define WIN32_LEAN_AND_MEAN
#define WINVER 0x500
#define _WIN32_WINDOWS 0x410
#define _WIN32_IE 0x0500
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
//#pragma warning (push)
//#pragma warning (disable:4201)
#include <mmsystem.h>
//#pragma warning (pop) 
#define DIRECTDRAW_VERSION 0x0300
#include <ddraw.h>
#define DIRECTINPUT_VERSION 0x0500
#include <dinput.h>

#include <shellapi.h>
#include <commdlg.h>
#include <assert.h>
//#include <string>

#include "../mast/mast.h"
#include "resource.h"

typedef char s8;
typedef short s16;
typedef long s32;
typedef long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

#include "luaengine.h"
#include "luaconsole.h"

// Make the INLINE macro
#undef INLINE
#define INLINE inline

#define APP_TITLE "Dega S"

#define SetWindowRect(hwnd,rect) MoveWindow(hwnd, \
  (rect)->left, \
  (rect)->top, \
  (rect)->right-(rect)->left, \
  (rect)->bottom-(rect)->top, \
  1)

// main.cpp
extern HINSTANCE hAppInst;
extern int ScrnWidth,ScrnHeight;
extern int Fullscreen;
extern HACCEL hAccel; // Key accelerators
extern int StartInFullscreen;

// loop.cpp
extern int AppShowCmd;
extern int SetupPal;
extern int LoopPause;
extern int MenuHideOnLoad;
int LoopDo();

// misc.cpp
char *UnwrapString(char *Source,int StopAtSpace);
char *GetFilename(char *Full);
int AppError(const char *Text,int bWarning);
int AppDirectory();
char *GetStubName(char *Full);
int GetClientScreenRect(HWND hWnd,RECT *pRect);
int IntegerScale(RECT *pRect,int FitWidth,int FitHeight);
char *AppName(unsigned int MastVer);
int GetSurfaceFormat(IDirectDrawSurface *pSurf,unsigned int *pFormat,int *pBpp);
int AutodetectUseSys(IDirectDraw *pDirDraw);
int SurfaceClear(IDirectDrawSurface *Surf,unsigned int Col);

// run.cpp
int RunFrame(int Draw,short *pSound);
int RunStart();
int RunStop();
int RunText(const char *Text,int Len);
#define STATUS_HIDE 0
#define STATUS_AUTO 1
#define STATUS_SHOW 2
extern int StatusMode;
void SetStatusMode(int NewMode);
int StatusHeight();

// emu.cpp
extern char EmuRomName[];
extern char *EmuTitle;
int PortMemicmp(char *s1, const char *s2, size_t len);
int EmuInit();
int EmuExit();
int EmuLoad();
int EmuFree();
int EmuFrame();

// dsound.cpp
extern int DSoundSamRate;     // sample rate
extern int DSoundSegCount;    // Segs in the pdsbLoop buffer
extern int FramesPerSecond;    // Application fps
extern int RealFramesPerSecond; // Application fps taking into account speedup/slowdown
extern int FrameMult;         // frame multiplier 
extern int DSoundSegLen;      // Seg length in samples (calculated from Rate/Fps)
extern short *DSoundNextSound; // The next sound seg we will add to the sample loop
extern unsigned char DSoundPlaying;  // True if the Loop buffer is playing
extern int (*DSoundGetNextSound) (int Draw); // Callback used to request more sound
int DSoundCheck();
int DSoundInit(HWND hWnd);
int DSoundPlay();
int DSoundStop();
int DSoundExit();

// frame.cpp
extern HWND hFrameWnd; // Frame - Window handle
extern HMENU hFrameMenu;
int FrameInit();
int FrameExit();
int FrameSize();
extern HWND hFrameStatus; // Frame - status window
extern int SizeMultiplier;
extern DWORD MoveOK;
void AddRecentLuaFile(const char *filename);
extern char *recent_lua[];

// input.cpp
extern int UseJoystick;
extern unsigned char AutoHold[2];
int InputGet();

// disp.cpp
extern unsigned char *DispMem;
extern int DispMemPitch;
extern unsigned int DispFormat; // Either a FourCC code or a color depth
extern int DispBpp; // Bytes per pixel
extern int TryOverlay;
int DispInit();
int DispExit();
int DispPaint();
int DispDraw();

// render.cpp
extern int RedBlue3D;
int RenderInit();

// dinp.cpp
int DirInputInit(HINSTANCE hInst,HWND hPassWnd);
int DirInputExit();
int DirInputStart();
int DirInputState(int Code);
int DirInputFind();

// load.cpp
extern char RomFolder[];
extern char StateFolder[256];
extern char VgmName[];
int MenuLoadRom();
int MenuStatePort(int Export);
int MenuStateLoad(int Save);
int MenuVgmStart();
#define WMU_TOGGLEMENU       (WM_APP+0)
#define WMU_CHANGEDROM       (WM_APP+1)
#define WMU_COMMAND          (WM_APP+2)
#define WMU_STATELOAD        (WM_APP+3)
#define WMU_STATESAVE        (WM_APP+4)
#define WMU_STATEIMPORT      (WM_APP+5)
#define WMU_STATEEXPORT      (WM_APP+6)
#define WMU_VGMSTART         (WM_APP+7)
#define WMU_VIDEOPLAYBACK    (WM_APP+8)
#define WMU_VIDEORECORD      (WM_APP+10)
#define WMU_SIZE             (WM_APP+12)

// conf.cpp
int ConfLoad();
int ConfSave();

// state.cpp
extern char StateName[];
extern int AutoLoadSave;
extern int VideoReadOnly;
extern int SaveSlot;
char *MakeAutoName(int Battery, int Slot);
int StateLoad(int Meka);
int StateSave(int Meka);
int StateAuto(int Save);
int StateAutoState(int Save, int Slot);

// shot.cpp
int ShotStart();
void ShotLine();
int ShotExit();
int ShotSave();

// video.cpp
extern char VideoName[256];
int MenuVideo(int Action);
void VideoRecord();
void VideoPlayback();
void VideoProperties();

// keymap.cpp
#define KMODIFIERCOUNT    4
extern unsigned short KeyMappings[KMODIFIERCOUNT][KMAPCOUNT];
extern unsigned short ModifierKeys[KMODIFIERCOUNT];
extern int HeldModifierKeys[KMODIFIERCOUNT];
void CheckModifierKeys();
void KeyMapping();

// zipfn.cpp
int ZipOpen(char *ZipName);
int ZipClose();
int ZipRead(unsigned char **pMem,int *pLen);

#include "ramsearch.h"
#include "ramwatch.h"

extern CRITICAL_SECTION m_cs;

#endif
