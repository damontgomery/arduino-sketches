#include <IRLibSendBase.h>
#include <IRLib_P01_NEC.h>   
#include <IRLibCombo.h>

IRsend mySender;

char directions [] = "Press 'u' for volume up, 'd' for volume down.";
int signalDelay = 250;

void setup() {
  Serial.begin(9600);
  Serial.println(directions);
}

void loop() {
  // Sending by default is to pin Digital 3.

  String stringFromSerial;

  if(Serial.available() > 0) {
    stringFromSerial = Serial.readStringUntil('\n');

    // Switch statements can only take integers or characters.
    switch(stringFromSerial[0]) {
      
      // Volume up (twice).
      case 'u':
        mySender.send(NEC, 0xff827d, 0);
        delay(signalDelay);
        mySender.send(NEC, 0xff827d, 0);
        Serial.println(F("Sent volume up."));
        break;
        
      // Volume down (twice).
      case 'd':
        mySender.send(NEC, 0xffa25d, 0);
        delay(signalDelay);
        mySender.send(NEC, 0xffa25d, 0);
        Serial.println(F("Sent volume down."));
        break;
      
      default:
        //Serial.println(directions);
        break;
    }
  }
}
