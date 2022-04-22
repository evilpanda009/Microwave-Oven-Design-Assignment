#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#define   MATH_TYPE      IQ_MATH
#define   GLOBAL_Q       16
#include "IQmathLib.h"

#define PAGE_SIZE    8
#define MAX_ROWS     64
#define MAX_PAGES    MAX_ROWS / PAGE_SIZE
#define MAX_COLUMNS  128

struct PAGE
 { char data[MAX_COLUMNS];
 };

#define FONT_WIDTH   4
#define FONT_HEIGHT  6

#define K_INDEX      10    // index of 'k' char
#define DOT_INDEX    11    // index of '.' char
const char digits_top[] = {0x69, 0x26, 0x69, 0x79, 0x26, 0xE8, 0x68, 0xF1, 0x69, 0x69, 0x89, 0x00};
const char digits_mid[] = {0x99, 0x22, 0x16, 0x21, 0xAF, 0xE1, 0xE9, 0x12, 0x69, 0x97, 0xAC, 0x00};
const char digits_btm[] = {0x96, 0x2F, 0x8F, 0x96, 0x22, 0x1E, 0x96, 0x48, 0x96, 0x16, 0xA9, 0x66};

/*
symbol map (4x6):
digits_top[7..4]
digits_top[3..0]
digits_mid[7..4]
digits_mid[3..0]
digits_btm[7..4]
digits_btm[3..0]
*/

void print_char(int char_idx, int x_left, int y_bottom);
void print_str(const char* s, int x_left, int y_bottom);

#pragma DATA_SECTION(display_ram, "DisplayRAM");
struct PAGE display_ram[MAX_PAGES];

#define OUTPUT    1
#define INPUT     0

#define READ      1
#define WRITE     0

#define CTX_DATA  1
#define CTX_CMD   0

#define SET_DATA_DIRECTION(v)    if(v == OUTPUT) GpioCtrlRegs.GPADIR.all |= 0x000000FF; else GpioCtrlRegs.GPADIR.all &= 0xFFFFFF00;
#define SET_E(v)                 if(v == 1) GpioDataRegs.GPASET.bit.GPIO12 = 1; else GpioDataRegs.GPACLEAR.bit.GPIO12 = 1;
#define SET_RW(v)                if(v == READ) GpioDataRegs.GPBSET.bit.GPIO34 = 1; else GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
#define SET_A0(v)                if(v == CTX_DATA) GpioDataRegs.GPBSET.bit.GPIO35 = 1; else GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1;
#define GET_DATA()               (GpioDataRegs.GPADAT.all & 0x000000FF)
void SET_DATA(char v)            
 { unsigned long dw = GpioDataRegs.GPADAT.all;
   dw = (dw & 0xFFFFFF00) + v;
   GpioDataRegs.GPADAT.all = dw;
 }

void lcd_write(char isdata, char value)
 { SET_E(1);
   SET_RW(0);        // write
   SET_A0(isdata);
   SET_DATA(value);
   SET_E(0);
 }

char lcd_read(char isdata)
 { SET_E(1);
   SET_DATA_DIRECTION(INPUT);
   SET_RW(WRITE);       
   SET_A0(isdata);
   SET_E(0);
   isdata = GET_DATA();
   SET_DATA_DIRECTION(OUTPUT);
   return isdata;
 }

#define CMD_LCDON                0xAF     // LCD display ON
#define CMD_LCDOFF               0xAE     // LCD display OFF
#define CMD_LCDCOLADRINC         0xE0     // Column address increment at write = 1
#define CMD_SETDISPLAYSTARTADR   0x40     // Sets the display RAM display start line address
#define CMD_SETPAGEADR           0xB0     // Sets the display RAM page address
#define CMD_SETCOLUMNADRU        0x10     // Sets the most significant 4 bits of the display RAM column address
#define CMD_SETCOLUMNADRL        0x00     // Sets the least significant 4 bits of the display RAM column address
#define CMD_LCDDIRECTION         0xA0     // Select LCD display direction

#define DIRECTIONNORMAL          0
#define DIRECTIONREVERSE         1

void lcd_set_display_startline_address(char adr)
 { lcd_write(CTX_CMD, CMD_SETDISPLAYSTARTADR | (adr & 0x3F));
 }

void lcd_set_page_address(char adr)
 { lcd_write(CTX_CMD, CMD_SETPAGEADR | (adr & 0x0F));
 }

void lcd_set_column_address(char adr)
 { lcd_write(CTX_CMD, CMD_SETCOLUMNADRU | ((adr >> 4) & 0x0F));
   lcd_write(CTX_CMD, CMD_SETCOLUMNADRL | (adr & 0x0F));
 }

void lcd_init()
 { char c, p;
   lcd_write(CTX_CMD, CMD_LCDON);
   lcd_write(CTX_CMD, CMD_LCDCOLADRINC);
   lcd_write(CTX_CMD, CMD_LCDDIRECTION + DIRECTIONREVERSE);          // It seems that LCD model mistake REVERSE ADC with NORMAL one
   for (p = 0; p < MAX_PAGES; p++)
    { lcd_set_page_address(p);
      lcd_set_column_address(0);
      for (c = 0; c < MAX_COLUMNS; c++)
       { display_ram[p].data[c] = 0;
         lcd_write(CTX_DATA, 0);
       }      
    }
 }

/* 
void lcd_clearcolumn(int c)
 { char p;
   lcd_write(CTX_CMD, CMD_LCDON);
   lcd_write(CTX_CMD, CMD_LCDCOLADRINC);
   lcd_write(CTX_CMD, CMD_LCDDIRECTION + DIRECTIONREVERSE);          // It seems that LCD model mistake REVERSE ADC with NORMAL one
   for (p = 0; p < MAX_PAGES; p++)
    { lcd_set_page_address(p);
      lcd_set_column_address(c);
      display_ram[p].data[c] = 0;
      lcd_write(CTX_DATA, 0);
    }
 }
*/

void lcd_set_point(int x, int y)
 { int  p, idx, v;
   x &= (MAX_COLUMNS-1);
   y &= (MAX_ROWS-1);
   p = y / PAGE_SIZE;
   idx = y - p*PAGE_SIZE;
   v = 1 << idx;
   display_ram[p].data[x] |= v;
//   lcd_set_page_address(p);
//   lcd_set_column_address(x);
//   lcd_write(CTX_DATA, display_ram[p].data[x]);
 }

void lcd_line(int x1, int y1, int x2, int y2)
 { int dx, dy, sx, sy, i, check, e, x, y;
   dx = x2 - x1;  
   if (dx < 0)
    { sx = -1;
      dx = -dx;
    }
   else
      sx = 1;
   dy = y2 - y1;  
   if (dy < 0)
    { sy = -1;
      dy = -dy;
    }
   else
      sy = 1;
   x = x1;
   y = y1;
   check = 0;
   if (dy > dx)
    { dx += dy;
      dy = dx - dy;
      dx = dx - dy;
      check = 1;
    }
   e = 2 * dy - dx;
   for(i=0; i<dx; i++)
    { lcd_set_point(x, y);
      if (e >= 0)
       { if (check)
            x += sx;
         else
            y += sy;
         e -= 2*dx;
       }
      if (check)
         y += sy;
      else
         x += sx;
      e += 2*dy;
    }
 }

void lcd_paintbegin()
 { int p, c;
   for (p = 0; p < MAX_PAGES; p++)
    { for (c = 0; c < MAX_COLUMNS; c++)
       { display_ram[p].data[c] = 0;
       }      
    }
 }
 
void lcd_paintend()
 { int p, c;
   for (p = 0; p < MAX_PAGES; p++)
    { lcd_set_page_address(p);
      lcd_set_column_address(0);
      for (c = 0; c < MAX_COLUMNS; c++)
       { lcd_write(CTX_DATA, display_ram[p].data[c]);
       }      
    }
 } 

void print_char(int char_idx, int x_left, int y_bottom)
 { char i, msk, y;
   msk = 0x88;
   for (i = 0; i < FONT_WIDTH; i++)
    { y = y_bottom;
      if (digits_btm[char_idx] & msk & 0x0F)
       { lcd_set_point(x_left + i, y);
       }
      y--;
      if (digits_btm[char_idx] & msk & 0xF0)
       { lcd_set_point(x_left + i, y);
       }
      y--;

      if (digits_mid[char_idx] & msk & 0x0F)
       { lcd_set_point(x_left + i, y);
       }
      y--;
      if (digits_mid[char_idx] & msk & 0xF0)
       { lcd_set_point(x_left + i, y);
       }
      y--;

      if (digits_top[char_idx] & msk & 0x0F)
       { lcd_set_point(x_left + i, y);
       }
      y--;
      if (digits_top[char_idx] & msk & 0xF0)
       { lcd_set_point(x_left + i, y);
       }
      y--;
      msk >>= 1;
    }
 }

void print_str(const char* s, int x_left, int y_bottom)
// s should contain valid symbols
 { char char_idx;
   while(*s)
    { if (*s == 'k' || *s == 'K')
       { char_idx = K_INDEX;
       }
      else
      if (*s >= '0' && *s <= '9')
       { char_idx = *s - '0';
       }
      else
       { char_idx = DOT_INDEX;
       }  
      print_char(char_idx, x_left, y_bottom);
      x_left += FONT_WIDTH + 1;
      s++;
    }
 }
