#include <stddef.h>
#include <string.h>
#include <pico/stdlib.h>
#include "datamanager.h"
#include "kbdattchmnt.h"


//english
static uint16_t engunicodelist[256]= {
        0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
        32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
        64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
        80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
        96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
        112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
        127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
        143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
        159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
        175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190,
        191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206,
        207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222,
        223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238,
        239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
    };
static uint8_t engsingchnum[256]={0};
static uint8_t engdualchnum[50][3]={0};

static language Eng;
static layout Eng1;

// kannada
static uint16_t kanunicodeList[256] = {0};
static uint8_t kan1dualchnumList[50][3] = {0};
static uint8_t kansingchnumList[256] = {0};
static uint8_t restrictedkan[11] = {0};

static language Kan;
static layout Kan1;

// telugu
static uint16_t telunicodeList[256] = {0};
static uint8_t tel1dualchnumList[50][3] = {0};
static uint8_t tel1singchnumList[256] = {0};

static language Tel;
static layout Tel1;

// Hindi
static uint16_t hindiUnicodeList[256] = {0};
static uint8_t hindualchnum[50][3] = {0};
static uint8_t hinsingchnum[256] = {0};

static language Hin;
static layout Hin1;

// gpio_inits
uint8_t Kangp = 2;
uint8_t Telgp = 3;
uint8_t Hingp = 4;
uint8_t Engp = 5;

pico_gpio_init()
{
    gpio_init(Kangp);
    gpio_init(Telgp);
    gpio_init(Hingp);
     gpio_init(Engp);
    
   gpio_set_dir(Kangp, GPIO_IN);
   gpio_set_dir(Telgp, GPIO_IN);
   gpio_set_dir(Hingp, GPIO_IN);
   gpio_set_dir(Engp, GPIO_IN);

}

layout *selectedlayout(void)
{
    gpio_pull_up(Kangp);
    gpio_pull_up(Telgp);
    gpio_pull_up(Hingp);
    gpio_pull_up(Engp);

    uint8_t Kanread=gpio_get(Kangp);
    uint8_t Telread=gpio_get(Telgp);
    uint8_t Hinread=gpio_get(Hingp);
    uint8_t Enread=gpio_get(Engp);

    if(!Kanread)
    return &Kan1;
    if(!Hinread)
    return &Hin1;
    if(!Telread)
    return &Tel1;
    if(!Enread)
    return &Eng1;

}

language *selectedlanguage(void)
{
    gpio_pull_up(Kangp);
    gpio_pull_up(Telgp);
    gpio_pull_up(Hingp);
    gpio_pull_up(Engp);


    uint8_t Kanread=gpio_get(Kangp);
    uint8_t Telread=gpio_get(Telgp);
    uint8_t Hinread=gpio_get(Hingp);
    uint8_t Enread=gpio_get(Engp);

    if(!Kanread)
    return &Kan;
    if(!Hinread)
    return &Hin;
    if(!Telread)
    return &Tel;
    if(!Enread)
    return &Eng;

}

uint8_t dualkeylistatpos(uint8_t langid, uint8_t i, uint8_t j)
{
    if (langid == 1)
        return kan1dualchnumList[i][j];
    else if (langid == 2)
        return tel1dualchnumList[i][j];
    if (langid == 3)
        return hindualchnum[i][j];
    if(langid == 4)
    return engdualchnum[i][j];
}

void data_init_all()
{
    pico_gpio_init();
    data_init_kan();
    data_init_tel();
    data_init_hin();
    data_init_eng();
}

void data_init_kan()
{

    kanunicodeList[128] = 3201;
    kanunicodeList[126] = 3202;
    kanunicodeList[127] = 3203;
    kanunicodeList[81] = 3205;
    kanunicodeList[82] = 3206;
    kanunicodeList[83] = 3207;
    kanunicodeList[84] = 3208;
    kanunicodeList[85] = 3209;
    kanunicodeList[86] = 3210;
    kanunicodeList[94] = 3211;
    kanunicodeList[87] = 3214;
    kanunicodeList[88] = 3215;
    kanunicodeList[89] = 3216;
    kanunicodeList[90] = 3218;
    kanunicodeList[91] = 3219;
    kanunicodeList[92] = 3220;
    kanunicodeList[11] = 3221;
    kanunicodeList[12] = 3222;
    kanunicodeList[13] = 3223;
    kanunicodeList[14] = 3224;
    kanunicodeList[15] = 3225;
    kanunicodeList[21] = 3226;
    kanunicodeList[22] = 3227;
    kanunicodeList[23] = 3228;
    kanunicodeList[24] = 3229;
    kanunicodeList[25] = 3230;
    kanunicodeList[31] = 3231;
    kanunicodeList[32] = 3232;
    kanunicodeList[33] = 3233;
    kanunicodeList[34] = 3234;
    kanunicodeList[35] = 3235;
    kanunicodeList[41] = 3236;
    kanunicodeList[42] = 3237;
    kanunicodeList[43] = 3238;
    kanunicodeList[44] = 3239;
    kanunicodeList[45] = 3240;
    kanunicodeList[51] = 3242;
    kanunicodeList[52] = 3243;
    kanunicodeList[53] = 3244;
    kanunicodeList[54] = 3245;
    kanunicodeList[55] = 3246;
    kanunicodeList[61] = 3247;
    kanunicodeList[62] = 3248;
    kanunicodeList[70] = 3249;
    kanunicodeList[63] = 3250;
    kanunicodeList[69] = 3251;
    kanunicodeList[64] = 3253;
    kanunicodeList[65] = 3254;
    kanunicodeList[66] = 3255;
    kanunicodeList[67] = 3256;
    kanunicodeList[68] = 3257;
    kanunicodeList[112] = 3262;
    kanunicodeList[113] = 3263;
    kanunicodeList[114] = 3264;
    kanunicodeList[115] = 3265;
    kanunicodeList[116] = 3266;
    kanunicodeList[124] = 3267;
    kanunicodeList[117] = 3270;
    kanunicodeList[118] = 3271;
    kanunicodeList[119] = 3272;
    kanunicodeList[120] = 3274;
    kanunicodeList[121] = 3275;
    kanunicodeList[122] = 3276;
    kanunicodeList[110] = 3277;
    kanunicodeList[95] = 3296;
    kanunicodeList[0] = 3302;
    kanunicodeList[1] = 3303;
    kanunicodeList[2] = 3304;
    kanunicodeList[3] = 3305;
    kanunicodeList[4] = 3306;
    kanunicodeList[5] = 3307;
    kanunicodeList[6] = 3308;
    kanunicodeList[7] = 3309;
    kanunicodeList[8] = 3310;
    kanunicodeList[9] = 3311;

    kansingchnumList[0x3a] = 81;  // f1
    kansingchnumList[0x3b] = 82;  // f2
    kansingchnumList[0x3c] = 83;  // f3
    kansingchnumList[0x3d] = 84;  // f4
    kansingchnumList[0x3e] = 85;  // f5
    kansingchnumList[0x3f] = 86;  // f6
    kansingchnumList[0x40] = 87;  // f7
    kansingchnumList[0x41] = 88;  // f8
    kansingchnumList[0x42] = 89;  // f9
    kansingchnumList[0x43] = 90;  // f10
    kansingchnumList[0x44] = 91;  // f11
    kansingchnumList[0x45] = 92;  // f12
    kansingchnumList[0x06] = 124; // c
    kansingchnumList[0x27] = 0;   // 0
    kansingchnumList[0x1e] = 1;   // 1
    kansingchnumList[0x1f] = 2;   // 2
    kansingchnumList[0x20] = 3;   // 3
    kansingchnumList[0x21] = 4;   // 4
    kansingchnumList[0x22] = 5;   // 5
    kansingchnumList[0x23] = 6;   // 6
    kansingchnumList[0x24] = 7;   // 7
    kansingchnumList[0x25] = 8;   // 8
    kansingchnumList[0x26] = 9;   // 9
    kansingchnumList[0x04] = 61;  // a
    kansingchnumList[0x16] = 62;  // s
    kansingchnumList[0x07] = 63;  // d
    kansingchnumList[0x09] = 64;  // f
    kansingchnumList[0x0a] = 65;  // g
    kansingchnumList[0x0b] = 66;  // h
    kansingchnumList[0x0d] = 67;  // j
    kansingchnumList[0x0e] = 68;  // k
    kansingchnumList[0x0f] = 69;  // l
    kansingchnumList[0x1d] = 94;  // z
    kansingchnumList[0x1b] = 95;  // x
    kansingchnumList[0x19] = 70;  // v
    kansingchnumList[0x05] = 126; // b
    kansingchnumList[0x11] = 127; // n
    kansingchnumList[0x10] = 128; // m
    kansingchnumList[0x14] = 16; // q
    kansingchnumList[0x1a] = 17; // w
    kansingchnumList[0x08] = 18; // e
    kansingchnumList[0x15] = 19; // r
    kansingchnumList[0x17] = 20; // t
    kansingchnumList[0x1c] = 26; // y
    kansingchnumList[0x18] = 27;// u
    kansingchnumList[0x0c] = 28; // i
    kansingchnumList[0x12] = 29; // o
    kansingchnumList[0x13] = 30; // p



    kan1dualchnumList[0][0] = 110;
    kan1dualchnumList[0][1] = 0x02;
    kan1dualchnumList[0][2] = 0x3a; // shift+f1
    kan1dualchnumList[1][0] = 112;
    kan1dualchnumList[1][1] = 0x02;
    kan1dualchnumList[1][2] = 0x3b; // shift+f2
    kan1dualchnumList[2][0] = 113;
    kan1dualchnumList[2][1] = 0x02;
    kan1dualchnumList[2][2] = 0x3c; // shift+f3
    kan1dualchnumList[3][0] = 114;
    kan1dualchnumList[3][1] = 0x02;
    kan1dualchnumList[3][2] = 0x3d; // shift+f4
    kan1dualchnumList[4][0] = 115;
    kan1dualchnumList[4][1] = 0x02;
    kan1dualchnumList[4][2] = 0x3e; // shift+f5
    kan1dualchnumList[5][0] = 116;
    kan1dualchnumList[5][1] = 0x02;
    kan1dualchnumList[5][2] = 0x3f; // shift+f6
    kan1dualchnumList[6][0] = 117;
    kan1dualchnumList[6][1] = 0x02;
    kan1dualchnumList[6][2] = 0x40; // shift+f7
    kan1dualchnumList[7][0] = 118;
    kan1dualchnumList[7][1] = 0x02;
    kan1dualchnumList[7][2] = 0x41; // shift+f8
    kan1dualchnumList[8][0] = 119;
    kan1dualchnumList[8][1] = 0x02;
    kan1dualchnumList[8][2] = 0x42; // shift+f9
    kan1dualchnumList[9][0] = 120;
    kan1dualchnumList[9][1] = 0x02;
    kan1dualchnumList[9][2] = 0x43; // shift+f10
    kan1dualchnumList[10][0] = 121;
    kan1dualchnumList[10][1] = 0x02;
    kan1dualchnumList[10][2] = 0x44; // shift+f11
    kan1dualchnumList[11][0] = 122;
    kan1dualchnumList[11][1] = 0x02;
    kan1dualchnumList[11][2] = 0x45; // shift+f12
    kan1dualchnumList[12][0] = 11;
    kan1dualchnumList[12][1] = 0x14;
    kan1dualchnumList[12][2] = 0x1c; // q+y
    kan1dualchnumList[13][0] = 12;
    kan1dualchnumList[13][1] = 0x14;
    kan1dualchnumList[13][2] = 0x18; // q+u
    kan1dualchnumList[14][0] = 13;
    kan1dualchnumList[14][1] = 0x14;
    kan1dualchnumList[14][2] = 0x0c; // q+i
    kan1dualchnumList[15][0] = 14;
    kan1dualchnumList[15][1] = 0x14;
    kan1dualchnumList[15][2] = 0x12; // q+o
    kan1dualchnumList[16][0] = 15;
    kan1dualchnumList[16][1] = 0x14;
    kan1dualchnumList[16][2] = 0x13; // q+p
    kan1dualchnumList[17][0] = 21;
    kan1dualchnumList[17][1] = 0x1a;
    kan1dualchnumList[17][2] = 0x1c; // w+y
    kan1dualchnumList[18][0] = 22;
    kan1dualchnumList[18][1] = 0x1a;
    kan1dualchnumList[18][2] = 0x18; // w+u
    kan1dualchnumList[19][0] = 23;
    kan1dualchnumList[19][1] = 0x1a;
    kan1dualchnumList[19][2] = 0x0c; // w+i
    kan1dualchnumList[20][0] = 24;
    kan1dualchnumList[20][1] = 0x1a;
    kan1dualchnumList[20][2] = 0x12; // w+o
    kan1dualchnumList[21][0] = 25;
    kan1dualchnumList[21][1] = 0x1a;
    kan1dualchnumList[21][2] = 0x13; // w+p
    kan1dualchnumList[22][0] = 31;
    kan1dualchnumList[22][1] = 0x08;
    kan1dualchnumList[22][2] = 0x1c; // e+y
    kan1dualchnumList[23][0] = 32;
    kan1dualchnumList[23][1] = 0x08;
    kan1dualchnumList[23][2] = 0x18; // e+u
    kan1dualchnumList[24][0] = 33;
    kan1dualchnumList[24][1] = 0x08;
    kan1dualchnumList[24][2] = 0x0c; // e+i
    kan1dualchnumList[25][0] = 34;
    kan1dualchnumList[25][1] = 0x08;
    kan1dualchnumList[25][2] = 0x12; // e+o
    kan1dualchnumList[26][0] = 35;
    kan1dualchnumList[26][1] = 0x08;
    kan1dualchnumList[26][2] = 0x13; // e+p
    kan1dualchnumList[27][0] = 41;
    kan1dualchnumList[27][1] = 0x15;
    kan1dualchnumList[27][2] = 0x1c; // r+y
    kan1dualchnumList[28][0] = 42;
    kan1dualchnumList[28][1] = 0x15;
    kan1dualchnumList[28][2] = 0x18; // r+u
    kan1dualchnumList[29][0] = 43;
    kan1dualchnumList[29][1] = 0x15;
    kan1dualchnumList[29][2] = 0x0c; // r+i
    kan1dualchnumList[30][0] = 44;
    kan1dualchnumList[30][1] = 0x15;
    kan1dualchnumList[30][2] = 0x12; // r+o
    kan1dualchnumList[31][0] = 45;
    kan1dualchnumList[31][1] = 0x15;
    kan1dualchnumList[31][2] = 0x13; // r+p¶
    kan1dualchnumList[32][0] = 51;
    kan1dualchnumList[32][1] = 0x17;
    kan1dualchnumList[32][2] = 0x1c; // t+y
    kan1dualchnumList[33][0] = 52;
    kan1dualchnumList[33][1] = 0x17;
    kan1dualchnumList[33][2] = 0x18; // t+u
    kan1dualchnumList[34][0] = 53;
    kan1dualchnumList[34][1] = 0x17;
    kan1dualchnumList[34][2] = 0x0c; // t+i
    kan1dualchnumList[35][0] = 54;
    kan1dualchnumList[35][1] = 0x17;
    kan1dualchnumList[35][2] = 0x12; // t+o
    kan1dualchnumList[36][0] = 55;
    kan1dualchnumList[36][1] = 0x17;
    kan1dualchnumList[36][2] = 0x13; // t+p

    kan1dualchnumList[37][0] = 33;kan1dualchnumList[37][1] = 0x02; kan1dualchnumList[37][2]= 0x1E; // shift+1
    kan1dualchnumList[38][0] = 64;kan1dualchnumList[38][1] = 0x02; kan1dualchnumList[38][2]= 0x1F; // shift+2
    kan1dualchnumList[39][0] = 35;kan1dualchnumList[39][1] = 0x02; kan1dualchnumList[39][2]= 0x20; // shift+3
    kan1dualchnumList[40][0] = 36;kan1dualchnumList[40][1] = 0x02; kan1dualchnumList[40][2]= 0x21; // shift+4
    kan1dualchnumList[41][0] = 37;kan1dualchnumList[41][1] = 0x02; kan1dualchnumList[41][2]= 0x22; // shift+5
    kan1dualchnumList[42][0] = 94;kan1dualchnumList[42][1] = 0x02; kan1dualchnumList[42][2]= 0x23; // shift+6
    kan1dualchnumList[43][0] = 38;kan1dualchnumList[43][1] = 0x02; kan1dualchnumList[43][2]= 0x24; // shift+7
    kan1dualchnumList[44][0] = 42;kan1dualchnumList[44][1] = 0x02; kan1dualchnumList[44][2]= 0x25; // shift+8
    kan1dualchnumList[45][0] = 40;kan1dualchnumList[45][1] = 0x02; kan1dualchnumList[45][2]= 0x26; // shift+9
    kan1dualchnumList[46][0] = 41;kan1dualchnumList[46][1] = 0x02; kan1dualchnumList[46][2]= 0x27; // shift+0

    restrictedkan[1] = 0x14;
    restrictedkan[2] = 0x1a;
    restrictedkan[3] = 0x08;
    restrictedkan[4] = 0x15;
    restrictedkan[5] = 0x17;
    restrictedkan[6] = 0x1c;
    restrictedkan[7] = 0x18;
    restrictedkan[8] = 0x0c;
    restrictedkan[9] = 0x12;
    restrictedkan[10] = 0x13;

    Kan.langID = 1;
    Kan.charnumToUnico = kanunicodeList;

    Kan1.langID = 1;
    Kan1.singkeyList = kansingchnumList;
    Kan1.dualkeyList = kan1dualchnumList;
    Kan1.dualkeylen = 50;
    Kan1.restrkeylen = 10;
    Kan1.restrkeyList = restrictedkan;
}

void data_init_tel()
{
    telunicodeList[123] = 3074;
    telunicodeList[124] = 3075;
    telunicodeList[81] = 3077;
    telunicodeList[82] = 3078;
    telunicodeList[83] = 3079;
    telunicodeList[84] = 3080;
    telunicodeList[85] = 3081;
    telunicodeList[86] = 3082;
    telunicodeList[94] = 3083;
    telunicodeList[87] = 3086;
    telunicodeList[88] = 3087;
    telunicodeList[89] = 3088;
    telunicodeList[90] = 3090;
    telunicodeList[91] = 3091;
    telunicodeList[92] = 3092;
    telunicodeList[11] = 3093;
    telunicodeList[12] = 3094;
    telunicodeList[13] = 3095;
    telunicodeList[14] = 3096;
    telunicodeList[15] = 3097;
    telunicodeList[21] = 3098;
    telunicodeList[22] = 3099;
    telunicodeList[23] = 3100;
    telunicodeList[24] = 3101;
    telunicodeList[25] = 3102;
    telunicodeList[31] = 3103;
    telunicodeList[32] = 3104;
    telunicodeList[33] = 3105;
    telunicodeList[34] = 3106;
    telunicodeList[35] = 3107;
    telunicodeList[41] = 3108;
    telunicodeList[42] = 3109;
    telunicodeList[43] = 3110;
    telunicodeList[44] = 3111;
    telunicodeList[45] = 3112;
    telunicodeList[51] = 3114;
    telunicodeList[52] = 3115;
    telunicodeList[53] = 3116;
    telunicodeList[54] = 3117;
    telunicodeList[55] = 3118;
    telunicodeList[61] = 3119; //
    telunicodeList[62] = 3120;
    telunicodeList[63] = 3122;
    telunicodeList[69] = 3123;
    telunicodeList[64] = 3125;
    telunicodeList[65] = 3126;
    telunicodeList[66] = 3127;
    telunicodeList[67] = 3128;
    telunicodeList[68] = 3129;
    telunicodeList[112] = 3134;
    telunicodeList[113] = 3135;
    telunicodeList[114] = 3136;
    telunicodeList[115] = 3137;
    telunicodeList[116] = 3138;
    telunicodeList[124] = 3139;
    telunicodeList[117] = 3142;
    telunicodeList[118] = 3143;
    telunicodeList[119] = 3144;
    telunicodeList[120] = 3146;
    telunicodeList[121] = 3147;
    telunicodeList[122] = 3148;
    telunicodeList[110] = 3149;
    telunicodeList[95] = 3168;
    telunicodeList[0] = 3174;
    telunicodeList[1] = 3175;
    telunicodeList[2] = 3176;
    telunicodeList[3] = 3177;
    telunicodeList[4] = 3178;
    telunicodeList[5] = 3179;
    telunicodeList[6] = 3180;
    telunicodeList[7] = 3181;
    telunicodeList[8] = 3182;
    telunicodeList[9] = 3183;

    tel1singchnumList[0x3a] = 81;  // f1
    tel1singchnumList[0x3b] = 82;  // f2
    tel1singchnumList[0x3c] = 83;  // f3
    tel1singchnumList[0x3d] = 84;  // f4
    tel1singchnumList[0x3e] = 85;  // f5
    tel1singchnumList[0x3f] = 86;  // f6
    tel1singchnumList[0x40] = 87;  // f7
    tel1singchnumList[0x41] = 88;  // f8
    tel1singchnumList[0x42] = 89;  // f9
    tel1singchnumList[0x43] = 90;  // f10
    tel1singchnumList[0x44] = 91;  // f11
    tel1singchnumList[0x45] = 92;  // f12
    tel1singchnumList[0x06] = 124; // c
    tel1singchnumList[0x27] = 0;   // 0
    tel1singchnumList[0x1e] = 1;   // 1
    tel1singchnumList[0x1f] = 2;   // 2
    tel1singchnumList[0x20] = 3;   // 3
    tel1singchnumList[0x21] = 4;   // 4
    tel1singchnumList[0x22] = 5;   // 5
    tel1singchnumList[0x23] = 6;   // 6
    tel1singchnumList[0x24] = 7;   // 7
    tel1singchnumList[0x25] = 8;   // 8
    tel1singchnumList[0x26] = 9;   // 9
    tel1singchnumList[0x04] = 61;  // a
    tel1singchnumList[0x16] = 62;  // s
    tel1singchnumList[0x07] = 63;  // d
    tel1singchnumList[0x09] = 64;  // f
    tel1singchnumList[0x0a] = 65;  // g
    tel1singchnumList[0x0b] = 66;  // h
    tel1singchnumList[0x0d] = 67;  // j
    tel1singchnumList[0x0e] = 68;  // k
    tel1singchnumList[0x0f] = 69;  // l
    tel1singchnumList[0x1d] = 94;  // z
    tel1singchnumList[0x1b] = 95;  // x
    tel1singchnumList[0x19] = 128;  // v
    tel1singchnumList[0x05] = 126; // b
    tel1singchnumList[0x11] = 123; // n
    tel1singchnumList[0x10] = 124; // m
    tel1singchnumList[0x14] = 16; // q
    tel1singchnumList[0x1a] = 17; // w
    tel1singchnumList[0x08] = 18; // e
    tel1singchnumList[0x15] = 19; // r
    tel1singchnumList[0x17] = 20; // t
    tel1singchnumList[0x1c] = 26; // y
    tel1singchnumList[0x18] = 27;// u
    tel1singchnumList[0x0c] = 28; // i
    tel1singchnumList[0x12] = 29; // o
    tel1singchnumList[0x13] = 30; // p


    tel1dualchnumList[0][0] = 110;
    tel1dualchnumList[0][1] = 0x02;
    tel1dualchnumList[0][2] = 0x3a; // shift+f1
    tel1dualchnumList[1][0] = 112;
    tel1dualchnumList[1][1] = 0x02;
    tel1dualchnumList[1][2] = 0x3b; // shift+f2
    tel1dualchnumList[2][0] = 113;
    tel1dualchnumList[2][1] = 0x02;
    tel1dualchnumList[2][2] = 0x3c; // shift+f3
    tel1dualchnumList[3][0] = 114;
    tel1dualchnumList[3][1] = 0x02;
    tel1dualchnumList[3][2] = 0x3d; // shift+f4
    tel1dualchnumList[4][0] = 115;
    tel1dualchnumList[4][1] = 0x02;
    tel1dualchnumList[4][2] = 0x3e; // shift+f5
    tel1dualchnumList[5][0] = 116;
    tel1dualchnumList[5][1] = 0x02;
    tel1dualchnumList[5][2] = 0x3f; // shift+f6
    tel1dualchnumList[6][0] = 117;
    tel1dualchnumList[6][1] = 0x02;
    tel1dualchnumList[6][2] = 0x40; // shift+f7
    tel1dualchnumList[7][0] = 118;
    tel1dualchnumList[7][1] = 0x02;
    tel1dualchnumList[7][2] = 0x41; // shift+f8
    tel1dualchnumList[8][0] = 119;
    tel1dualchnumList[8][1] = 0x02;
    tel1dualchnumList[8][2] = 0x42; // shift+f9
    tel1dualchnumList[9][0] = 120;
    tel1dualchnumList[9][1] = 0x02;
    tel1dualchnumList[9][2] = 0x43; // shift+f10
    tel1dualchnumList[10][0] = 121;
    tel1dualchnumList[10][1] = 0x02;
    tel1dualchnumList[10][2] = 0x44; // shift+f11
    tel1dualchnumList[11][0] = 122;
    tel1dualchnumList[11][1] = 0x02;
    tel1dualchnumList[11][2] = 0x45; // shift+f12
    tel1dualchnumList[12][0] = 11;
    tel1dualchnumList[12][1] = 0x14;
    tel1dualchnumList[12][2] = 0x1c; // q+y
    tel1dualchnumList[13][0] = 12;
    tel1dualchnumList[13][1] = 0x14;
    tel1dualchnumList[13][2] = 0x18; // q+u
    tel1dualchnumList[14][0] = 13;
    tel1dualchnumList[14][1] = 0x14;
    tel1dualchnumList[14][2] = 0x0c; // q+i
    tel1dualchnumList[15][0] = 14;
    tel1dualchnumList[15][1] = 0x14;
    tel1dualchnumList[15][2] = 0x12; // q+o
    tel1dualchnumList[16][0] = 15;
    tel1dualchnumList[16][1] = 0x14;
    tel1dualchnumList[16][2] = 0x13; // q+p
    tel1dualchnumList[17][0] = 21;
    tel1dualchnumList[17][1] = 0x1a;
    tel1dualchnumList[17][2] = 0x1c; // w+y
    tel1dualchnumList[18][0] = 22;
    tel1dualchnumList[18][1] = 0x1a;
    tel1dualchnumList[18][2] = 0x18; // w+u
    tel1dualchnumList[19][0] = 23;
    tel1dualchnumList[19][1] = 0x1a;
    tel1dualchnumList[19][2] = 0x0c; // w+i
    tel1dualchnumList[20][0] = 24;
    tel1dualchnumList[20][1] = 0x1a;
    tel1dualchnumList[20][2] = 0x12; // w+o
    tel1dualchnumList[21][0] = 25;
    tel1dualchnumList[21][1] = 0x1a;
    tel1dualchnumList[21][2] = 0x13; // w+p
    tel1dualchnumList[22][0] = 31;
    tel1dualchnumList[22][1] = 0x08;
    tel1dualchnumList[22][2] = 0x1c; // e+y
    tel1dualchnumList[23][0] = 32;
    tel1dualchnumList[23][1] = 0x08;
    tel1dualchnumList[23][2] = 0x18; // e+u
    tel1dualchnumList[24][0] = 33;
    tel1dualchnumList[24][1] = 0x08;
    tel1dualchnumList[24][2] = 0x0c; // e+i
    tel1dualchnumList[25][0] = 34;
    tel1dualchnumList[25][1] = 0x08;
    tel1dualchnumList[25][2] = 0x12; // e+o
    tel1dualchnumList[26][0] = 35;
    tel1dualchnumList[26][1] = 0x08;
    tel1dualchnumList[26][2] = 0x13; // e+p
    tel1dualchnumList[27][0] = 41;
    tel1dualchnumList[27][1] = 0x15;
    tel1dualchnumList[27][2] = 0x1c; // r+y
    tel1dualchnumList[28][0] = 42;
    tel1dualchnumList[28][1] = 0x15;
    tel1dualchnumList[28][2] = 0x18; // r+u
    tel1dualchnumList[29][0] = 43;
    tel1dualchnumList[29][1] = 0x15;
    tel1dualchnumList[29][2] = 0x0c; // r+i
    tel1dualchnumList[30][0] = 44;
    tel1dualchnumList[30][1] = 0x15;
    tel1dualchnumList[30][2] = 0x12; // r+o
    tel1dualchnumList[31][0] = 45;
    tel1dualchnumList[31][1] = 0x15;
    tel1dualchnumList[31][2] = 0x13; // r+p
    tel1dualchnumList[32][0] = 51;
    tel1dualchnumList[32][1] = 0x17;
    tel1dualchnumList[32][2] = 0x1c; // t+y
    tel1dualchnumList[33][0] = 52;
    tel1dualchnumList[33][1] = 0x17;
    tel1dualchnumList[33][2] = 0x18; // t+u
    tel1dualchnumList[34][0] = 53;
    tel1dualchnumList[34][1] = 0x17;
    tel1dualchnumList[34][2] = 0x0c; // t+i
    tel1dualchnumList[35][0] = 54;
    tel1dualchnumList[35][1] = 0x17;
    tel1dualchnumList[35][2] = 0x12; // t+o
    tel1dualchnumList[36][0] = 55;
    tel1dualchnumList[36][1] = 0x17;
    tel1dualchnumList[36][2] = 0x13; // t+p
    tel1dualchnumList[37][0] = 33;tel1dualchnumList[37][1] = 0x02; tel1dualchnumList[37][2]= 0x1E; // shift+1
    tel1dualchnumList[38][0] = 64;tel1dualchnumList[38][1] = 0x02; tel1dualchnumList[38][2]= 0x1F; // shift+2
    tel1dualchnumList[39][0] = 35;tel1dualchnumList[39][1] = 0x02; tel1dualchnumList[39][2]= 0x20; // shift+3
    tel1dualchnumList[40][0] = 36;tel1dualchnumList[40][1] = 0x02; tel1dualchnumList[40][2]= 0x21; // shift+4
    tel1dualchnumList[41][0] = 37;tel1dualchnumList[41][1] = 0x02; tel1dualchnumList[41][2]= 0x22; // shift+5
    tel1dualchnumList[42][0] = 94;tel1dualchnumList[42][1] = 0x02; tel1dualchnumList[42][2]= 0x23; // shift+6
    tel1dualchnumList[43][0] = 38;tel1dualchnumList[43][1] = 0x02; tel1dualchnumList[43][2]= 0x24; // shift+7
    tel1dualchnumList[44][0] = 42;tel1dualchnumList[44][1] = 0x02; tel1dualchnumList[44][2]= 0x25; // shift+8
    tel1dualchnumList[45][0] = 40;tel1dualchnumList[45][1] = 0x02; tel1dualchnumList[45][2]= 0x26; // shift+9
    tel1dualchnumList[46][0] = 41;tel1dualchnumList[46][1] = 0x02; tel1dualchnumList[46][2]= 0x27; // shift+0


    Tel.langID = 2;
    Tel.charnumToUnico = telunicodeList;

    Tel1.langID = 2;
    Tel1.singkeyList = tel1singchnumList;
    Tel1.dualkeyList = tel1dualchnumList;
    Tel1.dualkeylen = 50;
   Tel1.restrkeylen = 1;
    Tel1.restrkeyList = NULL;
}

void data_init_hin()
{
    // hindiUnicodeList[128] = 3201;
    // hindiUnicodeList[126] = 3202;
    // hindiUnicodeList[127] = 3203;
    hindiUnicodeList[81] = 2309; // अ
    hindiUnicodeList[82] = 2310; // आ
    hindiUnicodeList[83] = 2311; // इ
    hindiUnicodeList[84] = 2312; // ई
    hindiUnicodeList[85] = 2313; // उ
    hindiUnicodeList[86] = 2314; // ऊ
    // hindiUnicodeList[94]  = 23;
    hindiUnicodeList[87] = 2315; // ऋ
    hindiUnicodeList[88] = 2320; // ऐ
    hindiUnicodeList[89] = 2317; // ऍ
    hindiUnicodeList[90] = 2323; // ओ
    hindiUnicodeList[91] = 2324; // औ
    // hindiUnicodeList[92]  = ; //
    hindiUnicodeList[11] = 2325; // क
    hindiUnicodeList[12] = 2326; // ख
    hindiUnicodeList[13] = 2327; // ग
    hindiUnicodeList[14] = 2328; // घ
    hindiUnicodeList[15] = 2329; // ङ

    hindiUnicodeList[21] = 2330; // च
    hindiUnicodeList[22] = 2331; // छ
    hindiUnicodeList[23] = 2332; // ज
    hindiUnicodeList[24] = 2333; // झ
    hindiUnicodeList[25] = 2334; // ञ

    hindiUnicodeList[31] = 2335; // ट
    hindiUnicodeList[32] = 2336; // ठ
    hindiUnicodeList[33] = 2337; // ड
    hindiUnicodeList[34] = 2338; // ढ
    hindiUnicodeList[35] = 2339; // ण

    hindiUnicodeList[41] = 2340; // त
    hindiUnicodeList[42] = 2341; // थ
    hindiUnicodeList[43] = 2342; // द
    hindiUnicodeList[44] = 2343; // ध
    hindiUnicodeList[45] = 2344; // न

    hindiUnicodeList[51] = 2346; // प
    hindiUnicodeList[52] = 2347; // फ
    hindiUnicodeList[53] = 2348; // ब
    hindiUnicodeList[54] = 2349; // भ
    hindiUnicodeList[55] = 2350; // म

    hindiUnicodeList[61] = 2351; // य
    hindiUnicodeList[62] = 2352; // र
    hindiUnicodeList[70] = 2354; // ल
    hindiUnicodeList[63] = 2357; // व
    hindiUnicodeList[69] = 2358; // श
    hindiUnicodeList[64] = 2359; // ष
    hindiUnicodeList[65] = 2360; // स
    hindiUnicodeList[66] = 2361; // ह
    // hindiUnicodeList[67]  = 3256; //
    // hindiUnicodeList[68]  = 3257; //

    hindiUnicodeList[112] = 2366; // ा
    hindiUnicodeList[113] = 2367; //$ि
    hindiUnicodeList[114] = 2368; // ी
    hindiUnicodeList[115] = 2369; //$ु
    hindiUnicodeList[116] = 2370; //$ू
    hindiUnicodeList[124] = 2375; //$े
    hindiUnicodeList[117] = 2376; //$ै
    hindiUnicodeList[118] = 2379; //$ो
    hindiUnicodeList[119] = 2380; //$ौ
    hindiUnicodeList[120] = 2306; //$ं
    hindiUnicodeList[121] = 2307; //$ः
    // hindiUnicodeList[122] = 2377;
    // hindiUnicodeList[110] = 2378;
    // hindiUnicodeList[95]  = 3296;
    hindiUnicodeList[0] = 2406; // ०
    hindiUnicodeList[1] = 2407; // १
    hindiUnicodeList[2] = 2408; // २
    hindiUnicodeList[3] = 2409; // ३
    hindiUnicodeList[4] = 2410; // ४
    hindiUnicodeList[5] = 2411; // ५
    hindiUnicodeList[6] = 2412; // ६
    hindiUnicodeList[7] = 2413; // ७
    hindiUnicodeList[8] = 2414; // ८
    hindiUnicodeList[9] = 2415; // ९

    hinsingchnum[0x3a] = 81;  // f1
    hinsingchnum[0x3b] = 82;  // f2
    hinsingchnum[0x3c] = 83;  // f3
    hinsingchnum[0x3d] = 84;  // f4
    hinsingchnum[0x3e] = 85;  // f5
    hinsingchnum[0x3f] = 86;  // f6
    hinsingchnum[0x40] = 87;  // f7
    hinsingchnum[0x41] = 88;  // f8
    hinsingchnum[0x42] = 89;  // f9
    hinsingchnum[0x43] = 90;  // f10
    hinsingchnum[0x44] = 91;  // f11
    hinsingchnum[0x45] = 92;  // f12
    hinsingchnum[0x06] = 124; // c
    hinsingchnum[0x27] = 0;   // 0
    hinsingchnum[0x1e] = 1;   // 1
    hinsingchnum[0x1f] = 2;   // 2
    hinsingchnum[0x20] = 3;   // 3
    hinsingchnum[0x21] = 4;   // 4
    hinsingchnum[0x22] = 5;   // 5
    hinsingchnum[0x23] = 6;   // 6
    hinsingchnum[0x24] = 7;   // 7
    hinsingchnum[0x25] = 8;   // 8
    hinsingchnum[0x26] = 9;   // 9
    hinsingchnum[0x04] = 61;  // a
    hinsingchnum[0x16] = 62;  // s
    hinsingchnum[0x07] = 70;  // d
    hinsingchnum[0x09] = 63;  // f
    hinsingchnum[0x0a] = 69;  // g
    hinsingchnum[0x0b] = 64;  // h
    hinsingchnum[0x0d] = 65;  // j
    hinsingchnum[0x0e] = 66;  // k
    hinsingchnum[0x0f] = 69;  // l
    hinsingchnum[0x1d] = 94;  // z
    hinsingchnum[0x1b] = 95;  // x
    hinsingchnum[0x19] = 70;  // v
    hinsingchnum[0x05] = 126; // b
    hinsingchnum[0x11] = 121; // n
    hinsingchnum[0x10] = 120; // m
    hinsingchnum[0x1a] = 17; // w
    hinsingchnum[0x14] = 16; // q
    hinsingchnum[0x08] = 18; // e
    hinsingchnum[0x15] = 19; // r
    hinsingchnum[0x17] = 20; // t
    hinsingchnum[0x1c] = 26; // y
    hinsingchnum[0x18] = 27;// u
    hinsingchnum[0x0c] = 28; // i
    hinsingchnum[0x12] = 29; // o
    hinsingchnum[0x13] = 30; // p



    hindualchnum[0][0] = 112;
    hindualchnum[0][1] = 0x02;
    hindualchnum[0][2] = 0x3a; // shift+f1
    hindualchnum[1][0] = 113;
    hindualchnum[1][1] = 0x02;
    hindualchnum[1][2] = 0x3b; // shift+f2
    hindualchnum[2][0] = 114;
    hindualchnum[2][1] = 0x02;
    hindualchnum[2][2] = 0x3c; // shift+f3
    hindualchnum[3][0] = 115;
    hindualchnum[3][1] = 0x02;
    hindualchnum[3][2] = 0x3d; // shift+f4
    hindualchnum[4][0] = 116;
    hindualchnum[4][1] = 0x02;
    hindualchnum[4][2] = 0x3e; // shift+f5
    hindualchnum[5][0] = 124;
    hindualchnum[5][1] = 0x02;
    hindualchnum[5][2] = 0x3f; // shift+f6
    hindualchnum[6][0] = 117;
    hindualchnum[6][1] = 0x02;
    hindualchnum[6][2] = 0x40; // shift+f7
    hindualchnum[7][0] = 118;
    hindualchnum[7][1] = 0x02;
    hindualchnum[7][2] = 0x41; // shift+f8
    hindualchnum[8][0] = 119;
    hindualchnum[8][1] = 0x02;
    hindualchnum[8][2] = 0x42; // shift+f9
    hindualchnum[9][0] = 120;
    hindualchnum[9][1] = 0x02;
    hindualchnum[9][2] = 0x43; // shift+f10
    hindualchnum[10][0] = 121;
    hindualchnum[10][1] = 0x02;
    hindualchnum[10][2] = 0x44; // shift+f11
    // hindualchnum[11][0] = 122; hindualchnum[11][1] = 0x02; hindualchnum[11][2] = 0x45; // shift+f12
    hindualchnum[12][0] = 11;
    hindualchnum[12][1] = 0x14;
    hindualchnum[12][2] = 0x1c; // q+y
    hindualchnum[13][0] = 12;
    hindualchnum[13][1] = 0x14;
    hindualchnum[13][2] = 0x18; // q+u
    hindualchnum[14][0] = 13;
    hindualchnum[14][1] = 0x14;
    hindualchnum[14][2] = 0x0c; // q+i
    hindualchnum[15][0] = 14;
    hindualchnum[15][1] = 0x14;
    hindualchnum[15][2] = 0x12; // q+o
    hindualchnum[16][0] = 15;
    hindualchnum[16][1] = 0x14;
    hindualchnum[16][2] = 0x13; // q+p
    hindualchnum[17][0] = 21;
    hindualchnum[17][1] = 0x1a;
    hindualchnum[17][2] = 0x1c; // w+y
    hindualchnum[18][0] = 22;
    hindualchnum[18][1] = 0x1a;
    hindualchnum[18][2] = 0x18; // w+u
    hindualchnum[19][0] = 23;
    hindualchnum[19][1] = 0x1a;
    hindualchnum[19][2] = 0x0c; // w+i
    hindualchnum[20][0] = 24;
    hindualchnum[20][1] = 0x1a;
    hindualchnum[20][2] = 0x12; // w+o
    hindualchnum[21][0] = 25;
    hindualchnum[21][1] = 0x1a;
    hindualchnum[21][2] = 0x13; // w+p
    hindualchnum[22][0] = 31;
    hindualchnum[22][1] = 0x08;
    hindualchnum[22][2] = 0x1c; // e+y
    hindualchnum[23][0] = 32;
    hindualchnum[23][1] = 0x08;
    hindualchnum[23][2] = 0x18; // e+u
    hindualchnum[24][0] = 33;
    hindualchnum[24][1] = 0x08;
    hindualchnum[24][2] = 0x0c; // e+i
    hindualchnum[25][0] = 34;
    hindualchnum[25][1] = 0x08;
    hindualchnum[25][2] = 0x12; // e+o
    hindualchnum[26][0] = 35;
    hindualchnum[26][1] = 0x08;
    hindualchnum[26][2] = 0x13; // e+p
    hindualchnum[27][0] = 41;
    hindualchnum[27][1] = 0x15;
    hindualchnum[27][2] = 0x1c; // r+y
    hindualchnum[28][0] = 42;
    hindualchnum[28][1] = 0x15;
    hindualchnum[28][2] = 0x18; // r+u
    hindualchnum[29][0] = 43;
    hindualchnum[29][1] = 0x15;
    hindualchnum[29][2] = 0x0c; // r+i
    hindualchnum[30][0] = 44;
    hindualchnum[30][1] = 0x15;
    hindualchnum[30][2] = 0x12; // r+o
    hindualchnum[31][0] = 45;
    hindualchnum[31][1] = 0x15;
    hindualchnum[31][2] = 0x13; // r+p
    hindualchnum[32][0] = 51;
    hindualchnum[32][1] = 0x17;
    hindualchnum[32][2] = 0x1c; // t+y
    hindualchnum[33][0] = 52;
    hindualchnum[33][1] = 0x17;
    hindualchnum[33][2] = 0x18; // t+u
    hindualchnum[34][0] = 53;
    hindualchnum[34][1] = 0x17;
    hindualchnum[34][2] = 0x0c; // t+i
    hindualchnum[35][0] = 54;
    hindualchnum[35][1] = 0x17;
    hindualchnum[35][2] = 0x12; // t+o
    hindualchnum[36][0] = 55;
    hindualchnum[36][1] = 0x17;
    hindualchnum[36][2] = 0x13; // t+p
    hindualchnum[37][0] = 33;hindualchnum[37][1] = 0x02; hindualchnum[37][2]= 0x1E; // shift+1
    hindualchnum[38][0] = 64;hindualchnum[38][1] = 0x02; hindualchnum[38][2]= 0x1F; // shift+2
    hindualchnum[39][0] = 35;hindualchnum[39][1] = 0x02; hindualchnum[39][2]= 0x20; // shift+3
    hindualchnum[40][0] = 36;hindualchnum[40][1] = 0x02; hindualchnum[40][2]= 0x21; // shift+4
    hindualchnum[41][0] = 37;hindualchnum[41][1] = 0x02; hindualchnum[41][2]= 0x22; // shift+5
    hindualchnum[42][0] = 94;hindualchnum[42][1] = 0x02; hindualchnum[42][2]= 0x23; // shift+6
    hindualchnum[43][0] = 38;hindualchnum[43][1] = 0x02; hindualchnum[43][2]= 0x24; // shift+7
    hindualchnum[44][0] = 42;hindualchnum[44][1] = 0x02; hindualchnum[44][2]= 0x25; // shift+8
    hindualchnum[45][0] = 40;hindualchnum[45][1] = 0x02; hindualchnum[45][2]= 0x26; // shift+9
    hindualchnum[46][0] = 41;hindualchnum[46][1] = 0x02; hindualchnum[46][2]= 0x27; // shift+0






    Hin.langID = 3;
    Hin.charnumToUnico = hindiUnicodeList;

    Hin1.langID = 3;
    Hin1.singkeyList = hinsingchnum;
    Hin1.dualkeyList = hindualchnum;
    Hin1.dualkeylen = 50;
    Hin1.restrkeylen = 1;
    Hin1.restrkeyList = NULL;
}

void data_init_eng()
{
 //engunicodelist[33]=33;
 
    engsingchnum[0x14] = 97; // q
    engsingchnum[0x1a] = 98; // w
    engsingchnum[0x08] = 99; // e
    engsingchnum[0x15] = 100; // r
    engsingchnum[0x17] = 101; // t
    engsingchnum[0x1c] = 102; // y
    engsingchnum[0x18] = 103;// u
    engsingchnum[0x0c] = 104; // i
    engsingchnum[0x12] = 105; // o
    engsingchnum[0x13] = 106; // p
    engsingchnum[0x04] = 107;  // a
    engsingchnum[0x16] = 108;  // s
    engsingchnum[0x07] = 109;  // d
    engsingchnum[0x09] = 110;  // f
    engsingchnum[0x0a] = 111;  // g
    engsingchnum[0x0b] = 112;  // h
    engsingchnum[0x0d] = 113;  // j
    engsingchnum[0x0e] = 114;  // k
    engsingchnum[0x0f] = 115;  // l
    engsingchnum[0x1d] = 116;  // z
    engsingchnum[0x1b] = 117;  // x
    engsingchnum[0x06] = 118; // c
    engsingchnum[0x19] = 119;  // v
    engsingchnum[0x05] = 120; // b
    engsingchnum[0x11] = 121; // n
    engsingchnum[0x10] = 122; //m

    //engdualchnum
    engdualchnum[0][0] = 65;engdualchnum[0][1] = 0x02; engdualchnum[0][2]= 0x14; // shift+q
    engdualchnum[1][0] = 66;engdualchnum[1][1] = 0x02; engdualchnum[1][2]= 0x1A; // shift+w
    engdualchnum[2][0] = 67; engdualchnum[2][1] = 0x02; engdualchnum[2][2] = 0x08; // shift+e
    engdualchnum[3][0] = 68;engdualchnum[3][1] = 0x02; engdualchnum[3][2]= 0x15; // shift+r
    engdualchnum[4][0] = 69;engdualchnum[4][1] = 0x02; engdualchnum[4][2]= 0x17; // shift+t
    engdualchnum[5][0] = 70;engdualchnum[5][1] = 0x02; engdualchnum[5][2]= 0x1C; // shift+y
    engdualchnum[6][0] = 71;engdualchnum[6][1] = 0x02; engdualchnum[6][2]= 0x18; // shift+u
    engdualchnum[7][0] = 72; engdualchnum[7][1] = 0x02; engdualchnum[7][2] = 0x0C; // shift+i
    engdualchnum[8][0] = 73;engdualchnum[8][1] = 0x02; engdualchnum[8][2]= 0x12; // shift+o
    engdualchnum[9][0] = 74;engdualchnum[9][1] = 0x02; engdualchnum[9][2]= 0x13; // shift+p
    engdualchnum[10][0] = 75; engdualchnum[10][1] = 0x02; engdualchnum[10][2] = 0x04; // shift+a
    engdualchnum[11][0] = 76;engdualchnum[11][1] = 0x02; engdualchnum[11][2]= 0x16; // shift+s
    engdualchnum[12][0] = 77; engdualchnum[12][1] = 0x02; engdualchnum[12][2] = 0x07; // shift+d
    engdualchnum[13][0] = 78; engdualchnum[13][1] = 0x02; engdualchnum[13][2] = 0x09; // shift+f
    engdualchnum[14][0] = 79; engdualchnum[14][1] = 0x02; engdualchnum[14][2] = 0x0A; // shift+g
    engdualchnum[15][0] = 80; engdualchnum[15][1] = 0x02; engdualchnum[15][2] = 0x0B; // shift+h
    engdualchnum[16][0] = 81; engdualchnum[16][1] = 0x02; engdualchnum[16][2] = 0x0D; // shift+j
    engdualchnum[17][0] = 82;engdualchnum[17][1] = 0x02; engdualchnum[17][2]= 0x0E; // shift+k
    engdualchnum[18][0] = 83;engdualchnum[18][1] = 0x02; engdualchnum[18][2]= 0x0F; // shift+l
    engdualchnum[19][0] = 84;engdualchnum[19][1] = 0x02; engdualchnum[19][2]= 0x1D; // shift+z
    engdualchnum[20][0] = 85;engdualchnum[20][1] = 0x02; engdualchnum[20][2]= 0x1B; // shift+x
    engdualchnum[21][0] = 86; engdualchnum[21][1] = 0x02; engdualchnum[21][2] = 0x06; // shift+c
    engdualchnum[22][0] = 87;engdualchnum[22][1] = 0x02; engdualchnum[22][2]= 0x19; // shift+v
    engdualchnum[23][0] = 88; engdualchnum[23][1] = 0x02; engdualchnum[23][2] = 0x05; // shift+b
    engdualchnum[24][0] = 89;engdualchnum[24][1] = 0x02; engdualchnum[24][2]= 0x11; // shift+n
    engdualchnum[25][0] = 90;engdualchnum[25][1] = 0x02; engdualchnum[25][2]= 0x10; // shift+m
    engdualchnum[26][0] = 33;engdualchnum[26][1] = 0x02; engdualchnum[26][2]= 0x1E; // shift+1
    engdualchnum[27][0] = 64;engdualchnum[27][1] = 0x02; engdualchnum[27][2]= 0x1F; // shift+2
    engdualchnum[28][0] = 35;engdualchnum[28][1] = 0x02; engdualchnum[28][2]= 0x20; // shift+3
    engdualchnum[29][0] = 36;engdualchnum[29][1] = 0x02; engdualchnum[29][2]= 0x21; // shift+4
    engdualchnum[30][0] = 37;engdualchnum[30][1] = 0x02; engdualchnum[30][2]= 0x22; // shift+5
    engdualchnum[31][0] = 94;engdualchnum[31][1] = 0x02; engdualchnum[31][2]= 0x23; // shift+6
    engdualchnum[32][0] = 38;engdualchnum[32][1] = 0x02; engdualchnum[32][2]= 0x24; // shift+7
    engdualchnum[33][0] = 42;engdualchnum[33][1] = 0x02; engdualchnum[33][2]= 0x25; // shift+8
    engdualchnum[34][0] = 40;engdualchnum[34][1] = 0x02; engdualchnum[34][2]= 0x26; // shift+9
    engdualchnum[35][0] = 41;engdualchnum[35][1] = 0x02; engdualchnum[35][2]= 0x27; // shift+0
    engdualchnum[36][0] = 60;engdualchnum[36][1] = 0x02; engdualchnum[36][2]= 0x36; // shift+,
    engdualchnum[37][0] = 62;engdualchnum[37][1] = 0x02; engdualchnum[37][2]= 0x37; // shift+.
    engdualchnum[38][0] = 63;engdualchnum[38][1] = 0x02; engdualchnum[38][2]= 0x38; // shift+/
    engdualchnum[39][0] = 58;engdualchnum[39][1] = 0x02; engdualchnum[39][2]= 0x33; // shift+;
    engdualchnum[40][0] = 34;engdualchnum[40][1] = 0x02; engdualchnum[40][2]= 0x34; // shift+'
    engdualchnum[41][0] = 123;engdualchnum[41][1] = 0x02; engdualchnum[41][2]= 0x2F; // shift+[
    engdualchnum[42][0] = 125;engdualchnum[42][1] = 0x02; engdualchnum[42][2]= 0x30; // shift+]
    engdualchnum[43][0] = 95;engdualchnum[43][1] = 0x02; engdualchnum[43][2]= 0x2D; // shift+-
    engdualchnum[44][0] = 43;engdualchnum[44][1] = 0x02; engdualchnum[44][2]= 0x2E; // shift+=
    engdualchnum[45][0] = 124;engdualchnum[45][1] = 0x02; engdualchnum[45][2]= 0x31; /* shift+\  */

    //engdualchnum[10][0] = 121;engdualchnum[10][1] = 0x02; engdualchnum[10][2]= 0x27; // shift+0
    //engdualchnum[10][0] = 121;engdualchnum[10][1] = 0x02; engdualchnum[10][2]= 0x27; // shift+0

 Eng.langID = 4;
    Eng.charnumToUnico = engunicodelist;

    Eng1.langID = 4;
    Eng1.singkeyList = engsingchnum;
    Eng1.dualkeyList = engdualchnum;
    Eng1.dualkeylen = 50;
    Eng1.restrkeylen = 1;
    Eng1.restrkeyList = NULL;

}
