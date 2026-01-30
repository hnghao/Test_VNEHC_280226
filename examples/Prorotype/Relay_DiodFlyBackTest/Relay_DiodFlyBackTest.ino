uint8_t pinA_Diod = A1;
uint8_t pinD_Diod = 7;
uint8_t pin_Relay = 9;

#define RELAY_ACTIVE  (1)

void setup() {
  // put your setup code here, to run once:
  pinMode(pinD_Diod, INPUT);
  pinMode(pin_Relay, OUTPUT);

  Serial.begin(115200);
  Serial.println(F("Start Test Relay"));

  Serial.print(F("ADC Start: "));
  Serial.println(analogRead(pinA_Diod));

  digitalWrite(pin_Relay, RELAY_ACTIVE);
  delay(200);

  Serial.print(F("ADC on: "));
  Serial.println(analogRead(pinA_Diod));
  
  Serial.print(F("ADC off: "));
  digitalWrite(pin_Relay, !RELAY_ACTIVE);

  // delayMicroseconds(200);
  // Serial.println(analogRead(pinA_Diod));

  int tempArr[10];
  for(int cf = 0; cf < 10; cf++)
  {
    tempArr[cf] = analogRead(pinA_Diod);
    delayMicroseconds(10);
  }

  Serial.println("====");
  for(int cf = 0; cf < 10; cf++)
  {
    Serial.println(tempArr[cf]);
  }

  
  
}

void loop() {
  // put your main code here, to run repeatedly:

}