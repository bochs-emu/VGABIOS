#
#  Copyright (C) 2001-2021 The LGPL VGABios developers Team
#

SHELL = /bin/sh

CC      = gcc
CFLAGS  = -g -O2 -Wall -Wstrict-prototypes
LDFLAGS = 

GCC = gcc
BCC = bcc
AS86 = as86

RELEASE = `pwd | sed "s-.*/--"`
RELDATE = `date '+%d %b %Y'`
RELVERS = `pwd | sed "s-.*/--" | sed "s/vgabios//" | sed "s/-//"`

VGABIOS_DATE = "-DVGABIOS_DATE=\"$(RELDATE)\""
RELEASE_BIN_LIST=README VGABIOS-lgpl-latest.bin VGABIOS-lgpl-latest.debug.bin \
                 VGABIOS-lgpl-latest.cirrus.bin VGABIOS-lgpl-latest.cirrus.debug.bin \
                 VGABIOS-lgpl-latest.banshee.bin

all: bios cirrus-bios banshee-bios

bios: vgabios.bin vgabios.debug.bin

cirrus-bios: vgabios-cirrus.bin vgabios-cirrus.debug.bin

banshee-bios: vgabios-banshee.bin

clean:
	/bin/rm -f  biossums vbetables-gen vbetables.h *.o *.s *.ld86 \
          temp.awk.* vgabios*.orig _vgabios_* _vgabios-debug_* core vgabios*.bin vgabios*.txt $(RELEASE).bin *.bak

dist-clean: clean

# source files
VGA_FILES := vgabios.c vgabios.h vgafonts.h vgatables.h
VBE_FILES := vbe.h vbe.c vbetables.h
BANSHEE_FILES := banshee.c

# build flags
vgabios.bin              : VGAFLAGS := -DVBE -DPCIBIOS
vgabios.debug.bin        : VGAFLAGS := -DVBE -DPCIBIOS -DDEBUG
vgabios-cirrus.bin       : VGAFLAGS := -DCIRRUS -DPCIBIOS
vgabios-cirrus.debug.bin : VGAFLAGS := -DCIRRUS -DPCIBIOS -DCIRRUS_DEBUG
vgabios-banshee.bin      : VGAFLAGS := -DBANSHEE -DPCIBIOS

# dist names
vgabios.bin              : DISTNAME := VGABIOS-lgpl-latest.bin
vgabios.debug.bin        : DISTNAME := VGABIOS-lgpl-latest.debug.bin
vgabios-cirrus.bin       : DISTNAME := VGABIOS-lgpl-latest.cirrus.bin
vgabios-cirrus.debug.bin : DISTNAME := VGABIOS-lgpl-latest.cirrus.debug.bin
vgabios-banshee.bin      : DISTNAME := VGABIOS-lgpl-latest.banshee.bin

# dependencies
vgabios.bin              : $(VGA_FILES) $(VBE_FILES) biossums
vgabios.debug.bin        : $(VGA_FILES) $(VBE_FILES) biossums
vgabios-cirrus.bin       : $(VGA_FILES) clext.c biossums
vgabios-cirrus.debug.bin : $(VGA_FILES) clext.c biossums
vgabios-banshee.bin      : $(VGA_FILES) banshee.c biossums

# build rule
%.bin:
	$(GCC) -E -P vgabios.c $(VGABIOS_VERS) $(VGAFLAGS) $(VGABIOS_DATE) > _$*_.c
	$(BCC) -o $*.s -C-c -D__i86__ -S -0 _$*_.c
	sed -e 's/^\.text//' -e 's/^\.data//' $*.s > _$*_.s
	$(AS86) _$*_.s -b $*.bin -u -w- -g -0 -j -O -l $*.txt
	rm -f _$*_.s _$*_.c $*.s
	mv $*.bin $(DISTNAME)
	./biossums $(DISTNAME)
	ls -l $(DISTNAME)

release: 
	VGABIOS_VERS=\"-DVGABIOS_VERS=\\\"$(RELVERS)\\\"\" make bios cirrus-bios banshee-bios
	make clean
	tar czvf ../$(RELEASE)-bin.tgz $(RELEASE_BIN_LIST)
	tar czvf ../$(RELEASE).tgz --exclude .svn -C .. $(RELEASE)/

biossums: biossums.c
	$(CC) -o biossums biossums.c

vbetables-gen: vbetables-gen.c
	$(CC) -o vbetables-gen vbetables-gen.c

vbetables.h: vbetables-gen
	./vbetables-gen > $@
