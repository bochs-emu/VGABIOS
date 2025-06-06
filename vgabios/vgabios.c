// ============================================================================================
//
//  Copyright (C) 2001-2025 The LGPL VGABios developers Team
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
//
// ============================================================================================
//
//  This VGA Bios is specific to the Bochs Emulated VGA card.
//  You can NOT drive any physical vga card with it.
//
// ============================================================================================
//
//  This file contains code ripped from :
//   - rombios.c of plex86
//
//  This VGA Bios contains fonts from :
//   - fntcol16.zip (c) by Joseph Gil avalable at :
//      ftp://ftp.simtel.net/pub/simtelnet/msdos/screen/fntcol16.zip
//     These fonts are public domain
//
//  This VGA Bios is based on information taken from :
//   - Kevin Lawton's vga card emulation for bochs/plex86
//   - Ralf Brown's interrupts list available at http://www.cs.cmu.edu/afs/cs/user/ralf/pub/WWW/files.html
//   - Finn Thogersons' VGADOC4b available at http://home.worldonline.dk/~finth/
//   - Michael Abrash's Graphics Programming Black Book
//   - Francois Gervais' book "programmation des cartes graphiques cga-ega-vga" edited by sybex
//   - DOSEMU 1.0.1 source code for several tables values and formulas
//
// Thanks for patches, comments and ideas to :
//   - techt@pikeonline.net
//
// ============================================================================================

#include "vgabios.h"

#ifdef VBE
#include "vbe.h"
#define VGAEXT
#define VGAEXT_init            vbe_init
#define VGAEXT_display_info    vbe_display_info
#define VGAEXT_is_8bpp_mode    vbe_is_8bpp_mode
#define VGAEXT_8bpp_write_char vbe_8bpp_write_char
#define VGAEXT_8bpp_copy       vbe_8bpp_copy
#define VGAEXT_8bpp_fill       vbe_8bpp_fill
#endif
#ifdef CIRRUS
#define VGAEXT
#define VGAEXT_init            cirrus_init
#define VGAEXT_display_info    cirrus_display_info
#define VGAEXT_is_8bpp_mode    cirrus_is_8bpp_mode
#define VGAEXT_8bpp_write_char cirrus_bitblt_write_char
#define VGAEXT_8bpp_copy       cirrus_bitblt_copy
#define VGAEXT_8bpp_fill       cirrus_bitblt_fill
#endif
#ifdef BANSHEE
#define VGAEXT
#define VGAEXT_init            banshee_init
#define VGAEXT_display_info    banshee_display_info
#define VGAEXT_is_8bpp_mode    banshee_is_8bpp_mode
#define VGAEXT_8bpp_write_char banshee_8bpp_write_char
#define VGAEXT_8bpp_copy       banshee_8bpp_copy
#define VGAEXT_8bpp_fill       banshee_8bpp_fill
#endif

#define USE_BX_INFO

/* Declares */
static Bit8u          read_byte();
static Bit8u          read_bda_byte();
static Bit16u         read_word();
static Bit16u         read_bda_word();
static void           write_byte();
static void           write_bda_byte();
static void           write_word();
static void           write_bda_word();
static void           outb();
static void           outw();

static Bit16u         get_SS();

// Output
static void           printf();
static void           unimplemented();
static void           unknown();

static void set_cursor_pos();

static void memsetb();
static void memsetw();
static void memcpyb();
static void memcpyw();

static void biosfn_set_video_mode();
static void biosfn_set_active_page();
static void biosfn_scroll();
static void biosfn_read_char_attr();
static void biosfn_write_char_attr();
static void biosfn_write_char_only();
static void biosfn_write_pixel();
static void biosfn_read_pixel();
static void biosfn_write_teletype();
static void biosfn_load_text_user_pat();
static void biosfn_load_text_8_14_pat();
static void biosfn_load_text_8_8_pat();
static void biosfn_load_text_8_16_pat();
static void biosfn_write_string();
extern Bit8u video_save_pointer_table[];

// This is for compiling with gcc2 and gcc3
#define ASM_START #asm
#define ASM_END   #endasm

ASM_START

MACRO SET_INT_VECTOR
  push ds
  xor ax, ax
  mov ds, ax
  mov ax, ?3
  mov ?1*4, ax
  mov ax, ?2
  mov ?1*4+2, ax
  pop ds
MEND

ASM_END

ASM_START
.text
.rom

#ifdef BANSHEE
.org 0x7ff8
.byte 0x1a, 0x12, 0x04, 0x00
#endif

.org 0

use16 386

vgabios_start:
.byte 0x55, 0xaa /* BIOS signature, required for BIOS extensions */

.byte 0x40       /* BIOS extension length in units of 512 bytes */


vgabios_entry_point:

  jmp vgabios_init_func

#ifdef PCIBIOS
.org 0x18
.word vgabios_pci_data
#endif

// Info from Bart Oldeman
.org 0x1e
.ascii  "IBM"
.byte   0x00

vgabios_name:
.ascii	"Bochs VGABios"
#ifdef PCIBIOS
.ascii	" (PCI)"
#endif
.ascii	" "
.byte	0x00

vgabios_version:
#ifndef VGABIOS_VERS
.ascii	"github-current"
#else
.ascii VGABIOS_VERS
#endif
.ascii	" "

vgabios_date:
.ascii  VGABIOS_DATE
.byte   0x0a,0x0d
.byte	0x00

vgabios_copyright:
.ascii	"(C) 2002-2025 the LGPL VGABios developers Team"
.byte	0x0a,0x0d
.byte	0x00

vgabios_license:
.ascii	"This VGA/VBE Bios is released under the GNU LGPL"
.byte	0x0a,0x0d
.byte	0x0a,0x0d
.byte	0x00

vgabios_website:
.ascii	"Please visit :"
.byte	0x0a,0x0d
.ascii	" . https://bochs.sourceforge.io"
.byte	0x0a,0x0d
.ascii	" . https://www.nongnu.org/vgabios"
.byte	0x0a,0x0d
.byte	0x0a,0x0d
.byte	0x00

#ifdef PCIBIOS
.align 4 // DWORD alignment required by PCI Firmware Specification
vgabios_pci_data:
.ascii "PCIR"
#ifdef CIRRUS
.word 0x1013
.word 0x00b8 // CLGD5446
#elif defined(PCI_VID) && defined(PCI_DID)
.word PCI_VID
.word PCI_DID
#elif defined(VBE)
.word 0x1234
.word 0x1111 // Bochs VBE support
#elif defined(BANSHEE)
.word 0x121a
.word 0x0003 // Banshee PCI
#else
#error "Unknown PCI vendor and device id"
#endif
.word 0 // reserved
.word 0x18 // dlen
.byte 0 // revision
.byte 0x0 // class,hi: vga display
.word 0x300 // class,lo: vga display
.word 0x40 // bios size
.word 1 // revision
.byte 0 // intel x86 data
.byte 0x80 // last image
.word 0 // reserved
#endif


;; ============================================================================================
;;
;; Init Entry point
;;
;; ============================================================================================
vgabios_init_func:

;; init vga card
  call init_vga_card

;; init basic bios vars
  call init_bios_area

;; set int10 vect
  SET_INT_VECTOR(0x10, #0xC000, #vgabios_int10_handler)

#ifdef VGAEXT
;; init vga extension functions
  call VGAEXT_init
#endif

;; init video mode and clear the screen
  mov ax,#0x0003
  int #0x10

;; show info
  call display_info

#ifdef VGAEXT
;; show vga extension info
  call VGAEXT_display_info
#endif

  retf
ASM_END

/*
 *  int10 handled here
 */
ASM_START
vgabios_int10_handler:
  pushf
#ifdef DEBUG
  push es
  push ds
  pusha
  mov   bx, #0xc000
  mov   ds, bx
  call _int10_call_debugmsg
  popa
  pop ds
  pop es
#endif
  push  #int10_end
  or    ah, ah
  jnz   int10_test_01
  push  ds
  push  bx
  push  cx
  mov   bx, #0xc000
  mov   ds, bx
  push  ax
  call  _biosfn_set_video_mode
  pop   ax
  pop   cx
  pop   bx
  pop   ds
  cmp   al, #0x06
  jne   no_mode06
  mov   al, #0x3f
  ret
no_mode06:
  cmp   al, #0x07
  ja    no_cga_modes
  mov al, #0x30
  ret
no_cga_modes:
  mov al, #0x20
  ret
int10_test_01:
  cmp   ah, #0x01
  jne   int10_test_02
  jmp   biosfn_set_cursor_shape
int10_test_02:
  cmp   ah, #0x02
  jne   int10_test_03
  jmp   biosfn_set_cursor_pos
int10_test_03:
  cmp   ah, #0x03
  jne   int10_test_04
  jmp   biosfn_get_cursor_pos
int10_test_04:
  cmp   ah, #0x04
  jne   int10_test_0F
  jmp   biosfn_read_light_pen_pos
int10_test_0F:
  cmp   ah, #0x0f
  jne   int10_test_1A
  jmp   biosfn_get_video_mode
int10_test_1A:
  cmp   ah, #0x1a
  jne   int10_test_0B
  jmp   biosfn_group_1A
int10_test_0B:
  cmp   ah, #0x0b
  jne   int10_test_11
  jmp   biosfn_group_0B
int10_test_11:
  cmp   ah, #0x11
  jne   int10_test_12
  jmp   biosfn_group_11
int10_test_12:
  cmp   ah, #0x12
  jne   int10_test_10
  cmp   bl, #0x10
  jne   int10_test_BL20
  jmp   biosfn_get_ega_info
int10_test_BL20:
  cmp   bl, #0x20
  jne   int10_test_BL30
  jmp   biosfn_alternate_prtsc
int10_test_BL30:
  cmp   bl, #0x30
  jne   int10_test_BL31
  jmp   biosfn_select_vert_res
int10_test_BL31:
  cmp   bl, #0x31
  jne   int10_test_BL32
  jmp   biosfn_enable_default_palette_loading
int10_test_BL32:
  cmp   bl, #0x32
  jne   int10_test_BL33
  jmp   biosfn_enable_video_addressing
int10_test_BL33:
  cmp   bl, #0x33
  jne   int10_test_BL34
  jmp   biosfn_enable_grayscale_summing
int10_test_BL34:
  cmp   bl, #0x34
  jne   int10_test_BL35
  jmp   biosfn_enable_cursor_emulation
int10_test_BL35:
  cmp   bl, #0x35
  jne   int10_test_BL36
  jmp   biosfn_switch_video_interface
int10_test_BL36:
  cmp   bl, #0x36
  jne   int10_normal
  jmp   biosfn_enable_video_refresh_control
int10_test_10:
  cmp   ah, #0x10
  jne   int10_test_1B
  jmp   biosfn_group_10
int10_test_1B:
  cmp   ah, #0x1b
  jne   int10_test_1C
  jmp   biosfn_read_state_info
int10_test_1C:
  cmp   ah, #0x1c
  jne   int10_normal
  cmp   al, #0x02
  je    int10_1C02
  ja    int10_normal
  cmp   al, #0x01
  je    int10_1C01
  jmp   biosfn_read_video_state_size
int10_1C01:
  jmp   biosfn_save_video_state
int10_1C02:
  jmp   biosfn_restore_video_state
int10_normal:
  push es
  push ds
  pusha
  mov   bx, #0xc000
  mov   ds, bx
  call _int10_func
  popa
  pop ds
  pop es
  ret

int10_end:
#ifdef DEBUG
  push es
  push ds
  pusha
  mov   bx, #0xc000
  mov   ds, bx
  call _int10_ret_debugmsg
  popa
  pop ds
  pop es
#endif
  popf
  iret
ASM_END

#include "vgatables.h"
#include "vgafonts.h"

/*
 * Boot time harware inits
 */
ASM_START
init_vga_card:
;; switch to color mode and enable CPU access 480 lines
  mov dx, #0x3C2
  mov al, #0xC3
  outb dx,al

;; more than 64k 3C4/04
  mov dx, #0x3C4
  mov al, #0x04
  outb dx,al
  mov dx, #0x3C5
  mov al, #0x02
  outb dx,al

#if defined(USE_BX_INFO) || defined(DEBUG)
  mov  bx, #msg_vga_init
  push bx
  call _printf
  inc  sp
  inc  sp
#endif
  ret

#if defined(USE_BX_INFO) || defined(DEBUG)
msg_vga_init:
.ascii "VGABios ID: vgabios.c 2025-01-08"
.byte  0x0a,0x00
#endif
ASM_END

// --------------------------------------------------------------------------------------------
/*
 *  Boot time bios area inits
 */
ASM_START
init_bios_area:
  push  ds
  mov   ax, # BIOSMEM_SEG
  mov   ds, ax

;; init detected hardware BIOS Area
  mov   bx, # BIOSMEM_INITIAL_MODE
  mov   ax, [bx]
  and   ax, #0xffcf
;; set 80x25 color (not clear from RBIL but usual)
  or    ax, #0x0020
  mov   [bx], ax

;; Just for the first int10 find its children

;; the default char height
  mov   bx, # BIOSMEM_CHAR_HEIGHT
  mov   al, #0x10
  mov   [bx], al

;; Clear the screen
  mov   bx, # BIOSMEM_VIDEO_CTL
  mov   al, #0x60
  mov   [bx], al

;; Set the basic screen we have
  mov   bx, # BIOSMEM_SWITCHES
  mov   al, #0xf9
  mov   [bx], al

;; Set the basic modeset options
  mov   bx, # BIOSMEM_MODESET_CTL
  mov   al, #0x51
  mov   [bx], al

;; Set the  default MSR
  mov   bx, # BIOSMEM_CURRENT_MSR
  mov   al, #0x09
  mov   [bx], al

  pop ds
  ret

_video_save_pointer_table:
  .word _video_param_table
  .word 0xc000

  .word 0 /* XXX: fill it */
  .word 0

  .word 0 /* XXX: fill it */
  .word 0

  .word 0 /* XXX: fill it */
  .word 0

  .word 0 /* XXX: fill it */
  .word 0

  .word 0 /* XXX: fill it */
  .word 0

  .word 0 /* XXX: fill it */
  .word 0

ASM_END

// --------------------------------------------------------------------------------------------
/*
 *  Tell who we are
 */

ASM_START
display_info:
 mov ax,#0xc000
 mov ds,ax
 mov si,#vgabios_name
 call _display_string
 mov si,#vgabios_version
 call _display_string

 mov si,#vgabios_license
 call _display_string
 mov si,#vgabios_website
 call _display_string
 ret
ASM_END

static void display_string()
{
 // Get length of string
ASM_START
 mov ax,ds
 mov es,ax
 mov di,si
 xor cx,cx
 not cx
 xor al,al
 cld
 repne
  scasb
 not cx
 dec cx
 push cx

 mov ax,#0x0300
 mov bx,#0x0000
 int #0x10

 pop cx
 mov ax,#0x1301
 mov bx,#0x000b
 mov bp,si
 int #0x10
ASM_END
}

// --------------------------------------------------------------------------------------------
#if defined(DEBUG) || defined(CIRRUS_DEBUG) || defined(BANSHEE_DEBUG)
static void int10_call_debugmsg(DI, SI, BP, SP, BX, DX, CX, AX, DS, ES, FLAGS)
  Bit16u DI, SI, BP, SP, BX, DX, CX, AX, ES, DS, FLAGS;
{
  // 0E is write char...
  if (GET_AH() != 0x0E)
    printf("vgabios call ah%02x al%02x bx%04x cx%04x dx%04x\n",GET_AH(),GET_AL(),BX,CX,DX);
}

static void int10_ret_debugmsg(DI, SI, BP, SP, BX, DX, CX, AX, DS, ES, FLAGS)
  Bit16u DI, SI, BP, SP, BX, DX, CX, AX, ES, DS, FLAGS;
{
  // 0E is write char...
  if (GET_AH() != 0x0E)
    printf("vgabios ret  ah%02x al%02x bx%04x cx%04x dx%04x\n",GET_AH(),GET_AL(),BX,CX,DX);
}
#endif

// --------------------------------------------------------------------------------------------
/*
 * int10 main dispatcher
 */
static void int10_func(DI, SI, BP, SP, BX, DX, CX, AX, DS, ES, FLAGS)
  Bit16u DI, SI, BP, SP, BX, DX, CX, AX, ES, DS, FLAGS;
{

 // BIOS functions
 switch(GET_AH())
  {
   case 0x05:
     biosfn_set_active_page(GET_AL());
     break;
   case 0x06:
     biosfn_scroll(GET_AL(),GET_BH(),GET_CH(),GET_CL(),GET_DH(),GET_DL(),0xFF,SCROLL_UP);
     break;
   case 0x07:
     biosfn_scroll(GET_AL(),GET_BH(),GET_CH(),GET_CL(),GET_DH(),GET_DL(),0xFF,SCROLL_DOWN);
     break;
   case 0x08:
     biosfn_read_char_attr(GET_BH(),&AX);
     break;
   case 0x09:
     biosfn_write_char_attr(GET_AL(),GET_BH(),GET_BL(),CX);
     break;
   case 0x0A:
     biosfn_write_char_only(GET_AL(),GET_BH(),GET_BL(),CX);
     break;
   case 0x0C:
     biosfn_write_pixel(GET_BH(),GET_AL(),CX,DX);
     break;
   case 0x0D:
     biosfn_read_pixel(GET_BH(),CX,DX,&AX);
     break;
   case 0x0E:
     // Ralf Brown Interrupt list is WRONG on bh(page)
     // We do output only on the current page !
     biosfn_write_teletype(GET_AL(),0xff,GET_BL(),NO_ATTR);
     break;
   case 0x11:
     switch(GET_AL())
      {
       case 0x00:
       case 0x10:
        biosfn_load_text_user_pat(GET_AL(),ES,BP,CX,DX,GET_BL(),GET_BH());
        break;
       case 0x01:
       case 0x11:
        biosfn_load_text_8_14_pat(GET_AL(),GET_BL());
        break;
       case 0x02:
       case 0x12:
        biosfn_load_text_8_8_pat(GET_AL(),GET_BL());
        break;
       case 0x04:
       case 0x14:
        biosfn_load_text_8_16_pat(GET_AL(),GET_BL());
        break;
#ifdef DEBUG
       default:
        unknown();
#endif
      }

     break;
   case 0x13:
     biosfn_write_string(GET_AL(),GET_BH(),GET_BL(),CX,GET_DH(),GET_DL(),ES,BP);
     break;
#ifdef DEBUG
   default:
     unknown();
#endif
  }
}

ASM_START
dac_palette_table:
.word _palette0
.word _palette1
.word _palette2
.word _palette3

;; Load DAC palette
  ; in - al: palette number
load_dac_palette:
  push  ax
  push  bx
  push  cx
  push  dx
  push  si
  push  ds
  mov   bx, #0xc000
  mov   ds, bx
  xor   ah, ah
  push  ax
  shl   ax, #1
  mov   bx, #dac_palette_table
  add   bx, ax
  mov   si, [bx]
  pop   ax
  mov   bx, #_dac_regs
  add   bx, ax
  mov   al, [bx]
  mov   bl, #0x03
  mul   bl
  add   ax, #0x03
  mov   cx, ax
  mov   bx, #0x0300
  mov   al, #0x00
  mov   dx, #VGAREG_DAC_WRITE_ADDRESS
  out   dx, al
  cld
  mov   dx, #VGAREG_DAC_DATA
dac_load_loop:
  lodsb
  out   dx, al
  dec   bx
  loop  dac_load_loop
  or    bx, bx
  jz    no_fill_dac
  mov   cx, bx
  xor   al, al
dac_fill_loop:
  out   dx, al
  loop  dac_fill_loop
no_fill_dac:
  pop   ds
  pop   si
  pop   dx
  pop   cx
  pop   bx
  pop   ax
  ret

_vga_clear_vram_pl4:
  pusha
  push es
  mov  dx, #VGAREG_SEQU_ADDRESS
  mov  al, #0x02
  out  dx, al
  inc  dx
  in   al, dx
  push ax
  mov  al, #0x0f
  out  dx, al
  mov  cx, #0xa000
  mov  es, cx
  xor  di, di
  xor  ax, ax
  mov  cx, #0x8000
  cld
  rep
       stosw
  pop  ax
  out  dx, al
  pop  es
  popa
  ret
ASM_END

// ============================================================================================
//
// BIOS functions
//
// ============================================================================================

static void biosfn_set_video_mode(mode) Bit8u mode;
{// mode: Bit 7 is 1 if no clear screen

  // Should we clear the screen ?
  Bit8u noclearmem=mode&0x80;
  Bit8u line,mmask,*palette,vpti;
  Bit16u i,twidth,theightm1,cheight;
  Bit8u modeset_ctl,video_ctl,vga_switches;
  Boolean textmode;

  // The real mode
  mode=mode&0x7f;

  // find the entry in the video modes
  line=find_vga_entry(mode);

#ifdef DEBUG
  printf("mode search %02x found line %02x\n",mode,line);
#endif

  if(line==0xFF)
    return;

  vpti=vga_modes[line].vpti;
  twidth=video_param_table[vpti].twidth;
  theightm1=video_param_table[vpti].theightm1;
  cheight=video_param_table[vpti].cheight;
  textmode = (vga_modes[line].class == TEXT);

  // Read the bios vga control
  video_ctl=read_bda_byte(BIOSMEM_VIDEO_CTL);

  // Read the bios vga switches
  vga_switches=read_bda_byte(BIOSMEM_SWITCHES);

  // Read the bios mode set control
  modeset_ctl=read_bda_byte(BIOSMEM_MODESET_CTL);

  set_vga_mode_hw(modeset_ctl, &video_param_table[vpti]);

#ifdef BANSHEE
ASM_START
  call banshee_set_vga_mode
ASM_END
#endif

  // if palette loading (bit 3 of modeset ctl = 0)
  set_vga_mode_palette(modeset_ctl, vga_modes[line].dacmodel);

  if (noclearmem==0x00)
  {
    if (textmode)
    {
      memsetw(vga_modes[line].sstart,0,0x0720,0x4000); // 32k
    }
    else if (mode < 0x0d)
    {
      memsetw(vga_modes[line].sstart,0,0x0000,0x4000); // 32k
    }
    else
    {
      vga_clear_vram_pl4();
    }
  }

  // Set the BIOS mem
  write_bda_byte(BIOSMEM_CURRENT_MODE,mode);
  write_bda_word(BIOSMEM_NB_COLS,twidth);
  write_bda_word(BIOSMEM_PAGE_SIZE,*(Bit16u *)&video_param_table[vpti].slength_l);
  if (video_param_table[vpti].miscreg & 1) {
    write_bda_word(BIOSMEM_CRTC_ADDRESS, VGAREG_VGA_CRTC_ADDRESS);
  } else {
    write_bda_word(BIOSMEM_CRTC_ADDRESS, VGAREG_MDA_CRTC_ADDRESS);
  }
  write_bda_byte(BIOSMEM_NB_ROWS,theightm1);
  write_bda_word(BIOSMEM_CHAR_HEIGHT,cheight);
  write_bda_byte(BIOSMEM_VIDEO_CTL,(0x60|noclearmem));
  write_bda_byte(BIOSMEM_SWITCHES,0xF9);
  write_bda_byte(BIOSMEM_MODESET_CTL,read_bda_byte(BIOSMEM_MODESET_CTL)&0x7f);

  // FIXME We nearly have the good tables. to be reworked
  write_bda_byte(BIOSMEM_DCC_INDEX,0x08);    // 8 is VGA should be ok for now
  write_bda_word(BIOSMEM_VS_POINTER, video_save_pointer_table);
  write_bda_word(BIOSMEM_VS_POINTER+2, 0xc000);

  // FIXME
  write_bda_byte(BIOSMEM_CURRENT_MSR,0x00); // Unavailable on vanilla vga, but...
  write_bda_byte(BIOSMEM_CURRENT_PAL,0x00); // Unavailable on vanilla vga, but...

  // Set cursor shape
  if (textmode)
  {
ASM_START
    mov  cx, #0x0607
    call biosfn_set_cursor_shape
ASM_END
  }

  // Set cursor pos for page 0..7
ASM_START
  xor  dx, dx
  xor  bh, bh
set_next_cp:
  call biosfn_set_cursor_pos
  inc  bh
  cmp  bh, #0x08
  jb   set_next_cp
ASM_END

  // Set active page 0
  biosfn_set_active_page(0x00);

  if (textmode)
  {
ASM_START
    ;; copy and activate font
    push dx
    call get_crtc_address
    mov  al, #0x09
    out  dx, al
    inc  dx
    in   al, dx
    and  al, #0x1f
    mov  dl, al
    mov  al, #0x01 ;; 8x14 font
    cmp  dl, #13
    je   set_text_font
    mov  al, #0x02 ;; 8x8 font
    cmp  dl, #7
    je   set_text_font
    mov  al, #0x04 ;; 8x16 font
set_text_font:
    mov  ah, #0x11
    mov  bl, #0x00
    int  #0x10
    mov  bl, #0x00
    call biosfn_set_text_block_specifier
    pop  dx
ASM_END
  }

  // Set the ints 0x1F and 0x43
ASM_START
  push bp
  mov  bp, sp
  push bx
  push dx
  mov  dl, _biosfn_set_video_mode.cheight + 2[bp]
  mov  bx, #_vgafont8
  cmp  dl, #0x08
  je   set_fontptr
  mov  bx, #_vgafont14
  cmp  dl, #0x0e
  je   set_fontptr
  mov  bx, #_vgafont16
  cmp  dl, #0x10
  jne  no_cheight
set_fontptr:
  push ds
  xor  ax, ax
  mov  ds, ax
  mov  ax, #0xc000
  mov  0x010c, bx ;; INT 0x43
  mov  0x010e, ax
  mov  bx, #_vgafont8+128*8
  mov  0x007c, bx ;; INT 0x1F
  mov  0x007e, ax
  pop  ds
no_cheight:
  pop  dx
  pop  bx
  pop  bp
ASM_END
}

ASM_START
_set_vga_mode_hw:
  push bp
  mov  bp, sp
  push ax
  push bx
  push cx
  push dx
  push ds
  mov  ax, #0xc000
  mov  ds, ax
  mov  bx, 6[bp]
  push bx
  ;; Reset Attribute Ctrl flip-flop
  mov  dx, #VGAREG_ACTL_RESET
  in   al, dx
  ;; Set Attribute Ctrl
  mov  dx, #VGAREG_ACTL_ADDRESS
  mov  cx, #0x0014
  xor  al, al
  cld
set_actl_regs:
  out  dx, al
  push ax
  mov  al, 35[bx]
  out  dx, al
  pop  ax
  inc  al
  inc  bx
  loop set_actl_regs
  mov  al, #0x14
  out  dx, al
  xor  al, al
  out  dx, al
  pop  bx
  push bx
  ;; Set Sequencer Ctrl
  mov  dx, #VGAREG_SEQU_ADDRESS
  mov  ax, #0x0300
  out  dx, ax

  mov  ah, 5[bx]
  mov  al, #0x01
  test 4[bp], #0x10 ;; set if 400 lines mode
  jnz  normal_cwidth
  or   ah, #0x01
normal_cwidth:
  out  dx, ax
  inc  bx
  mov  cx, #0x0003
  mov  al, #0x02
  cld
set_sequ_regs:
  mov  ah, 5[bx]
  out  dx, ax
  inc  al
  inc  bx
  loop set_sequ_regs
  pop  bx
  push bx
  ;; Set Graphics Ctrl
  mov  dx, #VGAREG_GRDC_ADDRESS
  mov  cx, #0x0009
  xor  al, al
  cld
set_grdc_regs:
  mov  ah, 55[bx]
  out  dx, ax
  inc  al
  inc  bx
  loop set_grdc_regs
  pop  bx
  ;; Set the misc register
  mov  dx, #VGAREG_WRITE_MISC_OUTPUT
  mov  al, 9[bx]
  test 4[bp], #0x10 ;; set if 400 lines mode
  jnz  normal_clock
  and  al, #0xf3
normal_clock:
  out  dx, al
  test al, #0x01
  jz   crtc_mono
  mov  dx, #VGAREG_VGA_CRTC_ADDRESS
  jnz  crtc_color
crtc_mono:
  mov  dx, #VGAREG_MDA_CRTC_ADDRESS
crtc_color:
  ;; Disable CRTC write protection
  mov  ax, #0x0011
  out  dx, ax
  ;; Set CRTC regs
  mov  cx, #0x0019
  xor  al, al
  cld
set_crtc_regs:
  mov  ah, 10[bx]
  out  dx, ax
  inc  al
  inc  bx
  loop set_crtc_regs
  mov  cl, 4[bp]
  and  cl, #0x90
  cmp  cl, #0x10
  je   mode_400_lines
  mov  bx, #0x08
  cmp  al, #0x80
  je   set_ch_lines
  mov  ax, #0x015e
  mov  bx, #0x0e
  call stdvga_set_scanlines
set_ch_lines:
  push bx
  call _set_scan_lines
  inc  sp
  inc  sp
mode_400_lines:
  ;; Enable video
  mov  dx, #VGAREG_ACTL_ADDRESS
  mov  al, #0x20
  out  dx, al
  mov  dx, #VGAREG_ACTL_RESET
  in   al, dx
  pop  ds
  pop  dx
  pop  cx
  pop  bx
  pop  ax
  pop  bp
  ret

_set_vga_mode_palette:
  push bp
  mov  bp, sp
  push ax
  push bx
  push cx
  push dx
  push ds
  mov  ax, #0xc000
  mov  ds, ax
  mov  bl, 4[bp] ;; modeset_ctl
  test bl, #0x08
  jnz  no_palette_loading
  ;; Set the PEL mask
  mov  dx, #VGAREG_PEL_MASK
  mov  al, #0xff
  out  dx, al
  mov  al, 6[bp] ;; dacmodel
  call load_dac_palette
  test bl, #0x02
  jz   no_palette_loading
  xor  bx, bx
  mov  cx, #0x0100
  call biosfn_perform_gray_scale_summing
no_palette_loading:
  pop  ds
  pop  dx
  pop  cx
  pop  bx
  pop  ax
  pop  bp
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_set_cursor_shape:
  push  ds
  push  ax
  push  bx
  push  dx
  mov   ax, # BIOSMEM_SEG
  mov   ds, ax
  and   cx, #0x3f1f
  mov   bx, # BIOSMEM_CURSOR_TYPE
  mov   [bx], cx
  mov   bx, # BIOSMEM_MODESET_CTL
  mov   al, [bx]
  and   al, #0x01
  jz    no_resize
  mov   bx, # BIOSMEM_CHAR_HEIGHT
  mov   dl, [bx]
  cmp   dl, #0x08
  jb    no_resize
  cmp   cl, #0x08
  jnb   no_resize
  cmp   ch, #0x20
  jnb   no_resize
  mov   al, ch
  inc   al
  cmp   cl, al
  jz    line_cursor
  mul   al, dl
  shr   ax, #3
  jmp   setup_ch
line_cursor:
  mov   al, cl
  inc   al
  mul   al, dl
  shr   ax, #3
  dec   al
setup_ch:
  dec   al
  mov   ch, al
recalc_cl:
  mov   al, cl
  inc   al
  mul   al, dl
  shr   ax, #3
  dec   al
  mov   cl, al
no_resize:
  mov   bx, # BIOSMEM_CRTC_ADDRESS
  mov   dx, [bx]
  mov   al, #0x0a
  mov   ah, ch
  out   dx, ax
  inc   al
  mov   ah, cl
  out   dx, ax
  pop   dx
  pop   bx
  pop   ax
  pop   ds
  ret

; --------------------------------------------------------------------------------------------
biosfn_set_cursor_pos:
  push  ds
  push  ax
  push  dx
  cmp   bh, #0x07
  ja    invalid_page_1
  mov   ax, # BIOSMEM_SEG
  mov   ds, ax
  push  cx
  push  bx
  mov   cl, bh
  mov   al, bh
  xor   ah, ah
  mov   bx, # BIOSMEM_CURSOR_POS
  shl   ax, #1
  add   bx, ax
  mov   [bx], dx
  mov   bx, # BIOSMEM_CURRENT_PAGE
  cmp   cl, [bx]
  jnz   not_set_hw_cursor
  mov   bx, # BIOSMEM_CURRENT_START
  mov   ax, [bx]
  shr   ax, #1
  push  ax
  mov   bx, # BIOSMEM_NB_COLS
  mov   ah, [bx]
  mov   al, dh
  mul   al, ah
  xor   dh, dh
  add   ax, dx
  pop   bx
  add   ax, bx
  push  ax
  mov   bx, # BIOSMEM_CRTC_ADDRESS
  mov   dx, [bx]
  mov   al, #0x0e
  out   dx, ax
  pop   ax
  mov   ah, al
  mov   al, #0x0f
  out   dx, ax
not_set_hw_cursor:
  pop   bx
  pop   cx
invalid_page_1:
  pop   dx
  pop   ax
  pop   ds
  ret

; this function is called from C code
_set_cursor_pos:
  push bp
  mov  bp, sp
  mov  ax, 4[bp]
  mov  bh, al
  mov  dx, 6[bp]
  call biosfn_set_cursor_pos
  pop  bp
  ret

;--------------------------------------------------------------------------------------------
biosfn_get_cursor_pos:
  push  ds
  push  ax
  cmp   bh, #0x07
  ja    invalid_page_2
  mov   ax, # BIOSMEM_SEG
  mov   ds, ax
  xor   ax, ax
  mov   al, bh
  push  bx
  mov   bx, # BIOSMEM_CURSOR_TYPE
  mov   cx, [bx]
  mov   bx, # BIOSMEM_CURSOR_POS
  shl   ax, #1
  add   bx, ax
  mov   dx, [bx]
  pop   bx
invalid_page_2:
  pop   ax
  pop   ds
  ret

; this function is called from C code
_get_cursor_pos:
  push  bp
  mov   bp, sp
  push  ds
  mov   ax, # BIOSMEM_SEG
  mov   ds, ax
  mov   ax, 4[bp]
  cmp   al, #0x07
  jbe   page_ok
  xor   ax, ax
  jz    page_ret
page_ok:
  xor   ah, ah
  shl   ax, #1
  mov   bx, # BIOSMEM_CURSOR_POS
  add   bx, ax
  mov   ax, [bx]
page_ret:
  pop   ds
  pop   bp
  ret

;--------------------------------------------------------------------------------------------
biosfn_read_light_pen_pos:
;; unsupported on VGA
#ifdef DEBUG
  call _unimplemented
#endif
  xor   ax, ax
  xor   bx, bx
  xor   cx, cx
  xor   dx, dx
  ret
ASM_END

// --------------------------------------------------------------------------------------------
static void biosfn_set_active_page (page)
Bit8u page;
{
 Bit16u cursor,crtc_addr;
 Bit16u pgsize,address;
 Bit8u line;

 if(page>7)return;

 // Get the mode
 line=find_vga_entry(0xff); // current mode
 if(line==0xFF)return;

 // Get pos curs pos for the right page
 cursor=get_cursor_pos(page);

 if(vga_modes[line].class==TEXT)
  {
   // Get the page size
   pgsize=read_bda_word(BIOSMEM_PAGE_SIZE);

   // Calculate the mem address
   address=pgsize*page;
   write_bda_word(BIOSMEM_CURRENT_START,address);

   // Now the CRTC start address
   address>>=1;
  }
 else
  {
   address = page * (*(Bit16u *)&video_param_table[vga_modes[line].vpti].slength_l);
  }

 // CRTC regs 0x0c and 0x0d
 crtc_addr=read_bda_word(BIOSMEM_CRTC_ADDRESS);
 outb(crtc_addr,0x0c);
 outb(crtc_addr+1,(address&0xff00)>>8);
 outb(crtc_addr,0x0d);
 outb(crtc_addr+1,address&0x00ff);

 // And change the BIOS page
 write_bda_byte(BIOSMEM_CURRENT_PAGE,page);

#ifdef DEBUG
 printf("Set active page %02x address %04x\n",page,address);
#endif

 // Display the cursor, now the page is active
 set_cursor_pos(page,cursor);
}

// --------------------------------------------------------------------------------------------
static void vgamem_copy_pl4(xstart,ysrc,ydest,cols,nbcols,cheight)
Bit8u xstart;Bit8u ysrc;Bit8u ydest;Bit8u cols;Bit8u nbcols;Bit8u cheight;
{
 Bit16u src,dest;
 Bit8u i;

 src=ysrc*cheight*nbcols+xstart;
 dest=ydest*cheight*nbcols+xstart;
 outw(VGAREG_GRDC_ADDRESS, 0x0105);
 for(i=0;i<cheight;i++)
  {
   memcpyb(0xa000,dest+i*nbcols,0xa000,src+i*nbcols,cols);
  }
 outw(VGAREG_GRDC_ADDRESS, 0x0005);
}

// --------------------------------------------------------------------------------------------
static void vgamem_fill_pl4(xstart,ystart,cols,nbcols,cheight,attr)
Bit8u xstart;Bit8u ystart;Bit8u cols;Bit8u nbcols;Bit8u cheight;Bit8u attr;
{
 Bit16u dest;
 Bit8u i;

 dest=ystart*cheight*nbcols+xstart;
 outw(VGAREG_GRDC_ADDRESS, 0x0205);
 for(i=0;i<cheight;i++)
  {
   memsetb(0xa000,dest+i*nbcols,attr,cols);
  }
 outw(VGAREG_GRDC_ADDRESS, 0x0005);
}

// --------------------------------------------------------------------------------------------
static void vgamem_copy_cga(xstart,ysrc,ydest,cols,nbcols,cheight)
Bit8u xstart;Bit8u ysrc;Bit8u ydest;Bit8u cols;Bit8u nbcols;Bit8u cheight;
{
 Bit16u src,dest;
 Bit8u i;

 src=((ysrc*cheight*nbcols)>>1)+xstart;
 dest=((ydest*cheight*nbcols)>>1)+xstart;
 for(i=0;i<cheight;i++)
  {
   if (i & 1)
     memcpyb(0xb800,0x2000+dest+(i>>1)*nbcols,0xb800,0x2000+src+(i>>1)*nbcols,cols);
   else
     memcpyb(0xb800,dest+(i>>1)*nbcols,0xb800,src+(i>>1)*nbcols,cols);
  }
}

// --------------------------------------------------------------------------------------------
static void vgamem_fill_cga(xstart,ystart,cols,nbcols,cheight,attr)
Bit8u xstart;Bit8u ystart;Bit8u cols;Bit8u nbcols;Bit8u cheight;Bit8u attr;
{
 Bit16u dest;
 Bit8u i;

 dest=((ystart*cheight*nbcols)>>1)+xstart;
 for(i=0;i<cheight;i++)
  {
   if (i & 1)
     memsetb(0xb800,0x2000+dest+(i>>1)*nbcols,attr,cols);
   else
     memsetb(0xb800,dest+(i>>1)*nbcols,attr,cols);
  }
}

// --------------------------------------------------------------------------------------------
ASM_START
; arguments: xstart,ysrc,ydest,cols,nbcols,cheight
_vgamem_copy_lin:
  push bp
  mov  bp, sp
#ifdef VGAEXT
  call VGAEXT_is_8bpp_mode
  or   ax, ax
  jz   lin_copy_vga
  call VGAEXT_8bpp_copy
  pop  bp
  ret
lin_copy_vga:
#endif
  push ax
  push bx
  push cx
  push dx
  push ds
  push es
  push si
  push di
  mov  ax, 4[bp] ; xstart
  shl  ax, #3
  push ax
  mov  al, 12[bp] ; nbcols
  mov  bl, 14[bp] ; cheight
  mul  bl
  shl  ax, #3
  mov  cx, ax
  xor  ax, ax
  mov  dx, 6[bp] ; ysrc
  or   dx, dx
  jz   lin_copy_set_start1
  mov  ax, cx
  mov  bx, dx
  mul  bx
lin_copy_set_start1:
  pop  bx
  push bx
  add  ax, bx
  mov  si, ax
  xor  ax, ax
  mov  dx, 8[bp] ; ydest
  or   dx, dx
  jz   lin_copy_set_start2
  mov  ax, cx
  mov  bx, dx
  mul  bx
lin_copy_set_start2:
  pop  bx
  add  ax, bx
  mov  di, ax
  mov  ax, #0xa000
  mov  ds, ax
  mov  es, ax
  mov  bl, 14[bp] ; cheight
  mov  cx, 10[bp] ; cols
  shl  cx, #2
  mov  dx, 12[bp] ; nbcols
  shl  dx, #3
lin_copy_loop:
  push cx
  push si
  push di
  cld
  rep
    movsw
  pop  di
  pop  si
  pop  cx
  dec  bl
  jz   lin_copy_end
  add  si, dx
  add  di, dx
  jmp  lin_copy_loop
lin_copy_end:
  pop  di
  pop  si
  pop  es
  pop  ds
  pop  dx
  pop  cx
  pop  bx
  pop  ax
  pop  bp
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
; arguments: xstart,ystart,cols,rows,nbcols,cheight,attr
_vgamem_fill_lin:
  push bp
  mov  bp, sp
#ifdef VGAEXT
  call VGAEXT_is_8bpp_mode
  or   ax, ax
  jz   lin_fill_vga
  call VGAEXT_8bpp_fill
  pop  bp
  ret
lin_fill_vga:
#endif
  push ax
  push bx
  push cx
  push dx
  push es
  push di
  mov  ax, 4[bp] ; xstart
  shl  ax, #3
  push ax
  xor  ax, ax
  mov  dx, 6[bp] ; ystart
  or   dx, dx
  jz   lin_fill_set_start
  mov  al, 12[bp] ; nbcols
  mov  bl, 14[bp] ; cheight
  mul  bl
  shl  ax, #3
  mov  bx, dx
  mul  bx
lin_fill_set_start:
  pop  bx
  add  ax, bx
  mov  di, ax
  mov  ax, #0xa000
  mov  es, ax
  mov  bl, 14[bp] ; cheight
  mov  al, 10[bp] ; rows
  mul  bl
  mov  bx, ax
  mov  al, 16[bp] ; attr
  mov  ah, al
  mov  cx, 8[bp] ; cols
  shl  cx, #2
  mov  dx, 12[bp] ; nbcols
  shl  dx, #3
lin_fill_loop:
  push cx
  push di
  cld
  rep
    stosw
  pop  di
  pop  cx
  dec  bx
  jz   lin_fill_end
  add  di, dx
  jmp  lin_fill_loop
lin_fill_end:
  pop  di
  pop  es
  pop  dx
  pop  cx
  pop  bx
  pop  ax
  pop  bp
  ret
ASM_END

// --------------------------------------------------------------------------------------------
static void biosfn_scroll (nblines,attr,rul,cul,rlr,clr,page,dir)
Bit8u nblines;Bit8u attr;Bit8u rul;Bit8u cul;Bit8u rlr;Bit8u clr;Bit8u page;Bit8u dir;
{
 // page == 0xFF if current

 Bit8u line,cheight,bpp,cols;
 Bit16u nbcols,nbrows,i;
 Bit16u address,pgsize,chattr;

 if(rul>rlr)return;
 if(cul>clr)return;

 // Get the mode
 line=find_vga_entry(0xff); // current mode
 if(line==0xFF)return;

 // Get the dimensions
 nbrows=read_bda_byte(BIOSMEM_NB_ROWS)+1;
 nbcols=read_bda_word(BIOSMEM_NB_COLS);

 // Get the current page
 if(page==0xFF)
  page=read_bda_byte(BIOSMEM_CURRENT_PAGE);

 if(rlr>=nbrows)rlr=nbrows-1;
 if(clr>=nbcols)clr=nbcols-1;
 if(nblines>nbrows)nblines=0;
 cols=clr-cul+1;

 if(vga_modes[line].class==TEXT)
  {
   // Get the page size
   pgsize=read_bda_word(BIOSMEM_PAGE_SIZE);
   // Compute the address
   address=pgsize*page;
   chattr = ((Bit16u)attr << 8) + ' ';
#ifdef DEBUG
   printf("Scroll, address %04x (%04x %04x %02x)\n",address,nbrows,nbcols,page);
#endif

   if(nblines==0&&rul==0&&cul==0&&rlr==nbrows-1&&clr==nbcols-1)
    {
     memsetw(vga_modes[line].sstart,address,chattr,nbrows*nbcols);
    }
   else
    {// if Scroll up
     if(dir==SCROLL_UP)
      {for(i=rul;i<=rlr;i++)
        {
         if((i+nblines>rlr)||(nblines==0))
          memsetw(vga_modes[line].sstart,address+(i*nbcols+cul)*2,chattr,cols);
         else
          memcpyw(vga_modes[line].sstart,address+(i*nbcols+cul)*2,vga_modes[line].sstart,((i+nblines)*nbcols+cul)*2,cols);
        }
      }
     else
      {for(i=rlr;i>=rul;i--)
        {
         if((i<rul+nblines)||(nblines==0))
          memsetw(vga_modes[line].sstart,address+(i*nbcols+cul)*2,chattr,cols);
         else
          memcpyw(vga_modes[line].sstart,address+(i*nbcols+cul)*2,vga_modes[line].sstart,((i-nblines)*nbcols+cul)*2,cols);
         if (i>rlr) break;
        }
      }
    }
  }
 else
  {
   // FIXME gfx modes > 8 bpp not supported
   cheight=read_bda_byte(BIOSMEM_CHAR_HEIGHT);
   if(nblines==0&&rul==0&&cul==0&&rlr==nbrows-1&&clr==nbcols-1&&vga_modes[line].memmodel!=LINEAR8)
    {
     switch(vga_modes[line].memmodel)
      {
       case PLANAR4:
       case PLANAR1:
         outw(VGAREG_GRDC_ADDRESS, 0x0205);
         memsetb(vga_modes[line].sstart,0,attr,nbrows*nbcols*cheight);
         outw(VGAREG_GRDC_ADDRESS, 0x0005);
         break;
       case CGA:
         bpp=vga_modes[line].pixbits;
         memsetb(vga_modes[line].sstart,0,attr,nbrows*nbcols*cheight*bpp);
         break;
      }
    }
   else
    {
     switch(vga_modes[line].memmodel)
      {
       case PLANAR4:
       case PLANAR1:
         if(dir==SCROLL_UP)
          {for(i=rul;i<=rlr;i++)
            {
             if((i+nblines>rlr)||(nblines==0))
              vgamem_fill_pl4(cul,i,cols,nbcols,cheight,attr);
             else
              vgamem_copy_pl4(cul,i+nblines,i,cols,nbcols,cheight);
            }
          }
         else
          {for(i=rlr;i>=rul;i--)
            {
             if((i<rul+nblines)||(nblines==0))
              vgamem_fill_pl4(cul,i,cols,nbcols,cheight,attr);
             else
              vgamem_copy_pl4(cul,i-nblines,i,cols,nbcols,cheight);
            }
          }
         break;
       case CGA:
         bpp=vga_modes[line].pixbits;
         if(bpp==2)
          {
           cul<<=1;
           cols<<=1;
           nbcols<<=1;
          }
         // if Scroll up
         if(dir==SCROLL_UP)
          {for(i=rul;i<=rlr;i++)
            {
             if((i+nblines>rlr)||(nblines==0))
              vgamem_fill_cga(cul,i,cols,nbcols,cheight,attr);
             else
              vgamem_copy_cga(cul,i+nblines,i,cols,nbcols,cheight);
            }
          }
         else
          {for(i=rlr;i>=rul;i--)
            {
             if((i<rul+nblines)||(nblines==0))
              vgamem_fill_cga(cul,i,cols,nbcols,cheight,attr);
             else
              vgamem_copy_cga(cul,i-nblines,i,cols,nbcols,cheight);
            }
          }
         break;
       case LINEAR8:
         if(nblines==0)
          vgamem_fill_lin(cul,rul,cols,rlr-rul+1,nbcols,cheight,attr);
         else if(dir==SCROLL_UP)
          {for(i=rul;i<=rlr;i++)
            {
             if(i+nblines>rlr)
              vgamem_fill_lin(cul,i,cols,1,nbcols,cheight,attr);
             else
              vgamem_copy_lin(cul,i+nblines,i,cols,nbcols,cheight);
            }
          }
         else
          {for(i=rlr;i>=rul;i--)
            {
             if(i<rul+nblines)
              vgamem_fill_lin(cul,i,cols,1,nbcols,cheight,attr);
             else
              vgamem_copy_lin(cul,i-nblines,i,cols,nbcols,cheight);
            }
          }
         break;
#ifdef DEBUG
       default:
         printf("Scroll in graphics mode ");
         unimplemented();
#endif
      }
    }
  }
}

// --------------------------------------------------------------------------------------------
static void biosfn_read_char_attr (page,car)
Bit8u page;Bit16u *car;
{Bit16u ss=get_SS();
 Bit8u xcurs,ycurs,line;
 Bit16u nbcols,pgsize,address;
 Bit16u cursor;

 // Get the mode
 line=find_vga_entry(0xff); // current mode
 if(line==0xFF)return;

 // Get the cursor pos for the page
 cursor=get_cursor_pos(page);
 xcurs=cursor&0x00ff;ycurs=(cursor&0xff00)>>8;

 // Get the dimensions
 nbcols=read_bda_word(BIOSMEM_NB_COLS);

 if(vga_modes[line].class==TEXT)
  {
   // Get the page size
   pgsize=read_bda_word(BIOSMEM_PAGE_SIZE);
   // Compute the address
   address=pgsize*page+(xcurs+ycurs*nbcols)*2;

   write_word(ss,car,read_word(vga_modes[line].sstart,address));
  }
 else
  {
   // FIXME gfx mode
#ifdef DEBUG
   unimplemented();
#endif
  }
}

// --------------------------------------------------------------------------------------------
static void write_gfx_char_pl4(car,attr,xcurs,ycurs,nbcols,cheight)
Bit8u car;Bit8u attr;Bit8u xcurs;Bit8u ycurs;Bit8u nbcols;Bit8u cheight;
{
 Bit8u i,j,mask;
 Bit8u fdata;
 Bit16u fseg,foffs;
 Bit16u addr,dest,src;

 addr=xcurs+ycurs*cheight*nbcols;
 foffs = read_word(0x0, 0x43*4);
 fseg = read_word(0x0, 0x43*4+2);
 src = foffs + car * cheight;
ASM_START
  push bp
  mov  bp, sp
  push ax
  push bx
  push dx
  mov  dx, # VGAREG_SEQU_ADDRESS
  mov  ax, #0x0f02
  out  dx, ax
  mov  dx, # VGAREG_GRDC_ADDRESS
  mov  ax, #0x0205
  out  dx, ax
  mov  ax, #0x0003
  mov  bx, 22[bp] ; attr
  test bl, #0x80
  jz   no_xor
  mov  ah, #0x18
no_xor:
  out  dx, ax
  pop  dx
  pop  bx
  pop  ax
  pop  bp
ASM_END
 for(i=0;i<cheight;i++)
  {
   fdata = read_byte(fseg, src + i);
   dest=addr+i*nbcols;
   for(j=0;j<8;j++)
    {
     mask=0x80>>j;
     outw(VGAREG_GRDC_ADDRESS, (mask << 8) | 0x08);
     read_byte(0xa000,dest);
     if(fdata&mask)
      {
       write_byte(0xa000,dest,attr&0x0f);
      }
     else
      {
       write_byte(0xa000,dest,0x00);
      }
    }
  }
ASM_START
  mov dx, # VGAREG_GRDC_ADDRESS
  mov ax, #0xff08
  out dx, ax
  mov ax, #0x0005
  out dx, ax
  mov ax, #0x0003
  out dx, ax
ASM_END
}

// --------------------------------------------------------------------------------------------
static void write_gfx_char_cga(car,attr,xcurs,ycurs,nbcols,bpp)
Bit8u car;Bit8u attr;Bit8u xcurs;Bit8u ycurs;Bit8u nbcols;Bit8u bpp;
{
 Bit8u i,j,mask,data;
 Bit8u *fdata;
 Bit16u addr,dest,src;

 fdata = &vgafont8;
 addr=(xcurs*bpp)+ycurs*320;
 src = car * 8;
 for(i=0;i<8;i++)
  {
   dest=addr+(i>>1)*80;
   if (i & 1) dest += 0x2000;
   mask = 0x80;
   if (bpp == 1)
    {
     if (attr & 0x80)
      {
       data = read_byte(0xb800,dest);
      }
     else
      {
       data = 0x00;
      }
     for(j=0;j<8;j++)
      {
       if (fdata[src+i] & mask)
        {
         if (attr & 0x80)
          {
           data ^= (attr & 0x01) << (7-j);
          }
         else
          {
           data |= (attr & 0x01) << (7-j);
          }
        }
       mask >>= 1;
      }
     write_byte(0xb800,dest,data);
    }
   else
    {
     while (mask > 0)
      {
       if (attr & 0x80)
        {
         data = read_byte(0xb800,dest);
        }
       else
        {
         data = 0x00;
        }
       for(j=0;j<4;j++)
        {
         if (fdata[src+i] & mask)
          {
           if (attr & 0x80)
            {
             data ^= (attr & 0x03) << ((3-j)*2);
            }
           else
            {
             data |= (attr & 0x03) << ((3-j)*2);
            }
          }
         mask >>= 1;
        }
       write_byte(0xb800,dest,data);
       dest += 1;
      }
    }
  }
}

// --------------------------------------------------------------------------------------------
ASM_START
; arguments: car,attr,xcurs,ycurs,nbcols,cheight
_write_gfx_char_lin:
  push bp
  mov  bp, sp
#ifdef VGAEXT
  call VGAEXT_is_8bpp_mode
  or   ax, ax
  jz   lin_wc_vga
  call VGAEXT_8bpp_write_char
  pop  bp
  ret
lin_wc_vga:
#endif
  push ax
  push bx
  push cx
  push dx
  push ds
  push es
  push si
  push di
  xor  ax, ax
  mov  ds, ax
  mov  bx, #0x10c ; INT 0x43
  mov  si, [bx]
  mov  ax, [bx+2]
  mov  ds, ax
  mov  bx, 14[bp] ; cheight
  mov  al, 4[bp]  ; character
  mul  bl
  add  si, ax
  mov  ax, 8[bp] ; xcurs
  shl  ax, #3
  push ax
  xor  ax, ax
  mov  dx, 10[bp] ; ycurs
  or   dx, dx
  jz   lin_wc_set_start
  mov  al, 12[bp] ; nbcols
  mov  bl, 14[bp] ; cheight
  mul  bl
  shl  ax, #3
  mov  bx, dx
  mul  bx
lin_wc_set_start:
  pop  bx
  add  ax, bx
  mov  di, ax
  mov  ax, #0xa000
  mov  es, ax
  mov  bl, 14[bp] ; cheight
  mov  dx, 12[bp] ; nbcols
  shl  dx, #3
lin_wc_loop1:
  push di
  lodsb
  mov  cx, #0x08
  cld
lin_wc_loop2:
  shl  al, #1
  push ax
  jnc  lin_wc_set_bkgnd
  mov  al, 6[bp] ; attr
  db    0xa9 ; skip next opcode (TEST AX, #0xC030)
lin_wc_set_bkgnd:
  xor  al, al
  stosb
  pop  ax
  loop lin_wc_loop2
  pop  di
  dec  bl
  jz   lin_wc_end
  add  di, dx
  jmp  lin_wc_loop1
lin_wc_end:
  pop  di
  pop  si
  pop  es
  pop  ds
  pop  dx
  pop  cx
  pop  bx
  pop  ax
  pop  bp
  ret
ASM_END

// --------------------------------------------------------------------------------------------
static void biosfn_write_char_attr (car,page,attr,count)
Bit8u car;Bit8u page;Bit8u attr;Bit16u count;
{
 Bit8u cheight,xcurs,ycurs,line,bpp;
 Bit16u nbcols,pgsize,address;
 Bit16u cursor,carattr;

 // Get the mode
 line=find_vga_entry(0xff); // current mode
 if(line==0xFF)return;

 // Get the cursor pos for the page
 cursor=get_cursor_pos(page);
 xcurs=cursor&0x00ff;ycurs=(cursor&0xff00)>>8;

 // Get the dimensions
 nbcols=read_bda_word(BIOSMEM_NB_COLS);

 if(vga_modes[line].class==TEXT)
  {
   // Get the page size
   pgsize=read_bda_word(BIOSMEM_PAGE_SIZE);
   // Compute the address
   address=pgsize*page+(xcurs+ycurs*nbcols)*2;

   carattr=((Bit16u)attr<<8)+car;
   memsetw(vga_modes[line].sstart,address,carattr,count);
  }
 else
  {
   // FIXME gfx modes > 8 bpp not supported
   cheight=read_bda_byte(BIOSMEM_CHAR_HEIGHT);
   bpp=vga_modes[line].pixbits;
   while((count-->0) && (xcurs<nbcols))
    {
     switch(vga_modes[line].memmodel)
      {
       case PLANAR4:
       case PLANAR1:
         write_gfx_char_pl4(car,attr,xcurs,ycurs,nbcols,cheight);
         break;
       case CGA:
         write_gfx_char_cga(car,attr,xcurs,ycurs,nbcols,bpp);
         break;
       case LINEAR8:
         write_gfx_char_lin(car,attr,xcurs,ycurs,nbcols,cheight);
         break;
#ifdef DEBUG
       default:
         unimplemented();
#endif
      }
     xcurs++;
    }
  }
}

// --------------------------------------------------------------------------------------------
static void biosfn_write_char_only (car,page,attr,count)
Bit8u car;Bit8u page;Bit8u attr;Bit16u count;
{
 Bit8u cheight,xcurs,ycurs,line,bpp;
 Bit16u nbcols,pgsize,address;
 Bit16u cursor;

 // Get the mode
 line=find_vga_entry(0xff); // current mode
 if(line==0xFF)return;

 // Get the cursor pos for the page
 cursor=get_cursor_pos(page);
 xcurs=cursor&0x00ff;ycurs=(cursor&0xff00)>>8;

 // Get the dimensions
 nbcols=read_bda_word(BIOSMEM_NB_COLS);

 if(vga_modes[line].class==TEXT)
  {
   // Get the page size
   pgsize=read_bda_word(BIOSMEM_PAGE_SIZE);
   // Compute the address
   address=pgsize*page+(xcurs+ycurs*nbcols)*2;

   while(count-->0)
    {write_byte(vga_modes[line].sstart,address,car);
     address+=2;
    }
  }
 else
  {
   // FIXME gfx modes > 8 bpp not supported
   cheight=read_bda_byte(BIOSMEM_CHAR_HEIGHT);
   bpp=vga_modes[line].pixbits;
   while((count-->0) && (xcurs<nbcols))
    {
     switch(vga_modes[line].memmodel)
      {
       case PLANAR4:
       case PLANAR1:
         write_gfx_char_pl4(car,attr,xcurs,ycurs,nbcols,cheight);
         break;
       case CGA:
         write_gfx_char_cga(car,attr,xcurs,ycurs,nbcols,bpp);
         break;
       case LINEAR8:
         write_gfx_char_lin(car,attr,xcurs,ycurs,nbcols,cheight);
         break;
#ifdef DEBUG
       default:
         unimplemented();
#endif
      }
     xcurs++;
    }
  }
}

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_group_0B:
  cmp   bh, #0x00
  je    biosfn_set_border_color
  cmp   bh, #0x01
  je    biosfn_set_palette
#ifdef DEBUG
  call  _unknown
#endif
  ret
biosfn_set_border_color:
  push  ax
  push  bx
  push  cx
  push  dx
  mov   dx, # VGAREG_ACTL_RESET
  in    al, dx
  mov   dx, # VGAREG_ACTL_ADDRESS
  mov   al, #0x00
  out   dx, al
  mov   al, bl
  and   al, #0x0f
  test  al, #0x08
  jz    set_low_border
  add   al, #0x08
set_low_border:
  out   dx, al
  mov   cl, #0x01
  and   bl, #0x10
set_intensity_loop:
  mov   dx, # VGAREG_ACTL_ADDRESS
  mov   al, cl
  out   dx, al
  mov   dx, # VGAREG_ACTL_READ_DATA
  in    al, dx
  and   al, #0xef
  or    al, bl
  mov   dx, # VGAREG_ACTL_ADDRESS
  out   dx, al
  inc   cl
  cmp   cl, #0x04
  jne   set_intensity_loop
  mov   al, #0x20
  out   dx, al
  pop   dx
  pop   cx
  pop   bx
  pop   ax
  ret
biosfn_set_palette:
  push  ax
  push  bx
  push  cx
  push  dx
  mov   dx, # VGAREG_ACTL_RESET
  in    al, dx
  mov   cl, #0x01
  and   bl, #0x01
set_cga_palette_loop:
  mov   dx, # VGAREG_ACTL_ADDRESS
  mov   al, cl
  out   dx, al
  mov   dx, # VGAREG_ACTL_READ_DATA
  in    al, dx
  and   al, #0xfe
  or    al, bl
  mov   dx, # VGAREG_ACTL_ADDRESS
  out   dx, al
  inc   cl
  cmp   cl, #0x04
  jne   set_cga_palette_loop
  mov   al, #0x20
  out   dx, al
  pop   dx
  pop   cx
  pop   bx
  pop   ax
  ret
ASM_END

// --------------------------------------------------------------------------------------------
static void biosfn_write_pixel (BH,AL,CX,DX) Bit8u BH;Bit8u AL;Bit16u CX;Bit16u DX;
{
 Bit8u line,mask,attr,data;
 Bit16u addr;

 // Get the mode
 line=find_vga_entry(0xff); // current mode
 if(line==0xFF)return;
 if(vga_modes[line].class==TEXT)return;

 switch(vga_modes[line].memmodel)
  {
   case PLANAR4:
   case PLANAR1:
     addr = CX/8+DX*read_bda_word(BIOSMEM_NB_COLS);
     mask = 0x80 >> (CX & 0x07);
     outw(VGAREG_GRDC_ADDRESS, (mask << 8) | 0x08);
     outw(VGAREG_GRDC_ADDRESS, 0x0205);
     data = read_byte(0xa000,addr);
     if (AL & 0x80)
      {
       outw(VGAREG_GRDC_ADDRESS, 0x1803);
      }
     write_byte(0xa000,addr,AL);
ASM_START
     mov dx, # VGAREG_GRDC_ADDRESS
     mov ax, #0xff08
     out dx, ax
     mov ax, #0x0005
     out dx, ax
     mov ax, #0x0003
     out dx, ax
ASM_END
     break;
   case CGA:
     if(vga_modes[line].pixbits==2)
      {
       addr=(CX>>2)+(DX>>1)*80;
      }
     else
      {
       addr=(CX>>3)+(DX>>1)*80;
      }
     if (DX & 1) addr += 0x2000;
     data = read_byte(0xb800,addr);
     if(vga_modes[line].pixbits==2)
      {
       attr = (AL & 0x03) << ((3 - (CX & 0x03)) * 2);
       mask = 0x03 << ((3 - (CX & 0x03)) * 2);
      }
     else
      {
       attr = (AL & 0x01) << (7 - (CX & 0x07));
       mask = 0x01 << (7 - (CX & 0x07));
      }
     if (AL & 0x80)
      {
       data ^= attr;
      }
     else
      {
       data &= ~mask;
       data |= attr;
      }
     write_byte(0xb800,addr,data);
     break;
   case LINEAR8:
     addr=CX+DX*(read_bda_word(BIOSMEM_NB_COLS)*8);
     write_byte(0xa000,addr,AL);
     break;
#ifdef DEBUG
   default:
     unimplemented();
#endif
  }
}

// --------------------------------------------------------------------------------------------
static void biosfn_read_pixel (BH,CX,DX,AX) Bit8u BH;Bit16u CX;Bit16u DX;Bit16u *AX;
{
 Bit8u line,mask,attr,data,i;
 Bit16u addr;
 Bit16u ss=get_SS();

 // Get the mode
 line=find_vga_entry(0xff); // current mode
 if(line==0xFF)return;
 if(vga_modes[line].class==TEXT)return;

 switch(vga_modes[line].memmodel)
  {
   case PLANAR4:
   case PLANAR1:
     addr = CX/8+DX*read_bda_word(BIOSMEM_NB_COLS);
     mask = 0x80 >> (CX & 0x07);
     attr = 0x00;
     for(i=0;i<4;i++)
      {
       outw(VGAREG_GRDC_ADDRESS, (i << 8) | 0x04);
       data = read_byte(0xa000,addr) & mask;
       if (data > 0) attr |= (0x01 << i);
      }
     break;
   case CGA:
     addr=(CX>>2)+(DX>>1)*80;
     if (DX & 1) addr += 0x2000;
     data = read_byte(0xb800,addr);
     if(vga_modes[line].pixbits==2)
      {
       attr = (data >> ((3 - (CX & 0x03)) * 2)) & 0x03;
      }
     else
      {
       attr = (data >> (7 - (CX & 0x07))) & 0x01;
      }
     break;
   case LINEAR8:
     addr=CX+DX*(read_bda_word(BIOSMEM_NB_COLS)*8);
     attr=read_byte(0xa000,addr);
     break;
   default:
#ifdef DEBUG
     unimplemented();
#endif
     attr = 0;
  }
 write_word(ss,AX,(read_word(ss,AX) & 0xff00) | attr);
}

// --------------------------------------------------------------------------------------------
static void biosfn_write_teletype (car, page, attr, flag)
Bit8u car;Bit8u page;Bit8u attr;Bit8u flag;
{// flag = WITH_ATTR / NO_ATTR

 Bit8u cheight,xcurs,ycurs,line,bpp;
 Bit16u nbcols,nbrows,pgsize,address;
 Bit16u cursor;

 // special case if page is 0xff, use current page
 if(page==0xff)
  page=read_bda_byte(BIOSMEM_CURRENT_PAGE);

 // Get the mode
 line=find_vga_entry(0xff); // current mode
 if(line==0xFF)return;

 // Get the cursor pos for the page
 cursor=get_cursor_pos(page);
 xcurs = cursor & 0x00ff; ycurs = (Bit8u)(cursor >> 8);

 // Get the dimensions
 nbrows=read_bda_byte(BIOSMEM_NB_ROWS)+1;
 nbcols=read_bda_word(BIOSMEM_NB_COLS);

 switch(car)
  {
   case 7:
    //FIXME should beep
    break;

   case 8:
    if(xcurs>0)xcurs--;
    break;

   case '\r':
    xcurs=0;
    break;

   case '\n':
    ycurs++;
    break;

   case '\t':
    do {
      biosfn_write_teletype(' ',page,attr,flag);
      cursor = get_cursor_pos(page);
      xcurs = cursor & 0x00ff; ycurs = (Bit8u)(cursor >> 8);
     } while ((xcurs % 8) == 0);
    break;

   default:

    if(vga_modes[line].class==TEXT)
     {
      // Get the page size
      pgsize=read_bda_word(BIOSMEM_PAGE_SIZE);
      // Compute the address
      address=pgsize*page+(xcurs+ycurs*nbcols)*2;

      // Write the char
      write_byte(vga_modes[line].sstart,address,car);

      if(flag==WITH_ATTR)
       write_byte(vga_modes[line].sstart,address+1,attr);
     }
    else
     {
      // FIXME gfx modes > 8 bpp not supported
      cheight=read_bda_byte(BIOSMEM_CHAR_HEIGHT);
      bpp=vga_modes[line].pixbits;
      switch(vga_modes[line].memmodel)
       {
        case PLANAR4:
        case PLANAR1:
          write_gfx_char_pl4(car,attr,xcurs,ycurs,nbcols,cheight);
          break;
        case CGA:
          write_gfx_char_cga(car,attr,xcurs,ycurs,nbcols,bpp);
          break;
        case LINEAR8:
          write_gfx_char_lin(car,attr,xcurs,ycurs,nbcols,cheight);
          break;
#ifdef DEBUG
        default:
          unimplemented();
#endif
       }
     }
    xcurs++;
  }

 // Do we need to wrap ?
 if(xcurs==nbcols)
  {xcurs=0;
   ycurs++;
  }

 // Do we need to scroll ?
 if(ycurs==nbrows)
  {
   if(vga_modes[line].class==TEXT)
    {
     address=pgsize*page+(xcurs+(ycurs-1)*nbcols)*2;
     attr=read_byte(vga_modes[line].sstart,address+1);
     biosfn_scroll(0x01,attr,0,0,nbrows-1,nbcols-1,page,SCROLL_UP);
    }
   else
    {
     biosfn_scroll(0x01,0x00,0,0,nbrows-1,nbcols-1,page,SCROLL_UP);
    }
   ycurs-=1;
  }

 // Set the cursor for the page
 cursor = ((Bit16u)ycurs << 8) | xcurs;
 set_cursor_pos(page,cursor);
}

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_get_video_mode:
  push  ds
  mov   ax, # BIOSMEM_SEG
  mov   ds, ax
  push  bx
  mov   bx, # BIOSMEM_CURRENT_PAGE
  mov   al, [bx]
  pop   bx
  mov   bh, al
  push  bx
  mov   bx, # BIOSMEM_VIDEO_CTL
  mov   ah, [bx]
  and   ah, #0x80
  mov   bx, # BIOSMEM_CURRENT_MODE
  mov   al, [bx]
  or    al, ah
  mov   bx, # BIOSMEM_NB_COLS
  mov   ah, [bx]
  pop   bx
  pop   ds
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_group_10:
  cmp   al, #0x00
  jne   int10_test_1001
  jmp   biosfn_set_single_palette_reg
int10_test_1001:
  cmp   al, #0x01
  jne   int10_test_1002
  jmp   biosfn_set_overscan_border_color
int10_test_1002:
  cmp   al, #0x02
  jne   int10_test_1003
  jmp   biosfn_set_all_palette_reg
int10_test_1003:
  cmp   al, #0x03
  jne   int10_test_1007
  jmp   biosfn_toggle_intensity
int10_test_1007:
  cmp   al, #0x07
  jne   int10_test_1008
  jmp   biosfn_get_single_palette_reg
int10_test_1008:
  cmp   al, #0x08
  jne   int10_test_1009
  jmp   biosfn_read_overscan_border_color
int10_test_1009:
  cmp   al, #0x09
  jne   int10_test_1010
  jmp   biosfn_get_all_palette_reg
int10_test_1010:
  cmp   al, #0x10
  jne   int10_test_1012
  jmp  biosfn_set_single_dac_reg
int10_test_1012:
  cmp   al, #0x12
  jne   int10_test_1013
  jmp   biosfn_set_all_dac_reg
int10_test_1013:
  cmp   al, #0x13
  jne   int10_test_1015
  jmp   biosfn_select_video_dac_color_page
int10_test_1015:
  cmp   al, #0x15
  jne   int10_test_1017
  jmp   biosfn_read_single_dac_reg
int10_test_1017:
  cmp   al, #0x17
  jne   int10_test_1018
  jmp   biosfn_read_all_dac_reg
int10_test_1018:
  cmp   al, #0x18
  jne   int10_test_1019
  jmp   biosfn_set_pel_mask
int10_test_1019:
  cmp   al, #0x19
  jne   int10_test_101A
  jmp   biosfn_read_pel_mask
int10_test_101A:
  cmp   al, #0x1a
  jne   int10_test_101B
  jmp   biosfn_read_video_dac_state
int10_test_101B:
  cmp   al, #0x1b
  jne   int10_group_10_unknown
  jmp   biosfn_perform_gray_scale_summing
int10_group_10_unknown:
#ifdef DEBUG
  call  _unknown
#endif
  ret

biosfn_set_single_palette_reg:
  cmp   bl, #0x14
  ja    no_actl_reg1
  push  ax
  push  dx
  mov   dx, # VGAREG_ACTL_RESET
  in    al, dx
  mov   dx, # VGAREG_ACTL_ADDRESS
  in    al, dx
  and   al, #0x20
  or    al, bl
  out   dx, al
  mov   al, bh
  out   dx, al
  mov   al, #0x20
  out   dx, al
  pop   dx
  pop   ax
no_actl_reg1:
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_set_overscan_border_color:
  push  bx
  mov   bl, #0x11
  call  biosfn_set_single_palette_reg
  pop   bx
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_set_all_palette_reg:
  push  ax
  push  bx
  push  cx
  push  dx
  mov   bx, dx
  mov   dx, # VGAREG_ACTL_RESET
  in    al, dx
  mov   cl, #0x00
  mov   dx, # VGAREG_ACTL_ADDRESS
  in    al, dx
  and   al, #0x20
  mov   ch, al
set_palette_loop:
  mov   al, cl
  or    al, ch
  out   dx, al
  seg   es
  mov   al, [bx]
  out   dx, al
  inc   bx
  inc   cl
  cmp   cl, #0x10
  jne   set_palette_loop
  mov   al, #0x11
  or    al, ch
  out   dx, al
  seg   es
  mov   al, [bx]
  out   dx, al
  mov   al, #0x20
  out   dx, al
  pop   dx
  pop   cx
  pop   bx
  pop   ax
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_toggle_intensity:
  push  ax
  push  bx
  push  dx
  mov   dx, # VGAREG_ACTL_RESET
  in    al, dx
  mov   dx, # VGAREG_ACTL_ADDRESS
  in    al, dx
  and   al, #0x20
  or    al, #0x10
  out   dx, al
  mov   dx, # VGAREG_ACTL_READ_DATA
  in    al, dx
  and   al, #0xf7
  and   bl, #0x01
  shl   bl, 3
  or    al, bl
  mov   dx, # VGAREG_ACTL_ADDRESS
  out   dx, al
  mov   al, #0x20
  out   dx, al
  pop   dx
  pop   bx
  pop   ax
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_get_single_palette_reg:
  cmp   bl, #0x14
  ja    no_actl_reg2
  push  ax
  push  dx
  mov   dx, # VGAREG_ACTL_RESET
  in    al, dx
  mov   dx, # VGAREG_ACTL_ADDRESS
  in    al, dx
  and   al, #0x20
  or    al, bl
  out   dx, al
  mov   dx, # VGAREG_ACTL_READ_DATA
  in    al, dx
  mov   bh, al
  mov   dx, # VGAREG_ACTL_RESET
  in    al, dx
  mov   dx, # VGAREG_ACTL_ADDRESS
  mov   al, #0x20
  out   dx, al
  pop   dx
  pop   ax
no_actl_reg2:
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_read_overscan_border_color:
  push  ax
  push  bx
  mov   bl, #0x11
  call  biosfn_get_single_palette_reg
  mov   al, bh
  pop   bx
  mov   bh, al
  pop   ax
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_get_all_palette_reg:
  push  ax
  push  bx
  push  cx
  push  dx
  mov   bx, dx
  mov   cl, #0x00
get_palette_loop:
  mov   dx, # VGAREG_ACTL_RESET
  in    al, dx
  mov   dx, # VGAREG_ACTL_ADDRESS
  in    al, dx
  and   al, #0x20
  or    al, cl
  out   dx, al
  mov   dx, # VGAREG_ACTL_READ_DATA
  in    al, dx
  seg   es
  mov   [bx], al
  inc   bx
  inc   cl
  cmp   cl, #0x10
  jne   get_palette_loop
  mov   dx, # VGAREG_ACTL_RESET
  in    al, dx
  mov   dx, # VGAREG_ACTL_ADDRESS
  in    al, dx
  and   al, #0x20
  or    al, #0x11
  out   dx, al
  mov   dx, # VGAREG_ACTL_READ_DATA
  in    al, dx
  seg   es
  mov   [bx], al
  mov   dx, # VGAREG_ACTL_RESET
  in    al, dx
  mov   dx, # VGAREG_ACTL_ADDRESS
  mov   al, #0x20
  out   dx, al
  pop   dx
  pop   cx
  pop   bx
  pop   ax
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_set_single_dac_reg:
  push  ax
  push  dx
  mov   ah, dh
  mov   dx, # VGAREG_DAC_WRITE_ADDRESS
  mov   al, bl
  out   dx, al
  mov   dx, # VGAREG_DAC_DATA
  mov   al, ah
  out   dx, al
  mov   al, ch
  out   dx, al
  mov   al, cl
  out   dx, al
  pop   dx
  pop   ax
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_set_all_dac_reg:
  push  ax
  push  cx
  push  dx
  push  si
  push  ds
  mov   si, dx
  mov   ax, es
  mov   ds, ax
  mov   dx, # VGAREG_DAC_WRITE_ADDRESS
  mov   al, bl
  out   dx, al
  mov   dx, # VGAREG_DAC_DATA
  cld
set_dac_loop:
  lodsb
  out   dx, al
  lodsb
  out   dx, al
  lodsb
  out   dx, al
  loop  set_dac_loop
  pop   ds
  pop   si
  pop   dx
  pop   cx
  pop   ax
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_select_video_dac_color_page:
  push  ax
  push  bx
  push  dx
  mov   dx, # VGAREG_ACTL_RESET
  in    al, dx
  mov   dx, # VGAREG_ACTL_ADDRESS
  mov   al, #0x10
  out   dx, al
  mov   dx, # VGAREG_ACTL_READ_DATA
  in    al, dx
  and   bl, #0x01
  jnz   set_dac_page
  and   al, #0x7f
  shl   bh, 7
  or    al, bh
  mov   dx, # VGAREG_ACTL_ADDRESS
  out   dx, al
  jmp   set_actl_normal
set_dac_page:
  push  ax
  mov   dx, # VGAREG_ACTL_RESET
  in    al, dx
  mov   dx, # VGAREG_ACTL_ADDRESS
  mov   al, #0x14
  out   dx, al
  pop   ax
  and   al, #0x80
  jnz   set_dac_16_page
  shl   bh, 2
set_dac_16_page:
  and   bh, #0x0f
  mov   al, bh
  out   dx, al
set_actl_normal:
  mov   al, #0x20
  out   dx, al
  pop   dx
  pop   bx
  pop   ax
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_read_single_dac_reg:
  push  ax
  push  dx
  mov   dx, # VGAREG_DAC_READ_ADDRESS
  mov   al, bl
  out   dx, al
  mov   dx, # VGAREG_DAC_DATA
  in    al, dx
  mov   ah, al
  in    al, dx
  mov   ch, al
  in    al, dx
  mov   cl, al
  pop   dx
  mov   dh, ah
  pop   ax
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_read_all_dac_reg:
  push  ax
  push  cx
  push  dx
  push  di
  mov   di, dx
  mov   dx, # VGAREG_DAC_READ_ADDRESS
  mov   al, bl
  out   dx, al
  mov   dx, # VGAREG_DAC_DATA
  cld
read_dac_loop:
  in    al, dx
  stosb
  in    al, dx
  stosb
  in    al, dx
  stosb
  loop  read_dac_loop
  pop   di
  pop   dx
  pop   cx
  pop   ax
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_set_pel_mask:
  push  ax
  push  dx
  mov   dx, # VGAREG_PEL_MASK
  mov   al, bl
  out   dx, al
  pop   dx
  pop   ax
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_read_pel_mask:
  push  ax
  push  dx
  mov   dx, # VGAREG_PEL_MASK
  in    al, dx
  mov   bl, al
  pop   dx
  pop   ax
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_read_video_dac_state:
  push  ax
  push  dx
  mov   dx, # VGAREG_ACTL_RESET
  in    al, dx
  mov   dx, # VGAREG_ACTL_ADDRESS
  mov   al, #0x10
  out   dx, al
  mov   dx, # VGAREG_ACTL_READ_DATA
  in    al, dx
  mov   bl, al
  shr   bl, 7
  mov   dx, # VGAREG_ACTL_RESET
  in    al, dx
  mov   dx, # VGAREG_ACTL_ADDRESS
  mov   al, #0x14
  out   dx, al
  mov   dx, # VGAREG_ACTL_READ_DATA
  in    al, dx
  mov   bh, al
  and   bh, #0x0f
  test  bl, #0x01
  jnz   get_dac_16_page
  shr   bh, 2
get_dac_16_page:
  mov   dx, # VGAREG_ACTL_RESET
  in    al, dx
  mov   dx, # VGAREG_ACTL_ADDRESS
  mov   al, #0x20
  out   dx, al
  pop   dx
  pop   ax
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_perform_gray_scale_summing:
  push  ax
  push  bx
  push  cx
  push  dx
  mov   dx, #VGAREG_ACTL_RESET
  in    al, dx
  mov   dx, #VGAREG_ACTL_ADDRESS
  mov   al, #0x00
  out   dx, al
dac_gss_loop:
  mov   al, bl
  mov   dx, #VGAREG_DAC_READ_ADDRESS
  push  bx
  out   dx, al
  mov   dx, #VGAREG_DAC_DATA
  in    al, dx
  mov   bl, #0x4d
  mul   al, bl
  push  ax
  in    al, dx
  mov   bl, #0x97
  mul   al, bl
  push  ax
  in    al, dx
  mov   bl, #0x1c
  mul   al, bl
  mov   dx, ax
  pop   ax
  add   ax, dx
  mov   dx, ax
  pop   ax
  add   ax, dx
  add   ax, #0x80
  shr   ax, #8
  cmp   al, #0x3f
  jna   dac_no_clip
  mov   al, #0x3f
dac_no_clip:
  pop   bx
  push  ax
  mov   al, bl
  mov   dx, #VGAREG_DAC_WRITE_ADDRESS
  out   dx, al
  pop   ax
  mov   dx, #VGAREG_DAC_DATA
  out   dx, al
  out   dx, al
  out   dx, al
  inc   bl
  dec   cx
  jnz   dac_gss_loop
  mov   dx, #VGAREG_ACTL_RESET
  in    al, dx
  mov   dx, #VGAREG_ACTL_ADDRESS
  mov   al, #0x20
  out   dx, al
  pop   dx
  pop   cx
  pop   bx
  pop   ax
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_group_11:
  cmp   al, #0x03
  jne   int10_test_1120
  jmp   biosfn_set_text_block_specifier
int10_test_1120:
  cmp   al, #0x20
  jne   int10_test_1121
  jmp   biosfn_load_gfx_8_8_chars
int10_test_1121:
  cmp   al, #0x21
  jne   int10_test_1122
  jmp   biosfn_load_gfx_user_chars
int10_test_1122:
  cmp   al, #0x22
  jne   int10_test_1123
  jmp   biosfn_load_gfx_8_14_chars
int10_test_1123:
  cmp   al, #0x23
  jne   int10_test_1124
  jmp   biosfn_load_gfx_8_8_dd_chars
int10_test_1124:
  cmp   al, #0x24
  jne   int10_test_1130
  jmp   biosfn_load_gfx_8_16_chars
int10_test_1130:
  cmp   al, #0x30
  jne   int10_group_11_normal
  jmp   biosfn_get_font_info
int10_group_11_normal:
  jmp   int10_normal
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
_get_font_access:
  mov dx, # VGAREG_SEQU_ADDRESS
  mov ax, #0x0100
  out dx, ax
  mov ax, #0x0402
  out dx, ax
  mov ax, #0x0704
  out dx, ax
  mov ax, #0x0300
  out dx, ax
  mov dx, # VGAREG_GRDC_ADDRESS
  mov ax, #0x0204
  out dx, ax
  mov ax, #0x0005
  out dx, ax
  mov ax, #0x0406
  out dx, ax
  ret

_release_font_access:
  mov dx, # VGAREG_SEQU_ADDRESS
  mov ax, #0x0100
  out dx, ax
  mov ax, #0x0302
  out dx, ax
  mov ax, #0x0304
  out dx, ax
  mov ax, #0x0300
  out dx, ax
  mov dx, # VGAREG_READ_MISC_OUTPUT
  in  al, dx
  and al, #0x01
  shl al, 2
  or  al, #0x0a
  mov ah, al
  mov al, #0x06
  mov dx, # VGAREG_GRDC_ADDRESS
  out dx, ax
  mov ax, #0x0004
  out dx, ax
  mov ax, #0x1005
  out dx, ax
  ret

_set_scan_lines:
  push bp
  mov  bp, sp
  push ds
  mov  ax, # BIOSMEM_SEG
  mov  ds, ax
  mov  bx, # BIOSMEM_CRTC_ADDRESS
  mov  dx, [bx]
  mov  al, #0x09
  out  dx, al
  inc  dx
  in   al, dx
  and  al, #0xe0
  mov  ah, 4[bp] ;; lines
  dec  ah
  or   al, ah
  out  dx, al
  inc  ah
  cmp  ah, #0x08
  jne  scanlines_gt_8
  mov  cx, #0x0607
  call biosfn_set_cursor_shape
  jmp  set_char_height
scanlines_gt_8:
  mov  ch, ah
  sub  ch, #0x04 ; CH = lines-4
  mov  cl, ch
  inc  cl        ; CL = lines-3
  call biosfn_set_cursor_shape
set_char_height:
  mov  bx, # BIOSMEM_CHAR_HEIGHT
  mov  [bx], ah
  push ax
  call stdvga_get_scanlines
  pop  bx
  div  bh
  mov  ah, al
  dec  al
  mov  bx, # BIOSMEM_NB_ROWS
  mov  [bx], al
  mov  bx, # BIOSMEM_NB_COLS
  mov  al, [bx]
  mov  bl, ah
  mul  bl
  shl  ax, #1
  or   al, #0xff
  inc  ax
  mov  bx, # BIOSMEM_PAGE_SIZE
  mov  [bx], ax
  pop  ds
  pop  bp
  ret
ASM_END

static void biosfn_load_text_user_pat (AL,ES,BP,CX,DX,BL,BH) Bit8u AL;Bit16u ES;Bit16u BP;Bit16u CX;Bit16u DX;Bit8u BL;Bit8u BH;
{
 Bit16u blockaddr,dest,i,src;

 get_font_access();
 blockaddr = ((BL & 0x03) << 14) + ((BL & 0x04) << 11);
 for(i=0;i<CX;i++)
  {
   src = BP + i * BH;
   dest = blockaddr + (DX + i) * 32;
   memcpyb(0xA000, dest, ES, src, BH);
  }
 release_font_access();
 if(AL>=0x10)
  {
   set_scan_lines(BH);
  }
}

static void biosfn_load_text_8_14_pat (AL,BL) Bit8u AL;Bit8u BL;
{
 Bit16u blockaddr,dest,i,src;

 get_font_access();
 blockaddr = ((BL & 0x03) << 14) + ((BL & 0x04) << 11);
 for(i=0;i<0x100;i++)
  {
   src = i * 14;
   dest = blockaddr + i * 32;
   memcpyb(0xA000, dest, 0xC000, vgafont14+src, 14);
  }
 release_font_access();
 if(AL>=0x10)
  {
   set_scan_lines(14);
  }
}

static void biosfn_load_text_8_8_pat (AL,BL) Bit8u AL;Bit8u BL;
{
 Bit16u blockaddr,dest,i,src;

 get_font_access();
 blockaddr = ((BL & 0x03) << 14) + ((BL & 0x04) << 11);
 for(i=0;i<0x100;i++)
  {
   src = i * 8;
   dest = blockaddr + i * 32;
   memcpyb(0xA000, dest, 0xC000, vgafont8+src, 8);
  }
 release_font_access();
 if(AL>=0x10)
  {
   set_scan_lines(8);
  }
}

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_set_text_block_specifier:
  push  ax
  push  dx
  mov   dx, # VGAREG_SEQU_ADDRESS
  mov   ah, bl
  mov   al, #0x03
  out   dx, ax
  pop   dx
  pop   ax
  ret
ASM_END

// --------------------------------------------------------------------------------------------
static void biosfn_load_text_8_16_pat (AL,BL) Bit8u AL;Bit8u BL;
{
 Bit16u blockaddr,dest,i,src;

 get_font_access();
 blockaddr = ((BL & 0x03) << 14) + ((BL & 0x04) << 11);
 for(i=0;i<0x100;i++)
  {
   src = i * 16;
   dest = blockaddr + i * 32;
   memcpyb(0xA000, dest, 0xC000, vgafont16+src, 16);
  }
 release_font_access();
 if(AL>=0x10)
  {
   set_scan_lines(16);
  }
}

ASM_START
biosfn_load_gfx_8_8_chars:
  push ax
  push bx
  push ds
  xor  ax, ax
  mov  ds, ax
  mov  bx, #0x007c ;; INT 0x1F
  mov  [bx], bp
  mov  [bx+2], es
  mov  ax, #BIOSMEM_SEG
  mov  ds, ax
  mov  bx, #BIOSMEM_CHAR_HEIGHT
  mov  [bx], #0x08
  pop  ds
  pop  bx
  pop  ax
  ret
ASM_END

ASM_START
biosfn_load_gfx_user_chars:
  push ds
  xor  ax, ax
  mov  ds, ax
  mov  0x010c, bp ;; INT 0x43
  mov  0x010e, es
  pop  ds
  mov  ah, cl
  cmp  bl, #0x01
  jae  set_text_rows
  mov  al, dl
  dec  al
  jmp  set_text_rows_bda

biosfn_load_gfx_8_14_chars:
  SET_INT_VECTOR(0x43, #0xC000, #_vgafont14)
  mov  ah, #14
  jmp  set_text_rows

biosfn_load_gfx_8_8_dd_chars:
  SET_INT_VECTOR(0x43, #0xC000, #_vgafont8)
  mov  ah, #8
  jmp  set_text_rows

biosfn_load_gfx_8_16_chars:
  SET_INT_VECTOR(0x43, #0xC000, #_vgafont16)
  mov  ah, #16
set_text_rows:
  mov  al, #13
  cmp  bl, #0x01
  je   set_text_rows_bda
  mov  al, #42
  cmp  bl, #0x03
  je   set_text_rows_bda
  mov  al, #24
set_text_rows_bda:
  push ds
  push bx
  push ax
  mov  ax, #BIOSMEM_SEG
  mov  ds, ax
  pop  ax
  mov  bx, #BIOSMEM_NB_ROWS
  mov  [bx], al
  mov  bx, #BIOSMEM_CHAR_HEIGHT
  mov  [bx], ah
  pop  bx
  pop  ds
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_get_font_info:
  cmp  bh, #0x07
  ja   get_font_info_unknown
  push ax
  push bx
  push ds
  or   bh, bh
  jnz  get_font_info_1
  xor  ax, ax
  mov  ds, ax
  mov  bx, #0x007c ;; INT 0x1F
  mov  bp, [bx]
  mov  ax, [bx+2]
  mov  es, ax
  jmp  get_bda_data
get_font_info_1:
  cmp  bh, #0x01
  jne  get_font_info_2
  xor  ax, ax
  mov  ds, ax
  mov  bx, #0x010c ;; INT 0x43
  mov  bp, [bx]
  mov  ax, [bx+2]
  mov  es, ax
  jmp  get_bda_data
get_font_info_2:
  mov  ax, #0xc000
  mov  es, ax
  cmp  bh, #0x02
  jne  get_font_info_3
  mov  bp, #_vgafont14
  jmp  get_bda_data
get_font_info_3:
  cmp  bh, #0x03
  jne  get_font_info_4
  mov  bp, #_vgafont8
  jmp  get_bda_data
get_font_info_4:
  cmp  bh, #0x04
  jne  get_font_info_5
  mov  bp, #_vgafont8
  add  bp, #0x0400
  jmp  get_bda_data
get_font_info_5:
  cmp  bh, #0x05
  jne  get_font_info_6
  mov  bp, #_vgafont14alt
  jmp  get_bda_data
get_font_info_6:
  cmp  bh, #0x06
  jne  get_font_info_7
  mov  bp, #_vgafont16
  jmp  get_bda_data
get_font_info_7:
  mov  bp, #_vgafont16alt
get_bda_data:
  mov  ax, #BIOSMEM_SEG
  mov  ds, ax
  mov  cl, BIOSMEM_CHAR_HEIGHT
  xor  ch, ch
  mov  dl, BIOSMEM_NB_ROWS
  xor  dh, dh
  pop  ds
  pop  bx
  pop  ax
get_font_info_unknown:
  ret
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_get_ega_info:
  push  ds
  push  ax
  mov   ax, # BIOSMEM_SEG
  mov   ds, ax
  xor   ch, ch
  mov   bx, # BIOSMEM_SWITCHES
  mov   cl, [bx]
  and   cl, #0x0f
  mov   bx, # BIOSMEM_CRTC_ADDRESS
  mov   ax, [bx]
  mov   bx, #0x0003
  cmp   ax, # VGAREG_MDA_CRTC_ADDRESS
  jne   mode_ega_color
  mov   bh, #0x01
mode_ega_color:
  pop   ax
  pop   ds
  ret

biosfn_alternate_prtsc:
#ifdef DEBUG
  push  ax
  call  _unimplemented
  pop   ax
#endif
  ret

biosfn_select_vert_res:

; res : 00 200 lines, 01 350 lines, 02 400 lines

  push  ds
  push  bx
  push  dx
  mov   dl, al
  mov   ax, # BIOSMEM_SEG
  mov   ds, ax
  mov   bx, # BIOSMEM_MODESET_CTL
  mov   al, [bx]
  mov   bx, # BIOSMEM_SWITCHES
  mov   ah, [bx]
  cmp   dl, #0x01
  je    vert_res_350
  jb    vert_res_200
  cmp   dl, #0x02
  je    vert_res_400
#ifdef DEBUG
  mov   al, dl
  xor   ah, ah
  push  ax
  mov   bx, #msg_vert_res
  push  bx
  call  _printf
  add   sp, #4
#endif
  jmp   set_retcode
vert_res_400:

  ; reset modeset ctl bit 7 and set bit 4
  ; set switches bit 3-0 to 0x09

  and   al, #0x7f
  or    al, #0x10
  and   ah, #0xf0
  or    ah, #0x09
  jnz   set_vert_res
vert_res_350:

  ; reset modeset ctl bit 7 and bit 4
  ; set switches bit 3-0 to 0x09

  and   al, #0x6f
  and   ah, #0xf0
  or    ah, #0x09
  jnz   set_vert_res
vert_res_200:

  ; set modeset ctl bit 7 and reset bit 4
  ; set switches bit 3-0 to 0x08

  and   al, #0xef
  or    al, #0x80
  and   ah, #0xf0
  or    ah, #0x08
set_vert_res:
  mov   bx, # BIOSMEM_MODESET_CTL
  mov   [bx], al
  mov   bx, # BIOSMEM_SWITCHES
  mov   [bx], ah
set_retcode:
  mov   ax, #0x1212
  pop   dx
  pop   bx
  pop   ds
  ret

#ifdef DEBUG
msg_vert_res:
.ascii "Select vert res (%02x) was discarded"
.byte 0x0d,0x0a,0x00
#endif


biosfn_enable_default_palette_loading:
  push  ds
  push  bx
  push  dx
  mov   dl, al
  and   dl, #0x01
  shl   dl, 3
  mov   ax, # BIOSMEM_SEG
  mov   ds, ax
  mov   bx, # BIOSMEM_MODESET_CTL
  mov   al, [bx]
  and   al, #0xf7
  or    al, dl
  mov   [bx], al
  mov   ax, #0x1212
  pop   dx
  pop   bx
  pop   ds
  ret

biosfn_enable_video_addressing:
  push  bx
  push  dx
  mov   bl, al
  and   bl, #0x01
  xor   bl, #0x01
  shl   bl, 1
  mov   dx, # VGAREG_READ_MISC_OUTPUT
  in    al, dx
  and   al, #0xfd
  or    al, bl
  mov   dx, # VGAREG_WRITE_MISC_OUTPUT
  out   dx, al
  mov   ax, #0x1212
  pop   dx
  pop   bx
  ret

biosfn_enable_grayscale_summing:
  push  ds
  push  bx
  push  dx
  mov   dl, al
  and   dl, #0x01
  xor   dl, #0x01
  shl   dl, 1
  mov   ax, # BIOSMEM_SEG
  mov   ds, ax
  mov   bx, # BIOSMEM_MODESET_CTL
  mov   al, [bx]
  and   al, #0xfd
  or    al, dl
  mov   [bx], al
  mov   ax, #0x1212
  pop   dx
  pop   bx
  pop   ds
  ret

biosfn_enable_cursor_emulation:
  push  ds
  push  bx
  push  dx
  mov   dl, al
  and   dl, #0x01
  xor   dl, #0x01
  mov   ax, # BIOSMEM_SEG
  mov   ds, ax
  mov   bx, # BIOSMEM_MODESET_CTL
  mov   al, [bx]
  and   al, #0xfe
  or    al, dl
  mov   [bx], al
  mov   ax, #0x1212
  pop   dx
  pop   bx
  pop   ds
  ret

biosfn_switch_video_interface:
#ifdef DEBUG
  push  ax
  call  _unimplemented
  pop   ax
#endif
  ret

biosfn_enable_video_refresh_control:
  push bx
  push dx
  and  al, #0x01
  shl  al, #5
  mov  bl, al
  mov  dx, # VGAREG_SEQU_ADDRESS
  mov  al, #0x01
  out  dx, al
  inc  dx
  in   al, dx
  and  al, #0xdf
  or   al, bl
  out  dx, al
  mov  ax, #0x1212
  pop  dx
  pop  bx
  ret
ASM_END


// --------------------------------------------------------------------------------------------
static void biosfn_write_string (flag,page,attr,count,row,col,seg,offset)
Bit8u flag;Bit8u page;Bit8u attr;Bit16u count;Bit8u row;Bit8u col;Bit16u seg;Bit16u offset;
{
 Bit16u newcurs,oldcurs;
 Bit8u car;

 // Read curs info for the page
 oldcurs=get_cursor_pos(page);

 // if row=0xff special case : use current cursor position
 if(row==0xff)
  {col=oldcurs&0x00ff;
   row=(oldcurs&0xff00)>>8;
  }

 newcurs=row; newcurs<<=8; newcurs+=col;
 set_cursor_pos(page,newcurs);

 while(count--!=0)
  {
   car=read_byte(seg,offset++);
   if((flag&0x02)!=0)
    attr=read_byte(seg,offset++);

   biosfn_write_teletype(car,page,attr,WITH_ATTR);
  }

 // Set back curs pos
 if((flag&0x01)==0)
  set_cursor_pos(page,oldcurs);
}

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_group_1A:
  cmp   al, #0x00
  je    biosfn_read_display_code
  cmp   al, #0x01
  je    biosfn_set_display_code
#ifdef DEBUG
  call  _unknown
#endif
  ret
biosfn_read_display_code:
  push  ds
  push  ax
  mov   ax, # BIOSMEM_SEG
  mov   ds, ax
  mov   bx, # BIOSMEM_DCC_INDEX
  mov   al, [bx]
  mov   bl, al
  xor   bh, bh
  pop   ax
  mov   al, ah
  pop   ds
  ret
biosfn_set_display_code:
  push  ds
  push  ax
  push  bx
  mov   ax, # BIOSMEM_SEG
  mov   ds, ax
  mov   ax, bx
  mov   bx, # BIOSMEM_DCC_INDEX
  mov   [bx], al
#ifdef DEBUG
  mov   al, ah
  xor   ah, ah
  push  ax
  mov   bx, #msg_alt_dcc
  push  bx
  call  _printf
  add   sp, #4
#endif
  pop   bx
  pop   ax
  mov   al, ah
  pop   ds
  ret

#ifdef DEBUG
msg_alt_dcc:
.ascii "Alternate Display code (%02x) was discarded"
.byte 0x0d,0x0a,0x00
#endif
ASM_END

// --------------------------------------------------------------------------------------------
ASM_START
biosfn_read_state_info:
  and   bx, bx
  jnz   unsup_1B
  push  ds
  push  si
  push  di
  push  cx
  mov   ax, #BIOSMEM_SEG
  mov   ds, ax
  mov   ax, #_static_functionality
  stosw
  mov   ax, #0xc000
  stosw
  mov   si, #BIOSMEM_CURRENT_MODE
  mov   cx, #0x1e
  cld
  rep
     movsb
  mov   al, BIOSMEM_NB_ROWS
  stosb
  mov   ax, BIOSMEM_CHAR_HEIGHT
  stosw
  mov   al, BIOSMEM_DCC_INDEX
  stosb
  xor   al, al
  stosb
  mov   al, #0x10
  stosb
  xor   al, al
  stosb
  mov   al, #0x08
  stosb
  mov   al, #0x02
  stosb
  xor   ax, ax
  stosw
  mov   al, #0x03
  stosb
  xor   al, al
  stosb
  mov   cx, #0x0d
  rep
    stosb
  pop   cx
  pop   di
  pop   si
  pop   ds
  mov   ax, #0x1b1b
unsup_1B:
  ret
ASM_END

// -----------------------------------------------------------------------------
ASM_START
; called from VBE code
read_vga_state_size:
  xor   bx, bx
  test  cx, #0x01
  jz    no_hw_state
  add   bx, #0x46
no_hw_state:
  test  cx, #0x02
  jz    no_bda_state
  add   bx, #0x2d
no_bda_state:
  test  cx, #0x04
  jz    no_dac_state
  add   bx, #0x0304
no_dac_state:
  ret

biosfn_read_video_state_size:
  call  read_vga_state_size
  add   bx, #0x3f
  shr   bx, #6
  mov   ax, #0x001c
  ret

save_vga_state:
  push  dx
  push  di
  test  cx, #0x01
  jz    no_save_hw_state
  mov   di, bx
  mov   dx, #VGAREG_SEQU_ADDRESS
  in    al, dx
  stosb
  push  ds
  mov   ax, #BIOSMEM_SEG
  mov   ds, ax
  mov   dx, BIOSMEM_CRTC_ADDRESS
  in    al, dx
  stosb
  pop   ds
  push  dx
  mov   dx, #VGAREG_GRDC_ADDRESS
  in    al, dx
  stosb
  mov   dx, #VGAREG_ACTL_RESET
  in    al, dx
  mov   dx, #VGAREG_ACTL_ADDRESS
  in    al, dx
  stosb
  and   al, #0x20
  mov   bh, al
  mov   dx, #VGAREG_READ_FEATURE_CTL
  in    al, dx
  stosb
  mov   bl, #0x01 ;; VGA sequencer controller registers
save_seqc_loop:
  mov   dx, #VGAREG_SEQU_ADDRESS
  mov   al, bl
  out   dx, al
  mov   dx, #VGAREG_SEQU_DATA
  in    al, dx
  stosb
  inc   bl
  cmp   bl, #0x04
  jbe   save_seqc_loop
  mov   dx, #VGAREG_SEQU_ADDRESS
  xor   al, al
  out   dx, al
  mov   dx, #VGAREG_SEQU_DATA
  in    al, dx
  stosb
  xor   bl, bl ;; VGA CRTC registers
save_crtc_loop:
  pop   dx
  mov   al, bl
  out   dx, al
  push  dx
  inc   dx
  in    al, dx
  stosb
  inc   bl
  cmp   bl, #0x18
  jbe   save_crtc_loop
  xor   bl, bl ;; VGA attribute controller registers
save_actl_loop:
  mov   dx, #VGAREG_ACTL_RESET
  in    al, dx
  mov   dx, #VGAREG_ACTL_ADDRESS
  mov   al, bl
  or    al, bh
  out   dx, al
  mov   dx, #VGAREG_ACTL_READ_DATA
  in    al, dx
  stosb
  inc   bl
  cmp   bl, #0x13
  jbe   save_actl_loop
  mov   dx, #VGAREG_ACTL_RESET
  in    al, dx
  xor   bl, bl ;; VGA graphics controller registers
save_grdc_loop:
  mov   dx, #VGAREG_GRDC_ADDRESS
  mov   al, bl
  out   dx, al
  mov   dx, #VGAREG_GRDC_DATA
  in    al, dx
  stosb
  inc   bl
  cmp   bl, #0x08
  jbe   save_grdc_loop
  pop   ax ;; CRTC address
  stosw
  xor   bl, bl ;; VGA latches
  mov   dx, ax
  mov   al, #0x22
  out   dx, al
save_latches_loop:
  push  dx
  mov   dx, #VGAREG_GRDC_ADDRESS
  mov   al, #0x04
  mov   ah, bl
  out   dx, ax
  pop   dx
  inc   dx
  in    al, dx
  stosb
  dec   dx
  inc   bl
  cmp   bl, #0x04
  jb    save_latches_loop
  mov   dx, #VGAREG_GRDC_ADDRESS
  mov   ax, #0x0004
  out   dx, ax
  mov   bx, di
no_save_hw_state:
  test  cx, #0x02
  jz    no_save_bda_state
  push  ds
  push  cx
  push  si
  mov   ax, #BIOSMEM_SEG
  mov   ds, ax
  mov   si, #BIOSMEM_CURRENT_MODE
  mov   di, bx
  mov   cx, #0x1e
  cld
  rep
     movsb
  mov   si, #BIOSMEM_NB_ROWS
  mov   cx, #0x07
  cld
  rep
     movsb
  xor   ax, ax
  mov   ds, ax
  mov   eax, 0x007c ;; INT 0x1F
  stosd
  mov   eax, 0x010c ;; INT 0x43
  stosd
  mov   bx, di
  pop   si
  pop   cx
  pop   ds
no_save_bda_state:
  test  cx, #0x04
  jz    no_save_dac_state
  push  cx
  mov   di, bx
  mov   dx, #VGAREG_DAC_STATE
  in    al, dx
  stosb
  mov   dx, #VGAREG_DAC_WRITE_ADDRESS
  in    al, dx
  stosb
  mov   dx, #VGAREG_PEL_MASK
  in    al, dx
  stosb
  mov   dx, #VGAREG_DAC_READ_ADDRESS
  xor   al, al
  out   dx, al
  mov   cx, #0x0300
  cld
  mov   dx, #VGAREG_DAC_DATA
dac_read_loop:
  in    al, dx
  stosb
  loop  dac_read_loop
  mov   bl, #0x14
  call  biosfn_get_single_palette_reg
  mov   al, bh
  stosb
  mov   bx, di
  pop   cx
no_save_dac_state:
  pop   di
  pop   dx
  ret

biosfn_save_video_state:
  push  bx
  call  save_vga_state
  pop   bx
  mov   ax, #0x001c
  ret
ASM_END

// -----------------------------------------------------------------------------
ASM_START
restore_vga_state:
  test  cx, #0x01
  jz    no_rest_hw_state
  push  cx
  push  dx
  push  bx
  add   bx, #5
  mov   cl, #0x01 ;; VGA sequencer controller registers
  mov   dx, #VGAREG_SEQU_ADDRESS
rest_seqc_loop:
  mov   al, cl
  seg   es
  mov   ah, [bx]
  out   dx, ax
  inc   bx
  inc   cl
  cmp   cl, #0x04
  jbe   rest_seqc_loop
  mov   al, #0x00
  seg   es
  mov   ah, [bx]
  out   dx, ax
  inc   bx
  seg   es
  mov   ax, 0x36[bx]
  push  ax
  cmp   ax, #VGAREG_VGA_CRTC_ADDRESS
  jne   no_color
  mov   dx, #VGAREG_READ_MISC_OUTPUT
  in    al, dx
  or    al, #0x01 ;; set CRTC color address
  mov   dx, #VGAREG_WRITE_MISC_OUTPUT
  out   dx, al
no_color:
  pop   dx
  mov   ax, #0x0011 ;; Disable CRTC write protection
  out   dx, ax
  xor   cl, cl ;; VGA CRTC registers
rest_crtc_loop:
  mov   al, cl
  seg   es
  mov   ah, [bx]
  out   dx, ax
  inc   bx
  inc   cl
  cmp   cl, #0x18
  jbe   rest_crtc_loop
  mov   dx, #VGAREG_ACTL_RESET
  in    al, dx
  xor   cl, cl ;; VGA attribute controller registers
  mov   dx, #VGAREG_ACTL_ADDRESS
rest_actl_loop:
  mov   al, cl
  or    al, #0x20
  out   dx, al
  seg   es
  mov   al, [bx]
  out   dx, al
  inc   bx
  inc   cl
  cmp   cl, #0x13
  jbe   rest_actl_loop
  xor   cl, cl ;; VGA graphics controller registers
  mov   dx, #VGAREG_GRDC_ADDRESS
rest_grdc_loop:
  mov   al, cl
  seg   es
  mov   ah, [bx]
  out   dx, ax
  inc   bx
  inc   cl
  cmp   cl, #0x08
  jbe   rest_grdc_loop
  add   bx, #6
  pop   ax
  push  bx
  mov   bx, ax
  seg   es
  mov   al, [bx]
  mov   dx, #VGAREG_SEQU_ADDRESS
  out   dx, al
  inc   bx
  seg   es
  mov   al, [bx]
  call  get_crtc_address
  out   dx, al
  inc   bx
  seg   es
  mov   al, [bx]
  mov   dx, #VGAREG_GRDC_ADDRESS
  out   dx, al
  inc   bx
  mov   dx, #VGAREG_ACTL_RESET
  in    al, dx
  mov   dx, #VGAREG_ACTL_ADDRESS
  seg   es
  mov   al, [bx]
  out   dx, al
  mov   dx, #VGAREG_ACTL_RESET
  in    al, dx
  inc   bx
  seg   es
  mov   al, [bx]
  mov   dx, #VGAREG_VGA_WRITE_FEATURE_CTL
  pop   bx
  pop   dx
  pop   cx
no_rest_hw_state:
  test  cx, #0x02
  jz    no_rest_bda_state
  push  ds
  push  cx
  push  si
  push  di
  push  es
  pop   ds
  mov   si, bx
  mov   ax, #BIOSMEM_SEG
  mov   es, ax
  mov   di, #BIOSMEM_CURRENT_MODE
  mov   cx, #0x1e
  cld
  rep
     movsb
  mov   di, #BIOSMEM_NB_ROWS
  mov   cx, #0x07
  cld
  rep
     movsb
  xor   ax, ax
  mov   es, ax
  lodsd
  seg   es
  mov   0x007c, eax ;; INT 0x1F
  lodsd
  seg   es
  mov   0x010c, eax ;; INT 0x43
  mov   bx, si
  push  ds
  pop   es
  pop   di
  pop   si
  pop   cx
  pop   ds
no_rest_bda_state:
  test  cx, #0x04
  jz    no_rest_dac_state
  push  cx
  push  dx
  inc   bx
  seg   es
  mov   al, [bx]
  push  ax
  inc   bx
  seg   es
  mov   al, [bx]
  mov   dx, #VGAREG_PEL_MASK
  out   dx, al
  inc   bx
  mov   al, #0x00
  mov   dx, #VGAREG_DAC_WRITE_ADDRESS
  out   dx, al
  mov   cx, #0x0300
  cld
  mov   dx, #VGAREG_DAC_DATA
dac_write_loop:
  seg   es
  mov   al, [bx]
  out   dx, al
  inc   bx
  loop  dac_write_loop
  pop   ax
  mov   dx, #VGAREG_DAC_WRITE_ADDRESS
  out   dx, al
  seg   es
  mov   al, [bx]
  push  bx
  mov   bh, al
  mov   bl, #0x14
  call  biosfn_set_single_palette_reg
  pop   bx
  inc   bx
  pop   dx
  pop   cx
no_rest_dac_state:
  ret

biosfn_restore_video_state:
  push  bx
  call  restore_vga_state
  pop   bx
  mov   ax, #0x001c
  ret
ASM_END

// ============================================================================================
//
// Video Utils
//
// ============================================================================================

// --------------------------------------------------------------------------------------------
#ifdef CIRRUS
ASM_START
stdvga_is_4bpp_mode:
  push ax
  push dx
  mov  dx, #0x03ce
  mov  al, #0x05 ;; GR5: shift reg
  out  dx, al
  inc  dx
  in   al, dx
  and  al, #0x60
  jnz  no_4bpp_mode
  dec  dx
  mov  al, #0x06 ;; GR6: graphics mode and memory mapping
  out  dx, al
  inc  dx
  in   al, dx
  and  al, #0x0d
  cmp  al, #0x05
  jnz  no_4bpp_mode
  pop  dx
  pop  ax
  stc
  ret
no_4bpp_mode:
  pop  dx
  pop  ax
  clc
  ret
ASM_END
#endif

ASM_START
_find_vga_entry:
  push bp
  mov  bp, sp
  push cx
  push si
  mov  al, 4[bp]
  cmp  al, #0xff
  jne  fve_1
  push  ds
  mov   ax, # BIOSMEM_SEG
  mov   ds, ax
  push  bx
  mov   bx, # BIOSMEM_CURRENT_MODE
  mov   al, [bx]
  pop   bx
  pop   ds
fve_1:
  xor  cl, cl
  mov  si, #_vga_modes
fve_2:
 db 0x2e ;; cs:
  mov  ah, [si]
  cmp  al, ah
  jz   fve_5
  add  si, # VGA_MODE_SIZE
  inc  cl
  cmp  cl, # MODE_MAX
  jbe  fve_2
#ifdef VGAEXT
  cmp  4[bp], #0xff
  jne  fve_3
  call VGAEXT_is_8bpp_mode
  or   al, al
  jz   fve_3
  mov  cl, #14
  jnz  fve_5
fve_3:
#endif
#ifdef CIRRUS
  cmp  4[bp], #0xff
  jne  fve_4
  call stdvga_is_4bpp_mode
  jnc  fve_4
  mov  cl, #15
  jc   fve_5
fve_4:
#endif
  mov  cl, #0xff
fve_5:
  mov  al, cl
  pop  si
  pop  cx
  pop  bp
  ret
ASM_END

/* =========================================================== */
/*
 * Misc Utils
*/
/* =========================================================== */

// --------------------------------------------------------------------------------------------
static void memsetb(seg,offset,value,count)
  Bit16u seg;
  Bit16u offset;
  Bit16u value;
  Bit16u count;
{
ASM_START
  push bp
  mov  bp, sp

    push ax
    push cx
    push es
    push di

    mov  cx, 10[bp] ; count
    test cx, cx
    jz   memsetb_end
    mov  ax, 4[bp] ; segment
    mov  es, ax
    mov  ax, 6[bp] ; offset
    mov  di, ax
    mov  al, 8[bp] ; value
    cld
    rep
     stosb

memsetb_end:
    pop di
    pop es
    pop cx
    pop ax

  pop bp
ASM_END
}

// --------------------------------------------------------------------------------------------
static void memsetw(seg,offset,value,count)
  Bit16u seg;
  Bit16u offset;
  Bit16u value;
  Bit16u count;
{
ASM_START
  push bp
  mov  bp, sp

    push ax
    push cx
    push es
    push di

    mov  cx, 10[bp] ; count
    test cx, cx
    jz   memsetw_end
    mov  ax, 4[bp] ; segment
    mov  es, ax
    mov  ax, 6[bp] ; offset
    mov  di, ax
    mov  ax, 8[bp] ; value
    cld
    rep
     stosw

memsetw_end:
    pop di
    pop es
    pop cx
    pop ax

  pop bp
ASM_END
}

// --------------------------------------------------------------------------------------------
static void memcpyb(dseg,doffset,sseg,soffset,count)
  Bit16u dseg;
  Bit16u doffset;
  Bit16u sseg;
  Bit16u soffset;
  Bit16u count;
{
ASM_START
  push bp
  mov  bp, sp

    push ax
    push cx
    push es
    push di
    push ds
    push si

    mov  cx, 12[bp] ; count
    test cx, cx
    jz   memcpyb_end
    mov  ax, 4[bp] ; dsegment
    mov  es, ax
    mov  ax, 6[bp] ; doffset
    mov  di, ax
    mov  ax, 8[bp] ; ssegment
    mov  ds, ax
    mov  ax, 10[bp] ; soffset
    mov  si, ax
    cld
    rep
     movsb

memcpyb_end:
    pop si
    pop ds
    pop di
    pop es
    pop cx
    pop ax

  pop bp
ASM_END
}

// --------------------------------------------------------------------------------------------
static void memcpyw(dseg,doffset,sseg,soffset,count)
  Bit16u dseg;
  Bit16u doffset;
  Bit16u sseg;
  Bit16u soffset;
  Bit16u count;
{
ASM_START
  push bp
  mov  bp, sp

    push ax
    push cx
    push es
    push di
    push ds
    push si

    mov  cx, 12[bp] ; count
    test cx, cx
    jz   memcpyw_end
    mov  ax, 4[bp] ; dsegment
    mov  es, ax
    mov  ax, 6[bp] ; doffset
    mov  di, ax
    mov  ax, 8[bp] ; ssegment
    mov  ds, ax
    mov  ax, 10[bp] ; soffset
    mov  si, ax
    cld
    rep
     movsw

memcpyw_end:
    pop si
    pop ds
    pop di
    pop es
    pop cx
    pop ax

  pop bp
ASM_END
}

/* =========================================================== */
/*
 * These functions where ripped from Kevin's rombios.c
*/
/* =========================================================== */

// --------------------------------------------------------------------------------------------
static Bit8u
read_byte(seg, offset)
  Bit16u seg;
  Bit16u offset;
{
ASM_START
  push bp
  mov  bp, sp

    push bx
    push ds
    mov  ax, 4[bp] ; segment
    mov  ds, ax
    mov  bx, 6[bp] ; offset
    mov  al, [bx]
    ;; al = return value (byte)
    pop  ds
    pop  bx

  pop  bp
ASM_END
}

// --------------------------------------------------------------------------------------------
static Bit8u
read_bda_byte(offset)
  Bit16u offset;
{
ASM_START
  push bp
  mov  bp, sp

    push bx
    push ds
    mov  ax, #BIOSMEM_SEG ; segment
    mov  ds, ax
    mov  bx, 4[bp] ; offset
    mov  al, [bx]
    ;; al = return value (byte)
    pop  ds
    pop  bx

  pop  bp
ASM_END
}

// --------------------------------------------------------------------------------------------
static Bit16u
read_word(seg, offset)
  Bit16u seg;
  Bit16u offset;
{
ASM_START
  push bp
  mov  bp, sp

    push bx
    push ds
    mov  ax, 4[bp] ; segment
    mov  ds, ax
    mov  bx, 6[bp] ; offset
    mov  ax, [bx]
    ;; ax = return value (word)
    pop  ds
    pop  bx

  pop  bp
ASM_END
}

// --------------------------------------------------------------------------------------------
static Bit16u
read_bda_word(offset)
  Bit16u offset;
{
ASM_START
  push bp
  mov  bp, sp

    push bx
    push ds
    mov  ax, #BIOSMEM_SEG ; segment
    mov  ds, ax
    mov  bx, 4[bp] ; offset
    mov  ax, [bx]
    ;; ax = return value (word)
    pop  ds
    pop  bx

  pop  bp
ASM_END
}

// --------------------------------------------------------------------------------------------
static void
write_byte(seg, offset, data)
  Bit16u seg;
  Bit16u offset;
  Bit8u  data;
{
ASM_START
  push bp
  mov  bp, sp

    push ax
    push bx
    push ds
    mov  ax, 4[bp] ; segment
    mov  ds, ax
    mov  bx, 6[bp] ; offset
    mov  al, 8[bp] ; data byte
    mov  [bx], al  ; write data byte
    pop  ds
    pop  bx
    pop  ax

  pop  bp
ASM_END
}

// --------------------------------------------------------------------------------------------
static void
write_bda_byte(offset, data)
  Bit16u offset;
  Bit8u  data;
{
ASM_START
  push bp
  mov  bp, sp

    push ax
    push bx
    push ds
    mov  ax, #BIOSMEM_SEG ; segment
    mov  ds, ax
    mov  bx, 4[bp] ; offset
    mov  al, 6[bp] ; data byte
    mov  [bx], al  ; write data byte
    pop  ds
    pop  bx
    pop  ax

  pop  bp
ASM_END
}

// --------------------------------------------------------------------------------------------
static void
write_word(seg, offset, data)
  Bit16u seg;
  Bit16u offset;
  Bit16u data;
{
ASM_START
  push bp
  mov  bp, sp

    push ax
    push bx
    push ds
    mov  ax, 4[bp] ; segment
    mov  ds, ax
    mov  bx, 6[bp] ; offset
    mov  ax, 8[bp] ; data word
    mov  [bx], ax  ; write data word
    pop  ds
    pop  bx
    pop  ax

  pop  bp
ASM_END
}

// --------------------------------------------------------------------------------------------
static void
write_bda_word(offset, data)
  Bit16u offset;
  Bit16u data;
{
ASM_START
  push bp
  mov  bp, sp

    push ax
    push bx
    push ds
    mov  ax, #BIOSMEM_SEG ; segment
    mov  ds, ax
    mov  bx, 4[bp] ; offset
    mov  ax, 6[bp] ; data word
    mov  [bx], ax  ; write data word
    pop  ds
    pop  bx
    pop  ax

  pop  bp
ASM_END
}

// --------------------------------------------------------------------------------------------
  void
outb(port, val)
  Bit16u port;
  Bit8u  val;
{
ASM_START
  push bp
  mov  bp, sp

    push ax
    push dx
    mov  dx, 4[bp]
    mov  al, 6[bp]
    out  dx, al
    pop  dx
    pop  ax

  pop  bp
ASM_END
}

// --------------------------------------------------------------------------------------------
  void
outw(port, val)
  Bit16u port;
  Bit16u  val;
{
ASM_START
  push bp
  mov  bp, sp

    push ax
    push dx
    mov  dx, 4[bp]
    mov  ax, 6[bp]
    out  dx, ax
    pop  dx
    pop  ax

  pop  bp
ASM_END
}

Bit16u get_SS()
{
ASM_START
  mov  ax, ss
ASM_END
}

#ifdef DEBUG
void unimplemented()
{
 printf("--> Unimplemented\n");
}

void unknown()
{
 printf("--> Unknown int10\n");
}
#endif

ASM_START
idiv_u:
  xor dx,dx
  div bx
  ret

imodu:
  push dx
  xor  dx, dx
  div  bx
  mov  ax, dx
  pop  dx
  ret
ASM_END

// --------------------------------------------------------------------------------------------
#if defined(USE_BX_INFO) || defined(DEBUG) || defined(CIRRUS_DEBUG)
void printf(s)
  Bit8u *s;
{
  Bit8u c, format_char;
  Boolean  in_format, in_value;
  unsigned format_width, i;
  Bit16u  *arg_ptr;
  Bit16u   arg_seg, arg, digit, nibble, divider;

  arg_ptr = &s;
  arg_seg = get_SS();

  in_format = 0;
  format_width = 0;

  while (c = read_byte(0xc000, s)) {
    if ( c == '%' ) {
      in_format = 1;
      format_width = 0;
    } else if (in_format) {
      if ( (c>='0') && (c<='9') ) {
        format_width = (format_width * 10) + (c - '0');
      } else if (c == 'x') {
        arg_ptr++; // increment to next arg
        arg = read_word(arg_seg, arg_ptr);
        if (format_width == 0)
          format_width = 4;
        i = 0;
        digit = format_width - 1;
        for (i=0; i<format_width; i++) {
          nibble = (arg >> (4 * digit)) & 0x000f;
          if (nibble <= 9)
            outb(0x0500, nibble + '0');
          else
            outb(0x0500, (nibble - 10) + 'A');
          digit--;
        }
        in_format = 0;
      } else if (c == 'd') {
        arg_ptr++; // increment to next arg
        arg = read_word(arg_seg, arg_ptr);
        divider = 10000;
        in_value = 0;
        while (divider > 0) {
          digit = arg / divider;
          if ((digit > 0) || in_value) {
            outb(0x0500, digit + '0');
            in_value = 1;
          }
          arg = arg % divider;
          divider /= 10;
        }
        in_format = 0;
      }
    } else {
      outb(0x0500, c);
    }
    s++;
  }
}
#endif

ASM_START
#if defined(VBE) || defined(CIRRUS)
  ; get LFB address from PCI
  ; in - ax: PCI device vendor
  ; out - ax: LFB address (high 16 bit)
  ;; NOTE - may be called in protected mode
pci_get_lfb_addr:
  push bx
  push cx
  push dx
  push eax
    mov bx, ax
    xor cx, cx
    mov dl, #0x00
    call pci_read_reg
    cmp ax, #0xffff
    jz pci_get_lfb_addr_fail
 pci_get_lfb_addr_next_dev:
    mov dl, #0x00
    call pci_read_reg
    cmp ax, bx ;; check vendor
    jz pci_get_lfb_addr_found
    add cx, #0x8
    cmp cx, #0x200 ;; search bus #0 and #1
    jb pci_get_lfb_addr_next_dev
 pci_get_lfb_addr_fail:
    xor dx, dx ;; no LFB
    jmp pci_get_lfb_addr_return
 pci_get_lfb_addr_found:
    mov dl, #0x10 ;; I/O space #0
    call pci_read_reg
    test ax, #0xfff1
    jz pci_get_lfb_addr_success
    mov dl, #0x14 ;; I/O space #1
    call pci_read_reg
    test ax, #0xfff1
    jnz pci_get_lfb_addr_fail
 pci_get_lfb_addr_success:
    shr eax, #16
    mov dx, ax ;; LFB address
 pci_get_lfb_addr_return:
  pop eax
  mov ax, dx
  pop dx
  pop cx
  pop bx
  ret
#endif

  ; read PCI register
  ; in - cx: device/function
  ; in - dl: register
  ; out - eax: value
pci_read_reg:
  mov eax, #0x00800000
  mov ax, cx
  shl eax, #8
  mov al, dl
  mov dx, #0xcf8
  out dx, eax
  add dl, #4
  in  eax, dx
  ret

  ; get CRTC address from VGA misc output setting
  ; out - dx: CRTC address
get_crtc_address:
  push ax
  mov  dx, #VGAREG_READ_MISC_OUTPUT
  in   al, dx
  and  al, #0x01
  shl  al, #5
  mov  dx, #VGAREG_MDA_CRTC_ADDRESS
  add  dl, al
  pop  ax
  ret

;; in - ax:number of scanlines
stdvga_set_scanlines:
  push dx
  call get_crtc_address
  push ax
  dec  ax
  push ax
  mov  ah, al
  mov  al, #0x12
  out  dx, ax
  pop  ax
  mov  al, #0x07
  out  dx, al
  inc  dx
  in   al, dx
  and  al, #0xbd
  or   al, #0x10
  test ah, #0x01
  jz   bit8_clear
  or   al, #0x02
bit8_clear:
  test ah, #0x02
  jz   bit9_clear
  or   al, #0x40
bit9_clear:
  out  dx, al
  ;; set CRTC Y blanking start
  dec  dx
  pop  ax
  push ax
  mov  ah, al
  mov  al, #0x15
  out  dx, ax
  pop  ax
  mov  al, #0x07
  out  dx, al
  inc  dx
  in   al, dx
  and  al, #0xf7
  test ah, #0x01
  jz   bit8_clear2
  or   al, #0x08
bit8_clear2:
  out  dx, al
  dec  dx
  mov  al, #0x09
  out  dx, al
  inc  dx
  in   al, dx
  and  al, #0xdf
  test ah, #0x02
  jz   bit9_clear2
  or   al, #0x20
bit9_clear2:
  out  dx, al
  pop  dx
  ret

;; out - ax:number of scanlines
stdvga_get_scanlines:
  push bx
  push dx
  call get_crtc_address
  mov  al, #0x12
  out  dx, al
  inc  dx
  in   al, dx
  mov  bl, al
  dec  dx
  mov  al, #0x07
  out  dx, al
  inc  dx
  in   al, dx
  xor  ah, ah
  test al, #0x02
  jz   test_bit9
  or   ah, #0x01
test_bit9:
  test al, #0x40
  jz   inc_vde
  or   ah, #0x02
inc_vde:
  mov  al, bl
  inc  ax
  pop  dx
  pop  bx
  ret

; shared code for the VBE support of all extensions
#if defined(VBE) || defined(CIRRUS) || defined(BANSHEE)

; helper function for function 00h
strcpy:
  push cx
  push ds
  push di
  mov  ax, cs
  mov  ds, ax
  xor  cx, cx
strcpy_loop:
  lodsb
  stosb
  inc  cx
  or   al, al
  jnz  strcpy_loop
  mov  ax, cx
  pop  di
  pop  ds
  pop  cx
  ret

; VBE function 09h
vbe_biosfn_set_get_palette_data:
  cmp   bl, #0x80
  je    vbe_set_palette_data_vsync
  cmp   bl, #0x01
  jb    vbe_set_palette_data
  je    vbe_get_palette_data
  mov   ax, #0x024f ; unimplemented
  ret
vbe_set_palette_data_vsync:
  call  vbebios_vsync_wait
vbe_set_palette_data:
  push  cx
  push  dx
  push  di
  mov   al, dl
  mov   dx, # VGAREG_DAC_WRITE_ADDRESS
  out   dx, al
  mov   dx, # VGAREG_DAC_DATA
  cld
vbe_set_dac_loop:
  seg   es
  mov   al, [di+2]
  out   dx, al
  seg   es
  mov   al, [di+1]
  out   dx, al
  seg   es
  mov   al, [di]
  out   dx, al
  add   di, #4
  loop  vbe_set_dac_loop
  pop   di
  pop   dx
  pop   cx
  mov   ax, #0x004f
  ret
vbe_get_palette_data:
  push  cx
  push  dx
  push  di
  mov   al, dl
  mov   dx, # VGAREG_DAC_READ_ADDRESS
  out   dx, al
  mov   dx, # VGAREG_DAC_DATA
  cld
vbe_get_dac_loop:
  in    al, dx
  seg   es
  mov   [di+2], al
  in    al, dx
  seg   es
  mov   [di+1], al
  in    al, dx
  seg   es
  mov   [di], al
  xor   al, al
  seg   es
  mov   [di+3], al
  add   di, #4
  loop  vbe_get_dac_loop
  pop   di
  pop   dx
  pop   cx
  mov   ax, #0x004f
  ret

vbebios_vsync_wait:
  push dx
  mov  dx, #VGAREG_ACTL_RESET
vbebios_in_vsync:
  in   al, dx
  test al, #0x08
  jnz  vbebios_in_vsync
vbebios_wait_loop:
  in   al, dx
  test al, #0x08
  jz   vbebios_wait_loop
  pop  dx
  ret

#endif
ASM_END

#ifdef VBE
#include "vbe.c"
#endif

#ifdef CIRRUS
#include "clext.c"
#endif

#ifdef BANSHEE
#include "banshee.c"
#endif

// --------------------------------------------------------------------------------------------

ASM_START
;; DATA_SEG_DEFS_HERE
ASM_END

ASM_START
.ascii "vgabios end."
.byte  0x00
vgabios_end:
.byte 0xCB
;; BLOCK_STRINGS_BEGIN

ASM_END
