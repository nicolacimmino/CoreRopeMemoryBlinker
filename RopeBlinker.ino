
uint8_t rowPins[] = { 3, 4, 5, 6 };
uint8_t colPins[] = { 7, 8, 9, 10 };
uint8_t sensePins[] = { A0, A1, A2, A3 };

void setup() {
 
 pinMode(A0, INPUT);
 pinMode(A1, INPUT);
 pinMode(A2, INPUT);
 pinMode(A3, INPUT);
 
 Serial.begin(9600);
 analogReference(INTERNAL);
}


uint8_t readBit(uint8_t A, uint8_t B, uint8_t sense) {

  pinMode(sense, OUTPUT);
  digitalWrite(sense, LOW);
  delay(5);
  pinMode(sense, INPUT);
  
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  
  digitalWrite(A, LOW);
  digitalWrite(B, HIGH);  
  long value = analogRead(sense);

  digitalWrite(B, LOW);
  digitalWrite(A, HIGH);  
  value += analogRead(sense);

  pinMode(A, INPUT);
  pinMode(B, INPUT);
  
  return (value > 1) ? 1 : 0;
}

uint8_t readNibble(uint8_t address) {
  uint8_t result = 0;
  for(int iSense = 0; iSense < 4; iSense++) {
      result = (result << 1) | readBit(colPins[address & 0b11], rowPins[(address >> 2) & 0b11], sensePins[iSense]);      
  }

  return result;
}


void loop() {
  for(int address = 0; address < 16; address++) {
    if(address > 0 && address % 2 == 0) {
      Serial.print(".");
    }
    Serial.print(readNibble(address), HEX);     
  }
  
  Serial.println("");
  
  delay(1000);
}