
/*
** 文件名 ：music.h
**  
*/

#ifndef _MUSIC_H
#define _MUSIC_H

// 以4分音符为1拍
#define TEMPO   8
#define _1      TEMPO*4     //全音符
#define _1d     TEMPO*6     //附点全音符
#define _2      TEMPO*2     //二音符
#define _2d     TEMPO*3     //附点二音符
#define _4      TEMPO*1     //四分音符
#define _4d     TEMPO*3/2   //附点四分音符
#define _8      TEMPO*1/2   //八分音符
#define _8d     TEMPO*3/4   //附点八分音符
#define _16     TEMPO*1/4   //十六分音符
#define _16d    TEMPO*3/8   //附点十六分音符
#define _32     TEMPO*1/8   //32分音符

#define _1DO    1000000/(262*2)
#define _1RE    1000000/(294*2)
#define _1MI    1000000/(330*2)
#define _1FA    1000000/(349*2)
#define _1SO    1000000/(392*2)
#define _1LA    1000000/(440*2)
#define _1TI    1000000/(494*2)

#define _DO    1000000/(523*2)
#define _RE    1000000/(587*2)
#define _MI    1000000/(659*2)
#define _FA    1000000/(698*2)
#define _SO    1000000/(784*2)
#define _LA    1000000/(880*2)
#define _TI    1000000/(988*2)

#define _DO1    1000000/(1047*2)
#define _RE1    1000000/(1175*2)
#define _MI1    1000000/(1319*2)
#define _FA1    1000000/(1397*2)
#define _SO1    1000000/(1568*2)
#define _LA1    1000000/(1760*2)
#define _TI1    1000000/(1976*2)

#endif

