// ============================================================================================
//
//  Copyright (C) 2002      Jeroen Janssen
//  Copyright (C) 2003-2024 Volker Ruppert
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
//  This VBE is part of the VGA Bios specific to the VGA card emulated by Bochs.
//  You can NOT drive any physical vga card with it.
//
// ============================================================================================
//
//  This VBE Bios is based on information taken from :
//   - VESA BIOS EXTENSION (VBE) Core Functions Standard Version 3.0 located at www.vesa.org
//
// ============================================================================================


// defines available

// disable VESA/VBE2 check in vbe info
//#define VBE2_NO_VESA_CHECK


#include "vbe.h"
#include "vbetables.h"

// The current OEM Software Revision of this VBE Bios
#define VBE_OEM_SOFTWARE_REV 0x0002;

ASM_START
// FIXME: 'merge' these (c) etc strings with the vgabios.c strings?
vbebios_copyright:
.ascii       "Bochs VBE (C) 2002-2024 http://savannah.nongnu.org/projects/vgabios/"
.byte        0x00

vbebios_vendor_name:
.ascii       "LGPL VGABIOS Developers"
.byte        0x00

vbebios_product_name:
.ascii       "Bochs VBE Adapter"
.byte        0x00

vbebios_product_revision:
.ascii       "ID: vbe.c 2024-02-11"
.byte        0x00

vbebios_info_string:
.ascii      "Bochs VBE Display Adapter enabled"
.byte	0x0a,0x0d
.byte	0x0a,0x0d
.byte	0x00

no_vbebios_info_string:
.ascii      "NO Bochs VBE Support available!"
.byte	0x0a,0x0d
.byte	0x0a,0x0d
.byte 0x00

#if defined(USE_BX_INFO) || defined(DEBUG)
msg_vbe_init:
.ascii "VBE Bios ID: vbe.c 2024-02-11"
.byte  0x0a,0x00
#endif

  .align 2
vesa_pm_start:
  dw vesa_pm_set_window - vesa_pm_start
  dw vesa_pm_set_display_start - vesa_pm_start
  dw vesa_pm_set_palette_data - vesa_pm_start
  dw vesa_pm_io_ports_table - vesa_pm_start
vesa_pm_io_ports_table:
  dw VBE_DISPI_IOPORT_INDEX
  dw VBE_DISPI_IOPORT_DATA
  dw 0xffff
  dw 0xffff

  USE32
vesa_pm_set_window:
  cmp  bx, #0x00
  je  vesa_pm_set_display_window1
  mov  ax, #0x0100
  ret
vesa_pm_set_display_window1:
  mov  ax, dx
  push dx
  push ax
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_BANK
  out  dx, ax
  pop  ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  out  dx, ax
  in   ax, dx
  pop  dx
  cmp  dx, ax
  jne  illegal_window
  mov  ax, #0x004f
  ret
illegal_window:
  mov  ax, #0x014f
  ret

vesa_pm_set_display_start:
  cmp  bl, #0x80
  je   vesa_pm_set_display_start1
  cmp  bl, #0x00
  je   vesa_pm_set_display_start2
  mov  ax, #0x0100
  ret
vesa_pm_set_display_start1:
  push dx
  mov  dx, #VGAREG_ACTL_RESET
vesa_pm_wait_loop_1:
  in   al, dx
  test al, #0x08
  jz   vesa_pm_wait_loop_1
  pop  dx
vesa_pm_set_display_start2:
; convert offset to (X, Y) coordinate
; (would be simpler to change Bochs VBE API...)
  push eax
  push ecx
  push edx
  push esi
  push edi
  shl edx, #16
  and ecx, #0xffff
  or ecx, edx
  shl ecx, #2
  mov eax, ecx

  push eax
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_VIRT_WIDTH
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  in   ax, dx
  movzx ecx, ax

  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_BPP
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  in   ax, dx
  movzx esi, ax
  pop  eax

  cmp esi, #4
  jz bpp4_mode
  add esi, #7
  shr esi, #3
  imul ecx, esi
  xor edx, edx
  div ecx
  mov edi, eax
  mov eax, edx
  xor edx, edx
  div esi
  jmp set_xy_regs

bpp4_mode:
  shr ecx, #1
  xor edx, edx
  div ecx
  mov edi, eax
  mov eax, edx
  shl eax, #1

set_xy_regs:
  push dx
  push ax
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_X_OFFSET
  out  dx, ax
  pop  ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  out  dx, ax
  pop  dx

  mov  ax, di
  push dx
  push ax
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_Y_OFFSET
  out  dx, ax
  pop  ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  out  dx, ax
  pop  dx

  pop edi
  pop esi
  pop edx
  pop ecx
  pop eax
  mov  ax, #0x004f
  ret

vesa_pm_set_palette_data:
  cmp  bl, #0x80
  jne  vesa_pm_no_wait
  push dx
  mov  dx, #VGAREG_ACTL_RESET
vesa_pm_wait_loop_2:
  in   al, dx
  test al, #0x08
  jz   vesa_pm_wait_loop_2
  pop  dx
vesa_pm_no_wait:
  push  ecx
  push  edx
  push  edi
  mov   al, dl
  mov   dx, # VGAREG_DAC_WRITE_ADDRESS
  out   dx, al
  mov   dx, # VGAREG_DAC_DATA
  cld
vesa_pm_set_dac_loop:
  seg   es
  mov   al, [edi+2]
  out   dx, al
  seg   es
  mov   al, [edi+1]
  out   dx, al
  seg   es
  mov   al, [edi]
  out   dx, al
  add   edi, #4
  loop  vesa_pm_set_dac_loop
  pop   edi
  pop   edx
  pop   ecx
  mov   ax, #0x004f
  ret
  USE16
vesa_pm_end:

; DISPI ioport functions

dispi_get_id:
  push dx
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_ID
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  in   ax, dx
  pop  dx
  ret

dispi_set_id:
  push dx
  push ax
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_ID
  out  dx, ax
  pop  ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  out  dx, ax
  pop  dx
  ret

dispi_set_xres:
  push dx
  push ax
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_XRES
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  pop  ax
  out  dx, ax
  pop  dx
  ret

dispi_set_yres:
  push dx
  push ax
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_YRES
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  pop  ax
  out  dx, ax
  pop  dx
  ret

dispi_set_bpp:
  push dx
  push ax
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_BPP
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  pop  ax
  out  dx, ax
  pop  dx
  ret

; AL = bits per pixel / AH = bytes per pixel
dispi_get_bpp:
  push dx
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_BPP
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  in   ax, dx
  mov  ah, al
  shr  ah, 3
  test al, #0x07
  jz   get_bpp_noinc
  inc  ah
get_bpp_noinc:
  pop  dx
  ret

; get display capabilities

dispi_get_max_xres:
  push dx
  push bx
  call dispi_get_enable
  mov  bx, ax
  or   ax, # VBE_DISPI_GETCAPS
  call dispi_set_enable
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_XRES
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  in   ax, dx
  push ax
  mov  ax, bx
  call dispi_set_enable
  pop  ax
  pop  bx
  pop  dx
  ret

dispi_get_max_yres:
  push dx
  push bx
  call dispi_get_enable
  mov  bx, ax
  or   ax, # VBE_DISPI_GETCAPS
  call dispi_set_enable
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_YRES
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  in   ax, dx
  push ax
  mov  ax, bx
  call dispi_set_enable
  pop  ax
  pop  bx
  pop  dx
  ret

dispi_get_max_bpp:
  push dx
  push bx
  call dispi_get_enable
  mov  bx, ax
  or   ax, # VBE_DISPI_GETCAPS
  call dispi_set_enable
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_BPP
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  in   ax, dx
  push ax
  mov  ax, bx
  call dispi_set_enable
  pop  ax
  pop  bx
  pop  dx
  ret

dispi_support_bank_granularity_32k:
  push dx
  push bx
  call dispi_get_enable
  mov  bx, ax
  or   ax, # VBE_DISPI_GETCAPS
  call dispi_set_enable
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_BANK
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  in   ax, dx
  push ax
  mov  ax, bx
  call dispi_set_enable
  pop  ax
  shr  ax, #8
  and  al, #VBE_DISPI_BANK_GRANULARITY_32K
  pop  bx
  pop  dx
  ret

_dispi_set_enable:
dispi_set_enable:
  push dx
  push ax
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_ENABLE
  out  dx, ax
  pop  ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  out  dx, ax
  pop  dx
  ret

dispi_get_enable:
  push dx
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_ENABLE
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  in   ax, dx
  pop  dx
  ret

dispi_set_bank:
  test bl, #0x01
  jz   no_wr_bank
  or   ax, #VBE_DISPI_BANK_WR
no_wr_bank:
  test bl, #0x02
  jz   no_rd_bank
  or   ax, #VBE_DISPI_BANK_RD
no_rd_bank:
  push dx
  push ax
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_BANK
  out  dx, ax
  pop  ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  out  dx, ax
  pop  dx
  ret

dispi_get_bank:
  push dx
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_BANK
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  in   ax, dx
  pop  dx
  ret

dispi_set_bank_farcall:
  cmp  bx, #0x0100
  je   dispi_set_bank_farcall_get
  or   bx, bx
  jnz  dispi_set_bank_farcall_error
  mov  ax, dx
  or   ax, #VBE_DISPI_BANK_RW
  call dispi_set_bank
  in   ax, dx
  pop  dx
  cmp  dx, ax
  jne  dispi_set_bank_farcall_error
  mov  ax, #0x004f
  retf
dispi_set_bank_farcall_get:
  call dispi_get_bank
  mov  dx, ax
  mov  ax, #0x004f
  retf
dispi_set_bank_farcall_error:
  mov  ax, #0x014F
  retf

dispi_set_x_offset:
  push dx
  push ax
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_X_OFFSET
  out  dx, ax
  pop  ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  out  dx, ax
  pop  dx
  ret

dispi_get_x_offset:
  push dx
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_X_OFFSET
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  in   ax, dx
  pop  dx
  ret

dispi_set_y_offset:
  push dx
  push ax
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_Y_OFFSET
  out  dx, ax
  pop  ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  out  dx, ax
  pop  dx
  ret

dispi_get_y_offset:
  push dx
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_Y_OFFSET
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  in   ax, dx
  pop  dx
  ret

vga_set_virt_width:
  push ax
  push bx
  push dx
  mov  bx, ax
  call dispi_get_bpp
  cmp  al, #0x04
  ja   set_width_svga
  shr  bx, #1
set_width_svga:
  shr  bx, #3
  mov  dx, # VGAREG_VGA_CRTC_ADDRESS
  mov  ah, bl
  mov  al, #0x13
  out  dx, ax
  pop  dx
  pop  bx
  pop  ax
  ret

dispi_set_virt_width:
  call vga_set_virt_width
  push dx
  push ax
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_VIRT_WIDTH
  out  dx, ax
  pop  ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  out  dx, ax
  pop  dx
  ret

dispi_get_virt_width:
  push dx
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_VIRT_WIDTH
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  in   ax, dx
  pop  dx
  ret

dispi_get_virt_height:
  push dx
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_VIRT_HEIGHT
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  in   ax, dx
  pop  dx
  ret

vga_compat_setup:
  push ax
  push dx
  ;; set CRTC X resolution
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_XRES
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  in   ax, dx
  push ax
  mov  dx, # VGAREG_VGA_CRTC_ADDRESS
  mov  al, #0x11
  out  dx, al
  inc  dx
  in   al, dx
  and  al, #0x7f
  out  dx, al
  dec  dx
  pop  ax
  push ax
  shr  ax, #3
  dec  ax
  mov  ah, al
  mov  al, #0x01
  out  dx, ax
  pop  ax
  call vga_set_virt_width
  ;; set CRTC Y resolution
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  ax, # VBE_DISPI_INDEX_YRES
  out  dx, ax
  mov  dx, # VBE_DISPI_IOPORT_DATA
  in   ax, dx
  push ax
  dec  ax
  push ax
  mov  dx, # VGAREG_VGA_CRTC_ADDRESS
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
  ;; other settings
  mov  dx, # VGAREG_VGA_CRTC_ADDRESS
  mov  al, #0x09
  out  dx, al
  inc  dx
  in   al, dx
  and  al, #0xf0
  or   al, #0x40
  out  dx, al
  dec  dx
  mov  al, #0x17
  out  dx, al
  inc  dx
  in   al, dx
  or   al, #0x03
  out  dx, al
  dec  dx
  mov  ax, #0xff18
  out  dx, ax
  mov  dx, # VGAREG_ACTL_RESET
  in   al, dx
  mov  dx, # VGAREG_ACTL_ADDRESS
  mov  al, #0x10
  out  dx, al
  mov  dx, # VGAREG_ACTL_READ_DATA
  in   al, dx
  or   al, #0x01
  mov  dx, # VGAREG_ACTL_ADDRESS
  out  dx, al
  mov  al, #0x20
  out  dx, al
  mov  dx, # VGAREG_GRDC_ADDRESS
  mov  ax, #0x0506
  out  dx, ax
  mov  dx, # VGAREG_SEQU_ADDRESS
  mov  ax, #0x0f02
  out  dx, ax
  ;; settings for >= 8bpp
  call dispi_get_bpp
  cmp  al, #0x08
  jb   vga_compat_end
  mov  dx, # VGAREG_VGA_CRTC_ADDRESS
  mov  al, #0x14
  out  dx, al
  mov  dx, # VGAREG_VGA_CRTC_DATA
  in   al, dx
  or   al, #0x40
  out  dx, al
  mov  dx, # VGAREG_ACTL_RESET
  in   al, dx
  mov  dx, # VGAREG_ACTL_ADDRESS
  mov  al, #0x10
  out  dx, al
  mov  dx, # VGAREG_ACTL_READ_DATA
  in   al, dx
  or   al, #0x40
  mov  dx, # VGAREG_ACTL_ADDRESS
  out  dx, al
  mov  al, #0x20
  out  dx, al
  mov  dx, # VGAREG_SEQU_ADDRESS
  mov  al, #0x04
  out  dx, al
  mov  dx, # VGAREG_SEQU_DATA
  in   al, dx
  or   al, #0x08
  out  dx, al
  mov  dx, # VGAREG_GRDC_ADDRESS
  mov  al, #0x05
  out  dx, al
  mov  dx, # VGAREG_GRDC_DATA
  in   al, dx
  and  al, #0x9f
  or   al, #0x40
  out  dx, al
vga_compat_end:
  pop  dx
  pop  ax

;; code for 'write character' support in 8bpp graphics modes

;; called from C code
_vbe_is_8bpp_mode:
vbe_is_8bpp_mode:
  call vbe_has_vbe_display
  test ax, #0x01
  jz   no_vbe_8bpp_mode
  call dispi_get_enable
  and  ax, # VBE_DISPI_ENABLED
  jz   no_vbe_8bpp_mode
  call dispi_get_bpp
  cmp  al, #0x08
  je   is_8bpp_mode
no_vbe_8bpp_mode:
  xor  ax, ax
is_8bpp_mode:
  ret

vbe_8bpp_write_char:
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
  jz   vbe_wc_set_start
  mov  al, 12[bp] ;; nbcols
  mov  bl, 14[bp] ;; cheight
  mul  bl
  shl  ax, #3
  mov  bx, dx
  mul  bx
vbe_wc_set_start:
  pop  bx
  add  ax, bx
  jnc  vbe_wc_noinc1
  inc  dx
vbe_wc_noinc1:
  shl  dx, #1
  test ax, #0x8000
  jz   vbe_wc_noinc2
  and  ax, #0x7fff
  or   dx, #0x0001
vbe_wc_noinc2:
  mov  di, ax
  mov  ax, dx
  mov  bl, #0x01
  call dispi_set_bank
  mov  ax, #0xa000
  mov  es, ax
  mov  bl, 14[bp] ;; cheight
  mov  dx, 12[bp] ;; nbcols
  shl  dx, #3
vbe_wc_loop1:
  push di
  lodsb
  mov  cx, #0x08
  cld
vbe_wc_loop2:
  shl  al, #1
  push ax
  jnc  vbe_wc_set_bkgnd
  mov  al, 6[bp] ;; attr
  db   0xa9 ;; skip next opcode (TEST AX, #0xC030)
vbe_wc_set_bkgnd:
  xor  al, al
  stosb
  pop  ax
  loop vbe_wc_loop2
  pop  di
  dec  bl
  jz   vbe_wc_end
  add  di, dx
  jmp  vbe_wc_loop1
vbe_wc_end:
  pop  di
  pop  si
  pop  es
  pop  ds
  pop  dx
  pop  cx
  pop  bx
  pop  ax
  ret

vbe_8bpp_copy:
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
  jz   vbe_copy_set_start_src
  mov  ax, cx
  mov  bx, dx
  mul  bx
vbe_copy_set_start_src:
  pop  bx
  push bx
  add  ax, bx
  jnc  vbe_copy_noinc1s
  inc  dx
vbe_copy_noinc1s:
  shl  dx, #1
  test ax, #0x8000
  jz   vbe_copy_noinc2s
  and  ax, #0x7fff
  or   dx, #0x0001
vbe_copy_noinc2s:
  mov  si, ax
  mov  ax, dx
  mov  bl, #0x02
  call dispi_set_bank
  xor  ax, ax
  mov  dx, 8[bp] ;; ydest
  or   dx, dx
  jz   vbe_copy_set_start_dst
  mov  ax, cx
  mov  bx, dx
  mul  bx
vbe_copy_set_start_dst:
  pop  bx
  add  ax, bx
  jnc  vbe_copy_noinc1d
  inc  dx
vbe_copy_noinc1d:
  shl  dx, #1
  test ax, #0x8000
  jz   vbe_copy_noinc2d
  and  ax, #0x7fff
  or   dx, #0x0001
vbe_copy_noinc2d:
  mov  di, ax
  mov  ax, dx
  mov  bl, #0x01
  call dispi_set_bank
  mov  ax, #0xa000
  mov  ds, ax
  mov  es, ax
  mov  bl, 14[bp] ;; cheight
  mov  cx, 10[bp] ;; cols
  shl  cx, #2
  mov  dx, 12[bp] ;; nbcols
  shl  dx, #3
vbe_copy_loop:
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
  jz   vbe_copy_end
  add  si, dx
  add  di, dx
  jmp  vbe_copy_loop
vbe_copy_end:
  pop  di
  pop  si
  pop  es
  pop  ds
  pop  dx
  pop  cx
  pop  bx
  pop  ax
  ret

vbe_8bpp_fill:
  push ax
  push bx
  push cx
  push dx
  push es
  push di
vbe_fill_next_row:
  mov  ax, 4[bp] ;; xstart
  shl  ax, #3
  push ax
  xor  ax, ax
  mov  dx, 6[bp] ;; ystart
  or   dx, dx
  jz   vbe_fill_set_start
  mov  al, 12[bp] ;; nbcols
  mov  bl, 14[bp] ;; cheight
  mul  bl
  shl  ax, #3
  mov  bx, dx
  mul  bx
vbe_fill_set_start:
  pop  bx
  add  ax, bx
  jnc  vbe_fill_noinc1
  inc  dx
vbe_fill_noinc1:
  shl  dx, #1
  test ax, #0x8000
  jz   vbe_fill_noinc2
  and  ax, #0x7fff
  or   dx, #0x0001
vbe_fill_noinc2:
  mov  di, ax
  mov  ax, dx
  mov  bl, #0x01
  call dispi_set_bank
  mov  ax, #0xa000
  mov  es, ax
  mov  al, 16[bp] ;; attr
  mov  ah, al
  mov  bl, 14[bp] ;; cheight
  mov  cx, 8[bp] ;; cols
  shl  cx, #2
  mov  dx, 12[bp] ;; nbcols
  shl  dx, #3
vbe_fill_loop:
  push cx
  push di
  cld
  rep
    stosw
  pop  di
  pop  cx
  dec  bl
  jz   vbe_fill_check_row
  add  di, dx
  jmp  vbe_fill_loop
vbe_fill_check_row:
  inc  6[bp] ;; ystart
  dec  10[bp] ;; rows
  jnz  vbe_fill_next_row
  pop  di
  pop  es
  pop  dx
  pop  cx
  pop  bx
  pop  ax
  ret

;; ModeInfo helper function

_mode_info_find_mode:
  push bp
  mov  bp, sp
  push bx
  mov  ax, 4[bp]
  mov  si, #_mode_info_list
find_mode_loop:
  mov  bx, [si]
  cmp  bx, #0xffff
  jz   vbe_mode_not_found
  cmp  ax, bx
  jnz  next_mode_info
  mov  ax, si
  mov  bx, 6[bp] ;; LFB requested ?
  or   bx, bx
  jz   vbe_mode_found
  mov  bx, [si+2]
  test bx, #VBE_MODE_ATTRIBUTE_LINEAR_FRAME_BUFFER_MODE
  jnz  vbe_mode_found
next_mode_info:
  add  si, #0x44
  jnz  find_mode_loop
vbe_mode_not_found:
  xor  ax, ax
vbe_mode_found:
  pop  bx
  pop  bp
  ret

;; Has VBE display - Returns true if VBE display detected

vbe_has_vbe_display:
  push ds
  push bx
  mov  ax, # BIOSMEM_SEG
  mov  ds, ax
  mov  bx, # BIOSMEM_VBE_FLAG
  mov  al, [bx]
  and  al, #0x01
  xor  ah, ah
  pop  bx
  pop  ds
  ret

;; VBE Init - Initialise the Vesa Bios Extension Code
;; This function does a sanity check on the host side display code interface.

vbe_init:
  mov  ax, # VBE_DISPI_ID0
  call dispi_set_id
  call dispi_get_id
  cmp  ax, # VBE_DISPI_ID0
  jne  no_vbe_interface
  push ds
  push bx
  mov  ax, # BIOSMEM_SEG
  mov  ds, ax
  mov  bx, # BIOSMEM_VBE_FLAG
  mov  al, #0x01
  mov  [bx], al
  pop  bx
  pop  ds
  mov  ax, # VBE_DISPI_ID5
  call dispi_set_id
no_vbe_interface:
#if defined(USE_BX_INFO) || defined(DEBUG)
  mov  bx, #msg_vbe_init
  push bx
  call _printf
  inc  sp
  inc  sp
#endif
  ret

;; VBE Display Info - Display information on screen about the VBE

vbe_display_info:
  call vbe_has_vbe_display
  or   ax, ax
  jz   no_vbe_flag
  mov  ax, #0xc000
  mov  ds, ax
  mov  si, #vbebios_info_string
  jmp  _display_string
no_vbe_flag:
  mov  ax, #0xc000
  mov  ds, ax
  mov  si, #no_vbebios_info_string
  jmp  _display_string

;; set VBE mode helper function

_dispi_set_mode:
  push bp
  mov  bp, sp
  push ax
  push bx
  push si
  push ds
  push es
  push cs
  pop  ds
  ;; first disable current mode (when switching between vesa modi)
  mov  ax, #VBE_DISPI_DISABLED
  call dispi_set_enable
  mov  si, 6[bp] ;; pointer to info block
  mov  al, [si+25] ;; bpp
  xor  ah, ah
  cmp  al, #4
  jnz  no_vga_4bpp
  push ax
  push #0x6a
  call _biosfn_set_video_mode
  inc  sp
  inc  sp
  pop  ax
no_vga_4bpp:
  cmp  al, #8
  jnz  set_vbe_params
  push #3
  call _load_dac_palette
  inc  sp
  inc  sp
set_vbe_params:
  call dispi_set_bpp
  mov  ax, [si+18] ;; xres
  call dispi_set_xres
  mov  ax, [si+20] ;; yres
  call dispi_set_yres
  mov  ax, #VBE_DISPI_BANK_RW
  call dispi_set_bank
  call dispi_support_bank_granularity_32k
  or   ax, #VBE_DISPI_ENABLED
  test 4[bp], #VBE_MODE_LINEAR_FRAME_BUFFER
  jz   set_mode_no_lfb
  or   ax, #VBE_DISPI_LFB_ENABLED
set_mode_no_lfb:
  test 4[bp], #VBE_MODE_PRESERVE_DISPLAY_MEMORY
  jz   set_mode_clear_mem
  or   ax, #VBE_DISPI_NOCLEARMEM
set_mode_clear_mem:
  call dispi_set_enable
  call vga_compat_setup
  mov  ax, #BIOSMEM_SEG
  mov  es, ax
  mov  bx, #BIOSMEM_NB_COLS
  mov  ax, [si+18]
  shr  ax, #3
  seg  es
  mov [bx], ax
  mov  bx, #BIOSMEM_NB_ROWS
  mov  ax, [si+20]
  shr  ax, #4
  dec  ax
  seg  es
  mov [bx], ax
  mov  bx, #BIOSMEM_CHAR_HEIGHT
  mov  ax, #16
  seg  es
  mov [bx], ax
  mov  bx, #BIOSMEM_CURRENT_PAGE
  xor  ax, ax
  seg  es
  mov [bx], ax
  mov  bx, #BIOSMEM_CURSOR_POS
  seg  es
  mov [bx], ax
  mov  bx, #BIOSMEM_VBE_MODE
  mov  ax, 4[bp]
  and  ax, #0x1ff
  seg  es
  mov [bx], ax
  mov  bx, #BIOSMEM_VIDEO_CTL
  mov  al, #0x60
  test 4[bp], #VBE_MODE_PRESERVE_DISPLAY_MEMORY
  jz   set_bda_clear_mem
  or   al, #0x80
set_bda_clear_mem:
  seg  es
  mov [bx], al
  mov  al, [si+25] ;; bpp
  cmp  al, #4
  jz   vga_compat_mode
  mov  bx, #BIOSMEM_CURRENT_MODE
  mov  ax, #0x5f ;; FIXME
  seg  es
  mov [bx], al
vga_compat_mode:
  SET_INT_VECTOR(0x43, #0xC000, #_vgafont16)
  pop  es
  pop  ds
  pop  si
  pop  bx
  pop  ax
  pop  bp
  mov  ax, #0x004f
  ret

_dispi_set_vga_mode:
  push bp
  mov  bp, sp
  mov  ax, 4[bp]
  test al, #0x80
  jnz  invalid_mode_num
  test ax, #VBE_MODE_PRESERVE_DISPLAY_MEMORY
  jz   vga_clear_mem
  or   al, #0x80
vga_clear_mem:
  xor  ah, ah
  push ax
  call _biosfn_set_video_mode
  inc  sp
  inc  sp
  pop  bp
  mov  ax, #0x004f
  ret
invalid_mode_num:
  pop  bp
  mov  ax, #0x014f
  ret

msg_vbe2_sig_found:
.ascii       "VBE 00h: VBE2 signature found\n"
.byte        0x00
msg_vbe_modelist:
.ascii       "VBE 00h: reporting %d VBE modes supported\n"
.byte        0x00
ASM_END


/** Function 00h - Return VBE Controller Information
 *
 * Input:
 *              AX      = 4F00h
 *              ES:DI   = Pointer to buffer in which to place VbeInfoBlock structure
 *                        (VbeSignature should be VBE2 when VBE 2.0 information is desired and
 *                        the info block is 512 bytes in size)
 * Output:
 *              AX      = VBE Return Status
 *
 */
ASM_START
vbe_biosfn_return_controller_information:
  push ds
  push si
  push bx
  push cx
  mov  bp, di
  push es
  pop  ds
  xor  bx, bx
  cld
  mov  ax, [di]
  cmp  ax, #0x4256 ;; VB
  jnz  vbe00_1
  mov  ax, [di+2]
  cmp  ax, #0x3245 ;; E2
  jnz  vbe00_1
#ifdef DEBUG
  push #msg_vbe2_sig_found
  call _printf
  inc  sp
  inc  sp
#endif
  ;; VBE2
  lea  di, 0x100[bp]
  mov  bx, di
  mov  si, #vbebios_copyright
  call strcpy
  add  bx, ax
  mov  ax, di
  lea  di, 0x06[bp]
  stosw
  mov  ax, es
  stosw
  lea  di, 0x14[bp]
  mov  ax, #VBE_OEM_SOFTWARE_REV
  stosw
  push di
  mov  si, #vbebios_vendor_name
  mov  di, bx
  call strcpy
  add  bx, ax
  mov  ax, di
  pop  di
  stosw
  mov  ax, es
  stosw
  push di
  mov  si, #vbebios_product_name
  mov  di, bx
  call strcpy
  add  bx, ax
  mov  ax, di
  pop  di
  stosw
  mov  ax, es
  stosw
  push di
  mov  si, #vbebios_product_revision
  mov  di, bx
  call strcpy
  mov  ax, di
  pop  di
  stosw
  mov  ax, es
  stosw
vbe00_1:
  mov  di, bp
  mov  ax, #0x4556 ;; VE
  stosw
  mov  ax, #0x4153 ;; SA
  stosw
  mov  ax, #0x0200 ;; v2.00
  stosw
  or   bx, bx
  jnz  vbe00_2 ;; already set for VBE 2.0
  mov  ax, #vbebios_copyright
  stosw
  mov  ax, cs
  stosw
  jz   vbe00_3
vbe00_2:
  add  di, #4
vbe00_3:
  mov  ax, #VBE_CAPABILITY_8BIT_DAC ;; caps
  stosw
  xor  ax, ax
  stosw
  lea  ax, 0x22[bp]
  stosw
  mov  ax, es
  stosw
  push dx
  mov  dx, #VBE_DISPI_IOPORT_INDEX
  mov  ax, #VBE_DISPI_INDEX_VIDEO_MEMORY_64K
  out  dx, ax
  mov  dx, #VBE_DISPI_IOPORT_DATA
  in   ax, dx
  pop  dx
  stosw
  push cs ;; build dynamic mode list
  pop  ds
  xor  cx, cx
  lea  di, 0x22[bp]
  mov  si, #_mode_info_list
vbe00_4:
  mov  bx, [si]
  cmp  bx, #0xffff
  jz   vbe00_5
  call dispi_get_max_xres
  cmp  [si+20], ax
  ja   vbe00_6
  call dispi_get_max_yres
  cmp  [si+22], ax
  ja   vbe00_6
  call dispi_get_max_bpp
  cmp  [si+27], al
  ja   vbe00_6
vbe00_5:
  mov  ax, bx
  stosw
  inc  cx
vbe00_6:
  add  si, #0x44
  cmp  bx, #0xffff
  jnz  vbe00_4
#ifdef DEBUG
  dec  cx
  push cx
  push #msg_vbe_modelist
  call _printf
  add  sp, #4
#endif
  mov  ax, #0x004F ;; no error
  mov  di, bp
  pop  cx
  pop  bx
  pop  si
  pop  ds
  ret
ASM_END


/** Function 01h - Return VBE Mode Information
 *
 * Input:
 *              AX      = 4F01h
 *              CX      = Mode Number
 *              ES:DI   = Pointer to buffer in which to place ModeInfoBlock structure
 * Output:
 *              AX      = VBE Return Status
 *
 */
ASM_START
vbe_biosfn_return_mode_information:
  push ds
  push si
  push bx
  mov  bp, di
  push cs
  pop  ds
  push cx
  mov  ax, cx
  and  ax, #VBE_MODE_LINEAR_FRAME_BUFFER
  jz   mode_no_lfb
  mov  ax, #0x0001
mode_no_lfb:
  push ax
  and  cx, #0x01ff
  push cx
  call _mode_info_find_mode
  add  sp, #4
  or   ax, ax
  jz   mode_not_found
  add  ax, #2
  mov  si, ax
  mov  cx, #0x0021
  rep
    movsw
  xor  ax, ax
  mov  cx, #0x005f
  rep
    stosw
  mov  di, bp
  mov  al, #0x01
  seg  es
  mov  [di+26], al
#ifdef PCI_VID
  mov ax, #PCI_VID
#else
  mov ax, #0x1234 // experimental vendor
#endif
  call pci_get_lfb_addr
  jz   no_pci_lfb
  seg  es
  mov  [di+42], ax
  xor  ax, ax
  seg  es
  mov  [di+40], ax
no_pci_lfb:
  call dispi_support_bank_granularity_32k
  and  ax, #VBE_DISPI_BANK_GRANULARITY_32K
  jz   no_gran_32k
  mov  ax, #VBE_DISPI_BANK_GRANULARITY_KB
  seg  es
  mov  [di+4], ax
no_gran_32k:
  seg  es
  mov  al, [di+2]
  and  al, #VBE_WINDOW_ATTRIBUTE_RELOCATABLE
  jz   no_win_reloc
  mov  ax, #dispi_set_bank_farcall
  seg  es
  mov  [di+12], ax
  mov  ax, #0xc000
  seg  es
  mov  [di+14], ax
no_win_reloc:
  mov  ah, #0x00
  db   0xa9 ; skip next opcode
mode_not_found:
  mov  ah, #0x01
  mov  al, #0x4f
mode_ok:
  pop  cx
  mov  di, bp
  pop  bx
  pop  si
  pop  ds
  ret
ASM_END


/** Function 02h - Set VBE Mode
 *
 * Input:
 *              AX      = 4F02h
 *              BX      = Desired Mode to set
 * Output:
 *              AX      = VBE Return Status
 *
 */
void vbe_biosfn_set_mode(AX, BX)
Bit16u AX; Bit16u BX;
{
    ModeInfoListItem  *cur_info;
    Boolean           using_lfb;

    using_lfb = ((BX & VBE_MODE_LINEAR_FRAME_BUFFER) == VBE_MODE_LINEAR_FRAME_BUFFER);

    // check for non vesa mode
    if ((BX & 0x1ff) < VBE_MODE_VESA_DEFINED) {

        // call the vgabios in order to set the video mode
        // this allows for going back to textmode with a VBE call (some applications expect that to work)

        AX = dispi_set_vga_mode(BX);

    } else {

        cur_info = mode_info_find_mode(BX & 0x1ff, using_lfb, &cur_info);

        if (cur_info != 0) {
#ifdef DEBUG
            printf("VBE found mode %x, setting:\n", BX);
            printf("\t%d x %d x %d bpp\n",
                   cur_info->info.XResolution,
                   cur_info->info.YResolution,
                   cur_info->info.BitsPerPixel);
#endif
            AX = dispi_set_mode(BX, &cur_info->info);
        } else {
#ifdef DEBUG
            printf("VBE *NOT* found mode %x\n" , BX);
#endif
            AX = 0x014f;
        }
    }
}


/** Function 03h - Return Current VBE Mode
 *
 * Input:
 *              AX      = 4F03h
 * Output:
 *              AX      = VBE Return Status
 *              BX      = Current VBE Mode
 *
 */
ASM_START
vbe_biosfn_return_current_mode:
  push ds
  mov  ax, # BIOSMEM_SEG
  mov  ds, ax
  call dispi_get_enable
  and  ax, # VBE_DISPI_ENABLED
  jz   no_vbe_mode
  mov  bx, # BIOSMEM_VBE_MODE
  mov  ax, [bx]
  mov  bx, ax
  test bx, bx
  jnz  vbe_03_ok
no_vbe_mode:
  mov  bx, # BIOSMEM_CURRENT_MODE
  mov  al, [bx]
  mov  bl, al
  xor  bh, bh
vbe_03_ok:
  mov  ax, #0x004f
  pop  ds
  ret
ASM_END

/** Function 04h - Save/Restore State
 *
 * Input:
 *              AX      = 4F04h
 *              DL      = 00h Return Save/Restore State buffer size
 *                        01h Save State
 *                        02h Restore State
 *              CX      = Requested states
 *              ES:BX   = Pointer to buffer (if DL <> 00h)
 * Output:
 *              AX      = VBE Return Status
 *              BX      = Number of 64-byte blocks to hold the state buffer (if DL=00h)
 *
 */
ASM_START
vbe_biosfn_save_restore_state:
  cmp  dl, #0x01
  jb   vbe_read_state_size
  je   vbe_save_state
  cmp  dl, #0x02
  je   vbe_restore_state
  jmp  vbe_unimplemented
vbe_read_state_size:
  call read_vga_state_size
  test cx, #0x08
  jz   no_vbe_state_size
  mov  ax, #VBE_DISPI_INDEX_Y_OFFSET
  dec  ax
  shl  ax, #1
  add  bx, ax
no_vbe_state_size:
  add  bx, #0x3f
  shr  bx, #6
  mov  ax, #0x004f
  ret
vbe_save_state:
  push bx
  test cx, #0x07
  jz   no_save_vga_state
  call save_vga_state
no_save_vga_state:
  test cx, #0x08
  jz   no_save_vbe_state
  call vbe_biosfn_save_video_state
no_save_vbe_state:
  pop  bx
  mov  ax, #0x004f
  ret
vbe_restore_state:
  push bx
  test cx, #0x07
  jz   no_restore_vga_state
  call restore_vga_state
no_restore_vga_state:
  test cx, #0x08
  jz   no_rest_vbe_state
  call vbe_biosfn_restore_video_state
no_rest_vbe_state:
  pop  bx
  mov  ax, #0x004f
  ret

vbe_biosfn_save_video_state:
  call dispi_get_enable
  seg  es
  mov  [bx], ax
  add  bx, #0x02
  test ax, #VBE_DISPI_ENABLED
  jnz  vbe_save_full_state
  ret
vbe_save_full_state:
  push cx
  push dx
  mov  cx, #VBE_DISPI_INDEX_XRES
vbe_save_loop:
  cmp  cx, #VBE_DISPI_INDEX_ENABLE
  je   vbe_save_next_reg
  cmp  cx, #VBE_DISPI_INDEX_VIRT_HEIGHT
  je   vbe_save_next_reg
  mov  dx, #VBE_DISPI_IOPORT_INDEX
  mov  ax, cx
  out  dx, ax
  mov  dx, #VBE_DISPI_IOPORT_DATA
  in   ax, dx
  seg  es
  mov  [bx], ax
  add  bx, #0x02
vbe_save_next_reg:
  inc  cx
  cmp  cx, #VBE_DISPI_INDEX_Y_OFFSET
  jbe  vbe_save_loop
  pop  dx
  pop  cx
  ret

vbe_biosfn_restore_video_state:
  seg  es
  mov  ax, [bx]
  add  bx, #0x02
  test ax, #VBE_DISPI_ENABLED
  jnz  vbe_restore_full_state
  call dispi_set_enable
  ret
vbe_restore_full_state:
  push ax
  xor  ax, ax
  call dispi_set_enable
  seg  es
  mov  ax, [bx]
  call dispi_set_xres
  add  bx, #0x02
  seg  es
  mov  ax, [bx]
  call dispi_set_yres
  add  bx, #0x02
  seg  es
  mov  ax, [bx]
  call dispi_set_bpp
  add  bx, #0x02
  pop  ax
  call dispi_set_enable
  call vga_compat_setup
  push cx
  push dx
  mov  cx, #VBE_DISPI_INDEX_BANK
vbe_restore_loop:
  cmp  cx, #VBE_DISPI_INDEX_VIRT_HEIGHT
  je   vbe_save_next_reg
  mov  dx, #VBE_DISPI_IOPORT_INDEX
  mov  ax, cx
  out  dx, ax
  mov  dx, #VBE_DISPI_IOPORT_DATA
  seg  es
  mov  ax, [bx]
  out  dx, ax
  add  bx, #0x02
vbe_restore_next_reg:
  inc  cx
  cmp  cx, #VBE_DISPI_INDEX_Y_OFFSET
  jbe  vbe_restore_loop
  pop  dx
  pop  cx
  ret
ASM_END

/** Function 05h - Display Window Control
 *
 * Input:
 *              AX      = 4F05h
 *     (16-bit) BH      = 00h Set memory window
 *                      = 01h Get memory window
 *              BL      = Window number
 *                      = 00h Window A
 *                      = 01h Window B
 *              DX      = Window number in video memory in window
 *                        granularity units (Set Memory Window only)
 * Note:
 *              If this function is called while in a linear frame buffer mode,
 *              this function must fail with completion code AH=03h
 *
 * Output:
 *              AX      = VBE Return Status
 *              DX      = Window number in window granularity units
 *                        (Get Memory Window only)
 */
ASM_START
vbe_biosfn_display_window_control:
  cmp  bl, #0x00
  jne  vbe_05_failed
  cmp  bh, #0x01
  je   get_display_window
  jb   set_display_window
  mov  ax, #0x0100
  ret
set_display_window:
  mov  ax, dx
  or   ax, #VBE_DISPI_BANK_RW
  call dispi_set_bank
  call dispi_get_bank
  cmp  ax, dx
  jne  vbe_05_failed
  mov  ax, #0x004f
  ret
get_display_window:
  call dispi_get_bank
  mov  dx, ax
  mov  ax, #0x004f
  ret
vbe_05_failed:
  mov  ax, #0x014f
  ret
ASM_END


/** Function 06h - Set/Get Logical Scan Line Length
 *
 * Input:
 *              AX      = 4F06h
 *              BL      = 00h Set Scan Line Length in Pixels
 *                      = 01h Get Scan Line Length
 *                      = 02h Set Scan Line Length in Bytes
 *                      = 03h Get Maximum Scan Line Length
 *              CX      = If BL=00h Desired Width in Pixels
 *                        If BL=02h Desired Width in Bytes
 *                        (Ignored for Get Functions)
 *
 * Output:
 *              AX      = VBE Return Status
 *              BX      = Bytes Per Scan Line
 *              CX      = Actual Pixels Per Scan Line
 *                        (truncated to nearest complete pixel)
 *              DX      = Maximum Number of Scan Lines
 */
ASM_START
vbe_biosfn_set_get_logical_scan_line_length:
  mov  ax, cx
  cmp  bl, #0x01
  je   get_logical_scan_line_length
  cmp  bl, #0x02
  je   set_logical_scan_line_bytes
  jb   set_logical_scan_line_pixels
  mov  ax, #0x0100
  ret
set_logical_scan_line_bytes:
  push ax
  call dispi_get_bpp
  xor  bh, bh
  mov  bl, ah
  or   bl, bl
  jnz  no_4bpp_1
  shl  ax, #3
  mov  bl, #1
no_4bpp_1:
  xor  dx, dx
  pop  ax
  div  bx
set_logical_scan_line_pixels:
  call dispi_set_virt_width
get_logical_scan_line_length:
  call dispi_get_bpp
  xor  bh, bh
  mov  bl, ah
  call dispi_get_virt_width
  mov  cx, ax
  or   bl, bl
  jnz  no_4bpp_2
  shr  ax, #3
  mov  bl, #1
no_4bpp_2:
  mul  bx
  mov  bx, ax
  call dispi_get_virt_height
  mov  dx, ax
  mov  ax, #0x004f
  ret
ASM_END


/** Function 07h - Set/Get Display Start
 *
 * Input(16-bit):
 *              AX      = 4F07h
 *              BH      = 00h Reserved and must be 00h
 *              BL      = 00h Set Display Start
 *                      = 01h Get Display Start
 *                      = 02h Schedule Display Start (Alternate)
 *                      = 03h Schedule Stereoscopic Display Start
 *                      = 04h Get Scheduled Display Start Status
 *                      = 05h Enable Stereoscopic Mode
 *                      = 06h Disable Stereoscopic Mode
 *                      = 80h Set Display Start during Vertical Retrace
 *                      = 82h Set Display Start during Vertical Retrace (Alternate)
 *                      = 83h Set Stereoscopic Display Start during Vertical Retrace
 *              ECX     = If BL=02h/82h Display Start Address in bytes
 *                        If BL=03h/83h Left Image Start Address in bytes
 *              EDX     = If BL=03h/83h Right Image Start Address in bytes
 *              CX      = If BL=00h/80h First Displayed Pixel In Scan Line
 *              DX      = If BL=00h/80h First Displayed Scan Line
 *
 * Output:
 *              AX      = VBE Return Status
 *              BH      = If BL=01h Reserved and will be 0
 *              CX      = If BL=01h First Displayed Pixel In Scan Line
 *                        If BL=04h 0 if flip has not occurred, not 0 if it has
 *              DX      = If BL=01h First Displayed Scan Line
 *
 * Input(32-bit):
 *              BH      = 00h Reserved and must be 00h
 *              BL      = 00h Set Display Start
 *                      = 80h Set Display Start during Vertical Retrace
 *              CX      = Bits 0-15 of display start address
 *              DX      = Bits 16-31 of display start address
 *              ES      = Selector for memory mapped registers
 */
ASM_START
vbe_biosfn_set_get_display_start:
  cmp  bl, #0x80
  je   set_display_start
  cmp  bl, #0x01
  je   get_display_start
  jb   set_display_start
  mov  ax, #0x0100
  ret
set_display_start:
  mov  ax, cx
  call dispi_set_x_offset
  mov  ax, dx
  call dispi_set_y_offset
  mov  ax, #0x004f
  ret
get_display_start:
  call dispi_get_x_offset
  mov  cx, ax
  call dispi_get_y_offset
  mov  dx, ax
  xor  bh, bh
  mov  ax, #0x004f
  ret
ASM_END


/** Function 08h - Set/Get Dac Palette Format
 *
 * Input:
 *              AX      = 4F08h
 *              BL      = 00h set DAC palette width
 *                      = 01h get DAC palette width
 *              BH      = If BL=00h: desired number of bits per primary color
 * Output:
 *              AX      = VBE Return Status
 *              BH      = current number of bits per primary color (06h = standard VGA)
 */
ASM_START
vbe_biosfn_set_get_dac_palette_format:
  cmp  bl, #0x01
  je   get_dac_palette_format
  jb   set_dac_palette_format
  mov  ax, #0x0100
  ret
set_dac_palette_format:
  call dispi_get_enable
  cmp  bh, #0x06
  je   set_normal_dac
  cmp  bh, #0x08
  jne  vbe_08_unsupported
  or   ax, # VBE_DISPI_8BIT_DAC
  jnz  set_dac_mode
set_normal_dac:
  and  ax, #~ VBE_DISPI_8BIT_DAC
set_dac_mode:
  call dispi_set_enable
get_dac_palette_format:
  mov  bh, #0x06
  call dispi_get_enable
  and  ax, # VBE_DISPI_8BIT_DAC
  jz   vbe_08_ok
  mov  bh, #0x08
vbe_08_ok:
  mov  ax, #0x004f
  ret
vbe_08_unsupported:
  mov  ax, #0x014f
  ret
ASM_END


/** Function 09h - Set/Get Palette Data
 * Input:
 *           AX    = 4F09h
 *           BL    = 00h     Set palette data
 *           BL    = 01h     Get palette data
 *           BL    = 80h     Set palette data during Vertical Retrace
 *           CX    =         Number of palette registers to read/write
 *           DX    =         First of palette registers to read/write
 *           ES:DI =         Pointer to buffer with table of palette values
 *
 * Output:
 *              AX      = VBE Return Status
 *
 */

// Code moved to vgabios.c (shared with Banshee and Cirrus)


/** Function 0Ah - Return VBE Protected Mode Interface
 * Input:    AX    = 4F0Ah   VBE 2.0 Protected Mode Interface
 *           BL    = 00h     Return protected mode table
 *
 * Output:   AX    =         Status
 *           ES    =         Real Mode Segment of Table
 *           DI    =         Offset of Table
 *           CX    =         Length of Table including protected mode code
 *                           (for copying purposes)
 */
ASM_START
vbe_biosfn_return_protected_mode_interface:
  test bl, bl
  jnz _fail
  mov di, #0xc000
  mov es, di
  mov di, # vesa_pm_start
  mov cx, # vesa_pm_end
  sub cx, di
  mov ax, #0x004f
  ret
_fail:
  mov ax, #0x014f
  ret
ASM_END

/* DDC helper functions for VESA 15h */
ASM_START
vbe_ddc_delay:
  in   al, 0x61
  and  al, #0x10
  mov  ah, al
vbe_ddc_delay_01:
  nop
  in   al, 0x61
  and  al, #0x10
  cmp  al, ah
  jz   vbe_ddc_delay_01
  ret

vbe_ddc_set_dck:
  mov  ah, #0x01
  db   0xa9 ;; skip next opcode (TEST AX, #0x02b4)
vbe_ddc_set_dda:
  mov  ah, #0x02
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  al, # VBE_DISPI_INDEX_DDC
  out  dx, al
  inc  dx
  in   al, dx
  or   al, ah
  out  dx, al
  ret

vbe_ddc_clr_dck:
  mov  ah, #0x01
  db   0xa9 ;; skip next opcode (see above)
vbe_ddc_clr_dda:
  mov  ah, #0x02
  xor  ah, #0xff
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  al, # VBE_DISPI_INDEX_DDC
  out  dx, al
  inc  dx
  in   al, dx
  and  al, ah
  out  dx, al
  ret

vbe_ddc_init:
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  al, # VBE_DISPI_INDEX_DDC
  out  dx, al
  inc  dx
  mov  al, #0x83
  out  dx, al
  nop
  in   al, dx
  cmp  al, #0x08f
  ret

vbe_ddc_start:
  call vbe_ddc_clr_dda
  call vbe_ddc_clr_dck
  ret

vbe_ddc_stop:
  call vbe_ddc_set_dck
  call vbe_ddc_set_dda
  ret

vbe_ddc_get_dda:
  mov  dx, # VBE_DISPI_IOPORT_INDEX
  mov  al, # VBE_DISPI_INDEX_DDC
  out  dx, al
  inc  dx
  in   al, dx
  and  al, #0x08
  shl  al, #0x05
  ret

vbe_ddc_send_bit:
  push ax
  pushf
  call vbe_ddc_delay
  popf
  jc   vbe_ddc_send_bit_01
  call vbe_ddc_clr_dda
  jnz  vbe_ddc_send_bit_02
vbe_ddc_send_bit_01:
  call vbe_ddc_set_dda
vbe_ddc_send_bit_02:
  call vbe_ddc_set_dck
  call vbe_ddc_delay
  call vbe_ddc_clr_dck
  pop  ax
  ret

vbe_ddc_read_bit:
  push ax
  call vbe_ddc_delay
  call vbe_ddc_set_dck
  call vbe_ddc_get_dda
  pushf
  call vbe_ddc_delay
  call vbe_ddc_clr_dck
  popf
  pop  ax
  ret

vbe_ddc_send_byte:
  push cx
  mov  cx, #0x08
vbe_ddc_send_byte_01:
  shl  al, #0x01
  call vbe_ddc_send_bit
  loop vbe_ddc_send_byte_01
  call vbe_ddc_set_dda
  call vbe_ddc_delay
  call vbe_ddc_set_dck
  call vbe_ddc_get_dda
  pushf
  call vbe_ddc_clr_dck
  call vbe_ddc_clr_dda
  popf
  pop  cx
  ret

vbe_ddc_read_byte:
  push cx
  call vbe_ddc_set_dda
  mov  al, #0x00
  mov  cx, #0x08
vbe_ddc_read_byte_01:
  call vbe_ddc_read_bit
  rcl  al, #0x01
  loop vbe_ddc_read_byte_01
  pop  cx
  ret

vbe_ddc_send_status:
  cmp  cx, #0x01
  jz   vbe_ddc_send_status_01
  call vbe_ddc_clr_dda
vbe_ddc_send_status_01:
  call vbe_ddc_delay
  call vbe_ddc_set_dck
  call vbe_ddc_delay
  call vbe_ddc_clr_dck
  ret
ASM_END

/** Function 15h - Display Identification Extensions
 * Input:    AX    = 4F15h
 *           BL    = 00h     Get capabilities
 *           BL    = 01h     Read EDID
 *           CX    =         Controller unit number
 *           DX    =         EDID block number (if BL = 01h)
 *           ES:DI =         Null pointer/reserved (if BL = 00h)
 *           ES:DI =         Pointer to buffer to store EDID block (if BL = 01h)
 *
 * Output:   AX    =         Status
 *           BH    =         Approximate time to get EDID in seconds rounded up (if BL = 00h)
 *           BL    =         DDC level supported: (if BL = 00h)
 *                            Bit    Meaning if set
 *                             0      DDC1 supported
 *                             1      DDC2 supported
 *                             2      Screen blanked during transfer
 *           BH    =         Unchanged (if BL = 01h)
 *           CX    =         Unchanged
 *           ES:DI =         Unchanged
 */
ASM_START
vbe_biosfn_display_identification_extensions:
  cmp  bl,#0x01
  jb   vbe_edid_get_capabilities
  je   vbe_read_EDID
  jmp  vbe_unimplemented

vbe_edid_get_capabilities:
  test cx,cx
  jne  vbe_unimplemented
  mov  ax, #0x004f
  mov  bx, #0x0202
  ret

vbe_read_EDID:
  call vbe_ddc_init
  jnz  vbe_failed
  call vbe_ddc_start
  call vbe_ddc_delay
  mov  al, #0xa0
  call vbe_ddc_send_byte
  jc   vbe_failed
  mov  al, #0x00
  call vbe_ddc_send_byte
  jc   vbe_failed
  call vbe_ddc_stop
  call vbe_ddc_start
  call vbe_ddc_delay
  mov  al, #0xa1
  call vbe_ddc_send_byte
  jc   vbe_failed
  push cx
  push di
  mov  cx, #0x0080
  cld
vbe_read_edid_loop:
  call vbe_ddc_read_byte
  stosb
  call vbe_ddc_send_status
  loop vbe_read_edid_loop
  call vbe_ddc_stop
  pop  di
  pop  cx
  mov  ax, #0x004f
  ret

vbe_failed:
  mov  ax, #0x014f ;; function failed
  ret

vbe_unimplemented:
  mov  ax, #0x0100 ;; not implemented
  ret

; Handle INT 10h AH 4Fh
vbe_main_handler:
  push ax
  call vbe_has_vbe_display
  shr  al, #1
  pop  ax
  jnc  vbe_unimplemented
  cmp  al, #0x15
  ja   vbe_unimplemented
  push bp
  push bx
  xor  bx, bx
  mov  bl, al
  shl  bx, 1
 db 0x2e ;; cs:
  mov  bp, vbe_handlers[bx]
  pop  bx
  push #vbe_return
  push bp
  ret

vbe_return:
  pop  bp
  jmp  int10_end

;; call remaining C function
vbe_biosfn_set_mode:
  push ds
  push bx
  push ax
  mov  bx, #0xc000
  mov  ds, bx
  call _vbe_biosfn_set_mode
  pop  ax
  pop  bx
  pop  ds
  ret

vbe_handlers:
  ;; 00h
  dw vbe_biosfn_return_controller_information
  dw vbe_biosfn_return_mode_information
  dw vbe_biosfn_set_mode
  dw vbe_biosfn_return_current_mode
  ;; 04h
  dw vbe_biosfn_save_restore_state
  dw vbe_biosfn_display_window_control
  dw vbe_biosfn_set_get_logical_scan_line_length
  dw vbe_biosfn_set_get_display_start
  ;; 08h
  dw vbe_biosfn_set_get_dac_palette_format
  dw vbe_biosfn_set_get_palette_data
  dw vbe_biosfn_return_protected_mode_interface
  dw vbe_unimplemented
  ;; 0Ch
  dw vbe_unimplemented
  dw vbe_unimplemented
  dw vbe_unimplemented
  dw vbe_unimplemented
  ;; 10h
  dw vbe_unimplemented
  dw vbe_unimplemented
  dw vbe_unimplemented
  dw vbe_unimplemented
  ;; 14h
  dw vbe_unimplemented
  dw vbe_biosfn_display_identification_extensions
ASM_END
