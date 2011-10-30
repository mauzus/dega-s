#ifndef _LUAENGINE_H
#define _LUAENGINE_H
//#include "render.h"
#include "app.h"

enum LuaCallID
{
	LUACALL_BEFOREEMULATION,
	LUACALL_AFTEREMULATION,
	LUACALL_BEFOREEXIT,
	LUACALL_ONSTART,

	LUACALL_HOTKEY_1,
	LUACALL_HOTKEY_2,
	LUACALL_HOTKEY_3,
	LUACALL_HOTKEY_4,
	LUACALL_HOTKEY_5,

	LUACALL_COUNT
};
void CallRegisteredLuaFunctions(LuaCallID calltype);

enum LuaMemHookType
{
	LUAMEMHOOK_WRITE,
	LUAMEMHOOK_READ,
	LUAMEMHOOK_EXEC,

	LUAMEMHOOK_COUNT
};
void CallRegisteredLuaMemHook(unsigned int address, int size, unsigned int value, LuaMemHookType hookType);

//void DEGA_LuaWrite(UINT32 addr);
void DEGA_LuaFrameBoundary();
int DEGA_LoadLuaCode(const char *filename);
void DEGA_ReloadLuaCode();
void DEGA_LuaStop();
void DEGA_OpenLuaConsole();
int DEGA_LuaRunning();

int DEGA_LuaUsingJoypad(int which);
UINT32 DEGA_LuaReadJoypad(int which);
int DEGA_LuaSpeed();
//int DEGA_LuaFrameskip();
int DEGA_LuaRerecordCountSkip();

void DEGA_LuaGui(unsigned char *s, int width, int height, int bpp, int pitch);

void DEGA_LuaClearGui();
void DEGA_LuaEnableGui(u8 enabled);

char* DEGA_GetLuaScriptName();
struct lua_State* DEGA_GetLuaState();

void luasav_save(const char *filename);
void luasav_load(const char *filename);
void lua_init();

typedef unsigned char (__fastcall *LuaReadProgHandler)(unsigned int a);
typedef void (__fastcall *LuaWriteProgHandler)(unsigned int a, unsigned char v);
void LuaSetProgramReadHandler(LuaReadProgHandler handler);
void LuaSetProgramWriteHandler(LuaWriteProgHandler handler);

#endif
