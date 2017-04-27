// ------------------------------------------------------------------------------------
// Powered by PWMDAC_Synthライブラリ http://kamide.b.sourceforge.jp/pwmdac_synth_lib/
// ------------------------------------------------------------------------------------
// #define SD_MODE    // SD カードを使う場合は有効、MML をソースに含める場合は無効
#define USE_SERIAL    // シリアルで制御するか
#define NOTE_PORTC    // PORTC (A0～A5)にチャンネルのノート情報を反映 (noteON=LOW/noteOFF=HIGH)
// #define DEBUG_MODE  // デバッグ情報をシリアルに垂れ流すモード
#define AUTOSLEEPTIME  5000 // PAUSE から SLEEP に推移するまでの時間(msec)
#define INTERVALTIME  1000 // 曲間の待ち時間(msec)
// ------------------------------------------------------------------------------------
#define PWMDAC_OUTPUT_PIN  9
#include "PWMDAC_Synth.h"
#include <avr/sleep.h> 
#include <avr/pgspace.h>
#include <EEPROM.h>
#ifdef SD_MODE
#include <SD.h>
#define SD_CS 10
#endif
// ------------------------------------------------------------------------------------
#define MAX_TRACK 1    // MML の登録曲数 、"!" で MML の区切り。最初と最後には入れない事。
PROGMEM char mmlstr[] = 
// ------------------------------------------------------------------------------------
"\"t120o4r1r1r1r4r4b-8g+8d+8c+8c+4&c+8.g+32a32b-8g+8f+8>c+8<>c+2<g+8f+8f8f+8>c+2d+"
"2<b-8.b32b-32g+4b-8g+8d+8c+8c+2b-8g+8f+8>c+8<>c+2<g+8f+8f8f+8>c+2d+2<>f4d+4<b-8"
"g+8d+8c+8c+4&c+8.g+32a32b-8g+8f+8>c+8<>c+2<g+8f+8f8f+8>c+2d+2<b-8.b32b-32g+4b-8"
"g+8d+8c+8c+2b-8g+8f+8>c+8<>c+2<g+8f+8f8f+8>c+2d+2<>f4d+4&d+2r1r1r1r1r1r1r1r1r1r"
"1r1<r4r4b-8g+8d+8c+8c+4&c+8.g+32a32b-8g+8f+8>c+8<>c+2<g+8f+8f8f+8>c+2<f8f+8g+8b"
"-8b-2b-8g+8d+8c+8c+2b-8g+8f+8>c+8<>c+2<f8f+8g+8f+8f4.c+4d+8<b-8>g+8<>f+2b-8g+8d"
"+8c+8c+4&c+8.g+32a32b-8g+8f+8>c+8<>c+2<g+8f+8f8f+8>c+2<f8f+8g+8b-8b-8.b32b-32g+"
"4b-8g+8d+8c+8c+2b-8g+8f+8>c+8<>c+2<f8f+8g+8f+8<>f4.c+4d+8<b-8>d+8<>d+1r4>d+8d+8"
"d+8d+8d+8d+8d+8d+8d+8d+8d+8d+8f8c+8c+2&c+4<b-8>c+8<b-4g+4f+8g+8b-8d+8d+4>d+8d+8"
"d+8d+8d+8d+8d+4f4f4c+8<b-8g+4.>c+4.<f8f+8f+4g+4b-4r4>r4d+8d+8d+8d+8d+8d+8d+8d+8"
"d+8d+8d+8d+8f8c+8c+2&c+4<b-8>c+8<b-4g+4f+8g+8b-8d+8d+2b-2b-4g+4f4.f+8f+1r2b-4g+"
"4>d+8<r8r4f+4g+4g+4>c+8<r8f4g+4g+4>c+8<r8f4g+4b-4>d+8<r8b-4g+4>d+8<r8r4f+4g+4g+"
"4>c+8<r8f4g+4g+4>c+8<r8f4c+4d+2b-4g+4>d+8<r8r4f+4g+4g+4>c+8<r8f4g+4g+4>c+8<r8f4"
"g+4b-4>d+8<r8b-4g+4>d+8<r8r4f+4g+4g+4>c+8<r8f4g+4g+4>c+8<r8f4c+4d+2b-8g+8d+8c+8"
"c+4&c+8.g+32a32b-8g+8f+8>c+8<>c+2<g+8f+8f8f+8>c+2<f8f+8g+8b-8b-8.b32b-32g+4b-8g"
"+8d+8c+8c+2b-8g+8f+8>c+8<>c+2<f8f+8g+8f+8f4.c+4d+8<b-8>g+8<>f+2b-8g+8d+8c+8c+4&"
"c+8.g+32a32b-8g+8f+8>c+8<>c+2<g+8f+8f8f+8>c+2<f8f+8g+8b-8b-2b-8g+8d+8c+8c+2b-8g"
"+8f+8>c+8<>c+2<f8f+8g+8f+8f4.c+4d+8<b-8>g+8<>f+2b-8g+8d+8c+8c+2b-8g+8f+8>c+8<>c"
"+2<g+8f+8f8f+8>c+2<f8f+8g+8b-8b-2b-8g+8d+8c+8c+2b-8g+8f+8>c+8<>c+2<g+8f+8f8f+8>"
"c+2"
"\",\""
"t120o4r16r16d+16r16g+16r16b-16r16b16r16>d+16r16g+16r16b-16r16<c+16r16d+16r16f16"
"r16g+16r16b16r16>c+16r16g+16r16b-16r16<d+16r16f16r16b-16r16>d+16r16f16r16f+16r1"
"6g+16r16b-16r16<>b-16>c+16<b-16g+16>c+16<b-16g+16f+16r16g+16r16f16r16f+16r16d+1"
"6<r16r16d+16r16g+16r16b-16r16b16r16>d+16r16g+16r16b-16r16<c+16r16d+16r16f16r16g"
"+16r16b16r16>c+16r16g+16r16b-16r16<d+16r16f16r16b-16r16>d+16r16f16r16f+16r16g+1"
"6r16b-16r16<>b-16>c+16<b-16g+16>c+16<b-16g+16f+16r16g+16r16f16r16f+16r16d+16<r1"
"6r16d+16r16g+16r16b-16r16b16r16>d+16r16g+16r16b-16r16<c+16r16d+16r16f16r16g+16r"
"16b16r16>c+16r16g+16r16b-16r16<d+16r16f16r16b-16r16>d+16r16f16r16f+16r16g+16r16"
"b-16r16<r16>>c+16<b-16r16r16b-16r16f+16r16g+16f+16f16r16f+16f16d+16<r16r16d+16r"
"16g+16r16b-16r16b16r16>d+16r16g+16r16b-16r16<c+16r16d+16r16f16r16g+16r16b16r16>"
"c+16r16g+16r16b-16r16<d+16r16f16r16b-16r16>d+16r16f16r16f+16r16g+16r16b-16r16<r"
"16>>c+16<b-16g+16r16b-16g+16f+16r16g+16f+16f16r16f+16f16r16<r16r16d+16r16g+16r1"
"6b-16r16b16r16>d+16r16g+16r16b-16r16<c+16r16d+16r16f16r16g+16r16b16r16>c+16r16g"
"+16r16b-16r16<d+16r16f16r16b-16r16>d+16r16f16r16f+16r16g+16r16b-16r16<r16>>c+16"
"<b-16g+16r16b-16g+16f+16r16g+16f+16f16r16f+16f16d+16<r1r1r1r1r1r1r1r1r1r1r1r1r1"
"r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r"
"1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r16r16d+16r16g+16r16b-16r16b16r1"
"6>d+16r16g+16r16b-16r16<c+16r16d+16r16f16r16g+16r16b16r16>c+16r16g+16r16b-16r16"
"<d+16r16f16r16b-16r16>d+16r16f16r16f+16r16g+16r16b-16r16<>b-16>c+16<b-16g+16>c+"
"16<b-16g+16f+16r16g+16r16f16r16f+16r16d+16<r16r16d+16r16g+16r16b-16r16b16r16>d+"
"16r16g+16r16b-16r16<c+16r16d+16r16f16r16g+16r16b16r16>c+16r16g+16r16b-16r16<"
"\",\""
"t120o4<r16b-16r16b16r16b16r16b16r16b16r16b16r16b16r16b16>r16c+16r16c+16r16c+16r"
"16c+16r16c+16r16c+16r16c+16r16c+16r16r16r16d+16r16d+16r16d+16r16d+16r16d+16r16d"
"+16r16d+16d+16d+16r16r16d+16d+16r16r16d+16d+16r16r16d+16r16d+16d+16<r16b-16r16b"
"16r16b16r16b16r16b16r16b16r16b16r16b16>r16c+16r16c+16r16c+16r16c+16r16c+16r16c+"
"16r16c+16r16c+16r16r16r16d+16r16d+16r16d+16r16d+16r16d+16r16d+16r16d+16d+16d+16"
"r16r16d+16d+16r16r16d+16d+16r16r16d+16r16d+16d+16<r16b-16r16b16r16b16r16b16r16b"
"16r16b16r16b16r16b16>r16c+16r16c+16r16c+16r16c+16r16c+16r16c+16r16c+16r16c+16r1"
"6r16r16d+16r16d+16r16d+16r16d+16r16d+16r16d+16r16d+16d+16d+16r16r16d+16d+16r16r"
"16d+16d+16r16r16d+16r16d+16d+16<r16b-16r16b16r16b16r16b16r16b16r16b16r16b16r16b"
"16>r16c+16r16c+16r16c+16r16c+16r16c+16r16c+16r16c+16r16c+16r16r16r16d+16r16d+16"
"r16d+16r16d+16r16d+16r16d+16r16d+16d+16d+16r16r16d+16d+16r16r16d+16d+16r16r16d+"
"16r16d+16d+16<r16b-16r16b16r16b16r16b16r16b16r16b16r16b16r16b16>r16c+16r16c+16r"
"16c+16r16c+16r16c+16r16c+16r16c+16r16c+16r16r16r16d+16r16d+16r16d+16r16d+16r16d"
"+16r16d+16r16d+16d+16d+16r16r16d+16d+16r16r16d+16d+16r16r16d+16r16d+16d+16r1r1r"
"1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1"
"r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r1r2r16d+16r16r"
"16d+16r16d+16d+16<r16b-16r16b16r16b16r16b16r16b16r16b16r16b16r16b16>r16c+16r16c"
"+16r16c+16r16c+16r16c+16r16c+16r16c+16r16c+16r16r16r16d+16r16d+16r16d+16r16d+16"
"r16d+16r16d+16r16d+16d+16d+16r16r16d+16d+16r16r16d+16d+16r16r16d+16r16d+16d+16<"
"r16b-16r16b16r16b16r16b16r16b16r16b16r16b16r16b16>r16c+16r16c+16r16c+16r16c+16r"
"16c+16r16c+16r16c+16r16c+16r16r16r16d+16r16d+16r16d+16r16d+16r16d+16r16d+16r16d"
"+16d+16d+16r16r16d+16d+16r16r16d+16d+16r16r16d+16r16d+16d+16"
"\",\""
"t120v15o3r4r4r8b-16r16>c+16d+16r16f+16<r4r4r8>d+16r16b16g+16r16f16<r4r4r8r8>f16"
"f+16r16b-16r4r4r8r8>c+16<b-16r16g+16r4r4r8r8r8r8r4r4r8r8r8r8r4r4r8r8r8r8r4r4r8r"
"8>c+16<b-16r16g+16r4r4r8r8r8r8r4r4r8r8r8r8r4r4r8r8r8r8r4r4r8r8>c+16<b-16r16g+16"
"r4r4r8r8r8r8r4r4r8r8r8r8r4r4r8r8r8r8r4r4r8r8>c+16<b-16r16g+16r4r4r8r8r8r8r4r4r8"
"r8r8r8r4r4r8r8r8r8r4r4r8r8>c+16<b-16r16g+16<<f8f8r8f8r8f8r8f8><f8f8r8f8r8f8r8f8"
"><f8f8r8f8r8f8b-8f8><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8r8f8><f8f8"
"r8f8r8f8b-8f8><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8r8f8><f8f8r8f8r8"
"f8b-8f8><f8f8r8f8r4r4><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8b-8f8><f"
"8f8r8f8r4r4><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8b-8f8><f8f8r8f8r4r"
"4><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8b-8f8><f8f8r8f8r4r4><f8f8r8f"
"8r8f8r8f8><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8b-8f8><f8f8r8f8r4r4><f8f8r8f8r8f8r8f8>"
"<f8f8r8f8r8f8r8f8><f8f8r8f8r8f8b-8f8><f8f8r8f8r4r4><f8f8r8f8r8f8r8f8><f8f8r8f8r"
"8f8r8f8><f8f8r8f8r8f8b-8f8><f8f8r8f8r4r4><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8r8f8><f"
"8f8r8f8r8f8b-8f8><f8f8r8f8r4r4><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8r8f8><f8f8r8f8r8f"
"8b-8f8><f8f8r8f8r4r4><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8b-8f8><f8"
"f8r8f8r4r4><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8b-8f8><f8f8r8f8r4r4"
"><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8b-8f8><f8f8r8f8r4r4><f8f8r8f8"
"r8f8r8f8><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8b-8f8><f8f8r8f8r4r4><f8f8r8f8r8f8r8f8><"
"f8f8r8f8r8f8r8f8><f8f8r8f8r8f8b-8f8><f8f8r8f8r4r4><f8f8r8f8r8f8r8f8><f8f8r8f8r8"
"f8r8f8><f8f8r8f8r8f8b-8f8><f8f8r8f8r4r4><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8r8f8><f8"
"f8r8f8r8f8b-8f8><f8f8r8f8r4r4><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8r8f8><f8f8r8f8r8f8"
"b-8f8><f8f8r8f8r4r4>r4r4r8b-16r16>c+16d+16r16f+16<r4r4r8>d+16r16b16g+16r16f16<r"
"4r4r8r8>f16f+16r16b-16r4r4r8r8>c+16<b-16r16g+16r4r4r8r8r8r8r4r4r8r8r8r8r4r4r8r8"
"r8r8r4r4r8r8r8"
"\",\""
"t120o3r8d+8g+8d+8r8d+8r8d+8d+8d+8g+8d+8b-8d+8r8d+8d+8d+8g+8d+8b-8d+8r8d+8d+8d+8"
"g+8d+8b-8d+8r8d+8r8d+8g+8d+8r8d+8r8d+8d+8d+8g+8d+8b-8d+8r8d+8d+8d+8g+8d+8b-8d+8"
"r8d+8d+8d+8g+8d+8b-8d+8r8d+8r8d+8g+8d+8r8d+8r8d+8d+8d+8g+8d+8b-8d+8r8d+8d+8d+8g"
"+8d+8b-8d+8r8d+8d+8d+8g+8d+8b-8d+8r8d+8r8d+8g+8d+8r8d+8r8d+8d+8d+8g+8d+8b-8d+8r"
"8d+8d+8d+8g+8d+8b-8d+8r8d+8d+8d+8g+8d+8b-8d+8r8d+8r8d+8g+8d+8r8d+8r8d+8d+8d+8g+"
"8d+8b-8d+8r8d+8d+8d+8g+8d+8b-8d+8r8d+8d+8d+8g+8d+8b-8d+8r8d+8>>d+4<g+4b-4>d+4<>"
"d+4<g+4b-4>c+4<>c+4<g+4b-4>c+4<>d+4<g+4b-4>f8c+8<>d+4<g+4b-4>d+4<>d+4<g+4b-4>c+"
"4<>c+4<g+4b-4>c+4<>d+4<g+4b-4>f8c+8<>d+4<g+4b-4>d+4<>d+4<g+4b-4>c+4<>c+4<g+4b-4"
">c+4<>d+4<g+4r4r4<>>d+4<g+4b-4>d+4<>d+4<g+4b-4>c+4<>c+4<g+4b-4>c+4<>d+4<g+4b-4>"
"f8c+8<<>>d+4<g+4b-4>d+4<>d+4<g+4b-4>c+4<>c+4<g+4b-4>c+4<>d+4<g+4b-4>f8c+8<<>>d+"
"4<g+4b-4>d+4<>d+4<g+4b-4>c+4<>c+4<g+4b-4>c+4<>d+4<g+4b-4>f8c+8<<>>d+4<g+4b-4>d+"
"4<>d+4<g+4b-4>c+4<>c+4<g+4b-4>c+4<>d+4<g+4b-4>f8c+8<<>>d+4<g+4b-4>d+4<>d+4<g+4b"
"-4>c+4<>c+4<g+4b-4>c+4<>d+4<g+4b-4>f8c+8<<>>d+4<g+4b-4>d+4<>d+4<g+4b-4>c+4<>c+4"
"<g+4b-4>c+4<>d+4<g+4b-4>f8c+8<<>>d+4<g+4b-4>d+4<>d+4<g+4b-4>c+4<>c+4<g+4b-4>c+4"
"<>d+4<g+4b-4>f8c+8<<>>d+4<g+4b-4>d+4<>d+4<g+4b-4>c+4<>c+4<g+4b-4>c+4<>d+4<g+4b-"
"4>f8c+8<<>>d+4<g+4b-4>d+4<>d+4<g+4b-4>c+4<>c+4<g+4b-4>c+4<>d+4<g+4b-4>f8c+8<<>>"
"d+4<g+4b-4>d+4<>d+4<g+4b-4>c+4<>c+4<g+4b-4>c+4<>d+4<g+4b-4>f8c+8<<>>d+4<g+4b-4>"
"d+4<>d+4<g+4b-4>c+4<>c+4<g+4b-4>c+4<>d+4<g+4b-4>f8c+8<<>>d+4<g+4b-4>d+4<>d+4<g+"
"4b-4>c+4<>c+4<g+4b-4>c+4<>d+4<g+4b-4>f8c+8<<>>d+4<g+4b-4>d+4<>d+4<g+4b-4>c+4<>c"
"+4<g+4b-4>c+4<>d+4<g+4b-4>f8c+8<<>>d+4<g+4b-4>d+4<>d+4<g+4b-4>c+4<>c+4<g+4b-4>c"
"+4<>d+4<g+4b-4>f8c+8<<>>d+4<g+4b-4>d+4<>d+4<g+4b-4>c+4<>c+4<g+4b-4>c+4<>d+4<g+4"
"b-4>f8c+8<<>>d+4<g+4b-4>d+4<>d+4<g+4b-4>c+4<>c+4<g+4b-4>c+4<>d+4<g+4r8<d+8r8d+8"
"r8d+8g+8d+8r8d+8r8d+8d+8d+8g+8d+8b-8d+8r8d+8d+8d+8g+8d+8b-8d+8r8d+8d+8d+8g+8d+8"
"b-8d+8r8d+8r8d+8g+8d+8r8d+8r8d+8d+8d+8g+8d+8b-8d+8r8d+8d+8d+8g+8d+8b-8d+8r8d+8"
"\",\"\""
/*
"t120o3r1r1r1r1r1r1r1r1<f+4>d+8<r8r4>d+4<f+4>d+8<r8r4>d+4<f+4>d+8<r8r4>d+4<f+4>d"
"+8<r8r4>d+4<f+4>d+8<r8f+4>d+4<f+4>d+8<r8f+8f+8>d+8d+16d+16<f+4>d+4<r4>d+4<f+4>d"
"+4<r4>d+4<f+4>d+8<r8f+4>d+4<f+4>d+8<r8f+8f+8>d+8d+16d+16<f+4>d+4<r4>d+4<f+4>d+4"
"<>e16e16e16e16e16e16e16e16<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e"
"16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16e16e16e16"
"e16e16e16e16<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r"
"8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8r8e16r16r16e8e16<>e16r16r8"
"e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16"
"r16r8e16r16r8<>e16r16r8e16r16r8r8e16r16r16e8e16<>e16r16r8e16r16r8e16r16r8e16r16"
"r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r"
"8e16r16r8r8e16r16r16e8e16<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e1"
"6r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8r8e16r16r16e8"
"e16<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16"
"r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8r8e16r16r16e8e16<>e16r16r8e16r16r8e"
"16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r"
"16r8<>e16r16r8e16r16r8r8e16r16r16e8e16<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r1"
"6r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8"
"r8e16r16r16e8e16<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16"
"r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8r8e16r16r16e8e16<>e16r"
"16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r"
"8e16r16r8e16r16r8<>e16r16r8e16r16r8r8e16r16r16e8e16<>e16r16r8e16r16r8e16r16r8e1"
"6r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16"
"r16r8e16r16r8r8e16r16r16e8e16<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16"
"r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8r8e16r16r"
"16e8e16<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e1"
"6r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8r8e16r16r16e8e16<>e16r16r8e16r1"
"6r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8"
"e16r16r8<>e16r16r8e16r16r8r8e16r16r16e8e16<>e16r16r8e16r16r8e16r16r8e16r16r8<>e"
"16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r"
"16r8r8e16r16r16e8e16<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r"
"8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8r8e16r16r16e8e16<>"
"e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16"
"r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8r8e16r16r16e8e16<>e16r16r8e16r16r8e16r16"
"r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<"
">e16r16r8e16r16r8r8e16r16r16e8e16<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e1"
"6r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8e16r16r8e16r16r8<>e16r16r8e16r16r8r4r4<"
"\""
*/
// ------------------------------------------------------------------------------------
;
// EEPROM Address
#define VER_CODE 'A'
#define EEP_VERSION  0   //
#define EEP_PLAYED 1     // 直前まで演奏していたTRACK番号
#define EEP_AUTOSLEEP 2  // ポーズからスリープに遷移する時間(msec)
#define EEP_AUTOLOOP 4   // 1曲終わった時の処理
#define EEP_INTERVAL 5   // NEXT、RND の時次曲再生までの待ち時間

EnvelopeParam env_param;
word p  ;  // MMLポインタ

#define LOOP_ENABLE    // X,Y のループ処理を使う(300byteほど使う)
#define MAX_CHANNEL 6

#ifdef SD_MODE
File myFile;
#else
// word ymtrack[MAX_TRACK+1] ;    // MML の各トラックの先頭
word trackHead , trackSize ;
byte playing ;                 // 再生中の曲の番号 (0開始)
#define STARTPLAY 0            // 開始時に流れる曲 (
#endif

boolean mmlPauseFlag ;      // 再生のポーズフラグ（外部から指示可能）
int mmlSpeed ;              // 再生の速度(Tの値に加減される/外部から指示可能)

word ymbegin[MAX_CHANNEL] ;    // MML 開始地点
word ymfinish[MAX_CHANNEL] ;   // MML 終了地点
word ympointer[MAX_CHANNEL] ;  // MML 再生地点
byte ymoct[MAX_CHANNEL] ;      // デフォルトオクターブ
word ymtempo[MAX_CHANNEL] ;    // テンポ(BPM)
word ymorgtmp[MAX_CHANNEL] ;    // オリジナルのテンポ(BPM)
byte ymnote[MAX_CHANNEL] ;     // 直前のノート番号
byte ymonpu[MAX_CHANNEL] ;     // デフォルトの音符長
byte ymvol[MAX_CHANNEL] ;      // デフォルトの音量
byte ymmask[MAX_CHANNEL] ;     // トラックの演奏マスク
unsigned long ymstart[MAX_CHANNEL] ;  // 再生開始時間(ms)
unsigned long ymend[MAX_CHANNEL] ;    // 再生終了時間(ms)
unsigned long ymmid[MAX_CHANNEL] ;    // 再生途中の区切り時間(ms)
unsigned long mmlBegin ;
unsigned long mmlPlay ;
unsigned long mmlLast ;
// byte ch7buf ;         // マスクをかけるチャンネルの保持情報
boolean timeover ;    // 通常処理が間に合わなかった場合のフラグ
char ymKeyShift ;     // ノートキーのシフト値
byte autoloopset ;    // 自動ループ設定(初期)
byte autoloopnow ;    // 自動ループ設定(現状)
#ifdef SD_MODE
word ymFilePos[MAX_CHANNEL] ;  // SD カードの読み出しポインタ
#define TEMPOLED 8
#else
#define TEMPOLED 8
//#define TEMPOLED 13
#endif
#ifdef LOOP_ENABLE
unsigned long  ymmark[MAX_CHANNEL] ;     // ループ用のマーク地点
char ymloop[MAX_CHANNEL] ;     // ループ用のカウンタ
#endif
byte portinput ;      // PORT D の入力状態
byte lastport ;       // 前インターの入力
byte mainseq ;        // メインのシーケンス
byte bootplay ;       // 電源ON時の再生の有無 (0再生しない・指定曲 1再生する・指定曲 2再生しない・ランダム曲 3再生する・ランダム曲)
byte runswitch ;      // 曲を演奏するスイッチ状態(0トリガ 1押下再生 2解放再生)
byte pausemode ;      // ポーズに関する定義 (b0=0復旧時継続 1復旧で最初から)
unsigned long pausetimer ;
boolean tempoLed ;    // テンポ用ＬＥＤのピン番号
unsigned long tempoledtimer ;
word autoSleepMillis ;    // ポーズ状態からスリープへ遷移するまでの時間
//--------------------------------------------------------
void HandleNoteOn(byte channel, byte pitch, byte velocity) { 
  if( velocity == 0 || ymmask[channel-1]!=0 ) {
#ifdef NOTE_PORTC
    PORTC |= (0b00000001)<<(channel-1) ;
#endif
    PWM_SYNTH.noteOff(channel,pitch,velocity);
    return;
  }
#ifdef NOTE_PORTC
  PORTC &= (0b1111110)<<(channel-1) ;
#endif
  PWM_SYNTH.noteOn(channel,pitch,velocity);
}
//--------------------------------------------------------
void setup() {
#ifdef USE_SERIAL
  Serial.begin(9600) ;
#endif
  DDRC |= 0B00111111;  // PORTC (A0～A5)をデジタル出力として定義
  DDRD &= 0B00000011 ; // PORTD (D2～D7)をデジタル入力として定義
  PORTD |= 0B11111100 ;  // PORTD D2～D7 プルアップ
  DDRB = 0B00101111 ;  // PORTB (D8～D13)をデジタル入出力として定義
  pinMode(TEMPOLED,OUTPUT) ;  // テンポ LED
  tempoLed = true ;
  digitalWrite(TEMPOLED,tempoLed) ;
#ifdef SD_MODE
  if (!SD.begin(SD_CS)) {
    Serial.println("initialization failed!");
    return;
  }
#endif

#ifdef USE_SERIAL
  Serial.println("initialization done.");
#endif
  // 設定確認
  if (EEPROM.read(EEP_VERSION) != VER_CODE) {
    byte systemDefault[] = { VER_CODE,    // バージョンコード
      STARTPLAY,                          // 演奏していた曲番
      (AUTOSLEEPTIME & 0xff) ,            // ポーズからスリープに遷移する時間
      ((AUTOSLEEPTIME>>8) & 0xff) ,
      1 ,                                 // １曲終わった後の処理 (0停止 1リピート 2次曲 3ランダム)
      (INTERVALTIME & 0xff) ,             // 曲間の待ち時間
      ((INTERVALTIME>>8) & 0xff)
    } ;
    for (byte i=0 ; i<7 ; i++) {
      EEPROM.write(i,systemDefault[i]) ;
    }
  }
  playing = EEPROM.read(EEP_PLAYED) ;    // 再生中の曲番号
  autoSleepMillis = readWordEEPROM(EEP_AUTOSLEEP) ;  // スリープ遷移の時間
  set_sleep_mode(SLEEP_MODE_PWR_DOWN) ;    // SLEEPMODE の設定

//  pinMode( 2, INPUT_PULLUP) ;              // スリープからの復帰用(立下りエッヂで復帰)
/*
  available() は現在の位置から何バイト読み込めるか返します。
  available() の戻り値は int 型で読み込めるバイト数が32767を超える場合、
  戻り値は常に32767になります。
  
  ファイルのサイズは size() で取得できます。

  read() は1byte読み出します。read(buffer, length) とした場合、
  lengthバイト分のデータをbufferに読み込みます。

  position() は現在の読み込み位置を返します。
  seek() で読み込み位置を移動できます。

*/
  ymKeyShift = 0 ;
  PWM_SYNTH.setup();
  reset_ADSR () ;

  // 波形の定義
  // PWM_SYNTH.setWave( PWMDACSynth::sineWavetable ) ;  // すべてのMIDIチャンネルに波形をセット
  // PWM_SYNTH.setWave( PWMDACSynth::squareWavetable ) ;
  // PWM_SYNTH.setWave( PWMDACSynth::triangleWavetable ) ;
  // PWM_SYNTH.setWave( PWMDACSynth::shepardToneSineWavetable ) ;
  // 指定のMIDIチャンネルに波形をセット
  // PWM_SYNTH.getChannel(1)->wavetable = PWMDACSynth::sawtoothWavetable ;
  // ・sineWavetable[]    正弦波
  // ・squareWavetable[]  矩形波
  // ・triangleWavetable[]三角波
  // ・sawtoothWavetable[]のこぎり波
  // ・shepardToneSineWavetable[]	正弦波の無限音階（シェパードトーン）
    
  // ＡＤＳＲの定義
  // unsigned int attack_speed = 0x8000;
  // byte decay_time = 7 ;
  // unsigned int sustain_level = 0x8000 ;
  // byte release_time = 7 ;  // 大きいほど余韻が長い
  /*
  env_param.attack_speed = 0x8000 ; 
  env_param.decay_time = 7;
  env_param.sustain_level = 0x8000;
  env_param.release_time = 6 ;
  PWM_SYNTH.setEnvelope(env_param) ;  // 全ch同時
  // PWM_SYNTH.getChannel(1)->env_param = env_param ;  // 1ch のみ
  */
  // MML
#ifdef SD_MODE
  mml_init_SD() ;
#else
  // track_init() ;
  mml_init() ;
#endif
  mainseq = 0 ;
  bootplay = 0 ;  // b0=0停止1再生、b1=0指定曲2ランダム曲
  mmlSpeed = 0 ;
  pausemode = 0 ;
  mmlPauseFlag = bootplay & 1 ;  
  autoloopnow = autoloopset = EEPROM.read(EEP_AUTOLOOP) ; // 1曲終わった時の自動ループ設定 (0しない 1同じ曲 2次の曲 3ランダム)
  runswitch = 0 ;  // 曲を演奏するスイッチ状態(0トリガ 1押下再生 2解放再生)
}
//--------------------------------------------------------
void loop() {
#ifdef SD_MODE
  if (parseMML()==false) {
    if (autoloopnow > 0) {    // 再生する場合
      if (autoloopnow==2) {    // 次の曲
        playing ++ ;
        if (playing>=MAX_TRACK) {
          playing = 0 ;
        }
      } else if (autoloopnow==3) {    // ランダム
        playing = micros() % MAX_TRACK ; 
      }
//      delay(readWordEEPROM(EEP_INTERVAL)) ;
      note_alloff() ;
      mml_init_SD() ;
    } else {
#ifdef NOTE_PORTC
      PORTC = 0x00 ;
#endif
      sleepAndWakeup();
    }
  }
#else
  if (parseMML()==false) {
    if (autoloopnow > 0) {    // 再生する場合
      if (autoloopnow==2) {    // 次の曲
        playing ++ ;
        if (playing>=MAX_TRACK) {
          playing = 0 ;
        }
      } else if (autoloopnow==3) {    // ランダム
        playing = micros() % MAX_TRACK ; 
      }
      note_alloff() ;
      unsigned long waitloop = millis() + /*readWordEEPROM(EEP_INTERVAL)*/ 1000 ;
      while (waitloop > millis()) {
        PWM_SYNTH.updateEnvelopeStatus();
      }
      mml_init() ;
    } else {
#ifdef NOTE_PORTC
      PORTC = 0x00 ;
#endif
      sleepAndWakeup();
    }
  }
#endif
  PWM_SYNTH.updateEnvelopeStatus();

  // スイッチ入力
  lastport = portinput ;
  portinput = PIND ; 
  switch(mainseq) {
    case 0:  // 起動直後の条件分岐
      if (runswitch==1) mainseq = 50 ;
      else if (runswitch==2) mainseq = 60 ;
      else mainseq = 10 ;
      pausetimer = millis() ;
      break ;
    case 10:    // トリガ(ボタンを押すとRUN/STOP)
      if (mmlPauseFlag) {  // ボタンが押され続けている状態でのスリープ遷移
        if ((millis() - pausetimer) > autoSleepMillis) {  // 5 SEC 以上ポーズ
            sleepAndWakeup();
            break ;
          }
      }
      if ((PIND & 4) != 0) mainseq = 15 ;  // ボタンリリースの確認
      break ;
    case 15:    // ボタンリリース状態
      if (mmlPauseFlag) {  // ボタンが離された状態でのスリープ遷移
        if ((millis() - pausetimer) > autoSleepMillis) {  // 5 SEC 以上ポーズ
            sleepAndWakeup();
            break ;
          }
      }
      if ((PIND & 4) == 0) mainseq = 20 ;  // ボタンONの確認
      break ;
    case 20:    // ボタンオン
      mmlPauseFlag = !mmlPauseFlag ;
      if (mmlPauseFlag) {  // PAUSE に入った時
        digitalWrite(TEMPOLED,HIGH) ;  // TEMPO LED 点灯
        note_alloff() ;
        pausetimer = millis() ;
      } else {            // PASUE から戻る時
        if ((pausemode & 1)!=0) {  // 復帰時に最初からの場合
#ifdef SD_MODE
          mml_init_SD() ;
#else
          mml_init() ;
#endif
        }
      }
      mainseq = 10 ;
      break ;
    case 50:    // ボタンを押している間だけ再生
    case 60:    // ボタンを離している間だけ再生
      mmlPauseFlag = true ;
      if (mmlPauseFlag) {
          if ((millis() - pausetimer) > autoSleepMillis) {  // 5 SEC 以上ポーズ
            sleepAndWakeup();  
            break ;
          }
      }
      if (mainseq==50 && (PIND & 4) == 0
       || mainseq==60 && (PIND & 4) != 0) {
        if ((pausemode & 1)!=0) {  // 復帰時に最初からの場合
#ifdef SD_MODE
          mml_init_SD() ;
#else
          mml_init() ;
#endif
        }
        mmlPauseFlag = false ;
        mainseq += 2 ;
      }
      break ;
    case 52:    // 
    case 62:    // 
      if (mainseq==52 &&(PIND & 4) != 0
       || mainseq==62 &&(PIND & 4) == 0 ){
        digitalWrite(TEMPOLED,HIGH) ;  // TEMPO LED 点灯
        note_alloff() ;
        pausetimer = millis() ;
        mainseq -= 2 ;
      }
      break ;
  }
  if ((portinput & 8) == 0 && (lastport & 8) != 0) {  // UP PUSH  
    playing ++ ;
    if (playing >= MAX_TRACK) playing = 0 ;
    note_alloff() ;
    reset_ADSR () ;
#ifdef SD_MODE
    mml_init_SD() ;
#else
    mml_init() ;
#endif
  }
  if ((portinput & 16) == 0 && (lastport & 16) != 0) {  // DOWN PUSH  
    if (playing == 0) {
      playing = MAX_TRACK - 1 ;
    } else playing-- ;
    note_alloff() ;
    reset_ADSR () ;
#ifdef SD_MODE
    mml_init_SD() ;
#else
    mml_init() ;
#endif
  }


#ifdef USE_SERIAL
  if (Serial.available()) {
    byte r=Serial.read() ;
    switch (r) {
    case 'p':
      mmlPauseFlag = !mmlPauseFlag ;
      if (mmlPauseFlag) {
        digitalWrite(TEMPOLED,HIGH) ;  // TEMPO LED 点灯
        note_alloff() ;
        if ((pausemode & 1)!=0) {  // 復帰時に最初からの場合
#ifdef SD_MODE
          mml_init_SD() ;
#else
          mml_init() ;
#endif
        }
      }        
      break ;
    case 'l':
      autoloopnow ++ ;
      if (autoloopnow >= 4) autoloopnow = 0 ;
      Serial.println(autoloopnow) ;      
      break ;

    case 's':
      autoloopnow = autoloopset ;
#ifdef SD_MODE
      mml_init_SD() ;
#else
      mml_init() ;
#endif    
      note_alloff() ;
      break ;
    case 'e':
      note_alloff() ;
      reset_ADSR () ;
      ymKeyShift = 0 ;
      for (r=0 ; r<MAX_CHANNEL ; r++) {
        ymtempo[r] = ymorgtmp[r] ;
      }
      break ;
    case '>':
      ymKeyShift ++ ;
      if (ymKeyShift > 12) ymKeyShift = 12 ; 
      break ;
    case '<':
      ymKeyShift -- ;
      if (ymKeyShift < -12) ymKeyShift = -12 ; 
      break ;
    case '-':
      for (r=0 ; r<MAX_CHANNEL ; r++) {
        ymtempo[r] -= 10 ;
        if (ymtempo[r] < 30) ymtempo[r] +=10 ;
      }
      break ;
    case '+':
      for (r=0 ; r<MAX_CHANNEL ; r++) {
        ymtempo[r] += 10 ;
        if (ymtempo[r] > 300) ymtempo[r] -=10 ;
      }
      break ;
    case 'd':
      sleepAndWakeup();
      break ;
    case 'A':
      PWM_SYNTH.setWave( PWMDACSynth::sineWavetable ) ;  // すべてのMIDIチャンネルに波形をセット
      break ;
    case 'B':
      PWM_SYNTH.setWave( PWMDACSynth::squareWavetable ) ;
      break ;
    case 'C':
      PWM_SYNTH.setWave( PWMDACSynth::triangleWavetable ) ;
      break ;
    case 'D':
      PWM_SYNTH.setWave( PWMDACSynth::sawtoothWavetable ) ;
      break ;
    case 'E':
      PWM_SYNTH.setWave( PWMDACSynth::shepardToneSineWavetable ) ;
      break ;
    case '!':
    case '"':
    case '#':
    case '$':
    case '%':
    case '&':
      {
        byte p = r - '!' ;
        ymmask[p] = 1-ymmask[p] ;
      }
      break ;
      
      
    default:
      reset_ADSR () ;
      byte p = r - '0' ;
      if (p>=0 && p<MAX_TRACK) {
        playing = p ;
#ifdef SD_MODE
        mml_init_SD() ;
#else
        mml_init() ;
#endif    
        note_alloff() ;
      }
      break ;      
    }    
  }
#endif
}
//--------------------------------------------------------
void reset_ADSR() {
  PWM_SYNTH.setWave( PWMDACSynth::squareWavetable ) ;  // 方形波に初期化
  env_param.attack_speed = 0x8000 ;                    // ADSR 値を初期化
  env_param.decay_time = 7;
  env_param.sustain_level = 0x8000;
  env_param.release_time = 5 ;
  PWM_SYNTH.setEnvelope(env_param) ;  // 全ch同時
}
//--------------------------------------------------------
void note_alloff() {
  for (byte i=0 ; i<MAX_CHANNEL ; i++) {
    for (byte j=0 ; j<127 ; j++) {
      HandleNoteOn(i+1, j, 0) ; 
    }
  }
}
//--------------------------------------------------------
byte getmml_char(word pos) {
#ifdef SD_MODE
  word prepos = myFile.position() ;
  myFile.seek(pos) ;
  byte c = myFile.read() ;
  myFile.seek(prepos) ;
  return c ;
#else
  return pgm_read_byte_near(mmlstr + pos) ;
#endif
}
//--------------------------------------------------------
void reset_mmlparam() {
  for (byte i=0 ; i<MAX_CHANNEL ; i++) {  
    ymoct[i] = 4 ;
    ymbegin[i] = 0 ;
    ymfinish[i] = 0 ;
    ymmid[i] = 0 ;
    ymtempo[i] = ymorgtmp[i] = 120 ;
    ymonpu[i] = 4 ;
    ympointer[i] = 0 ;
    ymmask[i] = 0 ;
#ifdef LOOP_ENABLE    
    ymloop[i] = 0 ;
    ymmark[i] = 0 ;
#endif
  }
}
//--------------------------------------------------------
#ifdef SD_MODE
int mml_init_SD() {
  int i , w , c , s ;
  char m ;
  reset_mmlparam() ;
  myFile = SD.open("test.txt", FILE_READ);
  if (!myFile) {  // ファイル開けない場合
    Serial.println("Can't read file.");
    return -1 ;
  }
  w = myFile.size() ;   // ファイルサイズの取得
#ifdef DEBUG_MODE  
  Serial.print("file size=");
  Serial.println(w);
#endif
  s = 0 ;
  c = 0 ;
  for (i=0 ; i<w ; i++) {
    m = myFile.read() ;
    switch (s) {
    case 0:  // 先頭の " 検出
      if (m == '"') {
#ifdef DEBUG_MODE
        Serial.print("Parse ch(") ;
        Serial.print(c) ;
        Serial.print(" / ") ;
        Serial.print(i+1) ;
#endif
        ymbegin[c] = i+1 ;
        s = 1 ;
      }
      break ;
    case 1:  // 文末の " 検出
      if (m == '"') {
#ifdef DEBUG_MODE
        Serial.print(i+1) ;
        Serial.println(")") ;
#endif
        ymfinish[c] = i ;
        // ch7buf &= (0b11111110)<<c ;  // 使うch有効に
        s = 2 ;
      }
      break ;
    case 2:  // , 検出
      if (m == ',') {
        c++ ;
        if (c>MAX_CHANNEL-1) i = w ;  // for 脱出用
        s = 0 ;
      }
      break ;
    }
  }
  unsigned long ms = micros() ;
  mmlLast = mmlBegin = ms ;
  mmlPlay = 0 ;
  for (i=0 ; i<MAX_CHANNEL ; i++) {  
    ymstart[i] = ms ;
    ymmid[i] = ymend[i] = ms ;
  }
  timeover =  false ;
  tempoledtimer = micros() ;
  tempoLed = false ;
  return c ;
}
#else
//--------------------------------------------------------
// MML 関連処理の初期化(各chの情報リセット)
// 使用するチャンネル数を返す
int mml_init() {
#ifdef DEBUG_MODE
        Serial.print("playing = ") ;
        Serial.println(playing) ;
#endif
  int il , w , c , s ;
  char m ;
  reset_mmlparam() ;
  w = strlen(mmlstr) ;    // サイズ
#ifdef DEBUG_MODE
  Serial.print("ALL SIZE = ") ;
  Serial.println(w) ;
#endif
  if (playing >= MAX_TRACK) playing = MAX_TRACK-1 ;  // 最大トラック以下(MAX_TRACK は 1以上であること)
  EEPROM.write(EEP_PLAYED,playing) ;
  
  if (playing==0) {  // 1 曲目
    trackHead = 0 ;
    for (il=0 ; il<w ; il++) {
      if (getmml_char(il) == '!') {    // トラックの区切り
        break ;
      }
    }
    trackSize = il ;
  } else {        // 2 曲目以降
    byte u = playing ;
    byte v = 0 ;    
    for (il=0 ; il<w ; il++) {
      if (getmml_char(il) == '!') {    // トラックの区切り
        if (v==0) {
          u-- ;
          if (u<=0) {
            trackHead = il+1 ;
            v++ ;
          }
        } else {
          break ;
        }
      }
    }
    trackSize = il - trackHead ;
  }
#ifdef DEBUG_MODE
  Serial.print("TrackHead=") ;
  Serial.println(trackHead) ;
  Serial.print("TrackSize=") ;
  Serial.println(trackSize) ;
#endif
  w = trackSize ;
  // チャンネルの設定
  s = 0 ;
  c = 0 ;
  for (il=0 ; il<w ; il++) {
    m = getmml_char(il+trackHead) ;
    switch (s) {
    case 0:  // 先頭の " 検出
      if (m == '"') {
#ifdef DEBUG_MODE
        Serial.print("CH") ;
        Serial.print(c) ;
        Serial.print("(") ;
        Serial.print(il+trackHead) ;
        Serial.print("'") ;
        for (byte j=0 ; j<3 ; j++) {
          byte k = getmml_char(il + j + trackHead) ;
          Serial.write(k) ;
        }
        Serial.print("'") ;
#endif
        ymbegin[c] = il+1 ;
        s = 1 ;

      } else {
        Serial.write(m) ;

      }
      
      break ;
    case 1:  // 文末の " 検出
      if (m == '"') {
#ifdef DEBUG_MODE
        Serial.print(" - ") ;
        Serial.print(il + trackHead) ;
        Serial.println(")") ;
#endif
        ymfinish[c] = il-1 ;
        // ch7buf &= (0b11111110)<<c ;  // 使うch有効に
        s = 2 ;
      }
      break ;
    case 2:  // , 検出
      if (m == ',') {
#ifdef DEBUG_MODE
        Serial.print("-ch") ;
        Serial.print(c) ;
        Serial.println(" end") ;
#endif
        c++ ;
        if (c>=MAX_CHANNEL) il = w ;  // for 脱出用
        s = 0 ;
      }
      break ;
    }
  }
  unsigned long ms = micros() ;
  mmlLast = mmlBegin = ms ;
  mmlPlay = 0 ;
  for (il=0 ; il<MAX_CHANNEL ; il++) {  
    ymstart[il] = ms ;
    ymmid[il] = ymend[il] = ms ;
  }
  timeover =  false ;
  tempoledtimer = micros() ;
  tempoLed = false ;
  return c ;
}
#endif
//--------------------------------------------------------
byte getNoteNum (byte o,byte c) {
  if (c>=128) return 128 ;
  return (byte) ((o + 1) * 12 + c + ymKeyShift) ;
}
//--------------------------------------------------------
#ifdef DEBUG_MODE
void noteToStr(byte n) {
  if (n==128) {Serial.print("R") ; return ;}
  byte o = n%12 ;
  switch (o) {
  case 0:    Serial.print("C") ; break ;
  case 1:    Serial.print("C+") ; break ;
  case 2:    Serial.print("D") ; break ;
  case 3:    Serial.print("D+") ; break ;
  case 4:    Serial.print("E") ; break ;
  case 5:    Serial.print("F") ; break ;
  case 6:    Serial.print("F+") ; break ;
  case 7:    Serial.print("G") ; break ;
  case 8:    Serial.print("G+") ; break ;
  case 9:    Serial.print("A") ; break ;
  case 10:    Serial.print("A+") ; break ;
  case 11:    Serial.print("B") ; break ;
  }
}
#endif
//--------------------------------------------------------
boolean parseMML () {
  byte n ;
  char lastCmd = 0 ;  // 有効な MML を解析中 0:NO 1:音程 2:音階 3:テンポ 4:デフォルト音符 5:ボリューム
  word parseNum ;     // パース中に取得した値
  boolean futen ;     // 付点の検出
  boolean minus ;     // 一部のコマンド用のマイナス符号
  boolean quote ;     // 有効な MML を検出した
  boolean onplay = false ;
  byte c ;
  byte q ;
  boolean loopflag ;
  byte ch ;
  unsigned long of;
  unsigned long st,ed,df ;
  boolean slah ;     // スラーフラグ
  unsigned long ms = micros() ;    // 現在の時間を取得

  // 再生のポーズがかかっている場合
  if (mmlPauseFlag) {
    df = ms - mmlLast ;
    for (ch=0 ; ch<MAX_CHANNEL ; ch++) {
      if (ymfinish[ch] == 0) continue ;  // MML 処理が終わっている場合は次チャンネル
      onplay = true ;
      ymend[ch] += df ;
      ymmid[ch] += df ;    
      ymstart[ch] += df ;
    }
    mmlLast = ms ;
    if (onplay) {    // 再生時間の更新(microSec)
      mmlBegin = ms - mmlPlay ; 
    }
    tempoledtimer += df ;
    return onplay ;
  }
  // テンポ LED
  if (ms >= tempoledtimer) {
    df = (unsigned long)(ymtempo[0] + mmlSpeed) ;
    of = 240000000L / ((unsigned long)8 * df) ; // 8分音符あたりのマイクロ秒
    df = (ms - tempoledtimer) ;              // 前のENDから超過した時間
    st = ms - df ;                       // 次の開始時間
    ed = st + of ;                       // 次の終了時間
    if (ed >= ms) {                      // オーバーしていない場合
      tempoledtimer = ed ;
    }
    tempoLed = !tempoLed ;
    digitalWrite(TEMPOLED,tempoLed) ;
  }
  // 各チャンネルの処理本体
  for (ch=0 ; ch<MAX_CHANNEL ; ch++) {
    if (ymfinish[ch] == 0) continue ;  // MML 処理が終わっている場合は次チャンネル
    onplay = true ;
    if (ms < ymend[ch]) {
      continue ;     // 音符再生が終わっていない場合も次チャンネル
    }
    p = ympointer[ch] ;                // 次の MML を取得
    if (ymfinish[ch] < ymbegin[ch] + p) {  // 対象の MMML の終了地点を通過した場合
      // ch7buf |= (0b00000001)<<ch ;    // 対応するチャンネルを消音
      ymfinish[ch] = 0 ;              // MML 解析が終わっている事を記録
      continue ;                      // 次チャンネル
    }
    // 再生
    n = 128 ;            // キー（デフォルト）
    q = 0 ;
    lastCmd = 0 ;        // 解析した処理系 0未 1音符 
    parseNum = 0 ;
    futen = false ;
    minus = false ;
    quote = false ;      // すでに一つの命令を解釈した後か
    loopflag = true ;
    slah = false ;

#ifdef SD_MODE
    myFile.seek(ymbegin[ch] + p) ;    // SD 読み出し
#endif
    while(loopflag) {
#ifdef SD_MODE
      if (q==0) {
        c = myFile.read() ;          // SD 読み出し
      } else {
        c = q ;
        q = 0 ;
      }
      if (c == '/' && getmml_char(ymbegin[ch] + p + 1) == '*') {
        p = findCommentEnd (ch,p) ;
        c = getmml_char(ymbegin[ch] + p) ;
      }
#else
      c = getmml_char(ymbegin[ch] + p + trackHead) ;  // 文字列から一文字取得
      if (c == '/' && getmml_char(ymbegin[ch] + p + 1 + trackHead) == '*') {
        p = findCommentEnd (ch,p) ;
        c = getmml_char(ymbegin[ch] + p + trackHead) ;
      }
#endif
      // Serial.write(c) ;
      switch (c) {
      case 'P':
        if (lastCmd==0) {
          lastCmd = 19 ;
        } 
        else quote = true ;
        break ;
      case '@':
        if (lastCmd==0) {
          lastCmd = 18 ;
        } 
        else quote = true ;
        break ;
      case '&':
        if (lastCmd==0) {
          slah = true ;
        } else {
          slah = false ;
          quote = true ;
        }
        break ;
      case 'c':
      case 'C':
        if (lastCmd==0) {
          n = 0 ;
          lastCmd = 1 ;
        } 
        else quote = true ;
        break ;
      case 'd':
      case 'D':
        if (lastCmd==0) {
          n = 2 ;
          lastCmd = 1 ;
        } 
        else quote = true ;
        break ;
      case 'e':
      case 'E':
        if (lastCmd==0) {
          n = 4 ;
          lastCmd = 1 ;
        } 
        else quote = true ;
        break ;
      case 'f':
      case 'F':
        if (lastCmd==0) {
          n = 5 ;
          lastCmd = 1 ;
        } 
        else quote = true ;
        break ;
      case 'j':
      case 'J':
        if (lastCmd==0) {
          n = 6 ;
          lastCmd = 1 ;
        } 
        else quote = true ;
        break ;
      case 'g':
      case 'G':
        if (lastCmd==0) {
          n = 7 ;
          lastCmd = 1 ;
        } 
        else quote = true ;
        break ;
      case 'a':
      case 'A':
        if (lastCmd==0) {
          n = 9 ;
          lastCmd = 1 ;
        } 
        else quote = true ;
        break ;
      case 'b':
      case 'B':
        if (lastCmd==0) {
          n = 11 ;
          lastCmd = 1 ;
        } 
        else quote = true ;
        break ;
      case 'r':
      case 'R':
        if (lastCmd==0) {
          n = 128 ;
          lastCmd = 1 ;
        } 
        else quote = true ;
        break ;
      case 'h':
      case 'H':
        if (lastCmd==0) {
          lastCmd = 11 ;
        } 
        else quote = true ;
        break ;
      case 'n':
      case 'N':
        if (lastCmd==0) {
          lastCmd = 12 ;
        } 
        else quote = true ;
        break ;
      case 'u':
      case 'U':
        if (lastCmd==0) {
          lastCmd = 13 ;
        } 
        else quote = true ;
        break ;
      case 'k':
      case 'K':
        if (lastCmd==0) {
          lastCmd = 14 ;
        } 
        else quote = true ;
        break ;
#ifdef LOOP_ENABLE    
      case 'X':
        if (lastCmd==0) {
          lastCmd = 15 ;
        } 
        else quote = true ;
        break ;
      case 'Y':
        if (lastCmd==0) {
          lastCmd = 16 ;
        } 
        else quote = true ;
        break ;
#endif
      case 'q':
      case 'Q':
        if (lastCmd==0) {
          lastCmd = 17 ;
        } 
        else quote = true ;
        break ;
      case '+':
      case '#':
        if (lastCmd==1 && n != 128) {
          n ++ ;
        }
        break ;
      case '-':
        if (lastCmd==1 && n != 128) {
          n -- ;
        } 
        else if (lastCmd==13 || lastCmd==14 || n==128) {  // 'U','K','R' の場合
          minus = true ;            
        }
        break ;
      case 'o':
      case 'O':
        if (lastCmd==0) {
          lastCmd = 2 ;
        } 
        else quote = true ;
        break ;
      case '>':
        if (lastCmd==0) {
          lastCmd = 9 ;
        } 
        else quote = true ;
        break ;
      case '<':
        if (lastCmd==0) {
          lastCmd = 10 ;
        } 
        else quote = true ;
        break ;
      case 't':
      case 'T':
        if (lastCmd==0) {
          lastCmd = 3 ;
        } 
        else quote = true ;
        break ;
      case 'l':
      case 'L':
        if (lastCmd==0) {
          lastCmd = 4 ;
        } 
        else quote = true ;
        break ;
      case 'v':
      case 'V':
        if (lastCmd==0) {
          lastCmd = 5 ;
        } 
        else quote = true ;
        break ;
      case 's':
      case 'S':
        if (lastCmd==0) {
          lastCmd = 7 ;
        } 
        else quote = true ;
        break ;
      case 'm':
      case 'M':
        if (lastCmd==0) {
          lastCmd = 8 ;
        } 
        else quote = true ;
        break ;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        parseNum *= 10 ;
        parseNum += (c - '0') ;
        break ;
      case '.':
        if (lastCmd==1) {
          futen = true ;
        }  
        break ;
      }
      if (quote) {    // 1コマンドを解析終わっていた場合
#ifdef DEBUG_MODE
        Serial.print(" /ch") ;        
        Serial.print(ch) ;        
        Serial.print("/p") ;        
        Serial.print(p) ;        
        Serial.print("/") ;
#endif       
        switch (lastCmd) {
        case 1:  // CDEFGABR
          {
            byte notenum ;    // 直前のノート番号 
            if (!slah) {
              HandleNoteOn(ch+1,ymnote[ch],0);
            }
            notenum = getNoteNum(ymoct[ch],n) ;
#ifdef DEBUG_MODE
            noteToStr(notenum) ; 
            if (parseNum==0) parseNum = ymonpu[ch] ;
            Serial.print(parseNum) ;
            if(futen) Serial.print(".") ;
            Serial.println() ;
#endif
            // set_ch(ch,notenum) ;
            if (notenum < 128) {
              if (!slah) {
                HandleNoteOn(ch+1,notenum,1);
              }
              ymnote[ch] = notenum ;    // 直前のノート番号
            }
            if (parseNum==0) parseNum = ymonpu[ch] ;
          }
          break ;
        case 2:  // O
          ymoct[ch] = parseNum ;
#ifdef DEBUG_MODE
          Serial.print("O") ;        
          Serial.print(parseNum) ;
#endif       
          break ;
        case 3:  // T
          ymtempo[ch] = ymorgtmp[ch] = parseNum ;
#ifdef DEBUG_MODE
          Serial.print("T") ;        
          Serial.print(parseNum) ;
#endif       
          break ;
        case 4:  // L
          ymonpu[ch] = parseNum ;
#ifdef DEBUG_MODE
          Serial.print("L") ;        
          Serial.print(parseNum) ;
#endif       
          break ;
        case 5:  // V
          ymvol[ch] = parseNum & 0x0f ;
          // write_data(0x08+ch, ymvol[ch]) ;
          break ;
        case 6:  // J (ADSR の S)
          env_param.sustain_level = parseNum * 0x1000;
          PWM_SYNTH.getChannel(ch+1)->env_param = env_param ;
#ifdef DEBUG_MODE
          Serial.print("J") ;        
          Serial.print(parseNum) ;
#endif       
          break ;
        case 7:  // S (シリアル送信)
#ifdef USE_SERIAL
          Serial.write(parseNum) ;
#endif
#ifdef DEBUG_MODE
          Serial.print("S") ;        
          Serial.print(parseNum) ;
#endif       
          break ;
        case 8:  // M (ADSR の R)
          env_param.release_time = parseNum ;
          PWM_SYNTH.getChannel(ch+1)->env_param = env_param ;  // 1ch のみ
#ifdef DEBUG_MODE
          Serial.print("M") ;        
          Serial.print(parseNum) ;
#endif       
          break ;
        case 9:  // >
          if (ymoct[ch] < 8) ymoct[ch]++ ; 
#ifdef DEBUG_MODE
          Serial.print(">") ;        
          Serial.print(ymoct[ch]) ;
#endif       
          break ;
        case 10:  // <
          if (ymoct[ch] > 1) ymoct[ch]-- ; 
#ifdef DEBUG_MODE
          Serial.print("<") ;
          Serial.print(ymoct[ch]) ;
#endif       
          break ;
        case 11:  // H (ADSR の A)
          env_param.attack_speed = parseNum * 0x1000;
          PWM_SYNTH.getChannel(ch+1)->env_param = env_param ;
#ifdef DEBUG_MODE
          Serial.print("H") ;        
          Serial.print(parseNum) ;
#endif       
          break ;
        case 12:  // N
          {
            byte notenum = parseNum ;
#ifdef DEBUG_MODE
            Serial.print("note=") ;        
            Serial.print(notenum) ;        
            Serial.print("/") ;
#endif       
            parseNum = ymonpu[ch] ;
#ifdef DEBUG_MODE
            Serial.print(parseNum) ;
#endif
//          set_ch(ch,notenum) ;
            lastCmd = 1 ;  // 音符として扱う
          }
          break ;
        case 13:  // U (ADSR の D)
          env_param.decay_time = parseNum ;
          PWM_SYNTH.getChannel(ch+1)->env_param = env_param ;
#ifdef DEBUG_MODE
          Serial.print("U") ;        
          Serial.print(parseNum) ;
#endif       
          break ;
        case 14:  // K
          if (minus) {
            ymKeyShift = -parseNum ;
          } 
          else {
            ymKeyShift = parseNum ;
          }
          break ;
#ifdef LOOP_ENABLE    
        case 15:  // X 
          if (parseNum == 0) {    // LOOP の始点登録
            ymmark[ch] = p ;      // 場所を保存
            ymloop[ch] = 1 ;
          } 
          else {                // ポインタを検出する
            if (ymloop[ch]>0) {    // ループ内ではない
              for (word r = ymbegin[ch] + ymmark[ch] ; r < ymfinish[ch]-1 ; r++) {
#ifdef SD_MODE
                word q = r ;
#else
                word q = r + trackHead ;
#endif

                if (getmml_char(q) == 'X') {
                  if (getmml_char(q+1) == '0'+ymloop[ch]) {
                    p = q ;
                    p++ ;
                    ymloop[ch] *= -1 ;
                    break ;
                  }
                } 
                else if (getmml_char(q) == '/' && getmml_char(q) == '*') {
                  p = findCommentEnd (ch,p) ;
                }
              }
            } 
            else {
              ymloop[ch] *= -1 ;
              ymloop[ch]++ ;
              p = ymmark[ch] ;
            }
          }
          break ;
        case 16:  // Y 
          if (parseNum > 0) {    // LOOP の始点登録
            ymmark[ch] = p ;      // 場所を保存
            ymloop[ch] = parseNum ;
          } 
          else {                // ポインタを検出する
            ymloop[ch]-- ;
            if (ymloop[ch]>0) {
              p = ymmark[ch] ;
            }
          }
          break ;
#endif
        case 18:  // @ 音色チェンジ
          switch(parseNum) {
          case 1:
            PWM_SYNTH.getChannel(ch+1)->wavetable = PWMDACSynth::squareWavetable ;  // 矩形波
            break ;
          case 2:
            PWM_SYNTH.getChannel(ch+1)->wavetable = PWMDACSynth::triangleWavetable ;  // 三角波
            break ;
          case 3:
            PWM_SYNTH.getChannel(ch+1)->wavetable = PWMDACSynth::sawtoothWavetable ; // のこぎり波
            break ;
          case 4:
            PWM_SYNTH.getChannel(ch+1)->wavetable = PWMDACSynth::shepardToneSineWavetable ; // 正弦波の無限音階（シェパードトーン）
            break ;
          default:
            PWM_SYNTH.getChannel(ch+1)->wavetable = PWMDACSynth::sineWavetable ; // 正弦波
            break ;
          }        
          break ;
        case 19:  // P (デジタルポート出力)
#ifndef NOTE_PORTC
          PORTC = (parseNum & 0b00111111) ;
#endif
          break ;
        }
        if (lastCmd==1) {      // 音符コマンド解析済の場合、次のコマンドを解析する時間を取得する
          if (minus && n==128) {    // マイナス値の休符の場合        
            // ch7buf |= (0b00000001)<<ch ;    // 対応するチャンネルを消音
            ymfinish[ch] = 0 ;              // MML 解析が終わっている事を記録
            onplay = true ;
            loopflag = false ;
          } else {
            df = (unsigned long)(ymtempo[ch] + mmlSpeed) ;
            of = 240000000L / ((unsigned long)parseNum * df) ; // 4分音符あたりのマイクロ秒
            if (futen) of = of * 3 / 2 ;         // 付点音符つきなら 1.5 倍
            df = (ms - ymend[ch]) ;              // 前のENDから超過した時間
            st = ms - df ;                       // 次の開始時間
            ed = st + of ;                       // 次の終了時間
            if (ed >= ms) {                      // オーバーしていない場合
              ymstart[ch] = st ;                 // 次の開始時間を記録
              ymend[ch] = ed ;                   // 次の終了時間
              ymmid[ch] = ed ;
              ympointer[ch] = p ;                // 次の MML 解析場所を保存(内部)
              loopflag = false ;
              break ;
            } else {                             // オーバーしていた場合
              timeover = true ;                  // 処理が間に合わなかった事の履歴
              ymstart[ch] = ed ;                 // 次の開始時間を記録
              ymmid[ch] = ymend[ch] = ed ;       // 次の終了時間
              lastCmd = 0 ;
              parseNum = 0 ;
              quote = false ;
            }
          }
        } else {  // 音符コマンド以外の場合次のコマンドを解析開始
          q = c ;
          lastCmd = 0 ;
          parseNum = 0 ;
          quote = false ;
        }
      } else {  // non Quote
        p ++ ;  // 次の文字を解析
#ifdef SD_MODE
        if (ymfinish[ch] <= ymbegin[ch] + p) {
          ymfinish[ch] = 0 ;              // MML 解析が終わっている事を記録
          loopflag = false ;
        }
#endif
      }
    } // while loop
  }  // for loop (チャンネル分のループ用)
  if (onplay) {    // 再生時間の更新(microSec)
    mmlPlay = ms - mmlBegin ; 
  }
  mmlLast = ms ;
  return onplay ;
}
//--------------------------------------------------------
// コメントの終わりまでポインタを進める
word findCommentEnd (byte ch,word p) {
  word q ;
  word o ;
#ifdef SD_MODE
  o = 0 ;
#else
  o = trackHead ;
#endif
  for (q = p ; q < ymfinish[ch]-1 ; q++) {
    if (getmml_char(q+o) == '*' && getmml_char(q+o+1) == '/') {
      break ;
    }
  }  
  return q+1 ;
}
//--------------------------------------------------------
int sleepAndWakeup(){
  delay(10) ;
  digitalWrite(TEMPOLED,LOW) ;  // TEMPO LED 消灯
  attachInterrupt(0,wakeup,FALLING);// 0は割り込みIDを示しています。
  noInterrupts();// 割り込みを無効にして休止を実行します。
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);// 休止モードを指定します。
  sleep_enable();// 休止処理を実行します。
  interrupts();// 割り込みを有効にして復帰を待ちます。
  sleep_cpu();// CPUも休止します。
  sleep_disable();// タクトスイッチが押されLOWレベルになる復帰開始
  detachInterrupt(0);// 割り込み処理を解除します。
  return 0;
}
//--------------------------------------------------------
void wakeup() {
  delay(10) ;
#ifdef SD_MODE
  mml_init_SD() ;
#else
  // track_init() ;
  mml_init() ;
#endif
  mmlPauseFlag = false ;
  mainseq = 0 ;
}
// ------------------------------------------------------
// EEPROM の long 値を読み出す
// ------------------------------------------------------
unsigned long readLongEEPROM(word addr) {
  byte b[4] ;
  int i ;
  for (i=0 ; i<4 ; i++) {
    b[i] = EEPROM.read(addr+i) ;
  }
  return ((unsigned long)(b[3]*0x1000000)+(b[2]*0x10000)+(b[1]*0x100)+b[0]) ;
}
// ------------------------------------------------------
// EEPROM の word 値を読み出す
// ------------------------------------------------------
word readWordEEPROM(word addr) {
  byte b1,b0 ;
  b0 = EEPROM.read(addr) ;
  b1 = EEPROM.read(addr+1) ;
  return ((b1<<8)|b0) ;
}
// ------------------------------------------------------
// EEPROM に long 値を書き込む
// ------------------------------------------------------
void writeLongEEPROM(word address, unsigned long value) {
  byte d ;
  for (byte i=0 ; i<4 ; i++) {
    d = (value >> (i*8)) & 0xff ;
    EEPROM.write(address+i,d) ;
  }
}
// ------------------------------------------------------
// EEPROM に word 値を書き込む
// ------------------------------------------------------
void writeWordEEPROM(word address, word value) {
  byte d ;
  for (byte i=0 ; i<2 ; i++) {
    d = (value >> (i*8)) & 0xff ;
    EEPROM.write(address+i,d) ;
  }
}
//--------------------------------------------------------
/*
無関係な文字は無視する。可読性のために () などで囲むのも有効
 Cコメント／＊　＊／で囲んだ領域は無視される

 C D E F G A B
 それぞれ、ドレミファソラシの音符。

 # + -
 音符の後につけて半音上げ下げを表す。#と+は同じ意味。

 R
 休符 -1の場合そこで演奏を止める

 数字 .
 音符や休符の後につけ、音の長さを表す。2=2分音符。.は付点で長さを1.5倍する。4.=付点4分音符。

 &
 二つの音符を連結するタイを表す。
 ※前後の音階が異なる場合、前のキーがノートＯＦＦにならないので注意。

 T
 テンポを指定。たとえば「T120」なら120BPMで演奏する。各ch独立しているので個別に指定すること。

 O
 オクターブ指定（１～８）

 > <
 オクターブの上下。>がアップ、<がダウン。

 L
 A～GやRの後に数字をつけないときの音の長さを指定。初期値は4。

 V
 音量（ボリューム）を指定、本アプリでは無視される。(V0 でも音が鳴る)

 N
 通常のオクターブ+CDEFGABではなく、midiのノート番号を数値で直接指定する。

 K
 音階のシフト。-12～12 の分だけノート番号をずらす。
 
 @
 波形の指定。0 正弦波 1 矩形波 2三角波 3のこぎり波 4正弦波の無限音階（シェパードトーン）

 Q
 I
 W
 Z

 H (0～15)
 ADSR の A を指定。(デフォルトは 8)

 U (0～15)
 ADSR の D を指定。(デフォルトは 7)

 J (0～15)
 ADSR の S を指定。(デフォルトは 8)

 M (0～15)
 ADSR の R を指定。(大きいほど長くなる。デフォルトは 5)

 S（大文字限定）(0～255)
 シリアルに文字コードを送信する。シリアル未使用モードでは無視される。

 P（大文字限定）(0～255)
 デジタルピン出力。PortC (C0～C5) の状態を設定して出力する。
 演奏には影響を与えない。NOTE_PORTC が定義されている時は無効

 X（大文字限定）
 条件ループ X0(PartA)X1(PartB)X2(PartC) のように指定すると、(PartA)(PartB)(PartA)(PartC) のように演奏する。
 ※Y とネストさせて使う事はできない、X2X4のように繋げる事はできない。
 
 Y（大文字限定）
 単純ループ Yxx～Y (もしくはY0) で区切られた区間を xx 回リピートする。
 ※X とネストさせて使う事はできない
  */

