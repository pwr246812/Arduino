#define trigPin 12
#define echoPin 11

void setup() {
  Serial.begin(1200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

void loop() {
  Serial.println(zmierzOdleglosc());
  wZasiegu(5, 8, 11, 14, 17, 20);
  delay(200);
}

float zmierzOdleglosc() {
  long czas {0};
  float odleglosc {0.0};

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  czas = pulseIn(echoPin, HIGH);
  odleglosc = czas / 58.0;

  return odleglosc;
}

void wZasiegu(float I, float II, float III, float IV, float V, float VI) {
  float odleglosc = zmierzOdleglosc();
  int przeskalowane {0};

  if (odleglosc > VI) przeskalowane = 0;
  else if ((odleglosc >= V) and (odleglosc < VI))   przeskalowane = 1;
  else if ((odleglosc >= IV) and (odleglosc < V))   przeskalowane = 2;
  else if ((odleglosc >= III) and (odleglosc < IV)) przeskalowane = 3;
  else if ((odleglosc >= II) and (odleglosc < III)) przeskalowane = 4;
  else if ((odleglosc >= I) and (odleglosc < II))   przeskalowane = 5;
  else if (odleglosc < I) przeskalowane = 6;
  
  for (int i {0}; i <= przeskalowane; i++) {
    digitalWrite(i + 1, HIGH);
  }

  for (int j {0}; j <= 6-przeskalowane; ++j) {
    digitalWrite(przeskalowane + j + 2, LOW);
  }
}
