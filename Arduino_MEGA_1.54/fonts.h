
#ifndef __FONTS_H
#define __FONTS_H
   
#include <stdint.h>
#include <avr/pgmspace.h>

#ifdef __cplusplus
 extern "C" {
#endif

//ASCII
typedef struct _tFont
{    
  const uint8_t *table;
  uint16_t w;//width
  uint16_t h;//height
  
} FONT;


extern FONT Font24;
extern FONT Font20;
extern FONT Font16;
extern FONT Font12;
extern FONT Font8;

#ifdef __cplusplus
}
#endif

#endif 
 
