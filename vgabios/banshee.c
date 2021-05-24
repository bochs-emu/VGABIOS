/*
 * $Id$
 */
//
//  3dfx Banshee VGABIOS Extension.
//
//  Copyright (C) 2021  Volker Ruppert
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

//#define BANSHEE_DEBUG

#define BANSHEE_MODE_SIZE 16

ASM_START

;; Banshee strings

threedfx_msg:
.ascii "3dfx Voodoo"
.byte 0x00

banshee_msg:
.ascii " Banshee "
.byte 0x00

voodoo3_msg:
.ascii "3 "
.byte 0x00

pci_msg:
.ascii "PCI"
.byte 0x00

agp_msg:
.ascii "AGP"
.byte 0x00

detected_msg:
.ascii " detected"
.byte 0x0d,0x0a
.byte 0x0d,0x0a,0x00

banshee_not_installed:
.ascii " Banshee-compatible VGA not detected"
.byte 0x0d,0x0a
.byte 0x0d,0x0a,0x00

banshee_vesa_vendorname:
banshee_vesa_productname:
banshee_vesa_oemname:
.ascii "VGABIOS Banshee extension"
.byte 0
banshee_vesa_productrevision:
.ascii "1.0"
.byte 0

;; Banshee mode tables

banshee_svga_sequ:
.byte 0x03,0x01,0x0f,0x00,0x0e

banshee_svga_grdc:
.byte 0x00,0x00,0x00,0x00,0x00,0x40,0x05,0x0f,0xff

banshee_640x400x8_crtc:
.byte 0x5f,0x4f,0x50,0x82,0x55,0x81,0xbf,0x1f
.byte 0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00
.byte 0x9c,0x8e,0x8f,0x50,0x40,0x96,0xb9,0xc3
.byte 0xff,0x00,0x00,0x00

banshee_640x480x8_crtc:
.byte 0x5f,0x4f,0x50,0x82,0x52,0x9e,0x0b,0x3e
.byte 0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00
.byte 0xea,0x0c,0xdf,0x50,0x40,0xe7,0x04,0xc3
.byte 0xff,0x00,0x00,0x00

banshee_800x600x8_crtc:
.byte 0x7f,0x63,0x64,0x82,0x69,0x19,0x72,0xf0
.byte 0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00
.byte 0x59,0x8d,0x57,0x64,0x40,0x58,0x73,0xc3
.byte 0xff,0x00,0x00,0x00

banshee_1024x768x8_crtc:
.byte 0xa3,0x7f,0x80,0x87,0x83,0x94,0x24,0xf5
.byte 0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00
.byte 0x03,0x09,0xff,0x80,0x40,0xff,0x25,0xc3
.byte 0xff,0x00,0x00,0x00

banshee_1280x1024x8_crtc:
.byte 0xce,0x9f,0xa0,0x91,0xa6,0x14,0x28,0x52
.byte 0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00
.byte 0x01,0x04,0xff,0xa0,0x40,0x01,0x28,0xc3
.byte 0xff,0x00,0x00,0x51

banshee_modes:
;; 640 x 400 x 8
.byte 0x5b ;; mode
.byte 8    ;; depth
.word 640  ;; xres
.word 400  ;; yres
.word banshee_640x400x8_crtc ;; CRTC settings
.word 0,0 ;; pllCtrl0 (0 = unused)
.byte 0x63 ;; misc reg
.byte 0x04 ;; VBE memory model
.word banshee_color_params_8bpp ;; color params
;; 640 x 480 x 8
.byte 0x5f
.byte 8
.word 640
.word 480
.word banshee_640x480x8_crtc
.word 0,0
.byte 0xe3
.byte 0x04
.word banshee_color_params_8bpp
;; 800 x 600 x 8
.byte 0x5c
.byte 8
.word 800
.word 600
.word banshee_800x600x8_crtc
.word 0xbc3e,0x0000 ;; 40 MHz
.byte 0xef
.byte 0x04
.word banshee_color_params_8bpp
;; 1024 x 768 x 8
.byte 0x5e
.byte 8
.word 1024
.word 768
.word banshee_1024x768x8_crtc
.word 0xe15d,0x0000 ;; 65 MHz
.byte 0xef
.byte 0x04
.word banshee_color_params_8bpp
;; 1280 x 1024 x 8
.byte 0x6b
.byte 8
.word 1280
.word 1024
.word banshee_1280x1024x8_crtc
.word 0xb358,0x0000 ;; 108 MHz
.byte 0xef
.byte 0x04
.word banshee_color_params_8bpp
;; 640 x 400 x 16
.byte 0x29 ;; mode
.byte 16   ;; depth
.word 640  ;; xres
.word 400  ;; yres
.word banshee_640x400x8_crtc ;; CRTC settings
.word 0,0 ;; pllCtrl0 (0 = unused)
.byte 0x63 ;; misc reg
.byte 0x06 ;; VBE memory model
.word banshee_color_params_16bpp ;; color params
;; 640 x 480 x 16
.byte 0x6e
.byte 16
.word 640
.word 480
.word banshee_640x480x8_crtc
.word 0,0
.byte 0xe3
.byte 0x06
.word banshee_color_params_16bpp
;; 800 x 600 x 16
.byte 0x70
.byte 16
.word 800
.word 600
.word banshee_800x600x8_crtc
.word 0xbc3e,0x0000 ;; 40 MHz
.byte 0xef
.byte 0x06
.word banshee_color_params_16bpp
;; 1024 x 768 x 16
.byte 0x72
.byte 16
.word 1024
.word 768
.word banshee_1024x768x8_crtc
.word 0xe15d,0x0000 ;; 65 MHz
.byte 0xef
.byte 0x06
.word banshee_color_params_16bpp
;; 1280 x 1024 x 16
.byte 0x74
.byte 16
.word 1280
.word 1024
.word banshee_1280x1024x8_crtc
.word 0xb358,0x0000 ;; 108 MHz
.byte 0xef
.byte 0x06
.word banshee_color_params_16bpp
;; 640 x 400 x 24
.byte 0x2a ;; mode
.byte 24   ;; depth
.word 640  ;; xres
.word 400  ;; yres
.word banshee_640x400x8_crtc ;; CRTC settings
.word 0,0 ;; pllCtrl0 (0 = unused)
.byte 0x63 ;; misc reg
.byte 0x06 ;; VBE memory model
.word banshee_color_params_24bpp ;; color params
;; 640 x 480 x 24
.byte 0x69 ;; mode
.byte 24   ;; depth
.word 640  ;; xres
.word 480  ;; yres
.word banshee_640x480x8_crtc ;; CRTC settings
.word 0,0 ;; pllCtrl0 (0 = unused)
.byte 0x63 ;; misc reg
.byte 0x06 ;; VBE memory model
.word banshee_color_params_24bpp ;; color params
;; 800 x 600 x 24
.byte 0x71
.byte 24
.word 800
.word 600
.word banshee_800x600x8_crtc
.word 0xbc3e,0x0000 ;; 40 MHz
.byte 0xef
.byte 0x06
.word banshee_color_params_24bpp
;; 1024 x 768 x 24
.byte 0x73
.byte 24
.word 1024
.word 768
.word banshee_1024x768x8_crtc
.word 0xe15d,0x0000 ;; 65 MHz
.byte 0xef
.byte 0x06
.word banshee_color_params_24bpp
;; 1280 x 1024 x 24
.byte 0x75
.byte 24
.word 1280
.word 1024
.word banshee_1280x1024x8_crtc
.word 0xb358,0x0000 ;; 108 MHz
.byte 0xef
.byte 0x06
.word banshee_color_params_24bpp
banshee_mode_list_end:
.byte 0xff

banshee_vesa_modelist:
;; 640x400x8
.word 0x0100, 0x005b
;; 640x480x8
.word 0x0101, 0x005f
;; 800x600x8
.word 0x0103, 0x005c
;; 1024x768x8
.word 0x0105, 0x005e
;; 1280x1024x8
.word 0x0107, 0x006b
;; 640x400x16
.word 0x018a, 0x0029
;; 640x400x24
.word 0x018b, 0x002a
;; 640x480x16
.word 0x0111, 0x006e
;; 640x480x24
.word 0x0112, 0x0069
;; 800x600x16
.word 0x0114, 0x0070
;; 800x600x24
.word 0x0115, 0x0071
;; 1024x768x16
.word 0x0117, 0x0072
;; 1024x768x24
.word 0x0118, 0x0073
;; 1280x1024x16
.word 0x011a, 0x0074
;; 1280x1024x24
.word 0x011b, 0x0075
;; invalid
.word 0xffff, 0xffff

banshee_color_params_8bpp:
.byte 0, 0, 0, 0, 0, 0, 0, 0
banshee_color_params_16bpp:
.byte 5, 11, 6, 5, 5, 0, 0, 0
banshee_color_params_24bpp:
.byte 8, 16, 8, 8, 8, 0, 0, 0

;; Banshee init code

banshee_detect:
  push si
  mov  ax,  #0xb103
  mov  ecx, #0x030000
  mov  si,  #0x0000
  int  #0x1a
  jc   not_detected
  mov  cx,  bx
  mov  dl,  #0x00
  call pci_read_reg
  cmp  ax,  #0x121a
  jne  not_detected
  shr  eax, #16
  cmp  ax,  #0x0003
  je   banshee_ok
  cmp  ax,  #0x0005
  jne  not_detected
banshee_ok:
  push ax
  mov  cx, bx
  mov  dl, #0x18
  call pci_read_reg
  mov  dh, ah
  pop  ax
  pop  si
  clc
  ret
not_detected:
  pop  si
  stc
  ret

banshee_init:
  call banshee_detect
  jc   no_banshee
  SET_INT_VECTOR(0x10, #0xC000, #banshee_int10_handler)
  mov  dl, #0x04 ;; pciInit0
  mov  eax, #0x0180f840
  out  dx, eax
  mov  dl, #0x28 ;; vgaInit0
  mov  eax, #0x00000140
  out  dx, eax
  mov  dl, #0x1c ;; dramInit1
  mov  eax, #0x00548031
  out  dx, eax
  mov  dl, #0x18 ;; dramInit0
  mov  eax, #0x0c17a16a
  out  dx, eax
  mov  dl, #0x10 ;; miscInit0
  xor  eax, eax
  out  dx, eax
  mov  dl, #0x34 ;; dramData
  mov  eax, #0x00000037
  out  dx, eax
  mov  dl, #0x30 ;; dramCommand
  mov  eax, #0x0000010d
  out  dx, eax
  mov  dl, #0x34
  mov  eax, #0xffffffff
  out  dx, eax
  mov  dl, #0x30
  mov  eax, #0x0000010e
  out  dx, eax
  mov  dl, #0x14 ;; miscInit1
  mov  eax, #0x00000001
  out  dx, eax
  mov  dl, #0x20 ;; agpInit
  mov  eax, #0x0000049e
  out  dx, eax
  mov  dl, #0x44 ;; pllCtrl1
  mov  eax, #0x00002805
  out  dx, eax
  mov  dl, #0x48 ;; pllCtrl2
  mov  eax, #0x00007919
  out  dx, eax
  mov  dl, #0x0c ;; lfbMemoryConfig
  mov  eax, #0x000a3fff
  out  dx, eax
  push dx
  call get_crtc_address
  pop  ax
  mov  al, #0x1c
  out  dx, ax
  push dx
  mov  dl, #0x14
  call pci_read_reg
  shr  eax, #16
  pop  dx
  mov  al, #0x1d
  out  dx, ax
no_banshee:
  ret

;; out dx:i/o base
banshee_get_io_base_address:
  push ax
  call get_crtc_address
  mov  al, #0x1c
  out  dx, al
  inc  dx
  in   al, dx
  mov  dh, al
  mov  dl, #0x00
  pop  ax
  ret

;; out ax:lfb base (upper 16 bits)
banshee_get_lfb_base_address:
  call get_crtc_address
  mov  al, #0x1d
  out  dx, al
  inc  dx
  in   al, dx
  mov  ah, al
  mov  al, #0x00
  ret

banshee_display_info:
  push ds
  push si
  push cs
  pop  ds
  mov  si, #threedfx_msg
  call _display_string
  call banshee_detect
  mov  si, #banshee_not_installed
  jc   banshee_show_msg
  mov  si, #banshee_msg
  cmp  al, #0x03
  je   is_banshee
  mov  si, #voodoo3_msg
is_banshee:
  push bx
  call _display_string
  pop  bx
  mov  si, #pci_msg
  mov  cx, bx
  mov  dl, #0x18
  call pci_read_reg
  and  ax, #0xfffe
  mov  dh, ah
  mov  dl, #0x38
  in   eax, dx
  test ax, #0x08
  jz   is_pci
  mov  si, #agp_msg
is_pci:
  call _display_string
  mov  si, #detected_msg
banshee_show_msg:
  call _display_string
  pop  si
  pop  ds
  ret

;; Banshee INT 10h handler (set video mode / VBE support)
banshee_int10_handler:
  pushf
  push bp
  cmp  ah, #0x00  ;; set video mode
  jz   banshee_set_video_mode
  cmp  ah, #0x4f  ;; VESA extension
  jnz  banshee_unhandled
  call banshee_vesa
banshee_return:
#ifdef BANSHEE_DEBUG
  call banshee_ret_debug_dump
#endif
  pop  bp
  popf
  iret

banshee_set_video_mode:
#ifdef BANSHEE_DEBUG
  call banshee_call_debug_dump
#endif
  push ax
  push bx
  xor  ax, ax ;; reset VBE mode
  mov  bx, #PM_BIOSMEM_VBE_MODE
  call set_bda_word
  pop  bx
  pop  ax
  push si
  push ax
  call banshee_get_modeentry
  jc   no_banshee_mode
  call banshee_switch_mode
  pop  ax
  test al, #0x80
  jnz  banshee_noclear
  call banshee_clear_vram
banshee_noclear:
  and  al, #0x7f
  call banshee_set_video_mode_bda
  SET_INT_VECTOR(0x43, #0xC000, #_vgafont16)
  pop  si
  mov  al, #0x20
  jmp  banshee_return
no_banshee_mode:
  pop  ax
  pop  si
banshee_unhandled:
  pop  bp
  popf
  jmp  vgabios_int10_handler

#ifdef BANSHEE_DEBUG
banshee_call_debug_dump:
  push es
  push ds
  pusha
  push cs
  pop ds
  call _banshee_call_debugmsg
  popa
  pop ds
  pop es
  ret

banshee_ret_debug_dump:
  push es
  push ds
  pusha
  push cs
  pop ds
  call _banshee_ret_debugmsg
  popa
  pop ds
  pop es
  ret
#endif

banshee_switch_mode:
  push bx
  push cx
  push dx
  push ds
  push cs
  pop  ds
  mov  dx, #VGAREG_SEQU_ADDRESS
  xor  ax, ax
  out  dx, ax
  mov  bx, #banshee_svga_sequ
  mov  cx, #0x0005
  call set_vgacore_regs
  mov  dx, #VGAREG_GRDC_ADDRESS
  mov  bx, #banshee_svga_grdc
  mov  cx, #0x0009
  call set_vgacore_regs
  call get_crtc_address
  mov  ax, #0x11
  out  dx, al
  inc  dx
  in   al, dx
  and  al, #0x7f ;; unlock CRTC
  out  dx, al
  dec  dx
  mov  bx, [si+6]
  mov  cx, #0x001c
  call set_vgacore_regs
  mov  dx, #VGAREG_WRITE_MISC_OUTPUT
  mov  al, [si+12]
  out  dx, al
  mov  dx, #VGAREG_ACTL_RESET
  in   al, dx
  mov  dx, #VGAREG_ACTL_ADDRESS
  mov ax, #0x10
  out  dx, al
  mov ax, #0x01
  out  dx, al
  call banshee_get_io_base_address
  xor  eax, eax
  mov  dl, #0x4c ;; dacMode
  out  dx, eax
  mov  dl, #0xe4 ;; vidDesktopStartAddr
  out  dx, eax
  mov  dl, #0x5c ;; reset vidProcCfg
  out  dx, eax
  mov  eax, [si+8]
  or   ax, ax
  jz   no_pll_setup
  mov  dl, #0x40 ;; pllCtrl0
  out  dx, eax
no_pll_setup:
  mov  ax, [si+4]
  shl  eax, #12
  or   ax, [si+2]
  mov  dl, #0x98 ;; vidScreenSize
  out  dx, eax
  and  eax, #0x00000fff
  mov  bl, [si+1]
  xor  bh, bh
  shr  bx, #3
  cmp  bl, #0x01
  jbe  set_mode_8bpp_1
  push dx
  xor  dx, dx
  mul  bx
  pop  dx
set_mode_8bpp_1:
  mov  dl, #0xe8 ;; vidDesktopOverlayStride
  out  dx, eax
  mov  eax, #0x00001140
  mov  dl, #0x28 ;; vgaInit0
  out  dx, eax
  mov  eax, #0x00100000
  mov  dl, #0x2c ;; vgaInit1
  out  dx, eax
  mov  eax, #0x00000081
  dec  bx
  jz   set_mode_8bpp_2
  and  ebx, #0x07
  shl  ebx, #18
  or   eax, ebx
  or   ax, #0x0c00
set_mode_8bpp_2:
  mov  dl, #0x5c ;; vidProcCfg
  out  dx, eax
  pop  ds
  pop  dx
  pop  cx
  pop  bx
  ret

banshee_get_modeentry:
  and  al, #0x7f
  mov  si, #banshee_modes
bgm_1:
 db 0x2e ;; cs:
  mov  ah, [si]
  cmp  al, ah
  jz   bgm_2
  cmp  ah, #0xff
  jz   bgm_4
  add  si, #BANSHEE_MODE_SIZE
  jmp  bgm_1
bgm_4:
  xor  si, si
  stc ;; video mode is not supported
  ret
bgm_2:
  clc ;; video mode is supported
  ret

;; Banshee set mode helper functions

banshee_set_video_mode_bda:
  push bx
  push cx
  push ds
  push cs
  pop ds
  mov bx, [si+2]
  shr bx, #3
  mov cx, [si+4]
  shr cx, #4 ;; cheight 16
  dec cx
  xor si, si
  mov ds, si
  mov [PM_BIOSMEM_CURRENT_MODE], al
  mov [PM_BIOSMEM_NB_COLS], bx
  mov [PM_BIOSMEM_NB_ROWS], cl
  mov al, #0x10
  xor ah, ah
  mov [PM_BIOSMEM_CHAR_HEIGHT], ax
  xor ax, ax
  mov [PM_BIOSMEM_CURSOR_POS], ax
  pop ds
  pop cx
  pop bx
  ret

set_bda_word:
  push ds
  push si
  xor  si, si
  mov  ds, si
  mov  [bx], ax
  pop  si
  pop  ds
  ret

set_vgacore_regs:
  push ds
  push cs
  pop  ds
  mov  al, #0x00
  cld
setregs_loop:
  mov  ah, [bx]
  out  dx, ax
  inc  bx
  inc  al
  loop setregs_loop
  pop  ds
  ret

banshee_clear_vram:
  push ax
  push bx
  push cx
  push dx
  push di
  push es
  mov  ax, #0xa000
  mov  es, ax
  call banshee_get_io_base_address
  mov  dl, #0x2c ;; vgaInit1
  xor  bx, bx
clear_loop:
  in   eax, dx
  and  eax, #0xfffffc00
  and  bx, #0x03ff
  or   ax, bx
  out  dx, eax
  mov  cx, #0x4000
  xor  ax, ax
  xor  di, di
  cld
  rep
    stosw
  inc  bx
  cmp  bx, #0x0200
  jb   clear_loop
  pop  es
  pop  di
  pop  dx
  pop  cx
  pop  bx
  pop  ax
  ret

;; Banshee i/o setup in VGA mode

banshee_set_vga_mode:
  push ds
  push ax
  push bx
  push cx
  push dx
  call banshee_get_io_base_address
  or   dh, dh
  jz   no_banshee2
  mov  eax, #0x00000140
  mov  dl, #0x28 ;; vgaInit0
  out  dx, eax
  xor  eax, eax
  mov  dl, #0x2c ;; vgaInit1
  out  dx, eax
  mov  dl, #0x4c ;; dacMode
  out  dx, eax
  mov  dl, #0xe4 ;; vidDesktopStartAddr
  out  dx, eax
  xor  ax, ax
  mov  ds, ax
  push dx
  mov  dx, #VGAREG_GRDC_ADDRESS
  mov  al, #0x06
  out  dx, al
  inc  dx
  in   al, dx
  and  al, #0x01
  mov  cl, al
  dec  dx
  mov  al, #0x05
  out  dx, al
  inc  dx
  in   al, dx
  test al, #0x40
  jz   no_256col
  or   cl, #0x02
no_256col:
  pop  dx
  test cl, #0x01
  jnz  vga_gfx_mode
  xor  ax, ax
  call get_crtc_xres
  mov  bx, ax
  xor  eax, eax
  call get_crtc_yres
  shr  ax, #4 ;; FIXME: use char height
  shl  eax, #12
  or   ax, bx
  mov  dl, #0x98 ;; vidScreenSize
  out  dx, eax
  xor  eax, eax
  mov  al, bl
  shl  ax, #1
  mov  dl, #0xe8 ;; vidDesktopOverlayStride
  out  dx, eax
  jmp  banshee_set_vga_mode_2
vga_gfx_mode:
  xor  ax, ax
  call get_crtc_xres
  mov  bx, ax
  test cl, #0x02
  jz   vga_16col_1
  shr  bx, #1
vga_16col_1:
  xor  eax, eax
  call get_crtc_yres
  test cl, #0x02
  jz   vga_16col_2
  shr  ax, #1
vga_16col_2:
  shl  eax, #12
  push bx
  shl  bx, #3
  or   ax, bx
  mov  dl, #0x98 ;; vidScreenSize
  out  dx, eax
  pop  ax
  test cl, #0x02
  jz   vga_16col_3
  shl  ax, #3
vga_16col_3:
  and  eax, #0xffff
  mov  dl, #0xe8 ;; vidDesktopOverlayStride
  out  dx, eax
banshee_set_vga_mode_2:
  xor  eax, eax
  mov  dl, #0x5c ;; vidProcCfg
  out  dx, eax
  mov  dl, #0x1c ;; dramInit1
  mov  eax, #0x00548031
  out  dx, eax
no_banshee2:
  pop  dx
  pop  cx
  pop  bx
  pop  ax
  pop  ds
  ret

;; VGA compatibility helper functions

get_crtc_xres:
  push dx
  call get_crtc_address
  mov  al, #0x01
  out  dx, al
  inc  dx
  in   al, dx
  inc  al
  pop  dx
  ret

get_crtc_yres:
  push bx
  push dx
  call get_crtc_address
  mov  al, #0x12
  out  dx, al
  inc  dx
  in   al, dx
  mov  bl, al
  xor  bh, bh
  dec  dx
  mov  al, #0x07
  out  dx, al
  inc  dx
  in   al, dx
  test al, #0x02
  jz   no_bit8
  or   bh, #0x01
no_bit8:
  test al, #0x40
  jz   no_bit9
  or   bh, #0x02
no_bit9:
  mov  ax, bx
  inc  ax
  pop  dx
  pop  bx
  ret

;; code for 'write character' support in 8bpp graphics modes

;; called from C code
_banshee_is_8bpp_mode:
banshee_is_8bpp_mode:
  push dx
  call banshee_get_io_base_address
  mov  dl, #0x5c ;; vidProcCfg
  in   eax, dx
  pop  dx
  test ax, #0x01
  jz   no_banshee_8bpp_mode
  shr  eax, #18
  and  al, #0x07
  inc  al
  cmp  al, #0x01
  jz   banshee_8bpp_mode
no_banshee_8bpp_mode:
  xor  ax, ax
banshee_8bpp_mode:
  ret

banshee_8bpp_write_char:
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
  mov  bx, #0x10c ;; INT 0x43
  mov  si, [bx]
  mov  ax, [bx+2]
  mov  ds, ax
  mov  bx, 14[bp] ;; cheight
  mov  al, 4[bp]  ;; character
  mul  bl
  add  si, ax
  mov  ax, 8[bp] ;; xcurs
  shl  ax, #3
  push ax
  mov  dx, 10[bp] ;; ycurs
  or   dx, dx
  jz   banshee_wc_set_start
  mov  al, 12[bp] ;; nbcols
  mov  bl, 14[bp] ;; cheight
  mul  bl
  shl  ax, #3
  mov  bx, dx
  mul  bx
banshee_wc_set_start:
  pop  bx
  add  ax, bx
  jnc  banshee_wc_noinc1
  inc  dx
banshee_wc_noinc1:
  shl  dx, #1
  test ax, #0x8000
  jz   banshee_wc_noinc2
  and  ax, #0x7fff
  or   dx, #0x0001
banshee_wc_noinc2:
  mov  di, ax
  mov  ax, dx
  mov  bl, #0x01
  call banshee_set_bank
  mov  ax, #0xa000
  mov  es, ax
  mov  bl, 14[bp] ;; cheight
  mov  dx, 12[bp] ;; nbcols
  shl  dx, #3
banshee_wc_loop1:
  push di
  lodsb
  mov  cx, #0x08
  cld
banshee_wc_loop2:
  shl  al, #1
  push ax
  jnc  banshee_wc_set_bkgnd
  mov  al, 6[bp] ;; attr
  db    0xa9 ;; skip next opcode (TEST AX, #0xC030)
banshee_wc_set_bkgnd:
  xor  al, al
  stosb
  pop  ax
  loop banshee_wc_loop2
  pop  di
  dec  bl
  jz   banshee_wc_end
  add  di, dx
  jmp  banshee_wc_loop1
banshee_wc_end:
  pop  di
  pop  si
  pop  es
  pop  ds
  pop  dx
  pop  cx
  pop  bx
  pop  ax
  ret

banshee_8bpp_copy:
  push ax
  push bx
  push cx
  push dx
  push ds
  push es
  push si
  push di
  mov  ax, 4[bp] ;; xstart
  shl  ax, #3
  push ax
  mov  al, 12[bp] ;; nbcols
  mov  bl, 14[bp] ;; cheight
  mul  bl
  shl  ax, #3
  mov  cx, ax
  xor  ax, ax
  mov  dx, 6[bp] ;; ysrc
  or   dx, dx
  jz   banshee_copy_set_start_src
  mov  ax, cx
  mov  bx, dx
  mul  bx
banshee_copy_set_start_src:
  pop  bx
  push bx
  add  ax, bx
  jnc  banshee_copy_noinc1s
  inc  dx
banshee_copy_noinc1s:
  shl  dx, #1
  test ax, #0x8000
  jz   banshee_copy_noinc2s
  and  ax, #0x7fff
  or   dx, #0x0001
banshee_copy_noinc2s:
  mov  si, ax
  mov  ax, dx
  mov  bl, #0x02
  call banshee_set_bank
  xor  ax, ax
  mov  dx, 8[bp] ;; ydest
  or   dx, dx
  jz   banshee_copy_set_start_dst
  mov  ax, cx
  mov  bx, dx
  mul  bx
banshee_copy_set_start_dst:
  pop  bx
  add  ax, bx
  jnc  banshee_copy_noinc1d
  inc  dx
banshee_copy_noinc1d:
  shl  dx, #1
  test ax, #0x8000
  jz   banshee_copy_noinc2d
  and  ax, #0x7fff
  or   dx, #0x0001
banshee_copy_noinc2d:
  mov  di, ax
  mov  ax, dx
  mov  bl, #0x01
  call banshee_set_bank
  mov  ax, #0xa000
  mov  ds, ax
  mov  es, ax
  mov  bl, 14[bp] ;; cheight
  mov  cx, 10[bp] ;; cols
  shl  cx, #2
  mov  dx, 12[bp] ;; nbcols
  shl  dx, #3
banshee_copy_loop:
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
  jz   banshee_copy_end
  add  si, dx
  add  di, dx
  jmp  banshee_copy_loop
banshee_copy_end:
  pop  di
  pop  si
  pop  es
  pop  ds
  pop  dx
  pop  cx
  pop  bx
  pop  ax
  ret

banshee_8bpp_fill:
  push ax
  push bx
  push cx
  push dx
  push es
  push di
banshee_fill_next_row:
  mov  ax, 4[bp] ;; xstart
  shl  ax, #3
  push ax
  xor  ax, ax
  mov  dx, 6[bp] ;; ystart
  or   dx, dx
  jz   banshee_fill_set_start
  mov  al, 12[bp] ;; nbcols
  mov  bl, 14[bp] ;; cheight
  mul  bl
  shl  ax, #3
  mov  bx, dx
  mul  bx
banshee_fill_set_start:
  pop  bx
  add  ax, bx
  jnc  banshee_fill_noinc1
  inc  dx
banshee_fill_noinc1:
  shl  dx, #1
  test ax, #0x8000
  jz   banshee_fill_noinc2
  and  ax, #0x7fff
  or   dx, #0x0001
banshee_fill_noinc2:
  mov  di, ax
  mov  ax, dx
  mov  bl, #0x01
  call banshee_set_bank
  mov  ax, #0xa000
  mov  es, ax
  mov  al, 16[bp] ;; attr
  mov  ah, al
  mov  bl, 14[bp] ;; cheight
  mov  cx, 8[bp] ;; cols
  shl  cx, #2
  mov  dx, 12[bp] ;; nbcols
  shl  dx, #3
banshee_fill_loop:
  push cx
  push di
  cld
  rep
    stosw
  pop  di
  pop  cx
  dec  bl
  jz   banshee_fill_check_row
  add  di, dx
  jmp  banshee_fill_loop
banshee_fill_check_row:
  inc  6[bp] ;; ystart
  dec  10[bp] ;; rows
  jnz  banshee_fill_next_row
  pop  di
  pop  es
  pop  dx
  pop  cx
  pop  bx
  pop  ax
  ret

;; Banshee VBE support

banshee_vesa:
#ifdef BANSHEE_DEBUG
  call banshee_call_debug_dump
#endif
  cmp  al, #0x15
  ja   banshee_vesa_unimplemented
  push bx
  xor  bx, bx
  mov  bl, al
  shl  bx, 1
 db 0x2e ;; cs:
  mov  bp, banshee_vesa_handlers[bx]
  pop  bx
  push bp
  ret

banshee_vesa_unimplemented:
  mov  ax, #0x014F ;; not implemented
  ret

banshee_vesa_00h:
  push ds
  push si
  mov  bp, di
  push es
  pop  ds
  cld
  mov  ax, [di]
  cmp  ax, #0x4256 ;; VB
  jnz  bv00_1
  mov  ax, [di+2]
  cmp  ax, #0x3245 ;; E2
  jnz  bv00_1
  ;; VBE2
  lea  di, 0x14[bp]
  mov  ax, #0x0100 ;; soft ver.
  stosw
  mov  ax, # banshee_vesa_vendorname
  stosw
  mov  ax, cs
  stosw
  mov  ax, # banshee_vesa_productname
  stosw
  mov  ax, cs
  stosw
  mov  ax, # banshee_vesa_productrevision
  stosw
  mov  ax, cs
  stosw
bv00_1:
  mov  di, bp
  mov  ax, #0x4556 ;; VE
  stosw
  mov  ax, #0x4153 ;; SA
  stosw
  mov  ax, #0x0200 ;; v2.00
  stosw
  mov  ax, # banshee_vesa_oemname
  stosw
  mov  ax, cs
  stosw
  xor  ax, ax ;; caps
  stosw
  stosw
  lea  ax, 0x40[bp]
  stosw
  mov  ax, es
  stosw
  mov  ax, #0x0100;; vram in 64k
  stosw

  push cs
  pop  ds
  lea  di, 0x40[bp]
  mov  si, #banshee_vesa_modelist
bv00_2:
  lodsw
  stosw
  add  si, #2
  cmp  ax, #0xffff
  jnz  bv00_2

  mov  ax, #0x004F
  mov  di, bp
  pop  si
  pop  ds
  ret

banshee_vesa_01h:
  mov  ax, cx
  and  ax, #0x3fff
  call banshee_vesamode_to_mode
  cmp  ax, #0xffff
  jnz  banshee_vesa_01h_1
  jmp  banshee_vesa_unimplemented
banshee_vesa_01h_1:
  push ds
  push si
  push cx
  push dx
  push bx
  mov  bp, di
  cld
  push cs
  pop  ds
  call banshee_get_modeentry

  push di
  xor  ax, ax
  mov  cx, #0x80
  rep
    stosw ;; clear buffer
  pop  di

  mov  ax, #0x003b ;; mode attr
  push bx
  mov  bl, [si+1] ;; bpp
  cmp  bl, #0x08
  jne  banshee_vesa_no_8bpp
  or   al, #0x04 ;; TTY support
banshee_vesa_no_8bpp:
  pop  bx
  stosw
  mov  ax, #0x0007 ;; win attr
  stosw
  mov  ax, #0x0020 ;; granularity =32K
  stosw
  mov  ax, #0x0040 ;; size =64K
  stosw
  mov  ax, #0xA000 ;; segment A
  stosw
  xor  ax, ax ;; no segment B
  stosw
  mov  ax, #banshee_vesa_05h_farentry
  stosw
  mov  ax, cs
  stosw
  call banshee_get_line_offset_entry
  stosw ;; bytes per scan line
  mov  ax, [si+2] ;; width
  stosw
  mov  ax, [si+4] ;; height
  stosw
  mov  ax, #0x08
  stosb
  mov  ax, #0x10
  stosb
  mov  al, #1 ;; count of planes
  stosb
  mov  al, [si+1] ;; bpp
  stosb
  mov  al, #0x1 ;; XXX number of banks
  stosb
  mov  al, [si+13] ;; memory model
  stosb
  mov  al, #0x0   ;; XXX size of bank in K
  stosb
  call banshee_get_line_offset_entry
  mov  bx, [si+4]
  mul  bx ;; dx:ax=vramdisp
  or   ax, ax
  jz   banshee_vesa_01h_3
  inc  dx
banshee_vesa_01h_3:
  mov  ax, #0x0100 ;; vram in 64k
  mov  cx, dx
  xor  dx, dx
  div  cx
  dec  ax
  stosb  ;; number of image pages = vramtotal/vramdisp-1
  mov  al, #0x00
  stosb

  ;; v1.2+ stuffs
  mov  ax, [si+14]
  push si
  mov  si, ax
  movsw
  movsw
  movsw
  movsw
  pop  si

  mov  ah, [si+1]
  mov  al, #0x0
  sub  ah, #9
  rcl  al, #1 ;; bit 0=palette flag
  stosb ;; direct screen mode info

  ;; v2.0+ stuffs
  ;; 32-bit LFB address
  xor  ax, ax
  stosw
  call banshee_get_lfb_base_address
  stosw
  or   ax, ax
  jz banshee_vesa_01h_4
  push di
  mov  di, bp
 db 0x26 ;; es:
  mov  ax, [di]
  or   ax, #0x0080 ;; mode bit 7:LFB
  stosw
  pop  di
banshee_vesa_01h_4:

  xor  ax, ax
  stosw ;; reserved
  stosw ;; reserved
  stosw ;; reserved

  mov  ax, #0x004F
  mov  di, bp
  pop  bx
  pop  dx
  pop  cx
  pop  si
  pop  ds

  test cx, #0x4000 ;; LFB flag
  jz banshee_vesa_01h_5
  push cx
 db 0x26 ;; es:
  mov  cx, [di]
  test cx, #0x0080 ;; is LFB supported?
  jnz  banshee_vesa_01h_6
  mov  ax, #0x014F ;; error - no LFB
banshee_vesa_01h_6:
  pop  cx
banshee_vesa_01h_5:
  ret

banshee_vesa_02h:
  test bx, #0x3e00
  jnz  banshee_vesa_unimplemented ;; unknown flags
  mov  ax, bx
  and  ax, #0x01ff
  cmp  ax, #0x0100
  jb   banshee_vesa_set_mode
  call banshee_vesamode_to_mode
  cmp  ax, #0xffff
  je   banshee_vesa_unimplemented
  test bx, #0x8000
  jz   banshee_vesa_set_mode
  or   al, #0x80
banshee_vesa_set_mode:
  int  #0x10
  push bx
  mov  ax, bx
  mov  bx, #PM_BIOSMEM_VBE_MODE
  call set_bda_word
  pop  bx
  mov  ax, #0x004F
  ret

banshee_vesa_03h:
  push ds
  xor  ax, ax
  mov  ds, ax
  mov  bx, #PM_BIOSMEM_VBE_MODE
  mov  ax, [bx]
  mov  bx, ax
  test bx, bx
  jnz  banshee_vesa_03h_1
  mov  bx, #PM_BIOSMEM_CURRENT_MODE
  mov  al, [bx]
  mov  bl, al
  xor  bh, bh
banshee_vesa_03h_1:
  mov  ax, #0x004f
  pop  ds
  ret

banshee_vesa_05h_farentry:
  call banshee_vesa_05h
  retf

banshee_vesa_05h:
  cmp  bl, #0x00
  jne  banshee_vesa_05h_failed
  cmp  bh, #0x01
  je   banshee_vesa_05h_get
  jb   banshee_vesa_05h_set
  mov  ax, #0x0100
  ret
banshee_vesa_05h_set:
  mov  ax, dx
  push bx
  mov  bl, #0x03
  call banshee_set_bank
  pop  bx
  cmp  ax, dx
  jne  banshee_vesa_05h_failed
  mov  ax, #0x004f
  ret
banshee_vesa_05h_get:
  call banshee_get_bank
  mov  dx, ax
  mov  ax, #0x004f
  ret
banshee_vesa_05h_failed:
  mov  ax, #0x014f
  ret

banshee_vesa_06h:
  mov  ax, cx
  cmp  bl, #0x01
  jb   banshee_vesa_06h_bl0
  je   banshee_vesa_06h_bl1
  cmp  bl, #0x03
  jb   banshee_vesa_06h_bl2
  je   banshee_vesa_06h_bl3
  mov  ax, #0x0100
  ret
banshee_vesa_06h_bl0:
  call banshee_get_bpp_bytes
  mov  bl, al
  xor  bh, bh
  mov  ax, cx
  mul  bx
banshee_vesa_06h_bl2:
  call banshee_set_line_offset
banshee_vesa_06h_bl1:
  call banshee_get_bpp_bytes
  mov  bl, al
  xor  bh, bh
  xor  dx, dx
  call banshee_get_line_offset
  push ax
  div  bx
  mov  cx, ax
  pop  bx
  mov  dx, #0x100 ;; vram in 64k
  xor  ax, ax
  div  bx
  mov  dx, ax
  mov  ax, #0x004f
  ret
banshee_vesa_06h_bl3:
  push dx
  call banshee_get_io_base_address
  mov  dl, #0x98 ;; vidScreenSize
  in   eax, dx
  shr  eax, #12
  mov  bx, ax
  mov  dx, #0x100 ;; vram in 64k
  xor  ax, ax
  div  bx
  push ax
  call banshee_get_bpp_bytes
  mov  cl, al
  xor  ch, ch
  pop  ax
  mov  bx, ax
  div  cx
  mov  cx, ax
  pop  dx
  mov  ax, #0x004f
  ret

banshee_vesa_07h:
  cmp  bl, #0x80
  je   banshee_vesa_07h_bl0
  cmp  bl, #0x01
  je   banshee_vesa_07h_bl1
  jb   banshee_vesa_07h_bl0
  mov  ax, #0x0100
  ret
banshee_vesa_07h_bl0:
  push dx
  call banshee_get_bpp_bytes
  mov  bl, al
  xor  bh, bh
  mov  ax, cx
  mul  bx
  pop  bx
  push ax
  call banshee_get_line_offset
  mul  bx
  pop  bx
  add  ax, bx
  jnc  banshee_vesa_07h_noinc
  inc  dx
banshee_vesa_07h_noinc:
  call banshee_set_start_addr
  mov  ax, #0x004f
  ret
banshee_vesa_07h_bl1:
  call banshee_get_start_addr
  push ax
  call banshee_get_line_offset
  mov  bx, ax
  pop  ax
  div  bx
  push ax
  push dx
  call banshee_get_bpp_bytes
  mov  bl, al
  xor  bh, bh
  pop  ax
  xor  dx, dx
  div  bx
  mov  cx, ax
  pop  dx
  mov  ax, #0x004f
  ret

banshee_vesa_15h:
  cmp bl, #0x01
  jb  banshee_vesa_get_capabilities
  je  banshee_vesa_read_EDID
  jmp banshee_vesa_unimplemented

banshee_vesa_get_capabilities:
  test cx, cx
  jne  banshee_vesa_unimplemented
  mov  ax, #0x004f
  mov  bx, #0x0202
  ret

banshee_vesa_read_EDID:
  call banshee_ddc_init
  jnz  banshee_vesa_unimplemented
  call banshee_ddc_start
  call banshee_ddc_delay
  mov  al, #0xa0
  call banshee_ddc_send_byte
  jc   banshee_vesa_unimplemented
  mov  al, #0x00
  call banshee_ddc_send_byte
  jc   banshee_vesa_unimplemented
  call banshee_ddc_stop
  call banshee_ddc_start
  call banshee_ddc_delay
  mov  al, #0xa1
  call banshee_ddc_send_byte
  jc   banshee_vesa_unimplemented
  push cx
  push di
  mov  cx, #0x0080
  cld
banshee_vesa_15h_01:
  call banshee_ddc_read_byte
  stosb
  call banshee_ddc_send_status
  loop banshee_vesa_15h_01
  call banshee_ddc_stop
  pop  di
  pop  cx
  mov  ax, #0x004f
  ret

;; VBE helper functions

;; in ax:vesamode, out ax:bansheemode
banshee_vesamode_to_mode:
  push ds
  push cx
  push si
  push cs
  pop  ds
  mov  cx, #0xffff
  mov  si, #banshee_vesa_modelist
bvtm_1:
  cmp  [si], ax
  jz   bvtm_2
  cmp  [si], cx
  jz   bvtm_2
  add  si, #4
  jmp  bvtm_1
bvtm_2:
  mov  ax,[si+2]
  pop  si
  pop  cx
  pop  ds
  ret

banshee_get_bank:
  push bx
  push cx
  push dx
  call banshee_get_io_base_address
  mov  dl, #0x2c ;; vgaInit1
  in   eax, dx
  and  ax, #0x03ff
  pop  dx
  pop  cx
  pop  bx
  ret

;; in AX:requested bank, BL:r/w mode, out AX:set bank
banshee_set_bank:
  push cx
  push dx
  push ax
  call banshee_get_io_base_address
  mov  dl, #0x2c ;; vgaInit1
  in   eax, dx
  pop  cx
  and  cx, #0x03ff
  test bl, #0x01
  jz   banshee_set_bank_1
  and  eax, #0xfffffc00
  or   ax, cx
banshee_set_bank_1:
  test bl, #0x02
  jz   banshee_set_bank_2
  push cx
  and  eax, #0xfff003ff
  shl  ecx, #10
  or   eax, ecx
  pop  cx
banshee_set_bank_2:
  out  dx, eax
  mov  ax, cx
  pop  dx
  pop  cx
  ret

banshee_get_line_offset_entry:
  mov  ax, [si+2] ;; width
  mov  bl, [si+1] ;; bpp
  xor  bh, bh
  shr  bx, #3
  mul  bx
  ret

banshee_get_bpp_bytes:
  call banshee_get_io_base_address
  mov  dl, #0x5c ;; vidProcCfg
  in   eax, dx
  shr  eax, #18
  and  ax, #0x03
  inc  al
  ret

banshee_get_line_offset:
  push dx
  call banshee_get_io_base_address
  mov  dl, #0xe8 ;; vidDesktopOverlayStride
  in   eax, dx
  and  ax, #0x7fff
  pop  dx
  ret

banshee_set_line_offset:
  push bx
  push ax
  call banshee_get_io_base_address
  mov  dl, #0xe8 ;; vidDesktopOverlayStride
  in   eax, dx
  pop  bx
  and  bx, #0x7fff
  or   ax, bx
  out  dx, eax
  pop  bx
  ret

banshee_get_start_addr:
  call banshee_get_io_base_address
  mov  dl, #0xe4 ;; vidDesktopStartAddr
  in   eax, dx
  mov  edx, eax
  shr  edx, #16
  ret

banshee_set_start_addr:
  push ax
  mov  ax, dx
  shl  eax, #16
  pop  ax
  call banshee_get_io_base_address
  mov  dl, #0xe4 ;; vidDesktopStartAddr
  out  dx, eax
  ret

;; DDC helper functions for VESA 15h

banshee_ddc_init:
  call banshee_get_io_base_address
  mov  dl, #0x78 ;; vidSerialParallelPort
  in   eax, dx
  or   eax, #0x040000
  out  dx, eax
  call banshee_ddc_clr_dck
  in   eax, dx
  shr  eax, #19
  and  al, #0x04
  pushf
  call banshee_ddc_set_dck
  popf
  ret

banshee_ddc_delay:
  in   al, 0x61
  and  al, #0x10
  mov  ah, al
banshee_ddc_delay_01:
  nop
  in   al, 0x61
  and  al, #0x10
  cmp  al, ah
  jz   banshee_ddc_delay_01
  ret

banshee_ddc_set_dck:
  mov  al, #0x01
  db   0xa9 ;; skip next opcode (TEST AX, #0x02b0)
banshee_ddc_set_dda:
  mov  al, #0x02
  push ebx
  xor  ebx, ebx
  mov  bl, al
  shl  ebx, #19
  in   eax, dx
  or   eax, ebx
  out  dx, eax
  pop  ebx
  ret

banshee_ddc_clr_dck:
  mov  al, #0x01
  db   0xa9 ;; skip next opcode (see above)
banshee_ddc_clr_dda:
  mov  al, #0x02
  push ebx
  xor  ebx, ebx
  mov  bl, al
  shl  ebx, #19
  xor  ebx, #0xffffffff
  in   eax, dx
  and  eax, ebx
  out  dx, eax
  pop  ebx
  ret

banshee_ddc_start:
  call banshee_ddc_clr_dda
  call banshee_ddc_clr_dck
  ret

banshee_ddc_stop:
  call banshee_ddc_set_dck
  call banshee_ddc_set_dda
  ret

banshee_ddc_get_dda:
  in   eax, dx
  shr  eax, #23
  ret

banshee_ddc_send_bit:
  push ax
  pushf
  call banshee_ddc_delay
  popf
  jc   banshee_ddc_send_bit_01
  call banshee_ddc_clr_dda
  jmp  banshee_ddc_send_bit_02
banshee_ddc_send_bit_01:
  call banshee_ddc_set_dda
banshee_ddc_send_bit_02:
  call banshee_ddc_set_dck
  call banshee_ddc_delay
  call banshee_ddc_clr_dck
  pop  ax
  ret

banshee_ddc_read_bit:
  push ax
  call banshee_ddc_delay
  call banshee_ddc_set_dck
  call banshee_ddc_get_dda
  pushf
  call banshee_ddc_delay
  call banshee_ddc_clr_dck
  popf
  pop  ax
  ret

banshee_ddc_send_byte:
  push cx
  mov  cx, #0x08
banshee_ddc_send_byte_01:
  shl  al, #0x01
  call banshee_ddc_send_bit
  loop banshee_ddc_send_byte_01
  call banshee_ddc_set_dda
  call banshee_ddc_delay
  call banshee_ddc_set_dck
  call banshee_ddc_get_dda
  pushf
  call banshee_ddc_clr_dck
  call banshee_ddc_clr_dda
  popf
  pop  cx
  ret

banshee_ddc_read_byte:
  push cx
  call banshee_ddc_set_dda
  mov  al, #0x00
  mov  cx, #0x08
banshee_ddc_read_byte_01:
  call banshee_ddc_read_bit
  rcl  al, #0x01
  loop banshee_ddc_read_byte_01
  pop  cx
  ret

banshee_ddc_send_status:
  cmp  cx, #0x01
  jz   banshee_ddc_send_status_01
  call banshee_ddc_clr_dda
banshee_ddc_send_status_01:
  call banshee_ddc_delay
  call banshee_ddc_set_dck
  call banshee_ddc_delay
  call banshee_ddc_clr_dck
  ret

banshee_vesa_handlers:
  ;; 00h
  dw banshee_vesa_00h
  dw banshee_vesa_01h
  dw banshee_vesa_02h
  dw banshee_vesa_03h
  ;; 04h
  dw banshee_vesa_unimplemented
  dw banshee_vesa_05h
  dw banshee_vesa_06h
  dw banshee_vesa_07h
  ;; 08h
  dw banshee_vesa_unimplemented
  dw banshee_vesa_unimplemented
  dw banshee_vesa_unimplemented
  dw banshee_vesa_unimplemented
  ;; 0Ch
  dw banshee_vesa_unimplemented
  dw banshee_vesa_unimplemented
  dw banshee_vesa_unimplemented
  dw banshee_vesa_unimplemented
  ;; 10h
  dw banshee_vesa_unimplemented
  dw banshee_vesa_unimplemented
  dw banshee_vesa_unimplemented
  dw banshee_vesa_unimplemented
  ;; 14h
  dw banshee_vesa_unimplemented
  dw banshee_vesa_15h
ASM_END

#ifdef BANSHEE_DEBUG
static void banshee_call_debugmsg(DI, SI, BP, SP, BX, DX, CX, AX, DS, ES, FLAGS)
  Bit16u DI, SI, BP, SP, BX, DX, CX, AX, ES, DS, FLAGS;
{
  printf("vgabios banshee call ah%02x al%02x bx%04x cx%04x dx%04x\n",GET_AH(),GET_AL(),BX,CX,DX);
}

static void banshee_ret_debugmsg(DI, SI, BP, SP, BX, DX, CX, AX, DS, ES, FLAGS)
  Bit16u DI, SI, BP, SP, BX, DX, CX, AX, ES, DS, FLAGS;
{
  printf("vgabios banshee ret  ah%02x al%02x bx%04x cx%04x dx%04x\n",GET_AH(),GET_AL(),BX,CX,DX);
}
#endif
