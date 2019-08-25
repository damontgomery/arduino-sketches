#include <IRLibSendBase.h>
#include <IRLib_P01_NEC.h>   
#include <IRLibCombo.h>

IRsend mySender;

// Configuration.
int micPin = 0;
int micVolumeThreshold = 20;
int sampleDelay = 50;
int signalDelay = 250;

bool speakerBoosted = false;

int micHistoryCount = 200;
int micHistoryIndex = 0;

// Volume history.
int micVolumeHistory [200];
long micVolumeTotal = 0;
int micVolumeAverage;

int micWarmupThreshold = 4;

void setup() {
  pinMode(micPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  initializeHistories();

  warmUpMic();
}

void loop() {
  int volume = readMicVolume();

  storeMicVolumeInHistory(volume);

  bool speakerShouldBeBoosted = (maxVolumeInHistory() >= micVolumeThreshold);
  
  if (speakerShouldBeBoosted && (speakerBoosted == false)) {
    boostSpeaker();
  }
  else if ((speakerShouldBeBoosted == false) && speakerBoosted) {
    lowerSpeaker();
  }

  delay(sampleDelay);
  
}

void warmUpMic() {
  int micVolume;
  
  // Warm up the mic by reading it a few times.
  for (int i = 0; i < micWarmupThreshold; i++) {
    micVolume = analogRead(micPin);
    delay(sampleDelay);
  }
}

void initializeHistories() {
  for (int i = 0; i < micHistoryCount; i++) {
    micVolumeHistory[i] = 0;
  }
}

int readMicVolume() {
  int volume = 0;
  int micVolume = analogRead(micPin);

  if (micVolume >= micVolumeThreshold) {
    volume = micVolume;
  }

  return volume;
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

