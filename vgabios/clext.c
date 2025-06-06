//
//  QEMU Cirrus CLGD 54xx VGABIOS Extension.
//
//  Copyright (c) 2004      Makoto Suzuki (suzu)
//  Copyright (C) 2004-2025 Volker Ruppert
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

//#define CIRRUS_VESA3_PMINFO
#ifdef VBE
#undef CIRRUS_VESA3_PMINFO
#endif

typedef struct
{
  /* + 0 */
  unsigned short mode;
  unsigned short width;
  unsigned short height;
  unsigned short depth;
  /* + 8 */
  unsigned short hidden_dac; /* 0x3c6 */
  unsigned short *seq; /* 0x3c4 */
  unsigned short *graph; /* 0x3ce */
  unsigned short *crtc; /* 0x3d4 */
  /* +16 */
  unsigned char bitsperpixel;
  unsigned char vesacolortype;
  unsigned char *color_params;
} cirrus_mode_t;
#define CIRRUS_MODE_SIZE 20


/* For VESA BIOS 3.0 */
#define CIRRUS_PM16INFO_SIZE 20

/* VGA */
unsigned short cseq_vga[] = {0x0007,0x0012,0xffff};
unsigned short cgraph_vga[] = {0x0009,0x000a,0x000b,0xffff};
unsigned short ccrtc_vga[] = {0x001a,0x001b,0x001d,0xffff};

/* extensions */
unsigned short cgraph_vgacolor[] = {
0x0000,0x0001,0x0002,0x0003,0x0004,0x0005,0x0506,0x0f07,0xff08,
0x0009,0x000a,0x000b,
0xffff
};
unsigned short cgraph_svgacolor[] = {
0x0000,0x0001,0x0002,0x0003,0x0004,0x4005,0x0506,0x0f07,0xff08,
0x0009,0x000a,0x000b,
0xffff
};
/* 640x480x8 */
unsigned short cseq_640x480x8[] = {
0x0300,0x2101,0x0f02,0x0003,0x0e04,0x1107,
0x0412,0x0013,0x2017,
0xffff
};
unsigned short ccrtc_640x480x8[] = {
0x2c11,
0x5f00,0x4f01,0x4f02,0x8003,0x5204,0x1e05,0x0b06,0x3e07,
0x4009,0x000c,0x000d,
0xea10,0xdf12,0x5013,0x4014,0xdf15,0x0b16,0xc317,0xff18,
0x001a,0x221b,0x001d,
0xffff
};
/* 640x480x16 */
unsigned short cseq_640x480x16[] = {
0x0300,0x2101,0x0f02,0x0003,0x0e04,0x1707,
0x0412,0x0013,0x2017,
0xffff
};
unsigned short ccrtc_640x480x16[] = {
0x2c11,
0x5f00,0x4f01,0x4f02,0x8003,0x5204,0x1e05,0x0b06,0x3e07,
0x4009,0x000c,0x000d,
0xea10,0xdf12,0xa013,0x4014,0xdf15,0x0b16,0xc317,0xff18,
0x001a,0x221b,0x001d,
0xffff
};
/* 640x480x24 */
unsigned short cseq_640x480x24[] = {
0x0300,0x2101,0x0f02,0x0003,0x0e04,0x1507,
0x0412,0x0013,0x2017,
0xffff
};
unsigned short ccrtc_640x480x24[] = {
0x2c11,
0x5f00,0x4f01,0x4f02,0x8003,0x5204,0x1e05,0x0b06,0x3e07,
0x4009,0x000c,0x000d,
0xea10,0xdf12,0x0013,0x4014,0xdf15,0x0b16,0xc317,0xff18,
0x001a,0x321b,0x001d,
0xffff
};
/* 800x600x4 */
unsigned short cseq_800x600x4[] = {
0x0300,0x0101,0x0f02,0x0003,0x0604,
0x0012,0x0013,0x0616,0x2117,0x530e,0x301e,
0x0007,
0xffff
};
unsigned short ccrtc_800x600x4[] = {
0x0b11,0x7f00,0x6301,0x6402,0x8203,0x6804,0x1205,0x6f06,0xf007,
0x6009,0x000c,0x000d,
0x5810,0x5712,0x3213,0x0014,0x5715,0x6f16,0xe317,0xff18,
0x001a,0x221b,0x001d,
0xffff
};
/* 800x600x8 */
unsigned short cseq_800x600x8[] = {
0x0300,0x2101,0x0f02,0x0003,0x0e04,
0x0412,0x0013,0x2017,0x510e,0x3a1e,
0x1107,
0xffff
};
unsigned short ccrtc_800x600x8[] = {
0x2311,0x7f00,0x6301,0x6402,0x8203,0x6b04,0x1b05,0x7206,0xf007,
0x6009,0x000c,0x000d,
0x5910,0x5712,0x6413,0x4014,0x5815,0x7316,0xc317,0xff18,
0x001a,0x221b,0x001d,
0xffff
};
/* 800x600x16 */
unsigned short cseq_800x600x16[] = {
0x0300,0x2101,0x0f02,0x0003,0x0e04,
0x0412,0x0013,0x2017,0x510e,0x3a1e,
0x1707,
0xffff
};
unsigned short ccrtc_800x600x16[] = {
0x2311,0x7f00,0x6301,0x6402,0x8203,0x6b04,0x1b05,0x7206,0xf007,
0x6009,0x000c,0x000d,
0x5910,0x5712,0xc813,0x4014,0x5815,0x7316,0xc317,0xff18,
0x001a,0x221b,0x001d,
0xffff
};
/* 800x600x24 */
unsigned short cseq_800x600x24[] = {
0x0300,0x2101,0x0f02,0x0003,0x0e04,
0x0412,0x0013,0x2017,0x510e,0x3a1e,
0x1507,
0xffff
};
unsigned short ccrtc_800x600x24[] = {
0x2311,0x7f00,0x6301,0x6402,0x8203,0x6b04,0x1b05,0x7206,0xf007,
0x6009,0x000c,0x000d,
0x5910,0x5712,0x8013,0x4014,0x5815,0x7316,0xc317,0xff18,
0x001a,0x321b,0x001d,
0xffff
};
/* 1024x768x4 */
unsigned short cseq_1024x768x4[] = {
0x0300,0x0101,0x0f02,0x0003,0x0604,
0x0012,0x0013,0x0616,0x2117,0x2c0e,0x101e,
0x0007,
0xffff
};
unsigned short ccrtc_1024x768x4[] = {
0x0b11,0x9f00,0x7f01,0x8002,0x8203,0x8404,0x9005,0x1e06,0xf507,
0x6009,0x000c,0x000d,
0x0310,0xff12,0x4013,0x0014,0xff15,0x1e16,0xe317,0xff18,
0x001a,0x221b,0x001d,
0xffff
};
/* 1024x768x8 */
unsigned short cseq_1024x768x8[] = {
0x0300,0x2101,0x0f02,0x0003,0x0e04,
0x0412,0x0013,0x2017,0x760e,0x341e,
0x1107,
0xffff
};
unsigned short ccrtc_1024x768x8[] = {
0x2911,0xa300,0x7f01,0x7f02,0x8603,0x8304,0x9405,0x2406,0xf507,
0x6009,0x000c,0x000d,
0x0310,0xff12,0x8013,0x4014,0xff15,0x2416,0xc317,0xff18,
0x001a,0x221b,0x001d,
0xffff
};
/* 1024x768x16 */
unsigned short cseq_1024x768x16[] = {
0x0300,0x2101,0x0f02,0x0003,0x0e04,
0x0412,0x0013,0x2017,0x760e,0x341e,
0x1707,
0xffff
};
unsigned short ccrtc_1024x768x16[] = {
0x2911,0xa300,0x7f01,0x7f02,0x8603,0x8304,0x9405,0x2406,0xf507,
0x6009,0x000c,0x000d,
0x0310,0xff12,0x0013,0x4014,0xff15,0x2416,0xc317,0xff18,
0x001a,0x321b,0x001d,
0xffff
};
/* 1024x768x24 */
unsigned short cseq_1024x768x24[] = {
0x0300,0x2101,0x0f02,0x0003,0x0e04,
0x0412,0x0013,0x2017,0x760e,0x341e,
0x1507,
0xffff
};
unsigned short ccrtc_1024x768x24[] = {
0x2911,0xa300,0x7f01,0x7f02,0x8603,0x8304,0x9405,0x2406,0xf507,
0x6009,0x000c,0x000d,
0x0310,0xff12,0x8013,0x4014,0xff15,0x2416,0xc317,0xff18,
0x001a,0x321b,0x001d,
0xffff
};
/* 1152x864x8 */
unsigned short cseq_1152x864x8[] = {
0x0300,0x2101,0x0f02,0x0003,0x0e04,
0x0412,0x0013,0x2017,0x420e,0x151e,
0x1107,
0xffff
};
unsigned short ccrtc_1152x864x8[] = {
0x2311,0x5700,0x4701,0x4802,0x9703,0x4b04,0x9105,0x8e06,0xff07,
0x6009,0x000c,0x000d,
0x6010,0x5f12,0x9013,0x0014,0x6015,0x9f16,0xe317,0xff18,
0x001a,0x221b,0x001d,
0xffff
};
/* 1280x1024x4 */
unsigned short cseq_1280x1024x4[] = {
0x0300,0x0101,0x0f02,0x0003,0x0604,
0x0012,0x0013,0x0616,0x2117,0x6e0e,0x2a1e,
0x0007,
0xffff
};
unsigned short ccrtc_1280x1024x4[] = {
0x0311,0xcf00,0x9f01,0xa002,0x8003,0xa904,0x1305,0x1406,0xb207,
0x6009,0x000c,0x000d,
0x0010,0xff12,0x5013,0x0014,0x0015,0x2a16,0xe317,0xff18,
0x011a,0x221b,0x001d,
0xffff
};
/* 1280x1024x8 */
unsigned short cseq_1280x1024x8[] = {
0x0300,0x2101,0x0f02,0x0003,0x0e04,
0x0412,0x0013,0x2017,0x6e0e,0x2a1e,
0x1107,
0xffff
};
unsigned short ccrtc_1280x1024x8[] = {
0x2911,0xbd00,0x9f01,0xa002,0x8003,0xa404,0x1905,0x2a06,0xb207,
0x6009,0x000c,0x000d,
0x0b10,0xff12,0xa013,0x4014,0xff15,0x2416,0xc317,0xff18,
0x011a,0x221b,0x001d,
0xffff
};
/* 1280x1024x16 */
unsigned short cseq_1280x1024x16[] = {
0x0300,0x2101,0x0f02,0x0003,0x0e04,
0x0412,0x0013,0x2017,0x06e0e,0x2a1e,
0x1707,
0xffff
};
unsigned short ccrtc_1280x1024x16[] = {
0x2911,0xbd00,0x9f01,0xa002,0x8003,0xa404,0x1905,0x2a06,0xb207,
0x6009,0x000c,0x000d,
0x0b10,0xff12,0x4013,0x4014,0xff15,0x2416,0xc317,0xff18,
0x011a,0x321b,0x001d,
0xffff
};

/* 1600x1200x8 */
unsigned short cseq_1600x1200x8[] = {
0x0300,0x2101,0x0f02,0x0003,0x0e04,
0x0412,0x0013,0x2017,0x880e,0x221e,
0x1107,
0xffff
};
unsigned short ccrtc_1600x1200x8[] = {
0x2911,0xe800,0xc701,0xc802,0x8603,0x8304,0x9405,0x7206,0xf007,
0x6009,0x000c,0x000d,
0x7d10,0x5712,0xc813,0x4014,0xff15,0x2416,0xc317,0xff18,
0x011a,0x221b,0x001d,
0xffff
};

/* 320x200x16 */
unsigned short cseq_320x200x16[] = {
0x0300,0x0101,0x0f02,0x0003,0x0e04,
0x0012,0x0013,0x2017,0x07e0e,0x331e,
0x1707,
0xffff
};
unsigned short ccrtc_320x200x16[] = {
0x2911,0x2d00,0x2701,0x2702,0x9103,0x2a04,0x9005,0xbf06,0x1f07,
0xc009,0x000c,0x000d,
0x9e10,0x8f12,0x5013,0x0014,0x9715,0xb816,0xe317,0xff18,
0x001a,0x221b,0x001d,
0xffff
};
/* 320x200x24 */
unsigned short cseq_320x200x24[] = {
0x0300,0x0101,0x0f02,0x0003,0x0e04,
0x0012,0x0013,0x2017,0x01d0e,0x161e,
0x1507,
0xffff
};
unsigned short ccrtc_320x200x24[] = {
0x2911,0x2d00,0x2701,0x2702,0x9103,0x2a04,0x9005,0xbf06,0x1f07,
0xc009,0x000c,0x000d,
0x9c10,0x8f12,0x0013,0x0014,0x9715,0xb816,0xe317,0xff18,
0x001a,0x321b,0x001d,
0xffff
};

unsigned char cirrus_color_params_8bpp[] = {
0, 0, 0, 0, 0, 0, 0, 0};
unsigned char cirrus_color_params_15bpp[] = {
5, 10, 5, 5, 5, 0, 1, 15};
unsigned char cirrus_color_params_16bpp[] = {
5, 11, 6, 5, 5, 0, 0, 0};
unsigned char cirrus_color_params_24bpp[] = {
8, 16, 8, 8, 8, 0, 0, 0};

cirrus_mode_t cirrus_modes[] =
{
 {0x5f,640,480,8,0x00,
   cseq_640x480x8,cgraph_svgacolor,ccrtc_640x480x8,8,
   4,cirrus_color_params_8bpp},
 {0x64,640,480,16,0xe1,
   cseq_640x480x16,cgraph_svgacolor,ccrtc_640x480x16,16,
   6,cirrus_color_params_16bpp},
 {0x66,640,480,15,0xf0,
   cseq_640x480x16,cgraph_svgacolor,ccrtc_640x480x16,16,
   6,cirrus_color_params_15bpp},
 {0x71,640,480,24,0xe5,
   cseq_640x480x24,cgraph_svgacolor,ccrtc_640x480x24,24,
   6,cirrus_color_params_24bpp},

 {0x58,800,600,4,0x00,
   cseq_800x600x4,cgraph_vgacolor,ccrtc_800x600x4,4,
   3,cirrus_color_params_8bpp},
 {0x5c,800,600,8,0x00,
   cseq_800x600x8,cgraph_svgacolor,ccrtc_800x600x8,8,
   4,cirrus_color_params_8bpp},
 {0x65,800,600,16,0xe1,
   cseq_800x600x16,cgraph_svgacolor,ccrtc_800x600x16,16,
   6,cirrus_color_params_16bpp},
 {0x67,800,600,15,0xf0,
   cseq_800x600x16,cgraph_svgacolor,ccrtc_800x600x16,16,
   6,cirrus_color_params_15bpp},

 {0x5d,1024,768,4,0x00,
   cseq_1024x768x4,cgraph_vgacolor,ccrtc_1024x768x4,4,
   3,cirrus_color_params_8bpp},
 {0x60,1024,768,8,0x00,
   cseq_1024x768x8,cgraph_svgacolor,ccrtc_1024x768x8,8,
   4,cirrus_color_params_8bpp},
 {0x74,1024,768,16,0xe1,
   cseq_1024x768x16,cgraph_svgacolor,ccrtc_1024x768x16,16,
   6,cirrus_color_params_16bpp},
 {0x68,1024,768,15,0xf0,
   cseq_1024x768x16,cgraph_svgacolor,ccrtc_1024x768x16,16,
   6,cirrus_color_params_15bpp},

 {0x78,800,600,24,0xe5,
   cseq_800x600x24,cgraph_svgacolor,ccrtc_800x600x24,24,
   6,cirrus_color_params_24bpp},
 {0x79,1024,768,24,0xe5,
   cseq_1024x768x24,cgraph_svgacolor,ccrtc_1024x768x24,24,
   6,cirrus_color_params_24bpp},

 {0x6c,1280,1024,4,0x00,
   cseq_1280x1024x4,cgraph_vgacolor,ccrtc_1280x1024x4,4,
   3,cirrus_color_params_8bpp},
 {0x6d,1280,1024,8,0x00,
   cseq_1280x1024x8,cgraph_svgacolor,ccrtc_1280x1024x8,8,
   4,cirrus_color_params_8bpp},
 {0x69,1280,1024,15,0xf0,
   cseq_1280x1024x16,cgraph_svgacolor,ccrtc_1280x1024x16,16,
   6,cirrus_color_params_15bpp},
 {0x75,1280,1024,16,0xe1,
   cseq_1280x1024x16,cgraph_svgacolor,ccrtc_1280x1024x16,16,
   6,cirrus_color_params_16bpp},

 {0x7b,1600,1200,8,0x00,
   cseq_1600x1200x8,cgraph_svgacolor,ccrtc_1600x1200x8,8,
   4,cirrus_color_params_8bpp},
 {0x7c,1152,864,8,0x4a,
   cseq_1152x864x8,cgraph_svgacolor,ccrtc_1152x864x8,8,
   4,cirrus_color_params_8bpp},

 {0x6f,320,200,16,0xe1,
   cseq_320x200x16,cgraph_svgacolor,ccrtc_320x200x16,16,
   6,cirrus_color_params_16bpp},
 {0x70,320,200,24,0xe5,
   cseq_320x200x24,cgraph_svgacolor,ccrtc_320x200x24,24,
   6,cirrus_color_params_24bpp},

 {0xfe,0,0,0,0,cseq_vga,cgraph_vga,ccrtc_vga,0,
   0xff,cirrus_color_params_8bpp},
 {0xff,0,0,0,0,0,0,0,0,
   0xff,0},
};

unsigned char cirrus_id_table[] = {
  // 5430
  0xA0, 0x32,
  // 5446
  0xB8, 0x39,

  0xff, 0xff
};


unsigned short cirrus_vesa_modelist[] = {
// 640x480x8
  0x101, 0x5f,
// 640x480x15
  0x110, 0x66,
// 640x480x16
  0x111, 0x64,
// 640x480x24
  0x112, 0x71,
// 800x600x4
  0x102, 0x58,
// 800x600x8
  0x103, 0x5c,
// 800x600x15
  0x113, 0x67,
// 800x600x16
  0x114, 0x65,
// 800x600x24
  0x115, 0x78,
// 1024x768x4
  0x104, 0x5d,
// 1024x768x8
  0x105, 0x60,
// 1024x768x15
  0x116, 0x68,
// 1024x768x16
  0x117, 0x74,
// 1024x768x24
  0x118, 0x79,
// 1280x1024x4
  0x106, 0x6c,
// 1280x1024x8
  0x107, 0x6d,
// 1280x1024x15
  0x119, 0x69,
// 1280x1024x16
  0x11a, 0x75,
// 1152x864x8
  0x7c,  0x7c,
// 320x200x16
  0x10e,  0x6f,
// 320x200x24
  0x10f,  0x70,
// invalid
  0xffff,0xffff
};


ASM_START

cirrus_installed:
.ascii "cirrus-compatible VGA is detected"
.byte 0x0d,0x0a
.byte 0x0d,0x0a,0x00

cirrus_not_installed:
.ascii "cirrus-compatible VGA is not detected"
.byte 0x0d,0x0a
.byte 0x0d,0x0a,0x00

cirrus_vesa_vendorname:
cirrus_vesa_productname:
cirrus_vesa_oemname:
.ascii "VGABIOS Cirrus extension"
.byte 0
cirrus_vesa_productrevision:
.ascii "1.0"
.byte 0

cirrus_init:
  call cirrus_check
  jnz no_cirrus
  SET_INT_VECTOR(0x10, #0xC000, #cirrus_int10_handler)
  mov al, #0x0f ; memory setup
  mov dx, #0x3C4
  out dx, al
  inc dx
  in  al, dx
  and  al, #0x98
  test al, #0x80
  jz   no_bankswitch
  mov  al, #0x47
no_bankswitch:
  mov ah, al
  mov al, #0x0a
  dec dx
  out dx, ax
  mov ax, #0x0007 ; set vga mode
  out dx, ax
  mov ax, #0x0431 ; reset bitblt
  mov dx, #0x3CE
  out dx, ax
  mov ax, #0x0031
  out dx, ax
  call cirrus_set_vga_vclk
  mov ax, #0x420d ; VCLK #2 setup
  out dx, ax
  mov ax, #0x1f1d
  out dx, ax
  mov ax, #0x7e0e ; VCLK #3 setup
  out dx, ax
  mov ax, #0x331e
  out dx, ax
no_cirrus:
  ret

cirrus_display_info:
  push ds
  push si
  push cs
  pop ds
  call cirrus_check
  mov si, #cirrus_not_installed
  jnz cirrus_msgnotinstalled
  mov si, #cirrus_installed

cirrus_msgnotinstalled:
  call _display_string
  pop si
  pop ds
  ret

cirrus_check:
  push ax
  push dx
  mov ax, #0x9206
  mov dx, #0x3C4
  out dx, ax
  inc dx
  in al, dx
  cmp al, #0x12
  pop dx
  pop ax
  ret


cirrus_int10_handler:
  pushf
  push bp
  cmp ah, #0x00  ;; set video mode
  jz cirrus_set_video_mode
  cmp ah, #0x12  ;; cirrus extension
  jz cirrus_extbios
  cmp ah, #0x4F  ;; VESA extension
  jz cirrus_vesa

cirrus_unhandled:
  pop bp
  popf
  jmp vgabios_int10_handler

cirrus_return:
#ifdef CIRRUS_DEBUG
  call cirrus_ret_debug_dump
#endif
  pop bp
  popf
  iret

cirrus_set_video_mode:
#ifdef CIRRUS_DEBUG
  call cirrus_call_debug_dump
#endif
  push si
  push ax
  push bx
  push ds
#ifdef CIRRUS_VESA3_PMINFO
 db 0x2e ;; cs:
  mov  si, [cirrus_vesa_sel0000_data]
#else
  xor  si, si
#endif
  mov  ds, si
  xor  bx, bx
  mov  [PM_BIOSMEM_VBE_MODE], bx
  pop  ds
  pop  bx
  call cirrus_get_modeentry
  jnc  cirrus_set_video_mode_extended
  mov  al, #0xfe
  call cirrus_get_modeentry_nomask
  call cirrus_switch_mode
  pop  ax
  pop  si
  jmp  cirrus_unhandled

cirrus_extbios:
  cmp bl, #0x80
  jb cirrus_unhandled
  cmp bl, #0xAF
  ja cirrus_unhandled
#ifdef CIRRUS_DEBUG
  call cirrus_call_debug_dump
#endif
  push bx
  and bx, #0x7F
  shl bx, 1
 db 0x2e ;; cs:
  mov bp, cirrus_extbios_handlers[bx]
  pop bx
  push #cirrus_return
  push bp
  ret

cirrus_vesa:
#ifdef CIRRUS_DEBUG
  call cirrus_call_debug_dump
#endif
  cmp al, #0x15
  ja cirrus_vesa_not_handled
  push bx
  xor bx, bx
  mov bl, al
  shl bx, 1
 db 0x2e ;; cs:
  mov bp, cirrus_vesa_handlers[bx]
  pop bx
  push #cirrus_return
  push bp
  ret

cirrus_vesa_not_handled:
  mov ax, #0x014F ;; not implemented
  jmp cirrus_return

#ifdef CIRRUS_DEBUG
cirrus_call_debug_dump:
  push es
  push ds
  pusha
  push cs
  pop ds
  call _int10_call_debugmsg
  popa
  pop ds
  pop es
  ret
cirrus_ret_debug_dump:
  push es
  push ds
  pusha
  push cs
  pop ds
  call _int10_ret_debugmsg
  popa
  pop ds
  pop es
  ret
#endif

cirrus_set_video_mode_extended:
  call cirrus_switch_mode
  pop  ax ;; mode
  test al, #0x80
  jnz  cirrus_set_video_mode_extended_1
  call cirrus_clear_vram
cirrus_set_video_mode_extended_1:
  and  al, #0x7f
  call cirrus_set_video_mode_bda
  SET_INT_VECTOR(0x43, #0xC000, #_vgafont16)
  mov  ax, #0x20
  pop  si
  jmp  cirrus_return

cirrus_set_video_mode_bda:
  push bx
  push cx
  push ds
  push cs
  pop ds
  mov bx, [si+2]
  shr bx, #3
  mov cx, [si+4]
  shr cx, #4 ; cheight 16
  dec cx
#ifdef CIRRUS_VESA3_PMINFO
 db 0x2e ;; cs:
  mov si, [cirrus_vesa_sel0000_data]
#else
  xor si, si
#endif
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

  .align 2
cirrus_vesa_pm_start:
  dw cirrus_vesa_pm_set_window - cirrus_vesa_pm_start
  dw cirrus_vesa_pm_set_display_start - cirrus_vesa_pm_start
  dw cirrus_vesa_pm_set_palette_data - cirrus_vesa_pm_start
  dw 0x0000

  USE32
cirrus_vesa_pm_set_window:
  or   bx, bx
  jnz  cirrus_vesa_pm_get_window
  cmp dl, #0x40 ;; address must be < 0x40
  jnc cirrus_vesa_pm_get_window
  mov al, bl ;; bl=bank number
  add al, #0x09
  mov ah, dl ;; dx=window address in granularity
  shl ah, #2
  mov dx, #0x3ce
  out dx, ax
cirrus_vesa_pm_get_window:
  mov al, bl ;; bl=bank number
  add al, #0x09
  mov dx, #0x3ce
  out dx, al
  inc dx
  in  al, dx
  shr al, #2
  xor ah, ah
  mov dx, ax
  ret

cirrus_vesa_pm_set_display_start:
  cmp  bl, #0x80
  je   cirrus_vesa_pm_set_display_start1
  cmp  bl, #0x00
  je   cirrus_vesa_pm_set_display_start2
  mov  ax, #0x0100
  ret
cirrus_vesa_pm_set_display_start1:
  push dx
  mov  dx, #VGAREG_ACTL_RESET
cirrus_vesa_pm_wait_1:
  in   al, dx
  test al, #0x08
  jz   cirrus_vesa_pm_wait_1
  pop  dx
cirrus_vesa_pm_set_display_start2:
  push bx
  push dx
  push cx
  mov  dx, #VGAREG_VGA_CRTC_ADDRESS
  mov  al, #0x0d
  out  dx, al
  inc  dx
  pop  ax
  out  dx, al
  dec  dx
  mov  al, #0x0c
  out  dx, ax
  mov  al, #0x1d
  out  dx, al
  inc  dx
  in   al, dx
  and  al, #0x7f
  pop  bx
  mov  ah, bl
  shl  bl, #4
  and  bl, #0x80
  or   al, bl
  out  dx, al
  dec  dx
  mov  bl, ah
  and  ah, #0x01
  shl  bl, #1
  and  bl, #0x0c
  or   ah, bl
  mov  al, #0x1b
  out  dx, al
  inc  dx
  in   al, dx
  and  al, #0xf2
  or   al, ah
  out  dx, al
  pop  bx
  mov  ax, #0x004f
  ret

cirrus_vesa_pm_set_palette_data:
  cmp  bl, #0x80
  jne  cirrus_vesa_pm_no_wait
  push dx
  mov  dx, #VGAREG_ACTL_RESET
cirrus_vesa_pm_wait_2:
  in   al, dx
  test al, #0x08
  jz   cirrus_vesa_pm_wait_2
  pop  dx
cirrus_vesa_pm_no_wait:
  push  ecx
  push  edx
  push  edi
  mov   al, dl
  mov   dx, # VGAREG_DAC_WRITE_ADDRESS
  out   dx, al
  mov   dx, # VGAREG_DAC_DATA
  cld
cirrus_vesa_pm_set_dac_loop:
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
  loop  cirrus_vesa_pm_set_dac_loop
  pop   edi
  pop   edx
  pop   ecx
  mov   ax, #0x004f
  ret
  USE16
cirrus_vesa_pm_end:

#ifdef CIRRUS_VESA3_PMINFO
cirrus_vesa_pmbios_init:
  retf
cirrus_vesa_pmbios_entry:
  pushf
  push bp
  cmp ah, #0x4F
  jnz cirrus_vesa_pmbios_unimplemented
  cmp al, #0x0F
  ja cirrus_vesa_pmbios_unimplemented
  push bx
  xor bx, bx
  mov bl, al
  shl bx, 1
 db 0x2e ;; cs:
  mov bp, cirrus_vesa_handlers[bx]
  pop bx
  push #cirrus_vesa_pmbios_return
  push bp
  ret
cirrus_vesa_pmbios_unimplemented:
  mov ax, #0x014F
cirrus_vesa_pmbios_return:
  pop bp
  popf
  retf
#endif

; in si:mode table
cirrus_switch_mode:
  push ds
  push bx
  push dx
  push cs
  pop ds

  call cirrus_set_vga_vclk

  mov bx, [si+10] ;; seq
  mov dx, #0x3c4
  mov ax, #0x1206
  out dx, ax ;; Unlock cirrus special
  call cirrus_switch_mode_setregs

  mov bx, [si+12] ;; graph
  mov dx, #0x3ce
  call cirrus_switch_mode_setregs

  mov bx, [si+14] ;; crtc
  call get_crtc_address
  call cirrus_switch_mode_setregs
  mov  al, #0x11
  out  dx, al
  inc  dx
  in   al, dx
  or   al, #0x80
  out  dx, al ;; lock CRTC regs 0 - 7

  mov  bx, [si+2]
  or   bx, bx
  jz   no_vclk_setup
  mov  dx, #VGAREG_READ_MISC_OUTPUT
  in   al, dx
  and  al, #0xf3
  cmp  bx, #0x280
  jbe  set_vclk_sel
  and  al, #0x33
  or   al, #0x0c
set_vclk_sel:
  mov  dx, #VGAREG_WRITE_MISC_OUTPUT
  out  dx, al
no_vclk_setup:

  mov dx, #0x3c6
  mov al, #0x00
  out dx, al
  in al, dx
  in al, dx
  in al, dx
  in al, dx
  mov al, [si+8]  ;; hidden dac
  out dx, al
  mov al, #0xff
  out dx, al

  mov  al, #0x00
  mov  ah, #0xbe
  mov  bl, [si+17]  ;; memory model
  or   bl, bl
  jz   is_text_mode
  mov  al, #0x01
  mov  ah, #0xb2
  cmp  bl, #0x03
  jz   is_text_mode
  or   al, #0x40
is_text_mode:
  mov  bl, #0x10 ;; mode control
  call biosfn_get_single_palette_reg
  and  bh, ah
  or   bh, al
  call biosfn_set_single_palette_reg
  and  bh, #0x01
  xor  bh, #0x01
  shl  bh, #3
  mov  bl, #0x13 ;; horiz. pel panning
  call biosfn_set_single_palette_reg

  cmp  [si+6], #0x08  ;; bpp
  ja   no_8bpp_mode
  mov  al, #0x03
  cmp  [si+6], #0x04
  ja   palette_ok
  dec  al
palette_ok:
  call load_dac_palette
no_8bpp_mode:

  pop dx
  pop bx
  pop ds
  ret

cirrus_enable_16k_granularity:
  push ax
  push dx
  mov dx, #0x3ce
  mov al, #0x0b
  out dx, al
  inc dx
  in al, dx
  or al, #0x20 ;; enable 16k
  out dx, al
  pop dx
  pop ax
  ret

cirrus_disable_16k_granularity:
  push ax
  push dx
  mov dx, #0x3ce
  mov al, #0x0b
  out dx, al
  inc dx
  in  al, dx
  and al, #0xdf ;; disable 16k
  out dx, al
  pop dx
  pop ax
  ret

cirrus_switch_mode_setregs:
csms_1:
  mov ax, [bx]
  cmp ax, #0xffff
  jz csms_2
  out dx, ax
  add bx, #0x2
  jmp csms_1
csms_2:
  ret

cirrus_set_vga_vclk:
  mov dx, #0x3C4
  mov ax, #0x4a0b ; VCLK #0 setup
  out dx, ax
  mov ax, #0x2b1b
  out dx, ax
  mov ax, #0x5b0c ; VCLK #1 setup
  out dx, ax
  mov ax, #0x2f1c
  out dx, ax
  ret

; code for 'write character' support in 8bpp graphics modes

; called from C code
_cirrus_is_8bpp_mode:
cirrus_is_8bpp_mode:
  push dx
  mov  dx, #0x03c4
  mov  al, #0x07
  out  dx, al
  inc  dx
  in   al, dx
  pop  dx
  and  al, #0x0f
  cmp  al, #0x01
  jz   is_cirrus_8bpp
  xor  ax, ax
is_cirrus_8bpp:
  ret

cirrus_pm_get_bda_word:
  push ds
  push bx
  push si
#ifdef CIRRUS_VESA3_PMINFO
 db 0x2e ;; cs:
  mov  si, [cirrus_vesa_sel0000_data]
#else
  xor  si, si
#endif
  mov  ds, si
  mov  bh, #0x04
  mov  bl, al
  mov  ax, [bx]
  pop  si
  pop  bx
  pop  ds
  ret

; in: regnum in BL, value in AX
cirrus_write_bltreg16:
  push dx
  push ax
  mov  ah, al
  mov  al, bl
  mov  dx, #0x03ce
  out  dx, ax
  pop  ax
  push ax
  inc  bl
  mov  al, bl
  out  dx, ax
  pop  ax
  pop  dx
  ret

; in: columns in AL rows in BL / out: size of line in AX
cirrus_write_bltsize:
  xor  ah, ah
  shl  ax, #3
  dec  ax
  push bx
  mov  bl, #0x20
  call cirrus_write_bltreg16
  pop  bx
  mov  ax, 14[bp] ; cheight
  push ax
  mul  bl
  dec  ax
  mov  bl, #0x22
  call cirrus_write_bltreg16
  mov  ax, 12[bp] ; nbrows
  shl  ax, #3
  mov  bl, #0x24
  call cirrus_write_bltreg16
  mov  bl, #0x26
  call cirrus_write_bltreg16
  pop bx
  mul bx
  ret

; in: size of line in AX, ypos in BL, regnum in BH, xpos in CL
cirrus_write_bltaddr:
  push ax
  push bx
  xor  bh, bh
  mul  bx
  mov  bl, cl
  shl  bx, #3
  add  ax, bx
  jnc  cirrus_addr_noinc
  inc  dx
cirrus_addr_noinc:
  pop  bx
  mov  bl, bh
  call cirrus_write_bltreg16
  mov  ax, dx
  mov  ah, al
  mov  al, bh
  inc  al
  inc  al
  mov  dx, #0x03ce
  out  dx, ax
  pop  ax
  ret

cirrus_bitblt_write_char:
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
  mov  cl, 8[bp]  ; xcurs
  mov  ch, 10[bp] ; ycurs
  mov  al, #0x01
  mov  bl, #0x01
  call cirrus_write_bltsize
  mov  bl, ch
  mov  bh, #0x28
  call cirrus_write_bltaddr
  mov  ax, #0x0001 ; src pitch 1
  mov  bl, #0x26
  call cirrus_write_bltreg16
  mov  dx, #0x03ce
  mov  ax, #0x8430 ; colorexpand memsrc
  out  dx, ax
  mov  ax, #0x0d32 ; ROP: copy
  out  dx, ax
  xor  ax, ax ; background
  out  dx, ax
  mov  ah, 6[bp] ; attr
  mov  al, #0x01
  out  dx, ax
  mov  ax, #0x0231 ; start
  out  dx, ax
  mov  ax, #0xa000
  mov  es, ax
  xor  di, di
  mov  cx, 14[bp] ; cheight
  cld
  rep
    movsb
  pop  di
  pop  si
  pop  es
  pop  ds
  pop  dx
  pop  cx
  pop  bx
  pop  ax
  ret

cirrus_bitblt_copy:
  push ax
  push bx
  push cx
  push dx
  mov  cl, 4[bp] ; xstart
  mov  ch, 8[bp] ; ydest
  mov  al, 10[bp] ; cols
  mov  bl, #0x01
  call cirrus_write_bltsize
  mov  bl, 6[bp] ; ysrc
  mov  bh, #0x2c
  call cirrus_write_bltaddr
  mov  bl, ch
  mov  bh, #0x28
  call cirrus_write_bltaddr
  mov  dx, #0x03ce
  mov  ax, #0x0030 ; normal blt
  out  dx, ax
  mov  ax, #0x0d32 ; ROP: copy
  out  dx, ax
  mov  ax, #0x0231 ; start
  out  dx, ax
  pop  dx
  pop  cx
  pop  bx
  pop  ax
  ret

cirrus_bitblt_fill:
  push ax
  push bx
  push dx
  mov  cl, 4[bp] ; xstart
  mov  ch, 6[bp] ; ydest
  mov  al, 8[bp] ; cols
  mov  bl, 10[bp] ; rows
  call cirrus_write_bltsize
  mov  bl, ch
  mov  bh, #0x28
  call cirrus_write_bltaddr
  mov  dx, #0x03ce
  mov  ax, #0x0030 ; normal blt
  out  dx, ax
  mov  ax, #0x0e32 ; ROP: 1
  out  dx, ax
  mov  ax, #0x0433 ; solidfill
  out  dx, ax
  mov  ah, 16[bp] ; attr
  mov  al, #0x01
  out  dx, ax
  mov  ax, #0x0231
  out  dx, ax
  pop  dx
  pop  bx
  pop  ax
  ret

cirrus_extbios_80h:
  push dx
  call get_crtc_address
  mov al, #0x27
  out dx, al
  inc dx
  in al, dx
  mov bx, #_cirrus_id_table
c80h_1:
 db 0x2e ;; cs:
  mov ah, [bx]
  cmp ah, al
  jz c80h_2
  cmp ah, #0xff
  jz c80h_2
  inc bx
  inc bx
  jmp c80h_1
c80h_2:
 db 0x2e ;; cs:
  mov al, 0x1[bx]
  pop dx
  mov ah, #0x00
  xor bx, bx
  ret

cirrus_extbios_81h:
  mov ax, #0x100 ;; XXX
  ret
cirrus_extbios_82h:
  push dx
  call get_crtc_address
  xor ax, ax
  mov al, #0x27
  out dx, al
  inc dx
  in al, dx
  and al, #0x03
  mov ah, #0xAF
  pop dx
  ret

cirrus_extbios_85h:
  push cx
  push dx
  mov dx, #0x3C4
  mov al, #0x0f ;; get DRAM band width
  out dx, al
  inc dx
  in al, dx
  ;; al = 4 << bandwidth
  mov cl, al
  shr cl, #0x03
  and cl, #0x03
  cmp cl, #0x03
  je c85h2
  mov al, #0x04
  shl al, cl
  jmp c85h3
c85h2:
;; 4MB or 2MB
  and al, #0x80
  mov al, #0x20 ;; 2 MB
  je c85h3
  mov al, #0x40 ;; 4 MB
c85h3:
  pop dx
  pop cx
  ret

cirrus_extbios_9Ah:
  mov ax, #0x4060
  mov cx, #0x1132
  ret

cirrus_extbios_A0h:
  call cirrus_get_modeentry
  mov ah, #0x01
  sbb ah, #0x00
  mov bx, cirrus_extbios_A0h_callback
  mov si, #0xffff
  mov di, bx
  mov ds, bx
  mov es, bx
  ret

cirrus_extbios_A0h_callback:
  ;; fatal: not implemented yet
  cli
  hlt
  retf

cirrus_extbios_A1h:
  mov bx, #0x0E00 ;; IBM 8512/8513, color
  ret

cirrus_extbios_A2h:
  mov al, #0x07   ;; HSync 31.5 - 64.0 kHz
  ret

cirrus_extbios_AEh:
  mov al, #0x01   ;; High Refresh 75Hz
  ret

cirrus_extbios_unimplemented:
  ret

cirrus_vesa_00h:
  push ds
  push si
  push bx
  mov  bp, di
  push es
  pop  ds
  xor  bx, bx
  cld
  mov  ax, [di]
  cmp  ax, #0x4256 ;; VB
  jnz  cv00_1
  mov  ax, [di+2]
  cmp  ax, #0x3245 ;; E2
  jnz  cv00_1
  ;; VBE2
  lea  di, 0x100[bp]
  mov  bx, di
  mov  si, #cirrus_vesa_oemname
  call strcpy
  add  bx, ax
  mov  ax, di
  lea  di, 0x06[bp]
  stosw
  mov  ax, es
  stosw
  lea  di, 0x14[bp]
  mov  ax, #0x0100 ;; soft ver.
  stosw
  push di
  mov  si, #cirrus_vesa_vendorname
  mov  di, bx
  call strcpy
  add  bx, ax
  mov  ax, di
  pop  di
  stosw
  mov  ax, es
  stosw
  push di
  mov  si, #cirrus_vesa_productname
  mov  di, bx
  call strcpy
  add  bx, ax
  mov  ax, di
  pop  di
  stosw
  mov  ax, es
  stosw
  push di
  mov  si, #cirrus_vesa_productrevision
  mov  di, bx
  call strcpy
  mov  ax, di
  pop  di
  stosw
  mov  ax, es
  stosw
cv00_1:
  mov  di, bp
  mov  ax, #0x4556 ;; VE
  stosw
  mov  ax, #0x4153 ;; SA
  stosw
  mov  ax, #0x0200 ;; v2.00
  stosw
  or   bx, bx
  jnz  cv00_2 ; already set for VBE 2.0
  mov  ax, #cirrus_vesa_oemname
  stosw
  mov  ax, cs
  stosw
  jz   cv00_3
cv00_2:
  add  di, #4
cv00_3:
  xor  ax, ax ;; caps
  stosw
  stosw
  lea  ax, 0x22[bp]
  stosw
  mov  ax, es
  stosw
  call cirrus_extbios_85h ;; vram in 64k
  mov  ah, #0x00
  stosw

  push cs ; build mode list
  pop  ds
  lea  di, 0x22[bp]
  mov  si, #_cirrus_vesa_modelist
cv00_4:
  lodsw
  stosw
  add  si, #2
  cmp  ax, #0xffff
  jnz  cv00_4

  mov  ax, #0x004F ; no error
  mov  di, bp
  pop  bx
  pop  si
  pop  ds
  ret

cirrus_vesa_01h:
  mov ax, cx
  and ax, #0x3fff
  cmp ax, #0x0080
  jb  cirrus_no_vesamode
  call cirrus_vesamode_to_mode
  cmp ax, #0xffff
  jnz cirrus_vesa_01h_1
  jmp cirrus_vesa_unimplemented
cirrus_no_vesamode:
  push si
  call cirrus_get_modeentry_nomask
  pop  si
  jnc cirrus_vesa_01h_1
  jmp cirrus_vesa_unimplemented
cirrus_vesa_01h_1:
  push ds
  push si
  push cx
  push dx
  push bx
  mov bp, di
  cld
  push cs
  pop ds
  call cirrus_get_modeentry_nomask

  push di
  xor ax, ax
  mov cx, #0x80
  rep
    stosw ;; clear buffer
  pop di

  mov  ax, #0x001b ;; mode attr
  push bx
  mov  bl, [si+6] ;; bpp
  cmp  bl, #0x08
  ja   cirrus_vesa_01h_2
  or   al, #0x04 ;; TTY support
cirrus_vesa_01h_2:
  pop  bx
  stosw
  mov ax, #0x0007 ;; win attr
  stosw
  mov ax, #0x0040 ;; granularity =64K
  stosw
  mov ax, #0x0040 ;; size =64K
  stosw
  mov ax, #0xA000 ;; segment A
  stosw
  xor ax, ax ;; no segment B
  stosw
  mov ax, #cirrus_vesa_05h_farentry
  stosw
  mov ax, cs
  stosw
  call cirrus_get_line_offset_entry
  stosw ;; bytes per scan line
  mov ax, [si+2] ;; width
  stosw
  mov ax, [si+4] ;; height
  stosw
  mov ax, #0x08
  stosb
  mov ax, #0x10
  stosb
  mov al, #1 ;; count of planes
  cmp byte ptr [si+17], #0x03
  jne cirrus_vesa_01h_3
  mov al, #4
cirrus_vesa_01h_3:
  stosb
  mov al, [si+6] ;; bpp
  stosb
  mov al, #0x1 ;; XXX number of banks
  stosb
  mov al, [si+17]
  stosb ;; memory model
  mov al, #0x0   ;; XXX size of bank in K
  stosb
  call cirrus_get_line_offset_entry
  mov bx, [si+4]
  mul bx ;; dx:ax=vramdisp
  or ax, ax
  jz cirrus_vesa_01h_4
  inc dx
cirrus_vesa_01h_4:
  mov al, #0x04
  cmp byte ptr [si+17], #0x03
  je  cirrus_vesa_01h_5
  call cirrus_extbios_85h ;; al=vram in 64k
cirrus_vesa_01h_5:
  xor ah, ah
  mov cx, dx
  xor dx, dx
  div cx
  dec ax
  stosb  ;; number of image pages = vramtotal/vramdisp-1
  mov al, #0x01
  stosb

  ;; v1.2+ stuffs
  mov  ax, [si+18]
  push si
  mov  si, ax
  movsw
  movsw
  movsw
  movsw
  pop  si

  mov ah, [si+16]
  mov al, #0x0
  sub ah, #9
  rcl al, #1 ; bit 0=palette flag
  stosb ;; direct screen mode info

  ;; v2.0+ stuffs
  ;; 32-bit LFB address
  xor ax, ax
  stosw
  cmp byte ptr [si+17], #0x03
  je  cirrus_vesa_01h_6
  mov ax, #0x1013 ;; vendor Cirrus
  call pci_get_lfb_addr
cirrus_vesa_01h_6:
  stosw
  or ax, ax
  jz cirrus_vesa_01h_7
  push di
  mov di, bp
 db 0x26 ;; es:
  mov ax, [di]
  or ax, #0x0080 ;; mode bit 7:LFB
  stosw
  pop di
cirrus_vesa_01h_7:

  xor ax, ax
  stosw ; reserved
  stosw ; reserved
  stosw ; reserved

  mov ax, #0x004F
  mov di, bp
  pop bx
  pop dx
  pop cx
  pop si
  pop ds

  test cx, #0x4000 ;; LFB flag
  jz   cirrus_vesa_01h_9
  push cx
 db 0x26 ;; es:
  mov  cx, [di]
  test cx, #0x0080 ;; is LFB supported?
  jnz  cirrus_vesa_01h_8
  mov  ax, #0x014F ;; error - no LFB
cirrus_vesa_01h_8:
  pop  cx
cirrus_vesa_01h_9:
  ret

cirrus_vesa_02h:
  test bx, #0x3e00
  jnz cirrus_vesa_02h_2 ;; unknown flags
  mov ax, bx
  and ax, #0x1ff ;; bit 8-0 mode
  cmp ax, #0x100 ;; legacy VGA mode
  jb cirrus_vesa_02h_legacy
  call cirrus_vesamode_to_mode
  cmp ax, #0xffff
  jnz cirrus_vesa_02h_1
cirrus_vesa_02h_2:
  jmp  cirrus_vesa_unimplemented
cirrus_vesa_02h_legacy:
  test al, #0x80
  jnz  cirrus_vesa_02h_2
  test bx, #0x8000
  jz   cirrus_vesa_02h_legacy_noclear
  or   al, #0x80
cirrus_vesa_02h_legacy_noclear:
#ifdef CIRRUS_VESA3_PMINFO
 db 0x2e ;; cs:
  cmp  byte ptr [cirrus_vesa_is_protected_mode], #0
  jnz  cirrus_vesa_02h_2
#endif // CIRRUS_VESA3_PMINFO
  int #0x10
  mov ax, #0x004F
  ret
cirrus_vesa_02h_1:
  push si
  push ax
  call cirrus_get_modeentry_nomask
  call cirrus_switch_mode
  test bx, #0x8000 ;; no clear
  jnz cirrus_vesa_02h_3
  call cirrus_clear_vram
cirrus_vesa_02h_3:
  test bx, #0x4000 ;; LFB
  jnz cirrus_vesa_02h_4
  call cirrus_enable_16k_granularity
cirrus_vesa_02h_4:
  pop ax
  call cirrus_set_video_mode_bda
  SET_INT_VECTOR(0x43, #0xC000, #_vgafont16)
  push ds
#ifdef CIRRUS_VESA3_PMINFO
 db 0x2e ;; cs:
  mov si, [cirrus_vesa_sel0000_data]
#else
  xor si, si
#endif
  mov ds, si
  mov [PM_BIOSMEM_VBE_MODE], bx
  pop ds
  pop si
  mov ax, #0x004F
  ret

cirrus_vesa_03h:
  push ds
#ifdef CIRRUS_VESA3_PMINFO
 db 0x2e ;; cs:
  mov ax, [cirrus_vesa_sel0000_data]
#else
  xor ax, ax
#endif
  mov  ds, ax
  mov  bx, # PM_BIOSMEM_VBE_MODE
  mov  ax, [bx]
  mov  bx, ax
  test bx, bx
  jnz   cirrus_vesa_03h_1
  mov  bx, # PM_BIOSMEM_CURRENT_MODE
  mov  al, [bx]
  mov  bl, al
  xor  bh, bh
cirrus_vesa_03h_1:
  mov  ax, #0x004f
  pop  ds
  ret

cirrus_vesa_05h_farentry:
  call cirrus_vesa_05h
  retf

cirrus_vesa_05h:
  cmp bl, #0x01
  ja cirrus_vesa_05h_1
  cmp bh, #0x00
  jz cirrus_vesa_05h_setmempage
  cmp bh, #0x01
  jz cirrus_vesa_05h_getmempage
cirrus_vesa_05h_1:
  jmp cirrus_vesa_unimplemented
cirrus_vesa_05h_setmempage:
  cmp dx, #0x40 ;; address must be < 0x40
  jnc cirrus_vesa_05h_1
  push dx
  mov al, bl ;; bl=bank number
  add al, #0x09
  mov ah, dl ;; dx=window address in granularity
  shl ah, #2
  mov dx, #0x3ce
  out dx, ax
  pop dx
  mov ax, #0x004F
  ret
cirrus_vesa_05h_getmempage:
  mov al, bl ;; bl=bank number
  add al, #0x09
  mov dx, #0x3ce
  out dx, al
  inc dx
  in al, dx
  shr al, #2
  xor dx, dx
  mov dl, al ;; dx=window address in granularity
  mov ax, #0x004F
  ret

cirrus_vesa_06h:
  mov  ax, cx
  cmp  bl, #0x01
  je   cirrus_vesa_06h_bl1
  cmp  bl, #0x02
  je   cirrus_vesa_06h_bl2
  jb   cirrus_vesa_06h_bl0
  mov  ax, #0x0100
  ret
cirrus_vesa_06h_bl0:
  call cirrus_get_bpp_bytes
  jnc  cirrus_vesa_06h_bl0_4bpp
  mov  bl, al
  xor  bh, bh
  mov  ax, cx
  mul  bx
  jmp  cirrus_vesa_06h_bl2
cirrus_vesa_06h_bl0_4bpp:
  call cirrus_get_scanlines
  mov  bx, ax
  mov  dx, #0x04
  xor  ax, ax
  div  bx
  shl  ax, #3
  mov  bx, ax
  mov  ax, cx
  cmp  ax, bx
  jbe  cirrus_vesa_06h_bl0_4bpp_1
  mov  ax, bx
cirrus_vesa_06h_bl0_4bpp_1:
  shr  ax, #3
cirrus_vesa_06h_bl2:
  call cirrus_set_line_offset
cirrus_vesa_06h_bl1:
  call cirrus_get_line_offset
  mov  bx, ax
  call cirrus_get_bpp_bytes
  jnc  cirrus_vesa_06h_bl1_4bpp
  mov  cl, al
  xor  ch, ch
  mov  ax, bx
  xor  dx, dx
  div  cx
  mov  cx, ax
  jmp  cirrus_vesa_06h_bl1_1
cirrus_vesa_06h_bl1_4bpp:
  mov  cx, bx
  shl  cx, #3
cirrus_vesa_06h_bl1_1:
  call cirrus_get_vram_avail ;; al=vram in 64k
  mov  dl, al
  xor  dh, dh
  xor  ax, ax
  div  bx
  mov  dx, ax
  mov  ax, #0x004f
  ret

cirrus_vesa_07h:
  cmp  bl, #0x80
  je   cirrus_vesa_07h_bl80
  cmp  bl, #0x01
  je   cirrus_vesa_07h_bl1
  jb   cirrus_vesa_07h_bl0
  mov  ax, #0x0100
  ret
cirrus_vesa_07h_bl80:
  call vbebios_vsync_wait
cirrus_vesa_07h_bl0:
  push bx
  push dx
  call cirrus_get_bpp_bytes
  jnc  cirrus_vesa_07h_bl0_4bpp
  mov  bl, al
  xor  bh, bh
  mov  ax, cx
  mul  bx
  mov  cx, ax
  pop  bx
  push bx
  call cirrus_get_line_offset
  mul  bx
  add  ax, cx
  jnc  cirrus_vesa_07h_3
  inc  dx
cirrus_vesa_07h_3:
  shr  dx, #1
  rcr  ax, #1
  shr  dx, #1
  rcr  ax, #1
  jmp  cirrus_vesa_07h_bl0_set
cirrus_vesa_07h_bl0_4bpp:
  mov  bh, cl
  and  bh, #0x07
  mov  bl, #0x13 ;; horiz. pel panning
  call biosfn_set_single_palette_reg
  push cx
  shr  cx, #3
  call cirrus_get_line_offset
  mov  bx, dx
  mul  bx
  add  ax, cx
  pop  cx
  jnc  cirrus_vesa_07h_bl0_set
  inc  dx
cirrus_vesa_07h_bl0_set:
  call cirrus_set_start_addr
  pop  dx
  pop  bx
  mov  ax, #0x004f
  ret
cirrus_vesa_07h_bl1:
  call cirrus_get_start_addr
  call stdvga_is_4bpp_mode
  jc   cirrus_vesa_07h_bl1_4bpp_1
  shl  ax, #1
  rcl  dx, #1
  shl  ax, #1
  rcl  dx, #1
cirrus_vesa_07h_bl1_4bpp_1:
  push ax
  call cirrus_get_line_offset
  mov  bx, ax
  pop  ax
  div  bx
  push ax
  push dx
  call cirrus_get_bpp_bytes
  jnc  cirrus_vesa_07h_bl1_4bpp_2
  mov  bl, al
  xor  bh, bh
  pop  ax
  xor  dx, dx
  div  bx
  mov  cx, ax
  pop  dx
  mov  ax, #0x004f
  ret
cirrus_vesa_07h_bl1_4bpp_2:
  pop  cx
  shl  cx, #3
  mov  bl, #0x13 ;; horiz. pel panning
  call biosfn_get_single_palette_reg
  and  bh, #0x07
  or   cl, bh
  pop  dx
  mov  ax, #0x004f
  ret

cirrus_vesa_0ah:
  test bl, bl
  jnz cirrus_vesa_0ah_fail
  mov di, #0xc000
  mov es, di
  mov di, # cirrus_vesa_pm_start
  mov cx, # cirrus_vesa_pm_end
  sub cx, di
  mov ax, #0x004f
  ret
cirrus_vesa_0ah_fail:
  mov ax, #0x014f
  ret

cirrus_vesa_10h:
  cmp bl, #0x00
  jne cirrus_vesa_10h_01
  mov bx, #0x0f30
  mov ax, #0x004f
  ret
cirrus_vesa_10h_01:
  cmp bl, #0x01
  jne cirrus_vesa_10h_02
  push dx
  push ds
  mov dx, #0x40
  mov ds, dx
  mov [0xb9], bh
  pop ds
  pop dx
  mov ax, #0x004f
  ret
cirrus_vesa_10h_02:
  cmp bl, #0x02
  jne cirrus_vesa_unimplemented
  push dx
  push ds
  mov dx, #0x40
  mov ds, dx
  mov bh, [0xb9]
  pop ds
  pop dx
  mov ax, #0x004f
  ret

;; DDC helper functions for VESA 15h

cirrus_ddc_delay:
  in   al, 0x61
  and  al, #0x10
  mov  ah, al
cirrus_ddc_delay_01:
  nop
  in   al, 0x61
  and  al, #0x10
  cmp  al, ah
  jz   cirrus_ddc_delay_01
  ret

cirrus_ddc_set_dck:
  mov  ah, #0x01
  db   0xa9 ;; skip next opcode (TEST AX, #0x02b4)
cirrus_ddc_set_dda:
  mov  ah, #0x02
  mov  dx, #0x3c4
  mov  al, #0x08
  out  dx, al
  inc  dx
  in   al, dx
  or   al, ah
  out  dx, al
  ret

cirrus_ddc_clr_dck:
  mov  ah, #0x01
  db   0xa9 ;; skip next opcode (see above)
cirrus_ddc_clr_dda:
  mov  ah, #0x02
  xor  ah, #0xff
  mov  dx, #0x3c4
  mov  al, #0x08
  out  dx, al
  inc  dx
  in   al, dx
  and  al, ah
  out  dx, al
  ret

cirrus_ddc_init:
  mov  dx, #0x3c4
  mov  ax, #0x4308
  out  dx, ax
  call cirrus_ddc_clr_dck
  in   al, dx
  and  al, #0x04
  pushf
  call cirrus_ddc_set_dck
  popf
  ret

cirrus_ddc_start:
  call cirrus_ddc_clr_dda
  call cirrus_ddc_clr_dck
  ret

cirrus_ddc_stop:
  call cirrus_ddc_set_dck
  call cirrus_ddc_set_dda
  ret

cirrus_ddc_get_dda:
  mov  dx, #0x3c4
  mov  al, #0x08
  out  dx, al
  inc  dx
  in   al, dx
  and  al, #0x80
  shl  al, #0x01
  ret

cirrus_ddc_send_bit:
  push ax
  pushf
  call cirrus_ddc_delay
  popf
  jc   cirrus_ddc_send_bit_01
  call cirrus_ddc_clr_dda
  jnz  cirrus_ddc_send_bit_02
cirrus_ddc_send_bit_01:
  call cirrus_ddc_set_dda
cirrus_ddc_send_bit_02:
  call cirrus_ddc_set_dck
  call cirrus_ddc_delay
  call cirrus_ddc_clr_dck
  pop  ax
  ret

cirrus_ddc_read_bit:
  push ax
  call cirrus_ddc_delay
  call cirrus_ddc_set_dck
  call cirrus_ddc_get_dda
  pushf
  call cirrus_ddc_delay
  call cirrus_ddc_clr_dck
  popf
  pop  ax
  ret

cirrus_ddc_send_byte:
  push cx
  mov  cx, #0x08
cirrus_ddc_send_byte_01:
  shl  al, #0x01
  call cirrus_ddc_send_bit
  loop cirrus_ddc_send_byte_01
  call cirrus_ddc_set_dda
  call cirrus_ddc_delay
  call cirrus_ddc_set_dck
  call cirrus_ddc_get_dda
  pushf
  call cirrus_ddc_clr_dck
  call cirrus_ddc_clr_dda
  popf
  pop  cx
  ret

cirrus_ddc_read_byte:
  push cx
  call cirrus_ddc_set_dda
  mov  al, #0x00
  mov  cx, #0x08
cirrus_ddc_read_byte_01:
  call cirrus_ddc_read_bit
  rcl  al, #0x01
  loop cirrus_ddc_read_byte_01
  pop  cx
  ret

cirrus_ddc_send_status:
  cmp  cx, #0x01
  jz   cirrus_ddc_send_status_01
  call cirrus_ddc_clr_dda
cirrus_ddc_send_status_01:
  call cirrus_ddc_delay
  call cirrus_ddc_set_dck
  call cirrus_ddc_delay
  call cirrus_ddc_clr_dck
  ret

cirrus_vesa_15h:
  cmp bl,#0x01
  jb  cirrus_vesa_get_capabilities
  je  cirrus_vesa_read_EDID
  jmp cirrus_vesa_unimplemented

cirrus_vesa_get_capabilities:
  test cx,cx
  jne  cirrus_vesa_unimplemented
  mov  ax, #0x004f
  mov  bx, #0x0202
  ret

cirrus_vesa_read_EDID:
  mov  dx, #0x03c4
  in   al, dx
  push ax
  call cirrus_ddc_init
  jnz  cirrus_vesa_no_ddc
  call cirrus_ddc_start
  call cirrus_ddc_delay
  mov  al, #0xa0
  call cirrus_ddc_send_byte
  jc   cirrus_vesa_no_ddc
  mov  al, #0x00
  call cirrus_ddc_send_byte
  jc   cirrus_vesa_no_ddc
  call cirrus_ddc_stop
  call cirrus_ddc_start
  call cirrus_ddc_delay
  mov  al, #0xa1
  call cirrus_ddc_send_byte
  jc   cirrus_vesa_no_ddc
  push cx
  push di
  mov  cx, #0x0080
  cld
cirrus_vesa_15h_01:
  call cirrus_ddc_read_byte
  stosb
  call cirrus_ddc_send_status
  loop cirrus_vesa_15h_01
  call cirrus_ddc_stop
  pop  di
  pop  cx
  mov  dx, #0x03c4
  pop  ax
  out  dx, al
  mov  ax, #0x004f
  ret

cirrus_vesa_no_ddc:
  mov  dx, #0x03c4
  pop  ax
  out  dx, al
cirrus_vesa_unimplemented:
  mov  ax, #0x014F ;; not implemented
  ret


;; in ax:vesamode, out ax:cirrusmode
cirrus_vesamode_to_mode:
  push ds
  push cx
  push si
  push cs
  pop ds
  mov cx, #0xffff
  mov si, #_cirrus_vesa_modelist
cvtm_1:
  cmp [si],ax
  jz cvtm_2
  cmp [si],cx
  jz cvtm_2
  add si, #4
  jmp cvtm_1
cvtm_2:
  mov ax,[si+2]
  pop si
  pop cx
  pop ds
  ret

;; in - al:mode, out - cflag:result, si:table, ax:destroyed
cirrus_get_modeentry:
  and al, #0x7f
cirrus_get_modeentry_nomask:
  mov si, #_cirrus_modes
cgm_1:
 db 0x2e ;; cs:
  mov ah, [si]
  cmp al, ah
  jz cgm_2
  cmp ah, #0xff
  jz cgm_4
  add si, # CIRRUS_MODE_SIZE
  jmp cgm_1
cgm_4:
  xor si, si
  stc ;; video mode is not supported
  jmp cgm_3
cgm_2:
  clc ;; video mode is supported
cgm_3:
  ret

;; out - al:available vram in 64k
cirrus_get_vram_avail:
  call stdvga_is_4bpp_mode
  jc   limit_4bpp_vram
  jmp  cirrus_extbios_85h
limit_4bpp_vram:
  mov  al, #0x04
  ret

;; out - al:bytes per pixel
cirrus_get_bpp_bytes:
  push dx
  mov  dx, #0x03c4
  mov  al, #0x07
  out  dx, al
  inc  dx
  in   al, dx
  pop  dx
  test al, #0x01
  jz   no_svga_modes
  and  al, #0x0e
  cmp  al, #0x06
  jne  cirrus_get_bpp_bytes_1
  and  al, #0x02
cirrus_get_bpp_bytes_1:
  shr  al, #1
  cmp  al, #0x04
  je  cirrus_get_bpp_bytes_2
  inc  al
cirrus_get_bpp_bytes_2:
  stc
  ret
no_svga_modes:
  clc
  ret

;; in - ax: new line offset
cirrus_set_line_offset:
  shr  ax, #1
  call stdvga_is_4bpp_mode
  jc   set_line_offset_4bpp
  shr  ax, #2
set_line_offset_4bpp:
  cmp  ax, #0x200
  jb   cirrus_offset_ok
  mov  ax, #0x1ff
cirrus_offset_ok:
  push ax
  call get_crtc_address
  mov  al, #0x13
  out  dx, al
  inc  dx
  pop  ax
  out  dx, al
  dec  dx
  mov  al, #0x1b
  out  dx, al
  inc  dx
  shl  ah, #4
  in   al, dx
  and  al, #0xef
  or   al, ah
  out  dx, al
  ret

;; out - ax: active line offset
cirrus_get_line_offset:
  push dx
  push bx
  call get_crtc_address
  mov  al, #0x13
  out  dx, al
  inc  dx
  in   al, dx
  mov  bl, al
  dec  dx
  mov  al, #0x1b
  out  dx, al
  inc  dx
  in   al, dx
  mov  ah, al
  shr  ah, #4
  and  ah, #0x01
  mov  al, bl
  shl  ax, #1
  call stdvga_is_4bpp_mode
  jc   get_line_offset_4bpp
  shl  ax, #2
get_line_offset_4bpp:
  pop  bx
  pop  dx
  ret

;; in - si: table
;; out - ax: line offset for mode
cirrus_get_line_offset_entry:
  push bx
  mov  bx, [si+14] ;; crtc table
  push bx
offset_loop1:
  mov  ax, [bx]
  cmp  al, #0x13
  je   offset_found1
  inc  bx
  inc  bx
  jnz  offset_loop1
offset_found1:
  xor  al, al
  shr  ax, #5
  pop  bx
  push ax
offset_loop2:
  mov  ax, [bx]
  cmp  al, #0x1b
  je offset_found2
  inc  bx
  inc  bx
  jnz offset_loop2
offset_found2:
  pop  bx
  and  ax, #0x1000
  shr  ax, #1
  or   ax, bx
  pop  bx
  cmp byte ptr [si+17], #0x03
  jne offset_normal
  shr ax, #2
offset_normal:
  ret

;; in - new address in DX:AX
cirrus_set_start_addr:
  push bx
  push dx
  push ax
  call get_crtc_address
  mov  al, #0x0d
  out  dx, al
  inc  dx
  pop  ax
  out  dx, al
  dec  dx
  mov  al, #0x0c
  out  dx, al
  inc  dx
  mov  al, ah
  out  dx, al
  dec  dx
  mov  al, #0x1d
  out  dx, al
  inc  dx
  in   al, dx
  and  al, #0x7f
  pop  bx
  mov  ah, bl
  shl  bl, #4
  and  bl, #0x80
  or   al, bl
  out  dx, al
  dec  dx
  mov  bl, ah
  and  ah, #0x01
  shl  bl, #1
  and  bl, #0x0c
  or   ah, bl
  mov  al, #0x1b
  out  dx, al
  inc  dx
  in   al, dx
  and  al, #0xf2
  or   al, ah
  out  dx, al
  pop  bx
  ret

;; out - current address in DX:AX
cirrus_get_start_addr:
  push bx
  call get_crtc_address
  mov  al, #0x0c
  out  dx, al
  inc  dx
  in   al, dx
  mov  ah, al
  dec  dx
  mov  al, #0x0d
  out  dx, al
  inc  dx
  in   al, dx
  push ax
  dec  dx
  mov  al, #0x1b
  out  dx, al
  inc  dx
  in   al, dx
  dec  dx
  mov  bl, al
  and  al, #0x01
  and  bl, #0x0c
  shr  bl, #1
  or   bl, al
  mov  al, #0x1d
  out  dx, al
  inc  dx
  in   al, dx
  and  al, #0x80
  shr  al, #4
  or   bl, al
  mov  dl, bl
  xor  dh, dh
  pop  ax
  pop  bx
  ret

cirrus_clear_vram:
 db 0x2e ;; cs:
  cmp  [si+6], #0x04 ;; bpp
  ja   clear_vram_linear
  push ax
  push dx
  mov  dx, #0x03ce
  mov  ax, #0x0009
vga_clear_loop:
  out  dx, ax
  call _vga_clear_vram_pl4
  add  ah, #0x10
  cmp  ah, #0x40
  jb   vga_clear_loop
  xor  ah, ah
  out  dx, ax
  pop  dx
  pop  ax
  ret

clear_vram_linear:
  pusha
  push es

  call cirrus_enable_16k_granularity
  call cirrus_extbios_85h
  shl al, #2
  mov bl, al
  xor ah,ah
cirrus_clear_vram_1:
  mov al, #0x09
  mov dx, #0x3ce
  out dx, ax
  push ax
  mov cx, #0xa000
  mov es, cx
  xor di, di
  xor ax, ax
  mov cx, #8192
  cld
  rep
      stosw
  pop ax
  inc ah
  cmp ah, bl
  jne cirrus_clear_vram_1

  xor ah,ah
  mov dx, #0x3ce
  out dx, ax
  call cirrus_disable_16k_granularity

  pop es
  popa
  ret

cirrus_get_scanlines:
  call stdvga_get_scanlines
  push ax
  push dx
  call get_crtc_address
  mov  al, #0x1a
  out  dx, al
  inc  dx
  in   al, dx
  test al, #0x01
  pop  dx
  pop  ax
  jz   no_interlace
  shl  ax, #1
no_interlace:
  ret

cirrus_extbios_handlers:
  ;; 80h
  dw cirrus_extbios_80h
  dw cirrus_extbios_81h
  dw cirrus_extbios_82h
  dw cirrus_extbios_unimplemented
  ;; 84h
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_85h
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  ;; 88h
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  ;; 8Ch
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  ;; 90h
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  ;; 94h
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  ;; 98h
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_9Ah
  dw cirrus_extbios_unimplemented
  ;; 9Ch
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  ;; A0h
  dw cirrus_extbios_A0h
  dw cirrus_extbios_A1h
  dw cirrus_extbios_A2h
  dw cirrus_extbios_unimplemented
  ;; A4h
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  ;; A8h
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  ;; ACh
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_unimplemented
  dw cirrus_extbios_AEh
  dw cirrus_extbios_unimplemented

cirrus_vesa_handlers:
  ;; 00h
  dw cirrus_vesa_00h
  dw cirrus_vesa_01h
  dw cirrus_vesa_02h
  dw cirrus_vesa_03h
  ;; 04h
  dw cirrus_vesa_unimplemented
  dw cirrus_vesa_05h
  dw cirrus_vesa_06h
  dw cirrus_vesa_07h
  ;; 08h
  dw cirrus_vesa_unimplemented
  dw vbe_biosfn_set_get_palette_data
  dw cirrus_vesa_0ah
  dw cirrus_vesa_unimplemented
  ;; 0Ch
  dw cirrus_vesa_unimplemented
  dw cirrus_vesa_unimplemented
  dw cirrus_vesa_unimplemented
  dw cirrus_vesa_unimplemented
  ;; 10h
  dw cirrus_vesa_10h
  dw cirrus_vesa_unimplemented
  dw cirrus_vesa_unimplemented
  dw cirrus_vesa_unimplemented
  ;; 14h
  dw cirrus_vesa_unimplemented
  dw cirrus_vesa_15h


ASM_END

#ifdef CIRRUS_VESA3_PMINFO
ASM_START
cirrus_vesa_pminfo:
  /* + 0 */
  .byte 0x50,0x4d,0x49,0x44 ;; signature[4]
  /* + 4 */
  dw cirrus_vesa_pmbios_entry ;; entry_bios
  dw cirrus_vesa_pmbios_init  ;; entry_init
  /* + 8 */
cirrus_vesa_sel0000_data:
  dw 0x0000 ;; sel_00000
cirrus_vesa_selA000_data:
  dw 0xA000 ;; sel_A0000
  /* +12 */
cirrus_vesa_selB000_data:
  dw 0xB000 ;; sel_B0000
cirrus_vesa_selB800_data:
  dw 0xB800 ;; sel_B8000
  /* +16 */
cirrus_vesa_selC000_data:
  dw 0xC000 ;; sel_C0000
cirrus_vesa_is_protected_mode:
  ;; protected mode flag and checksum
  dw (~((0xf2 + (cirrus_vesa_pmbios_entry >> 8) + (cirrus_vesa_pmbios_entry) \
     + (cirrus_vesa_pmbios_init >> 8) + (cirrus_vesa_pmbios_init)) & 0xff) << 8) + 0x01
ASM_END
#endif // CIRRUS_VESA3_PMINFO
