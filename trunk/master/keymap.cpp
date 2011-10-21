#include "app.h"

struct MAPVIRTKEYS {
	unsigned short key;
	const char *name;
};

MAPVIRTKEYS MapVirtKeys[] = {
	{ VK_LBUTTON,		 "LBUTTON"				},
	{ VK_RBUTTON,		 "RBUTTON"				},
	{ VK_CANCEL,		 "Cancel"				},
	{ VK_MBUTTON,		 "MBUTTON"				},
	{ VK_BACK,			 "Backspace"			},
	{ VK_TAB,			 "Tab"					},
	{ VK_CLEAR,			 "Clear"				},
	{ VK_RETURN,		 "Enter"				},
	{ VK_SHIFT,			 "Shift"				},
	{ VK_LSHIFT,		 "LShift"				},
	{ VK_RSHIFT,		 "RShift"				},
	{ VK_CONTROL,		 "Control"				},
	{ VK_LCONTROL,		 "LControl"				},
	{ VK_RCONTROL,		 "RControl"				},
	{ VK_MENU,			 "Alt"					},
	{ VK_LMENU,			 "LAlt"					},
	{ VK_RMENU,			 "RAlt"					},
	{ VK_PAUSE,			 "Pause"				},
	{ VK_CAPITAL,		 "Caps Lock"			},
	{ VK_ESCAPE,		 "Escape"				},
	{ VK_SPACE,			 "Space"				},
	{ VK_PRIOR,			 "Prior"				},
	{ VK_NEXT,			 "Next"					},
	{ VK_END,			 "End"					},
	{ VK_HOME,			 "Home"					},
	{ VK_LEFT,			 "Left"					},
	{ VK_UP,			 "Up"					},
	{ VK_RIGHT,			 "Right"				},
	{ VK_DOWN,			 "Down"					},
	{ VK_SELECT,		 "Select"				},
	{ VK_PRINT,			 "Print"				},
	{ VK_EXECUTE,		 "Execute"				},
	{ VK_SNAPSHOT,		 "Snapshot"				},
	{ VK_INSERT,		 "Insert"				},
	{ VK_DELETE,		 "Delete"				},
	{ VK_HELP,			 "Help"					},
	{ WORD('0'),		 "0"					},
	{ WORD('1'),		 "1"					},
	{ WORD('2'),		 "2"					},
	{ WORD('3'),		 "3"					},
	{ WORD('4'),		 "4"					},
	{ WORD('5'),		 "5"					},
	{ WORD('6'),		 "6"					},
	{ WORD('7'),		 "7"					},
	{ WORD('8'),		 "8"					},
	{ WORD('9'),		 "9"					},
	{ WORD('A'),		 "A"					},
	{ WORD('B'),		 "B"					},
	{ WORD('C'),		 "C"					},
	{ WORD('D'),		 "D"					},
	{ WORD('E'),		 "E"					},
	{ WORD('F'),		 "F"					},
	{ WORD('G'),		 "G"					},
	{ WORD('H'),		 "H"					},
	{ WORD('I'),		 "I"					},
	{ WORD('J'),		 "J"					},
	{ WORD('K'),		 "K"					},
	{ WORD('L'),		 "L"					},
	{ WORD('M'),		 "M"					},
	{ WORD('N'),		 "N"					},
	{ WORD('O'),		 "O"					},
	{ WORD('P'),		 "P"					},
	{ WORD('Q'),		 "Q"					},
	{ WORD('R'),		 "R"					},
	{ WORD('S'),		 "S"					},
	{ WORD('T'),		 "T"					},
	{ WORD('U'),		 "U"					},
	{ WORD('V'),		 "V"					},
	{ WORD('W'),		 "W"					},
	{ WORD('X'),		 "X"					},
	{ WORD('Y'),		 "Y"					},
	{ WORD('Z'),		 "Z"					},
	{ VK_LWIN,			 "LWIN"					},
	{ VK_RWIN,			 "RWIN"					},
	{ VK_APPS,			 "APPS"					},
	{ VK_NUMPAD0,		 "NumPad0"				},
	{ VK_NUMPAD1,		 "NumPad1"				},
	{ VK_NUMPAD2,		 "NumPad2"				},
	{ VK_NUMPAD3,		 "NumPad3"				},
	{ VK_NUMPAD4,		 "NumPad4"				},
	{ VK_NUMPAD5,		 "NumPad5"				},
	{ VK_NUMPAD6,		 "NumPad6"				},
	{ VK_NUMPAD7,		 "NumPad7"				},
	{ VK_NUMPAD8,		 "NumPad8"				},
	{ VK_NUMPAD9,		 "NumPad9"				},
	{ VK_MULTIPLY,		 "NumpadMultiply"		},
	{ VK_ADD,			 "NumpadAdd"			},
	{ VK_SEPARATOR,		 "Separator"			},
	{ VK_SUBTRACT,		 "NumpadSubtract"		},
	{ VK_DECIMAL,		 "NumpadDecimal"		},
	{ VK_DIVIDE,		 "NumpadDivide"			},
	{ VK_F1,			 "F1"					},
	{ VK_F2,			 "F2"					},
	{ VK_F3,			 "F3"					},
	{ VK_F4,			 "F4"					},
	{ VK_F5,			 "F5"					},
	{ VK_F6,			 "F6"					},
	{ VK_F7,			 "F7"					},
	{ VK_F8,			 "F8"					},
	{ VK_F9,			 "F9"					},
	{ VK_F10,			 "F10"					},
	{ VK_F11,			 "F11"					},
	{ VK_F12,			 "F12"					},
	{ VK_F13,			 "F13"					},
	{ VK_F14,			 "F14"					},
	{ VK_F15,			 "F15"					},
	{ VK_F16,			 "F16"					},
	{ VK_F17,			 "F17"					},
	{ VK_F18,			 "F18"					},
	{ VK_F19,			 "F19"					},
	{ VK_F20,			 "F20"					},
	{ VK_F21,			 "F21"					},
	{ VK_F22,			 "F22"					},
	{ VK_F23,			 "F23"					},
	{ VK_F24,			 "F24"					},
	{ VK_NUMLOCK,		 "Num Lock"				},
	{ VK_SCROLL,		 "Scroll Lock"			},
	{ VK_ATTN,			 "Attention"			},
	{ VK_CRSEL,			 "CRSEL"				},
	{ VK_EXSEL,			 "EXSEL"				},
	{ VK_EREOF,			 "EREOF"				},
	{ VK_PLAY,			 "Play"					},
	{ VK_ZOOM,			 "Zoom"					},
	{ VK_NONAME,		 "No Name"				},
	{ VK_PA1,			 "PA1"					},
	{ VK_OEM_CLEAR,		 "Clear2"				},
	{ VK_OEM_1,			 "Semicolon;"			},
	{ VK_OEM_2,			 "Slash/"				},
	{ VK_OEM_3,			 "Tilde~"				},
	{ VK_OEM_4,			 "LBracket["			},
	{ VK_OEM_5,			 "Backslash\\"			},
	{ VK_OEM_6,			 "RBracket]"			},
	{ VK_OEM_7,			 "Apostrophe'"			},
	{ VK_OEM_8,			 "OEM8"					},
	{ VK_OEM_PLUS,		 "Plus+"				},
	{ VK_OEM_MINUS,		 "Minus-"				},
	{ VK_OEM_COMMA,		 "Comma,"				},
	{ VK_OEM_PERIOD,	 "Period."				},
#if 0
	{ VK_OEM_AX,		 "Apostrophe`"			},
	{ VK_OEM_102,		 "<> or \\|"			},
	{ VK_ICO_HELP,		 "ICO Help"				},
	{ VK_ICO_00,		 "ICO 00"				},
	{ VK_OEM_FJ_JISHO,	 "JISHO"				},
	{ VK_OEM_FJ_MASSHOU, "MASSHOU"				},
	{ VK_OEM_FJ_TOUROKU, "TOUROKU"				},
	{ VK_OEM_FJ_LOYA,	 "LOYA"					},
	{ VK_OEM_FJ_ROYA,	 "ROYA"					},
	{ VK_OEM_NEC_EQUAL,	 "Numpad Equals"		},
#endif
	{ 0, "unknown" }
};

static char KeyScanCode[8];

unsigned short KeyMappings[KMODIFIERCOUNT][KMAPCOUNT] = {
 {
  'Z',          /* "1" */
  'X',          /* "2" */
  VK_UP,        /* Up */
  VK_DOWN,      /* Down */
  VK_LEFT,      /* Left */
  VK_RIGHT,     /* Right */
  'C',          /* START */
  'P',          /* Pause */
  'O',          /* Frame Advance */
  VK_F5,        /* Quick Load */
  VK_F6,        /* Quick Save */
  VK_OEM_PLUS,  /* Speed Up */
  VK_OEM_MINUS, /* Slow Down */
  VK_F8,        /* Fast Forward */
  '0',          /* Save Slot 0 */
  '1',          /* Save Slot 1 */
  '2',          /* Save Slot 2 */
  '3',          /* Save Slot 3 */
  '4',          /* Save Slot 4 */
  '5',          /* Save Slot 5 */
  '6',          /* Save Slot 6 */
  '7',          /* Save Slot 7 */
  '8',          /* Save Slot 8 */
  '9',          /* Save Slot 9 */
  0,            /* Auto "1" */
  0,            /* Auto "2" */
  0,            /* Auto Up */
  0,            /* Auto Down */
  0,            /* Auto Left */
  0,            /* Auto Right */
  0,            /* Auto START */
  0,            /* Normal Speed */
  0,            /* Toggle Read-only */
  0,            /* Hold "1" */
  0,            /* Hold "2" */
  0,            /* Hold Up */
  0,            /* Hold Down */
  0,            /* Hold Left */
  0,            /* Hold Right */
  0,            /* Hold START */
  0,            /* P2 "1" */
  0,            /* P2 "2" */
  0,            /* P2 Up */
  0,            /* P2 Down */
  0,            /* P2 Left */
  0,            /* P2 Right */
  0,            /* P2 Auto "1" */
  0,            /* P2 Auto "2" */
  0,            /* P2 Auto Up */
  0,            /* P2 Auto Down */
  0,            /* P2 Auto Left */
  0,            /* P2 Auto Right */
  0,            /* P2 Hold "1" */
  0,            /* P2 Hold "2" */
  0,            /* P2 Hold Up */
  0,            /* P2 Hold Down */
  0,            /* P2 Hold Left */
  0,            /* P2 Hold Right */
  0,            /* Button State Toggle */
  0,            /* Frame Counter Toggle */
 },
};

unsigned short ModifierKeys[KMODIFIERCOUNT] = {
	0,
	VK_SHIFT,
	VK_CONTROL,
	VK_MENU,
};

int HeldModifierKeys[KMODIFIERCOUNT] = {
	0,
	0,
	0,
	0,
};

void CheckModifierKeys() {
	for (int i = 1; i < KMODIFIERCOUNT; ++i) {
		if (ModifierKeys[i] != 0) {
			HeldModifierKeys[i] = (GetAsyncKeyState(ModifierKeys[i]) & 0x8000) ? 1 : 0;
		} else {
			HeldModifierKeys[i] = 0;
		}
	}
}

static unsigned short NewKeyMappings[KMODIFIERCOUNT][KMAPCOUNT];
static int receivingKmap;

static HHOOK hook;

#define KMAPPING_PAGECOUNT 4
static HWND hwndPages[KMAPPING_PAGECOUNT];
static int activePage = 0;

static void EnableAll(int enable) {
	int i;
	for (i = 0; i < KMAPCOUNT; i++) {
		EnableWindow(GetDlgItem(hwndPages[activePage], ID_KMAP(i)), enable);
	}
}

static const char *TranslateKeyName(unsigned short key) {
	for (int j = 0; MapVirtKeys[j].key != 0; ++j) {
		if (MapVirtKeys[j].key == key) {
			return MapVirtKeys[j].name;
		}
	}

	snprintf(KeyScanCode, 8, "0x%04x", key);
	return KeyScanCode;
}

static void SetKeyText(unsigned short map[KMODIFIERCOUNT][KMAPCOUNT], int mapped) {
	char keyNameBuf[48];
	int pos = 0;
	for (int i = 1; i < KMODIFIERCOUNT; ++i) {
		if (map[i][mapped] != 0) {
			pos += snprintf(keyNameBuf + pos, 48 - pos, "%s+", TranslateKeyName(ModifierKeys[i]));
		}
	}
	snprintf(keyNameBuf + pos, 48 - pos, "%s", TranslateKeyName(map[0][mapped]));
	SetDlgItemText(hwndPages[activePage], ID_LAB_KMAP(mapped), keyNameBuf);
}

static LRESULT CALLBACK KeyMappingHook(int code, WPARAM wParam, LPARAM lParam) {
	if (code < 0) {
		return CallNextHookEx(hook, code, wParam, lParam);
	}

	unsigned short key = wParam;
	bool released = ((lParam & 0x80000000) != 0);

	int i;
	for (int i = 1; i < KMODIFIERCOUNT; ++i) {
		if (key == ModifierKeys[i]) {
			HeldModifierKeys[i] = released ? 0 : 1;
			if (released) break;
			return 1;
		}
	}

	NewKeyMappings[0][receivingKmap] = key;
	for (i = 1; i < KMODIFIERCOUNT; ++i) {
		NewKeyMappings[i][receivingKmap] = HeldModifierKeys[i];
	}

	SetKeyText(NewKeyMappings, receivingKmap);

	EnableAll(TRUE);

	UnhookWindowsHookEx(hook);
	hook = 0;

	return 1;
}

static BOOL CALLBACK KeyMappingPageProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND: {
		unsigned short cmd = wParam & 0xffff;
		if (IS_ID_KMAP(cmd)) {
			receivingKmap = KMAP_ID(cmd);
			hook = SetWindowsHookEx(WH_KEYBOARD, KeyMappingHook, 0, GetCurrentThreadId());
			CheckModifierKeys();

			EnableAll(FALSE);

			break;
		} else if (IS_ID_CLEAR_KMAP(cmd)) {
			for (int i = 0; i < KMODIFIERCOUNT; ++i) {
				NewKeyMappings[i][KMAP_ID_CLEAR(cmd)] = 0;
			}
			SetDlgItemText(hwndDlg, ID_LAB_KMAP(KMAP_ID_CLEAR(cmd)), "none");
		}
	}
		break;

	default:
		return FALSE;
	}
	return TRUE;
}

static BOOL CALLBACK KeyMappingProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  unsigned short cmd;
  switch (uMsg) {
	case WM_INITDIALOG: {
		char tabTitle[32];
		TCITEM tie;
		tie.mask = TCIF_TEXT;
		int i;
		for (i = 0; i < KMAPPING_PAGECOUNT; ++i) {
			hwndPages[i] = CreateDialog(hAppInst, MAKEINTRESOURCE(IDD_HOTKEYMAPPAGE(i)), GetDlgItem(hwndDlg, IDC_HOTKEYMAP_PAGES), KeyMappingPageProc);
			LoadString(hAppInst, IDS_HKMP_TITLE(i), tabTitle, 32);
			tie.pszText = tabTitle;
			SendDlgItemMessage(hwndDlg, IDC_HOTKEYMAP_PAGES, TCM_INSERTITEM, i, (LPARAM)&tie);
		}
		SendDlgItemMessage(hwndDlg, IDC_HOTKEYMAP_PAGES, TCM_SETCURSEL, 0, 0);
		NMHDR nmhdr;  
		nmhdr.code = TCN_SELCHANGE;  
		nmhdr.hwndFrom = hwndDlg;
		nmhdr.idFrom = IDC_HOTKEYMAP_PAGES;
		SendDlgItemMessage(hwndDlg, IDC_HOTKEYMAP_PAGES, WM_NOTIFY, MAKELONG(TCN_SELCHANGE, 0), (LPARAM)(&nmhdr));

		memcpy(NewKeyMappings, KeyMappings, sizeof(KeyMappings));
			
		break;
	}

	case WM_COMMAND:
		cmd = wParam & 0xffff;
		switch (cmd) {
			case IDOK:
				memcpy(KeyMappings, NewKeyMappings, sizeof(KeyMappings));
			case IDCANCEL:
				if (hook != 0) {
					UnhookWindowsHookEx(hook);
					hook = 0;
				}

				EndDialog(hwndDlg, 0);
				break;
		}
		break;

	case WM_NOTIFY:
		if (((LPNMHDR)lParam)->code == TCN_SELCHANGE) {
			RECT rc;
			GetClientRect(GetDlgItem(hwndDlg, IDC_HOTKEYMAP_PAGES), &rc);
			rc.top += 25;
			rc.left += 4;
			rc.right -= 4;
			rc.bottom -= 4;
			activePage = SendDlgItemMessage(hwndDlg, IDC_HOTKEYMAP_PAGES, TCM_GETCURSEL, 0, 0);
			MoveWindow(hwndPages[activePage], rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);

			int i;
			for (i = 0; i < KMAPCOUNT; i++) {
				if (KeyMappings[0][i] == 0) {
					SetDlgItemText(hwndPages[activePage], ID_LAB_KMAP(i), "none");
				} else {
					SetKeyText(KeyMappings, i);
				}
			}
			for (i = 0; i < KMODIFIERCOUNT; ++i) {
				if (activePage == i) {
					ShowWindow(hwndPages[i], TRUE);
				}
				else {
					ShowWindow(hwndPages[i], FALSE);
				}
			}
		}
		else return FALSE;
		break;

	default:
		return FALSE;
  }
  return TRUE;
}

#undef KMAPPING_PAGECOUNT

void KeyMapping() {
  DialogBox(hAppInst, MAKEINTRESOURCE(IDD_HOTKEYMAP), hFrameWnd, KeyMappingProc);
}
