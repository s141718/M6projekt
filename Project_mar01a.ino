#include "arduino_secrets.h"
/* 
  Sketch generated by the Arduino IoT Cloud Thing "Project"
  https://create.arduino.cc/cloud/things/c861a694-77c9-4193-bcea-21eaff650a20 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  float h;
  float t;
  float t_guard;
  bool t_alarm;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"

#include <Adafruit_SHT31.h>

Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

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

  // Initialize sensor
  if (! sht31.begin(0x44)) {
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  // Setup LED
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  ArduinoCloud.update();

  // Read sensor
  t = sht31.readTemperature();
  h = sht31.readHumidity();

  // Rule for temp guard
  t <= t_guard ? t_alarm = true : t_alarm = false;

  // Heartbeat
  digitalWrite(LED_BUILTIN, HIGH);
  delay(25);
  digitalWrite(LED_BUILTIN, LOW);
  
  delay(5000);
}


/*
  Since TGuard is READ_WRITE variable, onTGuardChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onTGuardChange()  {
  Serial.print("New warning temp: ");
    Serial.println(t_guard);
}