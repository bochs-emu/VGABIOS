
#ifndef vgabios_h_included
#define vgabios_h_included

/* Types */
typedef unsigned char  Bit8u;
typedef unsigned short Bit16u;
typedef unsigned long  Bit32u;
typedef unsigned short Boolean;

/* Defines */

#define GET_AL() ( AX & 0x00ff )
#define GET_BL() ( BX & 0x00ff )
#define GET_CL() ( CX & 0x00ff )
#define GET_DL() ( DX & 0x00ff )
#define GET_AH() ( AX >> 8 )
#define GET_BH() ( BX >> 8 )
#define GET_CH() ( CX >> 8 )
#define GET_DH() ( DX >> 8 )

#define SCROLL_DOWN 0
#define SCROLL_UP   1
#define NO_ATTR     2
#define WITH_ATTR   3

#endif
