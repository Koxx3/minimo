// Grey values
// 170 : 0xAD55
// 150 : 0x94B2
// 130 : 0x8410 
// 110 : 0x6B6D 
// 90 : 0x5ACB 
// 70 : 0x4228 
// 50 : 0x3186

#if TFT_MODEL == 1 // 2.4"
#define MY_TFT_DARK_DIGIT_DISABLED 0x3186 
#elif TFT_MODEL == 2 // 3.5" v1
#define MY_TFT_DARK_DIGIT_DISABLED 0x8410
#else  // 3.5" v2
#define MY_TFT_DARK_DIGIT_DISABLED (((0x40 & 0xf8)<<8) + ((0x40 & 0xfc)<<3)+(0x50>>3))
#endif

#define MY_TFT_DARK_DIGIT TFT_BLACK

// white color
#if TFT_MODEL == 3 // 3.5" v2
#define WHITE_RED 0xe0
#define WHITE_GREEN 0xe0
#define WHITE_BLUE 0xff
#define MY_TFT_WHITE (((WHITE_RED & 0xf8)<<8) + ((WHITE_GREEN & 0xfc)<<3)+(WHITE_BLUE>>3))
#else
#define MY_TFT_WHITE TFT_WHITE
#endif

#define MY_TFT_RED TFT_RED

