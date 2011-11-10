#include "app.h"
#include <string>

static const char *ClassName="Frame";
HWND hFrameWnd=NULL; // Frame - Window handle
HMENU hFrameMenu=NULL;
HWND hFrameStatus=NULL; // Frame - status window
DWORD MoveOK=0;

UINT32 mousex,mousey;

//Recent Lua Menu ----------------------------------------
static HMENU recentluamenu;   //Recent Lua Files Menu
char *recent_lua[] = {0,0,0,0,0};
const unsigned int LUA_FIRST_RECENT_FILE = 50000;
const unsigned int MAX_NUMBER_OF_LUA_RECENT_FILES = sizeof(recent_lua)/sizeof(*recent_lua);

void UpdateLuaRMenu(HMENU menu, char **strs, unsigned int mitem, unsigned int baseid)
{
	MENUITEMINFO moo;
	int x;

	moo.cbSize = sizeof(moo);
	moo.fMask = MIIM_SUBMENU | MIIM_STATE;

	GetMenuItemInfo(hFrameMenu, mitem, FALSE, &moo);
	moo.hSubMenu = menu;
	moo.fState = strs[0] ? MFS_ENABLED : MFS_GRAYED;

	SetMenuItemInfo(hFrameMenu, mitem, FALSE, &moo);

	// Remove all recent files submenus
	for(x = 0; x < MAX_NUMBER_OF_LUA_RECENT_FILES; x++)
	{
		RemoveMenu(menu, baseid + x, MF_BYCOMMAND);
	}

	// Recreate the menus
	for(x = MAX_NUMBER_OF_LUA_RECENT_FILES - 1; x >= 0; x--)
	{
		// Skip empty strings
		if(!strs[x])
			continue;

		std::string tmp = strs[x];
		
		//clamp this string to 128 chars
		if(tmp.size()>128)
			tmp = tmp.substr(0,128);

		moo.cbSize = sizeof(moo);
		moo.fMask = MIIM_DATA | MIIM_ID | MIIM_TYPE;
		
		// Insert the menu item
		moo.cch = tmp.size();
		moo.fType = 0;
		moo.wID = baseid + x;
		moo.dwTypeData = (LPSTR)tmp.c_str();
		InsertMenuItem(menu, 0, 1, &moo);
	}

	DrawMenuBar(hFrameWnd);
}

void UpdateRecentLuaArray(const char* addString, char** bufferArray, unsigned int arrayLen, HMENU menu, unsigned int menuItem, unsigned int baseId)
{
	// Try to find out if the filename is already in the recent files list.
	for(unsigned int x = 0; x < arrayLen; x++)
	{
		if(bufferArray[x])
		{
			if(!strcmp(bufferArray[x], addString))    // Item is already in list.
			{
				// If the filename is in the file list don't add it again.
				// Move it up in the list instead.

				int y;
				char *tmp;

				// Save pointer.
				tmp = bufferArray[x];

				for(y = x; y; y--)
				{
					// Move items down.
					bufferArray[y] = bufferArray[y - 1];
				}

				// Put item on top.
				bufferArray[0] = tmp;

				// Update the recent files menu
				UpdateLuaRMenu(menu, bufferArray, menuItem, baseId);

				return;
			}
		}
	}

	// The filename wasn't found in the list. That means we need to add it.

	// If there's no space left in the recent files list, get rid of the last
	// item in the list.
	if(bufferArray[arrayLen - 1])
	{
		free(bufferArray[arrayLen - 1]);
	}

	// Move the other items down.
	for(unsigned int x = arrayLen - 1; x; x--)
	{
		bufferArray[x] = bufferArray[x - 1];
	}

	// Add the new item.
	bufferArray[0] = (char*)malloc(strlen(addString) + 1); //mbg merge 7/17/06 added cast
	strcpy(bufferArray[0], addString);

	// Update the recent files menu
	UpdateLuaRMenu(menu, bufferArray, menuItem, baseId);
}

void AddRecentLuaFile(const char *filename)
{
	UpdateRecentLuaArray(filename, recent_lua, MAX_NUMBER_OF_LUA_RECENT_FILES, recentluamenu, MENU_LUA_RECENT, LUA_FIRST_RECENT_FILE); 
}

// The window procedure
static LRESULT CALLBACK WindowProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
  if (Msg==WM_CREATE)
  {
    hFrameStatus=CreateStatusWindow(WS_CHILD,"",hWnd,0);
    if (StatusMode==STATUS_SHOW) ShowWindow(hFrameStatus,SW_NORMAL);
    return 0;
  }

  if (Msg==WM_SIZE)
  {
    RECT Rect={0,0,0,0};
    DWORD time = timeGetTime();
    if (time-MoveOK > 2000) /* This is a stupid way to handle this,
                             * but is the only way I can think of given
                             * the seemingly random number of WM_SIZE
                             * messages Windows sends us.
                             * Thanks Microsoft!
                             */
    {
      SizeMultiplier=0;
    }
    GetWindowRect(hWnd,&Rect);
    MoveWindow(hFrameStatus,Rect.left,Rect.bottom,Rect.right,Rect.bottom,1);
    PostMessage(NULL,WMU_SIZE,0,0);
    return 0;
  }

  if (Msg==WM_PAINT) { DispPaint(); }
  if (Msg==WM_MOVE) { DispPaint(); }

  if (Msg==WM_KEYDOWN && wParam==VK_ESCAPE) { PostMessage(NULL,WMU_TOGGLEMENU,0,0); return 0; }
  if (Msg==WM_RBUTTONDOWN)                  { PostMessage(NULL,WMU_TOGGLEMENU,0,0); return 0; }

  if (Msg==WM_MOUSEMOVE)
  {
    mousex=LOWORD(lParam);
    mousey=HIWORD(lParam);
    return TRUE;
  }

  if (Msg==WM_COMMAND)
  {
    int Item=0;
    Item=wParam&0xffff;
    if (Item==ID_FILE_LOADROM) { MenuLoadRom(); return 0; }
    if (Item==ID_FILE_FREEROM)
    {
      EmuRomName[0]=0;
      PostMessage(NULL,WMU_CHANGEDROM,0,0);
      return 0;
    }
    if (Item==ID_STATE_LOADSTATE) { MenuStateLoad(0); return 0; }
    if (Item==ID_STATE_SAVESTATE) { MenuStateLoad(1); return 0; }
    if (Item==ID_STATE_IMPORT) { MenuStatePort(0); return 0; }
    if (Item==ID_STATE_EXPORT) { MenuStatePort(1); return 0; }
    if (Item==ID_SOUND_VGMLOG_START) { MenuVgmStart(); return 0; }
    if (Item==ID_VIDEO_PLAYBACK || Item==ID_VIDEO_RECORD)
      { MenuVideo(Item); return 0; }
    if (Item==ID_VIDEO_PROPERTIES) { VideoProperties(); return 0; }
    if (Item==ID_INPUT_KEYMAPPING) { KeyMapping(); return 0; }

		if(wParam >= LUA_FIRST_RECENT_FILE && wParam <= LUA_FIRST_RECENT_FILE + MAX_NUMBER_OF_LUA_RECENT_FILES - 1)
		{
			char*& fname = recent_lua[wParam - LUA_FIRST_RECENT_FILE];
			if(fname)
			{
				EnterCriticalSection(&m_cs);
				DEGA_LoadLuaCode(fname);
				LeaveCriticalSection(&m_cs);
			}
		}
		if (Item==ID_LUA_OPEN) {
			if(!LuaConsoleHWnd) {
				LuaConsoleHWnd = CreateDialog(hAppInst, MAKEINTRESOURCE(IDD_LUA), NULL, (DLGPROC) DlgLuaScriptDialog);
			}
			else
				SetForegroundWindow(LuaConsoleHWnd);
		}
		if (Item==ID_LUA_CLOSE_ALL) {
			if(LuaConsoleHWnd)
				PostMessage(LuaConsoleHWnd, WM_CLOSE, 0, 0);
		}
		if (Item==ID_RAM_SEARCH) {
			if(!RamSearchHWnd) {
				reset_address_info();
				RamSearchHWnd = CreateDialog(hAppInst, MAKEINTRESOURCE(IDD_RAMSEARCH), NULL, (DLGPROC) RamSearchProc);
			}
			else
				SetForegroundWindow(RamSearchHWnd);
		}
		if (Item==ID_RAM_WATCH) {
			if(!RamWatchHWnd) {
				RamWatchHWnd = CreateDialog(hAppInst, MAKEINTRESOURCE(IDD_RAMWATCH), NULL, (DLGPROC) RamWatchProc);
			}
			else
				SetForegroundWindow(RamWatchHWnd);
		}
    PostMessage(NULL,WMU_COMMAND,Item,0);
  }

  if (Msg==WM_CLOSE) { PostQuitMessage(0); return 0; }
  if (Msg==WM_DESTROY) { hFrameStatus=NULL; hFrameWnd=NULL; return 0; }

  return DefWindowProc(hWnd,Msg,wParam,lParam);
}

int FrameInit()
{
  WNDCLASSEX wc;
  ATOM Atom=0;

  // Register the window class
  memset(&wc,0,sizeof(wc));
  wc.cbSize=sizeof(wc);
  wc.style=CS_HREDRAW|CS_VREDRAW;
  wc.lpfnWndProc=WindowProc;
  wc.hInstance=hAppInst;
  wc.hIcon=LoadIcon(hAppInst,MAKEINTRESOURCE(IDI_ICON1));
  wc.hCursor=LoadCursor(NULL,IDC_ARROW);
  wc.lpszClassName=ClassName;
  Atom=RegisterClassEx(&wc);
  if (Atom==0) return 1;

  hFrameWnd=CreateWindow(ClassName,"",WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
    100,100,0,0,NULL,NULL,hAppInst,NULL);
  if (hFrameWnd==NULL) return 1;

  // Load the menu from the resource
  hFrameMenu=LoadMenu(hAppInst,MAKEINTRESOURCE(IDR_MENU1));

  recentluamenu = CreateMenu();
  UpdateLuaRMenu(recentluamenu, recent_lua, MENU_LUA_RECENT, LUA_FIRST_RECENT_FILE);

  return 0;
}

#if 0
int FrameSize()
{
  RECT WorkArea={0,0,640,480}; // Work area on the desktop
  int x=0,y=0,w=0,h=0;
  if (hFrameWnd==NULL) return 1;

  // Get the desktop work area
  SystemParametersInfo(SPI_GETWORKAREA,0,&WorkArea,0);
  // Find midpoint
  x=WorkArea.left  +WorkArea.right; x>>=1;
  y=WorkArea.bottom+WorkArea.top;   y>>=1;

  w=WorkArea.right -WorkArea.left;
  h=WorkArea.bottom-WorkArea.top;

  w=w*3/4; h=h*3/4;

  x-=w>>1;
  y-=h>>1;

  MoveWindow(hFrameWnd,x,y,w,h,1);
  return 0;
}
#else
int SizeMultiplier = 2;

int FrameSize()
{
  RECT size, client;
  int diffx=-42, diffy=-42, newdiffx, newdiffy;
  if (SizeMultiplier == 0) return 0;

  /* The loop is necessary because the size of the non-client area
   * may change when we resize (because of the menu)
   */
  while (1)
  {
    GetWindowRect(hFrameWnd,&size);
    GetClientRect(hFrameWnd,&client);

    newdiffx = (size.right-size.left) - client.right;
    newdiffy = (size.bottom-size.top) - client.bottom;

    if (diffx==newdiffx && diffy==newdiffy) break;

    diffx = newdiffx;
    diffy = newdiffy;

    MoveOK = timeGetTime();
    MoveWindow(hFrameWnd,
        size.left,
        size.top,
        diffx + ScrnWidth*SizeMultiplier,
        diffy + ScrnHeight*SizeMultiplier+StatusHeight(),
    1);
  }
  return 0;
}
#endif

int FrameExit()
{
  SetMenu(hFrameWnd,NULL);
  if (hFrameMenu!=NULL) DestroyMenu(hFrameMenu);  hFrameMenu=NULL;

  // Destroy window if not already destroyed
  if (hFrameWnd!=NULL) DestroyWindow(hFrameWnd);  hFrameWnd=NULL;

  // Unregister the window class
  UnregisterClass(ClassName,hAppInst);
  return 0;
}
