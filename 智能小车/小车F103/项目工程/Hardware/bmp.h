//////////////////////////////////////////////////////////////////////////////////	 
//  深圳市沃乐康科技有限公司  WWW.WLKLCD.COM  0755-32882855   专业液晶显示供应商   //
//  主程序 （硬件SPI2 ST7789+154ips-240*240 @STM32F103C8T6 MDK5.14）             //
//  https://wlklcd.1688.com/     https://shop341012592.taobao.com/                 //
//  例程仅供学习参考，不做量产使用，不得用于商业用途                              //
//******************************************************************************/
//取模软件 Image2LCD
//******************************************************************************/

#ifndef __BMP_H
#define __BMP_H
#define	u8 unsigned char
static u8 image[]={ /* 0X00,0X10,0X28,0X00,0X28,0X00,0X01,0X1B, */
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFD,0XFE,0X32,0XE2,0X91,0XE1,0XB8,0XEC,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7F,0XDF,
0XFF,0XDF,0X38,0XFD,0X0A,0XD0,0X0C,0XD8,0X09,0XD0,0X77,0XEC,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XEF,0X9B,0X0C,
0XDD,0X04,0X5C,0XCE,0XCE,0XF8,0X0D,0XD0,0X12,0XE0,0X3E,0XFE,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X3C,0XFF,0XD6,0XF5,0XB6,0XF5,0XBA,0XF6,0XFF,0XFF,0XFF,0XFF,0X3C,0XFF,0XD6,0XF5,
0XB6,0XF5,0XDB,0XF6,0XFF,0XFF,0XFF,0XFF,0X1C,0XFF,0XB6,0XF5,0XB6,0XF5,0XFB,0XFE,
0XFF,0XFF,0XFF,0XFF,0XB8,0XEF,0X96,0XEF,0XB7,0XEF,0XFF,0XFF,0X3E,0X86,0XDB,0X03,
0X1B,0X04,0X7E,0X0D,0XF6,0XB9,0X0F,0XF0,0X55,0XE6,0XEE,0XD7,0X72,0XE7,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X3E,0XC7,0XFE,0XB6,0XFE,0XBE,0X9E,0X96,
0X34,0XED,0X00,0XD0,0X00,0XD0,0X0B,0XE3,0XFF,0XFF,0XFF,0XFF,0X13,0XED,0X00,0XD0,
0X00,0XD0,0XAD,0XE3,0XFF,0XFF,0XFF,0XFF,0X71,0XEC,0X00,0XD0,0X00,0XD0,0X0F,0XEC,
0XFF,0XFF,0XFE,0XFF,0XC3,0XBE,0X60,0XAE,0XA0,0XB6,0XFC,0XFF,0X1F,0X7E,0X5A,0X03,
0X1D,0X04,0XFF,0X04,0X74,0X84,0X62,0XDE,0XE0,0XB7,0X60,0XAE,0XA1,0XB6,0XFD,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5D,0X86,0X1B,0X04,0X3B,0X04,0X9B,0X03,0XBC,0X24,
0X75,0XF5,0X00,0XD0,0X00,0XD0,0X6C,0XE3,0XFF,0XFF,0XFF,0XFF,0X34,0XED,0X00,0XD0,
0X00,0XD0,0XEE,0XE3,0XFF,0XFF,0XFF,0XFF,0XB2,0XEC,0X00,0XD0,0X00,0XD0,0X50,0XEC,
0XFF,0XFF,0XFF,0XFF,0XE3,0XC6,0X80,0XAE,0XC0,0XBE,0XFC,0XFF,0X9F,0XDF,0XDE,0X55,
0X9F,0X86,0X8E,0XE5,0X21,0XFD,0X26,0XCF,0XC0,0XAE,0X80,0XB6,0XC2,0XBE,0XFD,0XFF,
0XFF,0XFF,0XFF,0XFF,0X3D,0X7E,0XFB,0X03,0X5B,0X04,0XDB,0X03,0X1C,0X35,0XDF,0XF7,
0X75,0XF5,0X40,0XD0,0X00,0XD0,0X8D,0XE3,0XFF,0XFF,0XFF,0XFF,0X54,0XED,0X20,0XD0,
0X00,0XD0,0X0F,0XEC,0XFF,0XFF,0XFF,0XFF,0XD2,0XEC,0X00,0XD0,0X00,0XD0,0X71,0XEC,
0XFF,0XFF,0XFF,0XFF,0XE4,0XC6,0X80,0XAE,0XC1,0XBE,0XDC,0XF7,0XFF,0XFF,0XFF,0XFF,
0X6D,0XFE,0X60,0XFC,0X20,0XF4,0X11,0XFE,0X8E,0XD7,0X60,0XA6,0X4F,0XDF,0XFF,0XFF,
0XFF,0XFF,0XFD,0X75,0XDB,0X03,0X7B,0X04,0XFB,0X03,0X3C,0X3D,0XFF,0XFF,0XFF,0XFF,
0X75,0XF5,0X40,0XD0,0X00,0XD0,0X8D,0XE3,0XFF,0XFF,0XFF,0XFF,0X54,0XED,0X20,0XD0,
0X00,0XD0,0X0F,0XEC,0XFF,0XFF,0XFF,0XFF,0XD2,0XEC,0X00,0XD0,0X00,0XD0,0X71,0XEC,
0XFF,0XFF,0XFF,0XFF,0XE4,0XC6,0X80,0XAE,0XC1,0XBE,0XDC,0XF7,0XFF,0XFF,0XFF,0XFF,
0XA4,0XEC,0X00,0XEC,0X00,0XEC,0XE7,0XF4,0XFF,0XFF,0XD9,0XEF,0XFE,0XFF,0XFF,0XFF,
0XDD,0X65,0XDB,0X03,0X7C,0X04,0XFB,0X03,0X5C,0X45,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X75,0XF5,0X40,0XD0,0X00,0XD0,0X8D,0XE3,0XFF,0XFF,0XFF,0XFF,0X54,0XED,0X20,0XD0,
0X00,0XD0,0X0F,0XEC,0XFF,0XFF,0XFF,0XFF,0XD2,0XEC,0X00,0XD0,0X00,0XD0,0X71,0XEC,
0XFF,0XFF,0XFF,0XFF,0XE4,0XC6,0X80,0XAE,0XC1,0XBE,0XDC,0XF7,0XFF,0XFF,0XFF,0XFF,
0X7A,0XFF,0XAA,0XF5,0XE4,0XEC,0X51,0XF6,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X9C,0X55,
0XDB,0X03,0X7C,0X04,0XFB,0X03,0X7C,0X4D,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X75,0XF5,0X40,0XD0,0X00,0XD0,0X8D,0XE3,0XFF,0XFF,0XFF,0XFF,0X54,0XED,0X20,0XD0,
0X00,0XD0,0X0F,0XEC,0XFF,0XFF,0XFF,0XFF,0XD2,0XEC,0X00,0XD0,0X00,0XD0,0X70,0XEC,
0XFF,0XFF,0XFF,0XFF,0XE4,0XC6,0X80,0XAE,0XC1,0XBE,0XDC,0XF7,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5C,0X45,0XDB,0X03,
0X7C,0X04,0XFB,0X03,0X9C,0X55,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X75,0XF5,0X40,0XD0,0X00,0XD0,0XAD,0XE3,0XFF,0XFF,0XFF,0XFF,0XF3,0XEC,0X00,0XC8,
0X00,0XC8,0X6D,0XE3,0XFF,0XFF,0XFF,0XFF,0XD2,0XEC,0X00,0XD0,0X00,0XD0,0X70,0XEC,
0XFF,0XFF,0XFF,0XFF,0XE4,0XC6,0X80,0XAE,0XC1,0XBE,0XDC,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X9F,0XE7,0XFC,0X2C,0XFB,0X03,0X7C,0X04,
0X5B,0X04,0X1D,0X76,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X75,0XF5,0X40,0XD0,0X00,0XD0,0X6C,0XE3,0XFF,0XFF,0XFF,0XFF,0XDB,0XF6,0XB2,0XEC,
0XB1,0XEC,0X58,0XF6,0XFF,0XFF,0XFF,0XFF,0XB2,0XEC,0X00,0XD0,0X00,0XD0,0X91,0XEC,
0XFF,0XFF,0XFF,0XFF,0XE5,0XC6,0X80,0XB6,0XC0,0XBE,0XDB,0XF7,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X3E,0XBF,0XFB,0X03,0X1B,0X04,0X9C,0X04,
0X3B,0X04,0XDE,0XAE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X58,0XF6,0X80,0XD0,0X40,0XD0,0X07,0XDA,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X2B,0XE3,0X20,0XD0,0X00,0XD0,0X34,0XED,
0XFF,0XFF,0XFF,0XFF,0X07,0XCF,0X80,0XB6,0XA0,0XB6,0X95,0XEF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X5E,0XCF,0X7B,0X0C,0X1B,0X04,
0X7B,0X04,0X3B,0X04,0XBE,0X9E,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XDF,0XFF,0X84,0XD9,0X40,0XD0,0X40,0XD0,0X95,0XED,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X9A,0XF6,0XC2,0XD0,0X80,0XD8,0XA1,0XD0,0XDA,0XF6,
0XFF,0XFF,0XFF,0XFF,0X4E,0XD7,0X80,0XAE,0XA0,0XB6,0X08,0XCF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7E,0XDF,0X9B,0X14,
0X1B,0X04,0X5B,0X04,0X1B,0X04,0X9E,0X9E,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XEF,0XEB,0X00,0XD0,0X80,0XD8,0X02,0XD9,0X9A,0XF6,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X7D,0XFF,0XA5,0XD9,0X60,0XD0,0X00,0XD0,0XAA,0XE2,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XDA,0XF7,0XA0,0XB6,0XA0,0XB6,0X80,0XB6,0X2D,0XD7,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X9F,0XE7,
0XBB,0X1C,0X1B,0X04,0X5B,0X04,0XFB,0X03,0X9E,0X96,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XDF,0XFF,0XA5,0XD9,0X00,0XD0,0X80,0XD8,0X81,0XD0,0XCE,0XE3,0XD7,0XF5,
0XF7,0XF5,0X50,0XEC,0X02,0XD9,0X60,0XD0,0X40,0XD0,0XA1,0XD0,0XFB,0XFE,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X50,0XDF,0X80,0XAE,0XA0,0XB6,0XA0,0XB6,0XE5,0XC6,
0X72,0XE7,0X95,0XEF,0X95,0XE7,0X95,0XE7,0X73,0XE7,0XBA,0XF7,0XFF,0XFF,0XFF,0XFF,
0XBF,0XEF,0XFC,0X24,0XFB,0X03,0X5B,0X04,0X1B,0X04,0X9D,0X96,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X3C,0XFF,0X64,0XD9,0X00,0XD0,0X60,0XD0,0X00,0XD0,0X40,0XD0,
0X40,0XD0,0X00,0XD0,0X60,0XD0,0X00,0XD0,0XA1,0XD0,0X58,0XF6,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X4D,0XD7,0X80,0XAE,0XA0,0XB6,0XA0,0XB6,
0X80,0XAE,0XA0,0XB6,0XA0,0XB6,0XA0,0XB6,0X60,0XA6,0X4E,0XDF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XDF,0XF7,0X1C,0X35,0XFB,0X03,0X5B,0X04,0XFB,0X03,0X7D,0X8E,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X9E,0XFF,0X6C,0XE3,0X80,0XD0,0X00,0XD0,0X00,0XD0,
0X00,0XD0,0X00,0XD0,0X60,0XD0,0XA9,0XE2,0X1C,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X94,0XE7,0XC2,0XBE,0XA0,0XB6,
0X80,0XAE,0X80,0XAE,0X80,0XAE,0X80,0XAE,0X40,0XA6,0X2E,0XD7,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7C,0X45,0XDB,0X03,0XDB,0X03,0X7A,0X03,0XDD,0X5D,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFB,0XF6,0XF2,0XEC,0XEE,0XE3,
0XCE,0XE3,0XB1,0XEC,0X99,0XF6,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X97,0XEF,
0X6F,0XDF,0X4D,0XD7,0X4D,0XD7,0X4D,0XD7,0X2A,0XD7,0X96,0XEF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XF7,0X3D,0X7E,0XDD,0X65,0XDD,0X5D,0X7C,0X4D,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0X9F,0XFF,0XD8,0XEC,0XBA,0XF5,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X7C,0XF6,0XF6,0XEB,0X2F,0XD9,0X09,0XD0,0X5A,0XF5,0XFF,0XFF,0X5D,0X86,
0XFC,0X2C,0XFE,0XBE,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XBF,0XFF,0XAE,0XD8,0X0C,0XD0,0X37,0XEC,0XCF,0XD8,0X92,0XFA,0XBF,0XB7,0XFB,0X03,
0X3A,0X03,0X9C,0X55,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0X7F,0XFF,0X8E,0XD8,0X0A,0XD0,0X0F,0XD9,0X10,0XE1,0X72,0XFA,0XDE,0X55,0X1B,0X04,
0XDD,0X5D,0X3C,0X3D,0XBE,0XA6,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDE,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XDD,0XFD,0XB4,0XE8,0X10,0XE0,0X0B,0XD8,0X36,0XFC,0X1F,0X67,0X9F,0X04,
0XFF,0X0C,0XDB,0X03,0X1F,0XB7,0XFE,0XFF,0X89,0XF5,0X7B,0XFF,0XBD,0XFF,0XE4,0XEC,
0X17,0XFF,0XFF,0XFF,0X89,0XF5,0X9B,0XFF,0XD5,0XFE,0X05,0XED,0XE3,0XEC,0XCB,0XF5,
0XFF,0XFF,0XFF,0XFF,0XD5,0XFD,0XAC,0XF3,0XEF,0XE3,0X50,0XEC,0XF2,0XF4,0X57,0XFE,
0XBD,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFB,0XF7,0X51,0XDF,0X50,0XDD,0X19,0XED,0XDF,0XFF,0XDC,0XFF,0XD4,0XAD,
0X78,0X6D,0X3F,0XB7,0XFF,0XFF,0XBC,0XFF,0X82,0XEC,0X7B,0XFF,0X0E,0XF6,0X80,0XE3,
0X17,0XFF,0XB4,0XFE,0X49,0XF5,0XB4,0XF6,0X81,0XEC,0XF6,0XFE,0X38,0XFF,0X40,0XEC,
0X71,0XF6,0XFF,0XFF,0X52,0XBC,0X4E,0XBB,0XF2,0XFC,0XB2,0XEC,0XD1,0XFC,0XCF,0XC3,
0X8E,0XC3,0X50,0XEC,0XB5,0XF5,0X38,0XFE,0X50,0XEC,0X0F,0XE4,0XEF,0XE3,0X17,0XF6,
0X4F,0XDF,0X80,0XAE,0X21,0XBF,0XC0,0XB7,0XE8,0XC7,0X7B,0XF7,0X26,0XFD,0X80,0XFC,
0XE0,0XFC,0X6E,0XFE,0XFF,0XFF,0X9C,0XFF,0X68,0XF5,0X72,0XF6,0X8A,0XF5,0XAA,0XF5,
0X71,0XF6,0X6A,0XF5,0XD5,0XFE,0X26,0XF5,0XB4,0XFE,0XFF,0XFF,0XFF,0XFF,0X05,0XED,
0X0D,0XFE,0XFF,0XF7,0X3F,0X05,0XFF,0X96,0XFF,0XFF,0XFF,0XFF,0X1F,0X9F,0X5F,0X05,
0X3D,0X7E,0XB7,0XC5,0X74,0XFD,0XA8,0XFA,0X40,0XD8,0X2B,0XE3,0X27,0XDA,0X81,0XD0,
0XC4,0XBE,0X09,0XC7,0X4D,0XDF,0XC0,0XB6,0XE1,0XB6,0XEB,0XED,0XA3,0XF4,0X89,0XF5,
0X61,0XEC,0X00,0XEC,0XDE,0XFF,0XDE,0XFF,0X46,0XF5,0XA3,0XEC,0XD6,0XFE,0X71,0XF6,
0X81,0XEC,0X0F,0XF6,0X38,0XFF,0XC3,0XEC,0XDE,0XFF,0XFF,0XFF,0X7A,0XFF,0X80,0XEC,
0XB9,0XFF,0X1F,0XAF,0X7C,0X0C,0XDF,0XEF,0XFF,0XFF,0XFF,0XFF,0X9C,0X1C,0XFD,0X75,
0XFF,0XFF,0XFF,0XFF,0XDF,0XE7,0X9C,0X5D,0X79,0XE6,0XFF,0XFF,0XB6,0XF5,0X81,0XD8,
0X96,0XEF,0X07,0XCF,0XA0,0XB6,0XA0,0XAE,0XC8,0XCE,0X73,0XFE,0X27,0XF5,0XA2,0XEC,
0X68,0XF5,0X0E,0XF6,0XFF,0XFF,0X9C,0XFF,0XE0,0XEB,0X07,0XED,0XFF,0XFF,0X6A,0XF5,
0XE0,0XEB,0XBC,0XFF,0X9B,0XFF,0X20,0XEC,0X72,0XF6,0X59,0XFF,0XA2,0XEC,0XCC,0XF5,
0XFF,0XFF,0X9D,0X65,0XFB,0X03,0X9E,0X96,0X7E,0X96,0X1E,0XB7,0X3B,0X0C,0X7D,0X55,
0XDE,0XA6,0XDE,0XAE,0X5D,0X8E,0XFC,0X03,0X9F,0X76,0X9E,0X96,0XEC,0XBA,0XA9,0XEA,
0XFF,0XFF,0XE5,0XC6,0X60,0XA6,0XE2,0XBE,0X76,0XF7,0XEF,0XFD,0X16,0XFF,0X26,0XF5,
0XB4,0XF6,0XFF,0XFF,0XFF,0XFF,0X9C,0XFF,0X47,0XF5,0X17,0XFF,0XFF,0XFF,0XED,0XF5,
0XAB,0XF5,0XFF,0XFF,0XFF,0XFF,0X92,0XF6,0X04,0XF5,0XA7,0XFD,0XD2,0XFE,0XFF,0XFF,
0XFF,0XFF,0X1D,0X7E,0XBE,0X45,0XBE,0X45,0X7E,0X3D,0X7F,0XCF,0XFF,0XAE,0X5E,0X3D,
0XDC,0X2C,0X9D,0X55,0X9E,0X8E,0X9D,0X4D,0XDF,0X55,0X9F,0X2D,0XBE,0X96,0XFE,0XFF,
0XFF,0XFF,0XFD,0XFF,0X73,0XE7,0XFC,0XF7,0XFF,0XFF,0X06,0XED,0X17,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDE,0XFF,
0XB4,0XF6,0XFE,0XFF,0X7E,0XEF,0XBF,0XF7,0X1B,0XE7,0X59,0XCE,0X5F,0XDF,0X38,0XC6,
0X79,0XD6,0XFF,0XFF,0X3C,0XEF,0XF7,0XC5,0XDA,0XDE,0X79,0XD6,0X37,0XD6,0XDA,0XE6,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDE,0XFF,0XBA,0XDE,0X7D,0XF7,0XFF,0XFF,0XDF,0XF7,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X68,0XF5,0XAA,0XF5,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X7A,0XFF,
0XEC,0XF5,0XFF,0XFF,0X8E,0X73,0XAE,0X73,0XFF,0XFF,0X14,0X9D,0XE7,0X41,0X17,0XC6,
0X1C,0XE7,0XBE,0XF7,0X27,0X4A,0XD6,0XC5,0XEB,0X62,0XCA,0X5A,0X79,0XD6,0XBA,0XDE,
0XFF,0XFF,0X0F,0X84,0X6D,0X73,0XD2,0X9C,0XA2,0X18,0XCA,0X62,0XB2,0X9C,0X38,0XC6,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X38,0XFF,0X40,0XEC,0X17,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XAA,0XF5,
0X2E,0XFE,0XFF,0XFF,0X6D,0X6B,0XEF,0X7B,0XDF,0XFF,0X51,0X8C,0X48,0X4A,0XBA,0XD6,
0XFF,0XFF,0X75,0XAD,0X69,0X4A,0XFF,0XFF,0XAA,0X52,0XB6,0XB5,0XFF,0XFF,0XFF,0XFF,
0X9E,0XF7,0X24,0X29,0X50,0X8C,0XB2,0X94,0XC3,0X18,0XEB,0X62,0X65,0X31,0XFB,0XDE,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X50,0XF6,0X20,0XEC,
0XB3,0XF6,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XAA,0XF5,0X04,0XED,
0XFF,0XFF,0XFF,0XFF,0XCE,0X7B,0X96,0XB5,0X9A,0XD6,0X00,0X00,0X85,0X31,0X58,0XCE,
0XFF,0XFF,0X75,0XAD,0XEB,0X5A,0XF3,0X9C,0X65,0X31,0X34,0XAD,0X75,0XAD,0XBE,0XF7,
0XF7,0XBD,0X44,0X29,0X34,0XA5,0X2C,0X6B,0XA6,0X39,0XEB,0X5A,0X8A,0X52,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XB3,0XF6,
0XA1,0XEC,0X04,0XED,0XAB,0XF5,0X0D,0XF6,0XAA,0XF5,0XE3,0XEC,0X68,0XF5,0XBD,0XFF,
0XFF,0XFF,0X55,0XAD,0XC6,0X39,0XBA,0XD6,0X69,0X52,0X92,0X94,0X28,0X42,0XD3,0X9C,
0XFF,0XFF,0X10,0X84,0X4C,0X6B,0XAE,0X73,0XAE,0X73,0X2C,0X6B,0X28,0X42,0XFF,0XFF,
0X2C,0X63,0XA6,0X39,0XAE,0X73,0XE7,0X39,0XA6,0X31,0XCA,0X5A,0XDB,0XDE,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XDE,0XFF,0XB3,0XF6,0XEC,0XF5,0XCB,0XF5,0X0E,0XF6,0X38,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0X30,0X8C,0X95,0XB5,0X30,0X84,0X30,0X84,0XFF,0XFF,0XF3,0XA4,0X8E,0X73,
0XFF,0XFF,0XCA,0X5A,0X75,0XAD,0XAE,0X73,0X38,0XC6,0X14,0XA5,0X6D,0X6B,0XFF,0XFF,
0X4D,0X6B,0XEF,0X83,0X71,0X8C,0XEB,0X62,0XF3,0X9C,0XCE,0X7B,0X1C,0XE7,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XBE,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XBE,0XF7,0XDF,0XFF,0XBE,0XF7,0X9E,0XF7,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,
0XFF,0XFF,0XDF,0XFF,0XDF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XDF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
};



#endif


