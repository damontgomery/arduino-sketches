// Configuration.
int micPin = 0;
int sampleDelay = 0;

void setup() {
  pinMode(micPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  analogReference(INTERNAL);
  
  Serial.begin(115200);

  delay(1000);
}

void loop() {
  
  int micRaw;

  micRaw = analogRead(micPin);

  int hundreds = micRaw / 100 % 10;
  int tens = micRaw / 10 % 10;
  int singles = micRaw % 10;

  blink_digit(hundreds);
  blink_digit(tens);
  blink_digit(singles);
  
  delay(3000);
}

void blink_digit(int digit) {
  // We make a dash for 0.
  if (digit == 0) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
  else {
    for (int i = 1; i <= digit; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
    }
  }
  delay(500);
}

