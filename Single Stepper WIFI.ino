#include <AccelStepper.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#define STEP_PIN  12
#define DIR_PIN   13

#define M0_PIN   14
#define M1_PIN   27
#define M2_PIN   26

const char* ssid = "bob";
const char* password = "bobbobbob";

AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(M0_PIN, OUTPUT);
  pinMode(M1_PIN, OUTPUT);
  pinMode(M2_PIN, OUTPUT);

  digitalWrite(M0_PIN, HIGH);
  digitalWrite(M1_PIN, HIGH);
  digitalWrite(M2_PIN, HIGH);

  stepper.setMaxSpeed(5000);
  stepper.setSpeed(2000);

  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());

  server.on("/control", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasArg("up")) {
      digitalWrite(DIR_PIN, HIGH);
      stepper.setSpeed(abs(stepper.speed()));
    }
    if (request->hasArg("down")) {
      digitalWrite(DIR_PIN, LOW);
      stepper.setSpeed(-abs(stepper.speed()));
    }
    if (request->hasArg("stop")) {
      stepper.setSpeed(0);
    }
    request->send(200, "text/plain", "Motor command received");
  });

  server.begin();
}

void loop() {
  stepper.runSpeed();
}
