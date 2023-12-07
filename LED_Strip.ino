//! COMMENT THIS OUT IF YOU ARE NOT DEBUGGING
#define VERBOSE

#include "arduino_secrets.h"
#include "thingProperties.h"
#include "strip.h"
#include "Constants.h"

LedStrip strip(RED_PIN, GREEN_PIN, BLUE_PIN, WHITE_PIN);

bool lastNightMode = false;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1000);

  // Setup Cloud
  cloudSetup();
}

void cloudSetup() {
  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}


void loop() {
  ArduinoCloud.update();
  // Your code here
  
  if (nightMode.isActive() != lastNightMode) {
    onNightModeChange();
    lastNightMode = nightMode.isActive();
  }

  strip.update();
}

void printColorChange() {
  Serial.println("Color Changed");
  Serial.print("Brightness: ");
  Serial.println(color.getBrightness());
  Serial.print("Saturation: ");
  Serial.println(color.getSaturation());
  Serial.print("Switch: ");
  Serial.println(color.getSwitch());
}

void printModeChange() {
  Serial.print("Mode Changed: ");
  Serial.print((Modes)mode);
  Serial.print(" -> ");
  
  switch ((Modes)mode) {
    case Normal:
      Serial.println("Normal");
      break;
    case Breathe:
      Serial.println("Breathe");
      break;
    case Rainbow:
      Serial.println("Rainbow");
      break;
    case Off:
      Serial.println("Off");
      break;
  }
}

/*
  Since Color is READ_WRITE variable, onColorChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onColorChange() {
  printColorChange();
  strip.changeLedColor(&color);
}

/*
  Since Mode is READ_WRITE variable, onModeChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onModeChange()  {
  printModeChange();

  if (Modes(mode) == Normal) {
    onColorChange(); // this will also print the color change and also give us the last used color object
  }

  strip.changeMode(Modes(mode));
}


/*
  Since NightMode is READ_WRITE variable, onNightModeChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onNightModeChange()  {
  Serial.print("Night Mode is ");
  Serial.println(nightMode.isActive() ? "Active" : "Inactive");
  strip.setNightMode(nightMode.isActive());
}

void onCloudSerialChange() { // will give the last Command, use the internal library to handle it
  Serial.print("New Cloud Serial Command: ");
  Serial.println(cloudSerial);

  //TODO: add internal library to handle cloud serial commands.  
}
