#ifndef DATA_H_
#define DATA_H_


#include "kbdattchmnt.h"

//return pointer to selected language struct object
language *selectedlanguage(void);

//return pointer to selected layout struct object
layout *selectedlayout(void);

//intialise all the data
void data_init_all(void);

//dual keylist array 
uint8_t dualkeylistatpos(uint8_t langid,uint8_t i,uint8_t j);

//numkeypad scancodes
//extern uint8_t numpadkey[10]={0x62,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61};

#endif /*DATA_H_*/
