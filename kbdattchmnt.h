#include <stdint.h>
#include "tusb.h"
#ifndef KBDATTCHMNT_H_
#define KBDATTCHMNT_H_

void process_kbd_attachment(uint8_t *keyinfo,hid_keyboard_report_t *kbdreport);

typedef struct layout
{
    uint8_t *singkeyList;
    uint8_t *restrkeyList;
    uint8_t **dualkeyList;
    uint8_t restrkeylen;
    uint8_t dualkeylen;
    uint8_t langID;
} layout;

typedef struct language
{
    uint8_t langID;
    uint16_t *charnumToUnico;
} language;


#endif
