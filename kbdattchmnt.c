#include "kbdattchmnt.h"
#include "datamanager.h"

#include "tusb.h"

//function declarations
static void keymod(uint8_t *keylist,uint8_t *prevkeylist,uint8_t *listlen,uint8_t *prevlistlen,uint8_t *sendkeypress,uint8_t *keyinfo);
void sendkeyasitis(uint8_t *keyinfo);
void unicode_keypress(uint16_t uc,uint8_t *keyinfo);

void process_kbd_attachment(uint8_t *keyinfo,hid_keyboard_report_t *kbdreport)
{
    static uint8_t listlen=0;
    static uint8_t prevlistlen=0;
    static uint8_t keylist[3]={0};
    static uint8_t prevkeylist[3]={0};
    static uint8_t sendkeypress=1;
    static uint8_t zeroreport=1;

   
    listlen=0;
    for(uint8_t i=0;i<3;i++)
    {
    keylist[i]=0;
    }

    zeroreport=1;
    if(kbdreport->modifier)
    {
        keylist[listlen++]=kbdreport->modifier;
        zeroreport=0;
    }

    uint8_t temp=0;
    for(uint8_t i=0;i<6;i++)
    {
        temp=1;
        if(kbdreport->keycode[i]!=0)
        zeroreport=0;
        for(uint8_t j=0;j<3;j++)
        if(keylist[j]==kbdreport->keycode[i])
        {
           temp=0;
        }
        if(temp&&listlen<3)
        {
            keylist[listlen++]=kbdreport->keycode[i];
        }
    }

    if(prevlistlen==1&&sendkeypress)
    keymod(keylist,prevkeylist,&listlen,&prevlistlen,&sendkeypress,keyinfo);

    if(zeroreport)
    sendkeypress=1;
    
    for(uint8_t i=0;i<3;i++)
    {
    prevkeylist[i]=keylist[i];
    }
    prevlistlen=listlen;

}

static void keymod(uint8_t *keylist,uint8_t *prevkeylist,uint8_t *listlen,uint8_t *prevlistlen,uint8_t *sendkeypress,uint8_t *keyinfo)
{
    static layout* currentlayout=NULL;
    static language* currentlang=NULL;

    static uint8_t *singkeylist;
    static uint8_t **dualkeylist;
    static uint8_t dualkeylen;
    static uint8_t *restrkeylist;
    static uint8_t restrkeylen;
    static uint16_t *chnumtouc;
    static uint8_t langid;

    currentlayout=selectedlayout();
    currentlang=selectedlanguage();
    singkeylist=currentlayout->singkeyList;
    dualkeylist=currentlayout->dualkeyList;
    dualkeylen=currentlayout->dualkeylen;
    restrkeylist=currentlayout->restrkeyList;
    restrkeylen=currentlayout->restrkeylen;
    chnumtouc=currentlang->charnumToUnico;
    langid=currentlayout->langID;

    tud_cdc_write_str("        entered keymod           ");

    uint8_t restricts=0;
    //this block executes if a single key is pressed and released
    if(*listlen==0)
    {
        if(singkeylist[prevkeylist[0]]!=0)
        {
        unicode_keypress(chnumtouc[singkeylist[prevkeylist[0]]],keyinfo);
        *sendkeypress=0;
        }
    }
    else if(*listlen==2) //this block exeutes if a key is pressed while holding another key
    {
        for(uint8_t i=0;i<dualkeylen;i++)
        if(dualkeylistatpos(langid,i,1)==keylist[0]||dualkeylistatpos(langid,i,2)==keylist[0])
        {
            if(dualkeylistatpos(langid,i,1)==keylist[1]||dualkeylistatpos(langid,i,2)==keylist[1])
            {
                unicode_keypress(chnumtouc[dualkeylistatpos(langid,i,0)],keyinfo); //i'th row 0'th column
                *sendkeypress=0;
                break;
            }
        }
    }
    if(*sendkeypress )  //&&(*prevlistlen!=*listlen))
    {
        for(uint8_t i=1;i<restrkeylen;i++)
        {
        if(restrkeylist[i]==keylist[0]||restrkeylist[i]==keylist[1])
        {
            return;
            restricts=1;
            keyinfo[0]=0;
            break;
        }
        }
        if(!restricts)
        {
            sendkeyasitis(keyinfo);
        }
    }
}

void sendkeyasitis(uint8_t *keyinfo)
{
    tud_cdc_write_str("   entered sendkeyasitis      ");
    keyinfo[0]=1;
    keyinfo[1]=0;
}


void unicode_keypress(uint16_t uc,uint8_t *keyinfo)
{
    tud_cdc_write_str("     entered unicode      ");
    keyinfo[0]=1;
    keyinfo[1]=1;
    for(uint8_t i=5;i>1;i--)
    {
        keyinfo[i]=uc%10;
        uc/=10;
    }
}
