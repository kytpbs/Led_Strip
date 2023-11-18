#include "HardwareSerial.h"
#include "Led_Strip.h"
#include "HSBColor.h"


int temp_r, temp_g, temp_b, temp_w;

int hue = 0;

void SetupLedPins() {
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(white, OUTPUT);
}

void Fully_Saturate(int *colors) {
  int max_value = max(colors[2], max(colors[0], colors[1]));
  if (max_value == 0) {
    return;
  }
  int increase_by = 255 - max_value;
  if (colors[0] > 50) {
    colors[0] += increase_by;
  }
  if (colors[1] > 50) {
    colors[1] += increase_by;
  }
  if (colors[2] > 50) {
    colors[2] += increase_by;
  }
}

void changeLedColor(int RGB_Array[3], int Brightness, int Saturation, bool do_smoothly, bool verbose) {  
  int r = map(RGB_Array[0], 0, 255, 256, 0);
  int g = map(RGB_Array[1], 0, 255, 256, 0);
  int b = map(RGB_Array[2], 0, 255, 256, 0);
  int saturation = map(Saturation, 0, 100, 100, 0);
  double BrightnessPercentage = Brightness/100.0;
  int w = map(Brightness/100.0 * saturation, 0 , 100, 256, 0);
  
  if (verbose) {
    Serial.println("");
    Serial.println("Setting Leds: ");
    Serial.print("Red: ");
    Serial.println(r);
    Serial.print("Blue: ");
    Serial.println(b);
    Serial.print("Green: ");
    Serial.println(g);
    Serial.print("White: ");
    Serial.println(w);
    Serial.println("");
  }

  // new code part
  int pins[4] = {red, green, blue, white};
  int old_datas[4] = {temp_r, temp_g, temp_b, temp_w};
  int new_datas[4] = {r, g, b, w};
  
  if (do_smoothly) {
    if (verbose) Serial.println("Doing Smooth_Change");
    muiltple_smooth_analogWrite(pins, old_datas, new_datas, verbose);
  }
  else {
    if (verbose) Serial.println("Doing Normal Change");
    analogWrite(white, w);
    analogWrite(red, r);
    analogWrite(green, g);
    analogWrite(blue, b);  
  }
  
  temp_r = r;
  temp_g = g;
  temp_b = b;
  temp_w = w;
  
}

void smooth_analogWrite(int pin, int old_data, int new_data) {
  int diffrence = new_data - old_data;
  
  if (diffrence == 0) {
    return;
  }

  int del = 500/abs(diffrence);
  Serial.print("Delay: ");
  Serial.println(del);
  Serial.print("Diffrence: ");
  Serial.println(diffrence);
  if (diffrence <= 0) {
    for (int i = 0; i <= abs(diffrence); i++) {
      analogWrite(pin, old_data - i);
      delay(del);
    }
  }
  if (diffrence >= 0) {
    for (int i = 0; i <= abs(diffrence); i++) {
      analogWrite(pin, old_data + i);
      delay(del);
    }
  }

}

void muiltple_smooth_analogWrite(int pins[4], int old_datas[4], int new_datas[4], bool verbose) {
  int diffrences[4];
  int abs_max_diffrence = 1;
  if (verbose) Serial.println("Calculating Diffrences");
  for (int i = 0; i < 4; i++) {
    if (new_datas[i] == old_datas[i] && new_datas[i] == 0) {
      digitalWrite(pins[i], LOW);
      diffrences[i] = 0;
      continue;
    }
    //get max diffrence
    if (abs(new_datas[i] - old_datas[i]) > abs_max_diffrence) {
      abs_max_diffrence = abs(new_datas[i] - old_datas[i]);
    }
    diffrences[i] = new_datas[i] - old_datas[i];
  }
  if (abs_max_diffrence <= 1)  {
    return;
  }
  for (int i = 0; i <= abs_max_diffrence; i++) {
    for (int j = 0; j < 4; j++) {
      if (diffrences[j] < 0) {
        analogWrite(pins[j], old_datas[j] - i);
        diffrences[j] += 1;
      }
      if (diffrences[j] > 0) {
        analogWrite(pins[j], old_datas[j] + i);
        diffrences[j] -= 1;
      }
    }
    if (abs_max_diffrence != 0) {
      delay(1000/abs_max_diffrence);
    }
  }
  if (verbose) Serial.println("Done");
}

void doRainbow(CloudColoredLight *color) {
  int rgb_arr[3];
  int brightness = color->getBrightness();
  
  if (!color->getSwitch()) {
    color->setSwitch(true);
  }

  if (hue >= 360) {
    hue = 0;
  }

  color->setHue(hue);
  
  HSBtoSaturatedRGB(hue, 100, brightness, rgb_arr);
  changeLedColor(rgb_arr, brightness, 100, true, debugMode);

  hue += 8;
}

void breathe(int rgb_arr[3], int brightness) {
  int direction = 1;
  int closed_rgb[3] = {0,0,0};
  changeLedColor(closed_rgb, 0, 100, true, false);
  changeLedColor(rgb_arr, brightness, 100, true, false);
}
