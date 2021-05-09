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

#define BANSHEE_MODE_SIZE 13

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
;; 640 x 480 x 8
.byte 0x5f
.byte 8
.word 640
.word 480
.word banshee_640x480x8_crtc
.word 0,0
.byte 0xe3
;; 800 x 600 x 8
.byte 0x5c
.byte 8
.word 800
.word 600
.word banshee_800x600x8_crtc
.word 0xbc3e,0x0000 ;; 40 MHz
.byte 0xef
;; 1024 x 768 x 8
.byte 0x5e
.byte 8
.word 1024
.word 768
.word banshee_1024x768x8_crtc
.word 0xe15d,0x0000 ;; 65 MHz
.byte 0xef
;; 1280 x 1024 x 8
.byte 0x6b
.byte 8
.word 1280
.word 1024
.word banshee_1280x1024x8_crtc
.word 0xb358,0x0000 ;; 108 MHz
.byte 0xef
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
;; invalid
.word 0xffff, 0xffff

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
no_banshee:
  ret

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
  pop  bp
  popf
  iret

banshee_set_video_mode:
  push ax
  push bx
  xor  ax, ax ;; reset VBE mode
  mov  bx, #BIOSMEM_VBE_MODE
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
  push bx
  and  al, #0x7f
  mov  bx, #0x0049
  call set_bda_byte
  pop  bx
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
  mov  dl, #0xe8 ;; vidDesktopOverlayStride
  out  dx, eax
  mov  eax, #0x00001140
  mov  dl, #0x28 ;; vgaInit0
  out  dx, eax
  mov  eax, #0x00100000
  mov  dl, #0x2c ;; vgaInit1
  out  dx, eax
  mov  eax, #0x00000081
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

;; Banshee mode helper functions

set_bda_byte:
  push ds
  push si
  mov  si, #0x40
  mov  ds, si
  mov  [bx], al
  pop  si
  pop  ds
  ret

set_bda_word:
  push ds
  push si
  mov  si, #0x40
  mov  ds, si
  mov  [bx], ax
  pop  si
  pop  ds
  ret

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
  mov  ax, #BIOSMEM_SEG
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

;; Banshee VBE support

banshee_vesa:
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

  mov  ax, #0x003b ;; mode
  stosw
  mov  ax, #0x0007 ;; attr
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
  mov  al, #0x04 ;; memory model: packed pixel
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

  ;; v1.2+ stuffs (TODO)
;  push si
;  add  si, #18
;  movsw
;  movsw
;  movsw
;  movsw
;  pop  si
;; HACK
  xor  ax, ax
  stosw
  stosw
  stosw
  stosw

  mov  ah, [si+1]
  mov  al, #0x0
  sub  ah, #9
  rcl  al, #1 ; bit 0=palette flag
  stosb ;; direct screen mode info

  ;; v2.0+ stuffs
  ;; 32-bit LFB address
  xor  ax, ax
  stosw
  call banshee_get_lfb_addr
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
  stosw ; reserved
  stosw ; reserved
  stosw ; reserved

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
  cmp  cx, #0x0080 ;; is LFB supported?
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
  and  ax, #0x7fff
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
  mov  bx, #BIOSMEM_VBE_MODE
  call set_bda_word
  pop  bx
  mov  ax, #0x004F
  ret

banshee_vesa_03h:
  push ds
  mov  ax, #0x0040
  mov  ds, ax
  mov  bx, #BIOSMEM_VBE_MODE
  mov  ax, [bx]
  mov  bx, ax
  test bx, bx
  jnz  banshee_vesa_03h_1
  mov  bx, #BIOSMEM_CURRENT_MODE
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
  call banshee_set_bank
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
  mov  ax, #0x0100 ;; TODO
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

banshee_get_lfb_addr:
  push bx
  push cx
  push dx
  call banshee_get_io_base_address
  mov  dl, #0x16
  call pci_read_reg
  shr  eax, #16
  pop  dx
  pop  cx
  pop  bx
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

banshee_set_bank:
  push bx
  push cx
  push dx
  push ax
  call banshee_get_io_base_address
  mov  dl, #0x2c ;; vgaInit1
  in   eax, dx
  and  eax, #0xfffffc00
  pop  bx
  and  bx, #0x03ff
  or   ax, bx
  out  dx, eax
  mov  ax, bx
  pop  dx
  pop  cx
  pop  bx
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

banshee_get_line_offset:
  push dx
  call banshee_get_io_base_address
  mov  dl, #0xe8 ;; vidDesktopOverlayStride
  in   eax, dx
  and  ax, #0x7fff
  pop  dx
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
  dw banshee_vesa_unimplemented
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
  dw banshee_vesa_unimplemented
ASM_END
