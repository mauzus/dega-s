#DEBUG_SYM=1
#PROFILE_GENERATE=1
#PROFILE_USE=1

ifndef DEBUG_SYM
	OPTFLAGS=-O3 -fomit-frame-pointer -funroll-loops -Wall
#	OPTFLAGS=-O3 -fomit-frame-pointer -funroll-loops -march=i686 -mcpu=i686
#	OPTFLAGS=-xM -O3
ifndef PROFILE_GENERATE
	STRIP = strip
endif
endif

ifdef DEBUG_SYM
	OPTFLAGS=-Wall
endif

CC=gcc
#CC=icc
CXX=g++
#CXX=icpc
NASM=nasm
WINDRES=windres

CCVER = $(shell $(CC) -v 2>&1)

ifneq (,$(findstring mingw32,$(CCVER)))
	P=win
else ifneq (,$(findstring cygwin,$(CCVER)))
	P=win
else
	P=unix
endif

ifneq (,$(findstring x86_64,$(CCVER)))
	BITS=64
else
	BITS=32
endif

ifeq ($(P),unix)
	CFLAGS= $(OPTFLAGS) $(shell sdl-config --cflags) -DUSE_MENCODER -Imast -Idoze -Ilibmencoder -D__cdecl= -D__fastcall= -Iliblua
else ifeq ($(P),win)
	CFLAGS= $(OPTFLAGS) -DUSE_VFW -Imast -Idoze -Imaster -Iextra -Izlib -Ilibvfw -Iliblua
endif

ifndef Z80
	Z80=z80jb
endif

ifeq ($(Z80),z80jb)
	CFLAGS += -Iz80jb -DEMU_Z80JB
	Z80OBJ = z80jb/z80.o z80jb/z80daisy.o
else
	CFLAGS += -Idoze -DEMU_DOZE
	Z80OBJ = doze/doze.o doze/dozea.o
endif

ifdef DEBUG_SYM
	CFLAGS  += -ggdb
endif
ifdef PROFILE_GENERATE
	CFLAGS  += -fprofile-generate
endif
ifdef PROFILE_USE
	CFLAGS  += -fprofile-use
endif

CXXFLAGS= $(CFLAGS) -fno-exceptions

DAMOBJ = doze/dam.o doze/dama.o doze/damc.o doze/dame.o doze/damf.o doze/damj.o doze/damm.o doze/damo.o doze/damt.o
MASTOBJ = mast/area.o mast/dpsg.o mast/draw.o mast/emu2413.o mast/frame.o mast/load.o mast/map.o mast/mast.o mast/mem.o mast/samp.o mast/snd.o mast/vgm.o mast/video.o mast/osd.o mast/md5.o

ifeq ($(P),unix)
ifeq ($(BITS),64)
	NASM_FORMAT = elf64
else
	NASM_FORMAT = elf
endif
	EXEEXT =
	SOEXT = .so
	PLATOBJ = sdl/main.o
	EXTRA_LIBS = $(shell sdl-config --libs)
	DOZE_FIXUP = sed -f doze/doze.cmd.sed <doze/dozea.asm >doze/dozea.asm.new && mv doze/dozea.asm.new doze/dozea.asm
	ENCODER_OBJ = tools/degavi.o
	ENCODER_LIBS = libmencoder/libmencoder.a
	ENCODER_LDFLAGS = -lm
	EXTRA_LDFLAGS =
	GUI_LDFLAGS =
else ifeq ($(P),win)
	NASM_FORMAT = win32
	EXEEXT = .exe
	PLATOBJ = master/app.o master/conf.o master/dinp.o master/disp.o master/dsound.o master/emu.o master/frame.o master/input.o master/load.o master/loop.o master/main.o master/misc.o master/render.o master/run.o master/shot.o master/state.o master/video.o master/zipfn.o master/keymap.o zlib/libz.a master/ramwatch.o master/ramsearch.o master/luaconsole.o master/luasav.o master/luaengine.o liblua/liblua.a
	EXTRA_LIBS = -ldsound -ldinput -lddraw -ldxguid -lcomdlg32 -lcomctl32 -luser32 -lwinmm -lgdi32
	DOZE_FIXUP =
	ENCODER_OBJ = tools/wdegavi.o tools/degavirc.o
	ENCODER_LIBS = libvfw/libvfw.a
	ENCODER_LDFLAGS = -lcomdlg32 -lvfw32 -lmsacm32 -lole32 -lm -Wl,--subsystem,windows
	EXTRA_LDFLAGS = -static
	GUI_LDFLAGS = -Wl,--subsystem,windows
endif

ifdef PROFILE_GENERATE
	EXTRA_LDFLAGS  += -fprofile-arcs
endif

ALLOBJ = dega-s$(EXEEXT) # mmvconv$(EXEEXT) degavi$(EXEEXT)

ifeq ($(P),unix)

all: $(ALLOBJ)

release:
	git tag dega-$(R)
	git clone . ../dega-$(R)
	cd ../dega-$(R) && git checkout dega-$(R)
	rm -rf ../dega-$(R)/.git
	git clone ../libmencoder ../dega-$(R)/libmencoder
	rm -rf ../dega-$(R)/libmencoder/.git
	git clone ../libvfw ../dega-$(R)/libvfw
	rm -rf ../dega-$(R)/libvfw/.git
	cd .. && tar czf dega-$(R).tar.gz dega-$(R)

libmencoder/libmencoder.a:
	$(MAKE) -Clibmencoder CFLAGS="$(CFLAGS)" libmencoder.a

else ifeq ($(P),win)

all: $(ALLOBJ)

liblua/liblua.a:
	$(MAKE) -Cliblua PLAT=mingw CFLAGS="$(CFLAGS)" liblua.a

zlib/libz.a:
	$(MAKE) -Czlib CFLAGS="$(CFLAGS)" libz.a

libvfw/libvfw.a:
	$(MAKE) -Clibvfw CFLAGS="$(CFLAGS)" libvfw.a

release: all
	rm -rf dega-$(R)-win32-$(Z80)
	mkdir dega-$(R)-win32-$(Z80)
	cp dega.exe degavi.exe mmvconv.exe dega.txt dega-$(R)-win32-$(Z80)/
	$(STRIP) dega-$(R)-win32-$(Z80)/dega.exe dega-$(R)-win32-$(Z80)/degavi.exe dega-$(R)-win32-$(Z80)/mmvconv.exe
	cd dega-$(R)-win32-$(Z80) && zip -9 ../dega-$(R)-win32-$(Z80).zip dega.exe degavi.exe mmvconv.exe dega.txt

else

all:
	@echo Supported platforms are unix and win
	@false

endif

dega-s$(EXEEXT): $(PLATOBJ) $(Z80OBJ) $(MASTOBJ)
	$(CXX) $(EXTRA_LDFLAGS) $(GUI_LDFLAGS) -o dega-s$(EXEEXT) $(PLATOBJ) $(Z80OBJ) $(MASTOBJ) $(EXTRA_LIBS)
ifdef STRIP
	$(STRIP) dega-s$(EXEEXT)
endif

degavi$(EXEEXT): tools/avioutput.o $(ENCODER_OBJ) $(Z80OBJ) $(MASTOBJ) $(ENCODER_LIBS)
	$(CC) $(EXTRA_LDFLAGS) -o degavi$(EXEEXT) tools/avioutput.o $(ENCODER_OBJ) $(Z80OBJ) $(MASTOBJ) $(ENCODER_LIBS) $(ENCODER_LDFLAGS)

mmvconv$(EXEEXT): tools/mmvconv.o
	$(CC) $(EXTRA_LDFLAGS) -o mmvconv$(EXEEXT) tools/mmvconv.o

doze/dozea.o: doze/dozea.asm
	nasm -f $(NASM_FORMAT) -o doze/dozea.o doze/dozea.asm

doze/dozea.asm: doze/dam$(EXEEXT)
	cd doze ; $(WINE) ./dam
	$(DOZE_FIXUP)

doze/dam$(EXEEXT): $(DAMOBJ)
	$(CC) -o doze/dam$(EXEEXT) $(DAMOBJ)

master/app.o: master/app.rc
	cd master && $(WINDRES) -o app.o app.rc

tools/degavirc.o: tools/degavirc.rc
	cd tools && $(WINDRES) -o degavirc.o degavirc.rc

clean:
	rm -f $(Z80OBJ) $(DAMOBJ) $(MASTOBJ) $(PLATOBJ) tools/avioutput.o tools/degavi.o tools/degavirc.o tools/mmvconv.o tools/wdegavi.o doze/dozea.asm* doze/dam doze/dam.exe dega dega.exe degavi degavi.exe mmvconv mmvconv.exe
	make -Czlib clean
	make -Clibmencoder clean
	make -Clibvfw clean
	make -Cliblua clean

distclean: clean
	rm -f *~ */*~
