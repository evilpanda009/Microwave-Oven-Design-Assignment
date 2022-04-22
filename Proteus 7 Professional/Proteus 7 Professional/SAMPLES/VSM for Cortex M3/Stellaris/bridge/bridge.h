#include "include\lm3s_cmsis.h"
#include "include\system_lm3s.h"

typedef char bool;

#define DBG(a) *(unsigned char*)0xffffff00 = a;

#define SLAVE_ADDRESS   0x003b

#define print_ch(ch)                      \
 { DBG(ch)                                \
   DBG(0)                                 \
 }

 #define print(s)                         \
 { for(char* ptr = s; ptr && *ptr; ++ptr) \
    { DBG(*ptr)                           \
    } DBG(0)                              \
 }
