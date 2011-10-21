#include "mastint.h"
#include <stdio.h>
#ifdef unix
#include <unistd.h>
#include <sys/types.h>
#endif
#ifdef WIN32
#include <windows.h>
#include <io.h>
#endif

static FILE *videoFile = NULL;
static char videoFilename[256] = "";
static int videoMode = 0;
static struct MvidHeader currentMovie;

extern int VideoReadOnly;

int frameCount;

#define HEADER_SIZE 0xF4
#define SAVE_STATE_SIZE 41472

int DEGA_LuaRerecordCountSkip(); // ../master/luaengine.cpp

static int MastAcbVideoRead(struct MastArea *area) {
	return fread(area->Data, area->Len, 1, videoFile);
}

static int MastAcbVideoWrite(struct MastArea *area) {
	return fwrite(area->Data, area->Len, 1, videoFile);
}

int MvidReadHeader(FILE *file, struct MvidHeader *header) {
	int fileHeaderSize;
	char id[4];

	fseek(file, 0x0, SEEK_SET);
	fread(id, 4, 1, file);
	if (id[0]!='M'||id[1]!='M'||id[2]!='V'||id[3]!='\0') { // Bad file
	  return 0;
	}

	fread(&header->mastVer, 4, 1, file);
	fread(&header->vidFrameCount, 4, 1, file);
	fread(&header->rerecordCount, 4, 1, file);
	fread(&header->beginReset, 4, 1, file);

	fread(&header->stateOffset, 4, 1, file);
	fread(&header->inputOffset, 4, 1, file);
	fread(&header->packetSize, 4, 1, file);

	if (header->stateOffset==0 && !header->beginReset) {
		header->stateOffset = 0x60; // hardcoded constants from 1st version
	}

	if (header->inputOffset==0) {
		header->inputOffset = header->beginReset ? 0x60 : 0x60+25088; // hardcoded constants from 1st version
	}

	if (header->packetSize==0) {
		header->packetSize = 2; // hardcoded constants from 1st version
	}

	fread(header->videoAuthor, sizeof(header->videoAuthor), 1, file);

	fileHeaderSize = header->beginReset ? header->inputOffset : header->stateOffset;

	if (fileHeaderSize >= 0x64) {
		fread(&header->vidFlags, 4, 1, file);
	} else {
		header->vidFlags = 0;
	}

	if (fileHeaderSize >= 0xe4) {
		fread(header->vidRomName, 128, 1, file);
	} else {
		memset(header->vidRomName, 0, 128);
	}

	if (fileHeaderSize >= 0xf4) {
		fread(header->vidDigest, 16, 1, file);
	} else {
		memset(header->vidDigest, 0, 16);
	}

	return 1;
}

int MvidStart(char *filename, int mode, int reset, char *author) {
	int changed = 0; 

	if (videoFile != NULL) {
		fclose(videoFile);
		videoFile = NULL;
	}

	videoMode = mode;
	strncpy(videoFilename, filename, sizeof(videoFilename)); videoFilename[sizeof(videoFilename)-1] = 0;
	frameCount = 0;

	if (mode == PLAYBACK_MODE) {
		videoFile = fopen(filename, "rb");
		if (videoFile != NULL) {
			int result = MvidReadHeader(videoFile, &currentMovie);
			if (result==0) {
			 	fclose(videoFile);
				videoFile=0;
				return -1;
			}

			changed = ((currentMovie.vidFlags & VIDFLAG_PAL) != 0) ^ ((MastEx & MX_PAL) != 0);

			if (currentMovie.vidFlags & VIDFLAG_PAL) {
				MastEx |= MX_PAL;
			} else {
				MastEx &= ~MX_PAL;
			}
			if (changed) MvidModeChanged();

			if (currentMovie.vidFlags & VIDFLAG_JAPAN) {
				MastEx |= MX_JAPAN;
			} else {
				MastEx &= ~MX_JAPAN;
			}

			if (currentMovie.beginReset) {
				MastHardReset();
			} else {
				fseek(videoFile, currentMovie.stateOffset, SEEK_SET);
				MastAcb = MastAcbVideoRead;
				MastAreaDega();
			}
			fseek(videoFile, currentMovie.inputOffset, SEEK_SET);
		}
	} else if (mode == RECORD_MODE) {
		videoFile = fopen(filename, "wb");
		if (videoFile != NULL) {
			int zero = 0;
			VideoReadOnly = 0;

			fwrite("MMV", 4, 1, videoFile);    // 0000: identifier
			fwrite(&MastVer, 4, 1, videoFile); // 0004: version
			currentMovie.vidFrameCount = 0;
			fwrite(&zero, 4, 1, videoFile);    // 0008: frame count
			currentMovie.rerecordCount = 0;
			fwrite(&zero, 4, 1, videoFile);    // 000c: rerecord count
			currentMovie.beginReset = reset;
			fwrite(&reset, 4, 1, videoFile);   // 0010: begin from reset?
		
			currentMovie.stateOffset = reset ? 0 : HEADER_SIZE;
			fwrite(&currentMovie.stateOffset, 4, 1, videoFile); // 0014: offset of state information

			currentMovie.inputOffset = reset ? HEADER_SIZE : HEADER_SIZE+SAVE_STATE_SIZE;
			fwrite(&currentMovie.inputOffset, 4, 1, videoFile); // 0018: offset of input data

			currentMovie.packetSize = sizeof(MastInput);
			fwrite(&currentMovie.packetSize, 4, 1, videoFile);  // 001c: size of input packet

			memset(currentMovie.videoAuthor, 0, sizeof(currentMovie.videoAuthor));
			if (author) strncpy(currentMovie.videoAuthor, author, sizeof(currentMovie.videoAuthor));
			fwrite(currentMovie.videoAuthor, sizeof(currentMovie.videoAuthor), 1, videoFile); // 0020-005f: author

			currentMovie.vidFlags = 0;
			if (MastEx & MX_PAL) {
				currentMovie.vidFlags |= VIDFLAG_PAL;
			}
			if (MastEx & MX_JAPAN) {
				currentMovie.vidFlags |= VIDFLAG_JAPAN;
			}
			if (MastEx & MX_GG) {
				currentMovie.vidFlags |= VIDFLAG_GG;
			}
			fwrite(&currentMovie.vidFlags, 4, 1, videoFile);    // 0060: flags

			memcpy(currentMovie.vidRomName, MastRomName, 128);
			fwrite(MastRomName, 128, 1, videoFile); // 0064-00e3: rom name

			MastGetRomDigest(currentMovie.vidDigest);
			fwrite(currentMovie.vidDigest, 16, 1, videoFile);      // 00e4-00f3: rom digest

			if (reset) {
				MastHardReset();
			} else {
				MastAcb = MastAcbVideoWrite;
				MastAreaDega();
			}
		}
	}

	if (videoFile == NULL) {
		/* perror("while opening video file: fopen"); */
		return -1;
	}

	return currentMovie.vidFrameCount;
}

int MvidGetMode() {
	return videoMode;
}

int MvidGetFrameCount() {
	return currentMovie.vidFrameCount;
}

int MvidGetRerecordCount() {
	return currentMovie.rerecordCount;
}

void MvidStop() {
	if (videoFile != NULL) {
		fclose(videoFile);
		videoFile = NULL;
		MvidMovieStopped();
		MdrawRefresh();
	}
}

int MvidSetAuthor(char *author) {
	FILE *rwVideoFile;

	if (*videoFilename == 0) {
		return 0;
	}

	memset(currentMovie.videoAuthor, 0, sizeof(currentMovie.videoAuthor));
	strncpy(currentMovie.videoAuthor, author, sizeof(currentMovie.videoAuthor));

	if (videoFile) fflush(videoFile);
	rwVideoFile = fopen(videoFilename, "r+b");
	if (rwVideoFile == 0) return 0;
	fseek(rwVideoFile, 0x20, SEEK_SET);
	fwrite(currentMovie.videoAuthor, sizeof(currentMovie.videoAuthor), 1, rwVideoFile);
	fclose(rwVideoFile);

	return 1;
}

void MvidGetAuthor(char *author, int len) {
	if (len > sizeof(currentMovie.videoAuthor)+1) len = sizeof(currentMovie.videoAuthor)+1;
	strncpy(author, currentMovie.videoAuthor, len);
	author[len-1] = 0;
}

int MvidGotProperties() {
	return (*videoFilename != 0);
}

int MvidInVideo() {
	return (videoFile != 0);
}
int MvidInVideoPlayback() {
	if (videoFile != NULL)
		return (videoMode == PLAYBACK_MODE);
	else
		return 0;
}
int MvidInVideoRecord() {
	if (videoFile != NULL)
		return (videoMode == RECORD_MODE);
	else
		return 0;
}

#ifdef unix
static void Truncate(FILE *file, int size) {
	ftruncate(fileno(file), size);
}
#else
#ifdef WIN32
static void Truncate(FILE *file, int size) {
	_chsize(fileno(file), size);
}
#else
#error no truncate implementation available!
#endif
#endif

void MvidPreFrame() {
	int result;
	frameCount++;

	char zero = 0;
	int i;

	if (videoFile != NULL) {
		switch (videoMode) {
			case PLAYBACK_MODE:
				result = fread(MastInput, sizeof(MastInput), 1, videoFile);

				if (result == 0) {
					MvidStop();
				} else {
					fseek(videoFile, currentMovie.packetSize-sizeof(MastInput), SEEK_CUR);
				}

				break;
			case RECORD_MODE:
				fwrite(MastInput, sizeof(MastInput), 1, videoFile);
				for (i = 0; i < currentMovie.packetSize-sizeof(MastInput); i++) {
					fwrite(&zero, 1, 1, videoFile);
				}

				if (currentMovie.vidFrameCount > frameCount) {
					Truncate(videoFile, currentMovie.inputOffset+frameCount*currentMovie.packetSize);
				}
				currentMovie.vidFrameCount=frameCount;
				fseek(videoFile, 0x8, SEEK_SET);
				fwrite(&frameCount, 4, 1, videoFile);
				fseek(videoFile, 0, SEEK_END);

				break;
		}
	}
}

static struct EmbededVideoFileInfo {
	int vidFrameCount;
	int inputOffset;
	int packetSize;
	int isOldType;
} embInfo;

int MvidPreLoadState(int readonly) {
	int embFrameCount = MastAreaMvidFrameCount();
	if (videoFile != NULL) {
		unsigned char *data = (unsigned char *)malloc(0x20);
		MastSeekcb(MastAreaMvidOffset(), SEEK_SET); //magic numbers
		MastArea ma;
		ma.Len = 0x20; ma.Data = data;
		if (MastAcb(&ma) == 0) { /* old-style save state without embedded video file */
			embInfo.isOldType = 1;
			free(data);
			return -1;
		}

		int embVidFrameCount = *(int *)(data + 0x08);
		if (embVidFrameCount == 0) embVidFrameCount = min(frameCount, currentMovie.vidFrameCount);
		int embInputOffset = *(int *)(data + 0x18);
		if (embInputOffset == 0) embInputOffset = currentMovie.beginReset ? 0x60 : 0x60+25088;
		int embPacketSize = *(int *)(data + 0x1c);
		if (embPacketSize == 0) embPacketSize = 2;

		free(data);

		if (currentMovie.packetSize != embPacketSize) {
			return 1;
		}

		if (embFrameCount > embVidFrameCount) {
			return 2;
		}

		if (readonly != 0) {
			if (embFrameCount > currentMovie.vidFrameCount) {
				return 3;
			}

			unsigned int baseSize = embFrameCount*embPacketSize;
			unsigned char *videoFileData = (unsigned char *)malloc(baseSize);
			data = (unsigned char *)malloc(baseSize);

			long int originalPos = ftell(videoFile);
			fseek(videoFile, currentMovie.inputOffset, SEEK_SET);
			fread(videoFileData, baseSize, 1, videoFile);
			fseek(videoFile, originalPos, SEEK_SET);
			ma.Len = baseSize; ma.Data = data;
			MastSeekcb(embInputOffset - 0x20, SEEK_CUR);
			MastAcb(&ma);

			int Ret = memcmp(videoFileData, data, baseSize);
			if (Ret != 0) {
				free(data);
				free(videoFileData);
				return 4;
			}

			free(data);
			free(videoFileData);

			return 0;
		}

		embInfo.vidFrameCount = embVidFrameCount;
		embInfo.inputOffset = embInputOffset;
		embInfo.packetSize = embPacketSize;
		embInfo.isOldType = 0;
	}
	return 0;
}

void MvidPostLoadState(int readonly) {
	if (videoFile != NULL) {
		int newPos = currentMovie.inputOffset + frameCount*currentMovie.packetSize;
		if (readonly != 0) {
			if (videoMode == RECORD_MODE) {
				fclose(videoFile);
				videoFile = fopen(videoFilename, "rb");
				videoMode = PLAYBACK_MODE;
			}
		} else {
			fclose(videoFile);
			videoFile = fopen(videoFilename, "r+b");

			if(!DEGA_LuaRerecordCountSkip()) currentMovie.rerecordCount++;

			if (embInfo.isOldType != 0) {
				currentMovie.vidFrameCount = frameCount;

				Truncate(videoFile, newPos);
				fseek(videoFile, 0x8, SEEK_SET);
				fwrite(&currentMovie.vidFrameCount, 4, 1, videoFile);
				fwrite(&currentMovie.rerecordCount, 4, 1, videoFile);
			} else {
				currentMovie.vidFrameCount = embInfo.vidFrameCount;

				int size = embInfo.vidFrameCount*embInfo.packetSize;
				unsigned char *data = (unsigned char *)malloc(size);
				MastArea ma;
				ma.Len = size; ma.Data = data;
				MastSeekcb(embInfo.inputOffset, SEEK_CUR);
				MastAcb(&ma);

				Truncate(videoFile, currentMovie.inputOffset);
				fseek(videoFile, 0x8, SEEK_SET);
				fwrite(&currentMovie.vidFrameCount, 4, 1, videoFile);
				fwrite(&currentMovie.rerecordCount, 4, 1, videoFile);

				fseek(videoFile, 0, SEEK_END);
				fwrite(data, size, 1, videoFile);
				free(data);
			}
			videoMode = RECORD_MODE;
		}
		// memset(MastInput, 0, sizeof(MastInput));
		if (frameCount > 0) {
			fseek(videoFile, newPos - currentMovie.packetSize, SEEK_SET);
			if (fread(MastInput, sizeof(MastInput), 1, videoFile) == 0) {
				// hmmm
				MvidStop();
			}
		}
		fseek(videoFile, newPos, SEEK_SET);
	}
	memset(&embInfo, 0, sizeof(embInfo));
}

void MvidPostSaveState() {
	if (videoFile != NULL) {
		MastArea ma;
		unsigned char *data;
		int size;
		FILE *readVideoFile;

		fflush(videoFile); /* ensure video file fully written to disk */
		size = currentMovie.inputOffset + currentMovie.vidFrameCount*currentMovie.packetSize;
		data = (unsigned char *)malloc(size);

		readVideoFile = fopen(videoFilename, "rb");
		fread(data, size, 1, readVideoFile);

		*(int *)(data + 0x8) = currentMovie.vidFrameCount; /* TODO magic number */

		ma.Len = size; ma.Data = data; MastAcb(&ma);

		free(data);
		fclose(readVideoFile);
	}
}

// Partially implemented
void MvidPostHardReset() {
	frameCount = 0;
	memset(MastInput, 0, sizeof(MastInput));
}
