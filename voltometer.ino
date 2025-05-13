int zmierzoneNapiecie {0};
float przeliczoneNapiecie {0.000};
int wybor {0};

void setup() {
  Serial.begin(1200);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9,  OUTPUT);
  pinMode(8,  OUTPUT);
}

void loop() {
  zmierzoneNapiecie = analogRead(A5);
  przeliczoneNapiecie = (zmierzoneNapiecie * 5.0) / 1024.0;
  // Serial.println("Aktualne napięcie to: " + String(przeliczoneNapiecie) + "V");
  delay(100);
  
  wybor = map(zmierzoneNapiecie, 0, 1023, 1, 6);
  Serial.println(wybor);

  for (int i {0}; i < 7; i++) {
    digitalWrite(7 + i, LOW); 
    digitalWrite(7 + wybor, HIGH);
  }
  delay(100);
}
