#include <IRLibSendBase.h>
#include <IRLib_P01_NEC.h>   
#include <IRLibCombo.h>

IRsend mySender;

// Configuration.
const int micPin = 0;
int micVolumeThreshold = 20;
const int maxThreshold = 100;
const int sampleDelay = 50;
const int signalDelay = 250;

const int potentiometerPin = 1;

const bool debugging = false;

bool speakerBoosted = false;

const int micHistoryCount = 200;
int micHistoryIndex = 0;

// Volume history.
int micVolumeHistory [micHistoryCount];
long micVolumeTotal = 0;
int micVolumeAverage;

const int micWarmupThreshold = 4;

// Button states
boolean increaseButtonState = false;
boolean decreaseButtonState = false;

void setup() {
  if (debugging == true) {
    Serial.begin(9600);
  }
  
  pinMode(micPin, INPUT);
  pinMode(potentiometerPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  initializeHistories();

  warmUpMic();
}

void loop() {
  setMicVolumeThreshold();
  
  int volume = analogRead(micPin);

  storeMicVolumeInHistory(volume);

  bool speakerShouldBeBoosted = (maxVolumeInHistory() >= micVolumeThreshold);
  
  if (speakerShouldBeBoosted && (speakerBoosted == false)) {
    boostSpeaker();
    
    if (debugging == true) {
      Serial.println("boosted");
    }

    // The speaker boosting causes interference with the sound detector.
    warmUpMic();
  }
  else if ((speakerShouldBeBoosted == false) && speakerBoosted) {
    lowerSpeaker();

    if (debugging == true) {
      Serial.println("lowered");
    }

    // The speaker boosting causes interference with the sound detector.
    warmUpMic();
  }

  if (debugging == true) {
    Serial.println("Potentiometer:");
    Serial.println(analogRead(potentiometerPin));
    Serial.println("Volume:");
    Serial.println(volume);
    Serial.println("Mic volume threshold:");
    Serial.println(micVolumeThreshold); 
  }

  delay(sampleDelay);
}

void warmUpMic() {
  // Warm up the mic by reading it a few times.
  for (int i = 0; i < micWarmupThreshold; i++) {
    delay(sampleDelay);
  }
}

void initializeHistories() {
  for (int i = 0; i < micHistoryCount; i++) {
    micVolumeHistory[i] = 0;
  }
}

void storeMicVolumeInHistory(int volume) {
  micVolumeTotal = micVolumeTotal - micVolumeHistory[micHistoryIndex] + volume;
  micVolumeHistory[micHistoryIndex] = volume;
  micVolumeAverage = micVolumeTotal / micHistoryCount;

  // Increment counter.
  micHistoryIndex++;
  if (micHistoryIndex == micHistoryCount) {
    micHistoryIndex = 0;
  }
}

int maxVolumeInHistory() {
  int maxVolumeInHistory = 0;

  for (int i = 0; i < micHistoryCount; i++) {
    maxVolumeInHistory = max(maxVolumeInHistory, micVolumeHistory[i]);
  }

  return maxVolumeInHistory;
}

void setMicVolumeThreshold() {
  // We use a float here so that we keep the decimals during the math.
  float maxReading = 1023;

  // The potentiometer returns a value of 0 to 1023.  
  micVolumeThreshold = ((maxReading - analogRead(potentiometerPin)) / 1023) * maxThreshold;
}

void boostSpeaker() {
  digitalWrite(LED_BUILTIN, HIGH);
  
  speakerBoosted = true;
  
  sendIRCode(0xff827d);
}

void lowerSpeaker() {
  digitalWrite(LED_BUILTIN, LOW);
  
  speakerBoosted = false;
  
  sendIRCode(0xffa25d);
}

void sendIRCode(long code) {
  mySender.send(NEC, code, 0);
  delay(signalDelay);
  mySender.send(NEC, code, 0);
}

