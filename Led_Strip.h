#include <ArduinoIoTCloud.h>

// ESP32:
// #define green 4
// #define red 16
// #define blue 17
// #define white 14


// ESP8266(NODEMCU):
#define green 16
#define red 5
#define blue 4
#define white 0


#ifndef debugMode
#define debugMode false
#endif

#ifndef LED_STRIP_H
#define LED_STRIP_H
void SetupLedPins();
void Fully_Saturate(int *colors);
void changeLedColor(int RGB_Array[3], int Brightness, int Saturation, bool do_smoothly, bool verbose);
/**
 * @deprecated
*/
void smooth_analogWrite(int pin, int old_data, int new_data);
void muiltple_smooth_analogWrite(int pins[4], int old_datas[4], int new_datas[4], bool verbose);
void doRainbow(CloudColoredLight* color);
void breathe(int rgb_arr[3], int brightness);
#endif