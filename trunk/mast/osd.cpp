#include "mastint.h"

int MdrawOsdOptions = 3;

#define CHARWIDTH 7
#define CHARHEIGHT 7

static unsigned short Palette[2] = { 0x0033, 0x08CC };

static unsigned char CharDivider[CHARHEIGHT][CHARWIDTH] = {
{ 0, 0, 0, 2, 1, 0, 0 },
{ 0, 0, 0, 2, 1, 0, 0 },
{ 0, 0, 2, 1, 0, 0, 0 },
{ 0, 0, 2, 1, 0, 0, 0 },
{ 0, 2, 1, 0, 0, 0, 0 },
{ 0, 2, 1, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0 }
};

static unsigned char CharUp[CHARHEIGHT][CHARWIDTH] = {
{ 0, 0, 0, 2, 0, 0, 0 },
{ 0, 0, 2, 2, 2, 0, 0 },
{ 0, 2, 1, 2, 1, 2, 0 },
{ 0, 1, 0, 2, 1, 1, 0 },
{ 0, 0, 0, 2, 1, 0, 0 },
{ 0, 0, 0, 2, 1, 0, 0 },
{ 0, 0, 0, 1, 1, 0, 0 }
};

static unsigned char CharDown[CHARHEIGHT][CHARWIDTH] = {
{ 0, 0, 0, 2, 0, 0, 0 },
{ 0, 0, 0, 2, 1, 0, 0 },
{ 0, 0, 0, 2, 1, 0, 0 },
{ 0, 2, 1, 2, 1, 2, 0 },
{ 0, 1, 2, 2, 2, 1, 0 },
{ 0, 0, 1, 2, 1, 0, 0 },
{ 0, 0, 0, 1, 0, 0, 0 },
};

static unsigned char CharLeft[CHARHEIGHT][CHARWIDTH] = {
{ 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 2, 0, 0, 0, 0 },
{ 0, 2, 1, 0, 0, 0, 0 },
{ 2, 2, 2, 2, 2, 2, 0 },
{ 1, 2, 1, 1, 1, 1, 1 },
{ 0, 1, 2, 0, 0, 0, 0 },
{ 0, 0, 1, 0, 0, 0, 0 }
};

static unsigned char CharRight[CHARHEIGHT][CHARWIDTH] = {
{ 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 2, 0, 0, 0 },
{ 0, 0, 0, 1, 2, 0, 0 },
{ 2, 2, 2, 2, 2, 2, 0 },
{ 1, 1, 1, 1, 2, 1, 0 },
{ 0, 0, 0, 2, 1, 0, 0 },
{ 0, 0, 0, 1, 0, 0, 0 }
};

static unsigned char CharStart[CHARHEIGHT][CHARWIDTH] = {
{ 0, 0, 2, 0, 0, 0, 0 },
{ 0, 0, 2, 2, 0, 0, 0 },
{ 0, 0, 2, 2, 2, 1, 0 },
{ 0, 0, 2, 2, 2, 1, 0 },
{ 0, 0, 2, 2, 1, 0, 0 },
{ 0, 0, 2, 1, 0, 0, 0 },
{ 0, 0, 1, 0, 0, 0, 0 }
};

static unsigned char CharRecord[CHARHEIGHT][CHARWIDTH] = {
{ 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 2, 2, 2, 0, 0 },
{ 0, 2, 2, 2, 2, 2, 0 },
{ 0, 2, 2, 2, 2, 2, 1 },
{ 0, 2, 2, 2, 2, 2, 1 },
{ 0, 0, 2, 2, 2, 1, 0 },
{ 0, 0, 1, 1, 1, 0, 0 }
};
static unsigned char CharPlayback[CHARHEIGHT][CHARWIDTH] = {
{ 0, 0, 2, 0, 0, 0, 0 },
{ 0, 0, 2, 2, 0, 0, 0 },
{ 0, 0, 2, 2, 2, 1, 0 },
{ 0, 0, 2, 2, 2, 1, 0 },
{ 0, 0, 2, 2, 1, 0, 0 },
{ 0, 0, 2, 1, 0, 0, 0 },
{ 0, 0, 1, 0, 0, 0, 0 }
};

static unsigned char CharNumbers[10][CHARHEIGHT][CHARWIDTH] = {
{
{ 0, 0, 2, 2, 0, 0, 0 },
{ 0, 2, 1, 1, 2, 0, 0 },
{ 0, 2, 1, 0, 2, 1, 0 },
{ 0, 2, 1, 0, 2, 1, 0 },
{ 0, 2, 1, 0, 2, 1, 0 },
{ 0, 1, 2, 2, 1, 0, 0 },
{ 0, 0, 1, 1, 0, 0, 0 }
},
{
{ 0, 0, 2, 2, 0, 0, 0 },
{ 0, 0, 1, 2, 1, 0, 0 },
{ 0, 0, 0, 2, 1, 0, 0 },
{ 0, 0, 0, 2, 1, 0, 0 },
{ 0, 0, 0, 2, 1, 0, 0 },
{ 0, 0, 2, 2, 2, 0, 0 },
{ 0, 0, 0, 1, 1, 1, 0 }
},
{
{ 0, 0, 2, 2, 0, 0, 0 },
{ 0, 2, 1, 1, 2, 0, 0 },
{ 0, 1, 0, 2, 1, 0, 0 },
{ 0, 0, 2, 1, 0, 0, 0 },
{ 0, 2, 1, 0, 0, 0, 0 },
{ 0, 2, 2, 2, 2, 0, 0 },
{ 0, 0, 1, 1, 1, 1, 0 }
},
{
{ 0, 0, 2, 2, 0, 0, 0 },
{ 0, 2, 1, 1, 2, 1, 0 },
{ 0, 1, 0, 2, 1, 0, 0 },
{ 0, 0, 0, 1, 2, 1, 0 },
{ 0, 2, 0, 0, 2, 1, 0 },
{ 0, 1, 2, 2, 1, 0, 0 },
{ 0, 0, 1, 1, 0, 0, 0 }
},
{
{ 0, 0, 0, 2, 0, 0, 0 },
{ 0, 0, 2, 2, 0, 0, 0 },
{ 0, 2, 1, 2, 0, 0, 0 },
{ 0, 2, 2, 2, 2, 0, 0 },
{ 0, 1, 1, 2, 1, 0, 0 },
{ 0, 0, 0, 2, 1, 0, 0 },
{ 0, 0, 0, 1, 0, 0, 0 }
},
{
{ 0, 2, 2, 2, 2, 0, 0 },
{ 0, 2, 1, 1, 1, 0, 0 },
{ 0, 2, 2, 2, 0, 0, 0 },
{ 0, 1, 1, 1, 2, 0, 0 },
{ 0, 2, 0, 0, 2, 0, 0 },
{ 0, 1, 2, 2, 1, 0, 0 },
{ 0, 0, 1, 1, 0, 0, 0 }
},
{
{ 0, 0, 2, 2, 0, 0, 0 },
{ 0, 2, 1, 1, 0, 0, 0 },
{ 0, 2, 2, 2, 0, 0, 0 },
{ 0, 2, 1, 1, 2, 1, 0 },
{ 0, 2, 0, 0, 2, 1, 0 },
{ 0, 1, 2, 2, 1, 0, 0 },
{ 0, 0, 1, 1, 0, 0, 0 }
},
{
{ 0, 2, 2, 2, 2, 0, 0 },
{ 0, 1, 1, 1, 2, 1, 0 },
{ 0, 0, 0, 2, 1, 0, 0 },
{ 0, 0, 2, 1, 0, 0, 0 },
{ 0, 0, 2, 1, 0, 0, 0 },
{ 0, 2, 1, 0, 0, 0, 0 },
{ 0, 1, 0, 0, 0, 0, 0 }
},
{
{ 0, 0, 2, 2, 0, 0, 0 },
{ 0, 2, 1, 1, 2, 1, 0 },
{ 0, 1, 2, 2, 1, 0, 0 },
{ 0, 2, 0, 0, 2, 1, 0 },
{ 0, 2, 0, 0, 2, 1, 0 },
{ 0, 1, 2, 2, 1, 0, 0 },
{ 0, 0, 1, 1, 0, 0, 0 }
},
{
{ 0, 0, 2, 2, 1, 0, 0 },
{ 0, 2, 1, 0, 2, 1, 0 },
{ 0, 2, 1, 0, 2, 1, 0 },
{ 0, 1, 2, 2, 2, 1, 0 },
{ 0, 0, 1, 1, 2, 1, 0 },
{ 0, 0, 2, 2, 1, 0, 0 },
{ 0, 0, 1, 1, 0, 0, 0 }
}
};

#define OSD_PALETTE_START 0x40

void MdrawOsdInit() {
	unsigned int i;
	for (i = 0; i < sizeof(Palette)/sizeof(Palette[0]); i++) {
		Mdraw.Pal[OSD_PALETTE_START+i] = Palette[i];
	}
	Mdraw.PalChange = 1;
}

static void MdrawOsdChar(unsigned char character[CHARHEIGHT][CHARWIDTH], int x, int y) {
	int i;
	unsigned char *line;
	if (Mdraw.Line<y || Mdraw.Line>=y+CHARHEIGHT) return;
	line = character[Mdraw.Line-y];
	for (i = 0; i < CHARWIDTH; i++) {
		unsigned char ch = line[i];
		if (ch!=0) Mdraw.Data[x+i] = ch+OSD_PALETTE_START-1;
	}
}
	
void MdrawOsd() {
// gg  24 168
// sms 16 192
	char framestr[32];
	char *frameptr;
	int x;
	int xlef = MastEx&MX_GG?64:16, ybot = MastEx&MX_GG?168:192;
	int xrig = MastEx&MX_GG?220:268, ydif = MastEx&MX_GG?140:188;

	if (MdrawOsdOptions&OSD_BUTTONS) {
		if (MastInput[0]&0x01) MdrawOsdChar(CharUp, xlef+20, ybot-40);
		if (MastInput[0]&0x02) MdrawOsdChar(CharDown, xlef+20, ybot-20);
		if (MastInput[0]&0x04) MdrawOsdChar(CharLeft, xlef+10, ybot-30);
		if (MastInput[0]&0x08) MdrawOsdChar(CharRight, xlef+30, ybot-30);
		if (MastInput[0]&0x10) MdrawOsdChar(CharNumbers[1], xlef+45, ybot-30);
		if (MastInput[0]&0x20) MdrawOsdChar(CharNumbers[2], xlef+55, ybot-30);
		if (MastInput[0]&0x80) MdrawOsdChar(CharStart, xlef+70, ybot-30);
	}

	if (MdrawOsdOptions&OSD_FRAMECOUNT && MvidInVideo()) {
		snprintf(framestr, 16, "%d", frameCount);
		for (x = xlef, frameptr = framestr; *frameptr; frameptr++, x+=CHARWIDTH) {
			MdrawOsdChar(CharNumbers[*frameptr - '0'], x, ybot-CHARHEIGHT-1);
		}
	}
	if ((MdrawOsdOptions&OSD_FRAMECOUNT)) {
		snprintf(framestr, 32, "%d", frameCount);
		for (x = xlef, frameptr = framestr; *frameptr; frameptr++, x+=CHARWIDTH) {
			MdrawOsdChar(CharNumbers[*frameptr - '0'], x, ybot-CHARHEIGHT-1);
		}
		if (MvidInVideoPlayback()) {
			MdrawOsdChar(CharDivider, x, ybot-CHARHEIGHT-1);
			snprintf(framestr, 32, "%d", MvidGetFrameCount());
			for (x += CHARWIDTH-1, frameptr = framestr; *frameptr; frameptr++, x+=CHARWIDTH) {
				MdrawOsdChar(CharNumbers[*frameptr - '0'], x, ybot-CHARHEIGHT-1);
			}
		}
		if (MvidInVideoRecord())
			MdrawOsdChar(CharRecord, xrig-8, ybot-ydif);
		if (MvidInVideoPlayback())
			MdrawOsdChar(CharPlayback, xrig-8, ybot-ydif);
	}
}


