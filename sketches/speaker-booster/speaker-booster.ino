#include <IRLibSendBase.h>
#include <IRLib_P01_NEC.h>   
#include <IRLibCombo.h>

IRsend mySender;

// Configuration.
int micPin = 0;
int micVolumeThreshold = 4;
int sampleDelay = 50;
int signalDelay = 250;

bool speakerBoosted = false;

int micHistoryCount = 200;
int micHistoryIndex = 0;

// Raw value history.
int micRawHistory [200];
int micRawInitialValue = 730; // 732 USB, 730 wall.
long micRawTotal = long(micHistoryCount) * micRawInitialValue;
int micRawAverage;

// Volume history.
int micVolumeHistory [200];
long micVolumeTotal = 0;
int micVolumeAverage;

int micWarmup = 0;
int micWarmupThreshold = 4;
bool micWarmed = false;

void setup() {
  pinMode(micPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  analogReference(INTERNAL);
  
  Serial.begin(9600);

  // Initialize histories.
  for (int i = 0; i < micHistoryCount; i++) {
    micRawHistory[i] = micRawInitialValue;
    micVolumeHistory[i] = 0;
  }
  
}

void loop() {
  
  int micRaw;

  micRaw = analogRead(micPin);

  // Initial readings are bad, so throw them out.
  if (micWarmed == false) {
    micWarmup++;

    if (micWarmup == micWarmupThreshold) {
      micWarmed = true;
    }
    
    delay(sampleDelay);
    return;
  }

  // Record new raw values.
  micRawTotal = micRawTotal - micRawHistory[micHistoryIndex] + micRaw;
  micRawHistory[micHistoryIndex] = micRaw;
  micRawAverage = micRawTotal / micHistoryCount;

  // Calculate volume.
  int volume = 0;
  int micVolume = int(abs(micRawAverage - micRaw));

  if (micVolume >= micVolumeThreshold) {
    volume = micVolume;
  }

  // Record new volume values.
  micVolumeTotal = micVolumeTotal - micVolumeHistory[micHistoryIndex] + volume;
  micVolumeHistory[micHistoryIndex] = volume;
  micVolumeAverage = micVolumeTotal / micHistoryCount;

  // Find the max volume in the history.
  int maxVolumeInHistory = 0;

  // Initialize histories.
  for (int i = 0; i < micHistoryCount; i++) {
    maxVolumeInHistory = max(maxVolumeInHistory, micVolumeHistory[i]);
  }
  
  // Debug print to serial monitor.
  Serial.println(maxVolumeInHistory);

  // light the LED when the volume is "high".
  if (maxVolumeInHistory >= micVolumeThreshold) {
    if (speakerBoosted == false) {
      digitalWrite(LED_BUILTIN, HIGH);
      
      // Boost speaker.
      speakerBoosted = true;

      mySender.send(NEC,0xff827d, 0);
      delay(signalDelay);
      mySender.send(NEC,0xff827d, 0);
    }
  }
  else {
    if (speakerBoosted == true) {
      digitalWrite(LED_BUILTIN, LOW);
      
      // Lower speaker.
      speakerBoosted = false;
      
      mySender.send(NEC,0xffa25d, 0);
      delay(signalDelay);
      mySender.send(NEC,0xffa25d, 0);
    }
  }

  // Increment counter.
  micHistoryIndex++;
  if (micHistoryIndex == micHistoryCount) {
    micHistoryIndex = 0;
  }

  delay(sampleDelay);
  
}
