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

ASM_START

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

banshee_check:
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
  je   banshee_ok
not_detected:
  stc
  ret
banshee_ok:
  clc
  ret

banshee_init:
  call banshee_check
  jc   no_banshee
  mov  cx, bx
  mov  dl, #0x18
  call pci_read_reg
  and  ax, #0xfffe
  mov  dx, ax
  mov  dl, #0x04 ; pciInit0
  mov  eax, #0x0180f840
  out  dx, eax
  mov  dl, #0x28 ; vgaInit0
  mov  eax, #0x00000140
  out  dx, eax
  mov  dl, #0x1c ; dramInit1
  mov  eax, #0x00548031
  out  dx, eax
  mov  dl, #0x18 ; dramInit0
  mov  eax, #0x0c17a16a
  out  dx, eax
  mov  dl, #0x10 ; miscInit0
  xor  eax, eax
  out  dx, eax
  mov  dl, #0x34 ; dramData
  mov  eax, #0x00000037
  out  dx, eax
  mov  dl, #0x30 ; dramCommand
  mov  eax, #0x0000010d
  out  dx, eax
  mov  dl, #0x34
  mov  eax, #0xffffffff
  out  dx, eax
  mov  dl, #0x30
  mov  eax, #0x0000010e
  out  dx, eax
  mov  dl, #0x14 ; miscInit1
  mov  eax, #0x00000001
  out  dx, eax
  mov  dl, #0x20 ; agpInit
  mov  eax, #0x0000049e
  out  dx, eax
  mov  dl, #0x44 ; pllCtrl1
  mov  eax, #0x00002805
  out  dx, eax
  mov  dl, #0x48 ; pllCtrl2
  mov  eax, #0x00007919
  out  dx, eax
  mov  dl, #0x0c ; lfbMemoryConfig
  mov  eax, #0x000a3fff
  out  dx, eax
no_banshee:
  ret

banshee_display_info:
  push ds
  push si
  push cs
  pop  ds
  mov  si, #threedfx_msg
  call _display_string
  call banshee_check
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

banshee_set_vga_mode:
  push ds
  push ax
  push bx
  push cx
  push dx
  call banshee_check
  jc   no_banshee
  mov  cx, bx
  mov  dl, #0x18
  call pci_read_reg
  and  ax, #0xfffe
  mov  dx, ax
  mov  eax, #0x00000140
  mov  dl, #0x28 ; vgaInit0
  out  dx, eax
  xor  eax, eax
  mov  dl, #0x2c ; vgaInit1
  out  dx, eax
  mov  dl, #0x4c ; dacMode
  out  dx, eax
  mov  dl, #0xe4 ; vidDesktopStartAddr
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
  xor  eax, eax
  mov  al, BIOSMEM_NB_ROWS
  inc  al
  shl  eax, #12
  mov  bx, BIOSMEM_NB_COLS
  or   ax, bx
  mov  dl, #0x98 ; vidScreenSize
  out  dx, eax
  xor  eax, eax
  mov  ax, bx
  shl  ax, #1
  mov  dl, #0xe8 ; vidDesktopOverlayStride
  out  dx, eax
  jmp  banshee_set_vga_mode_2
vga_gfx_mode:
  xor  eax, eax
  push dx
  mov  dx, #VGAREG_VGA_CRTC_ADDRESS
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
  pop  dx
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
  test cl, #0x02
  jz   vga_16col_1
  shr  ax, #1
vga_16col_1:
  shl  eax, #12
  mov  bx, BIOSMEM_NB_COLS
  push bx
  shl  bx, #3
  or   ax, bx
  mov  dl, #0x98 ; vidScreenSize
  out  dx, eax
  pop  ax
  test cl, #0x02
  jz   vga_16col_2
  shl  ax, #3
vga_16col_2:
  and  eax, #0xffff
  mov  dl, #0xe8 ; vidDesktopOverlayStride
  out  dx, eax
banshee_set_vga_mode_2:
  xor  eax, eax
  mov  dl, #0x5c ; vidProcCfg
  out  dx, eax
  mov  dl, #0x1c ; dramInit1
  mov  eax, #0x00548031
  out  dx, eax
  pop  dx
  pop  cx
  pop  bx
  pop  ax
  pop  ds
  ret
ASM_END
