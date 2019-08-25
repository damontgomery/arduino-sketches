// Configuration.
int micPin = 0;
int sampleDelay = 10;

void setup() {
  pinMode(micPin, INPUT);
//  pinMode(LED_BUILTIN, OUTPUT);

  //analogReference(INTERNAL);
  
  Serial.begin(9600);
}

void loop() {
  
  int micRaw;

  micRaw = analogRead(micPin);

  Serial.println(micRaw);
  
  delay(sampleDelay);
}
