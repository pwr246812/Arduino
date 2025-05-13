const int czerwona {13};
const int zolta {12};
const int zielona {11};
const int przycisk {7};
String wybor {""};
int g_on {0};
int r_on {0};
int y_on {0};

void setup() {
  // put your setup code here, to run once:
  pinMode(czerwona, OUTPUT);
  pinMode(zolta, OUTPUT);
  pinMode(zielona, OUTPUT);
  digitalWrite(czerwona, LOW);
  digitalWrite(zolta, LOW);
  digitalWrite(zielona, LOW);

  pinMode(przycisk, INPUT_PULLUP);

  Serial.begin(1200);
  Serial.println("Wpisz powyżej którą diodą mam błysnąć i zatwierdź");
}

void loop() { 
  if (Serial.available()) {
    wybor = Serial.readStringUntil('\n');

    if (wybor == "zielona")        g_on++;
    else if (wybor == "czerwona")  r_on++;
    else if (wybor == "zolta")     y_on++;
    else Serial.println("Dostępne [zielona, czerwona, zolta], popraw się ;)");
    
  if (g_on%2 == 1) digitalWrite(zielona, HIGH);   else  digitalWrite(zielona, LOW); 
  if (y_on%2 == 1) digitalWrite(zolta, HIGH);     else  digitalWrite(zolta, LOW);   
  if (r_on%2 == 1) digitalWrite(czerwona, HIGH);  else  digitalWrite(czerwona, LOW);
  }
  
}
