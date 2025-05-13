#include <LiquidCrystal.h> //Dołączenie bilbioteki
#define matematyka A5
#define fizyka A4
#define B 11
#define G 10
#define R 9

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

int procentyMatematyka {0};
int procentyFizyka {0};

float wagi[3] = {1, 1, 1};
double suma {0.0};
int ileOk {0};
int epoki {0};
int iteracje {0};
int limitEpok {3000};


const int iloscOdpowiedzi {21};
const int iloscWejsc {2};
int wyniki[iloscOdpowiedzi][iloscWejsc] = {{9, 62}, {13, 44}, {20, 10}, {24, 84}, {39, 86}, {40, 70}, {27, 45}, {26, 29},
                                          {40, 15}, {45, 38}, {46, 94}, {50, 61}, {52, 87}, {59, 21}, {60, 11}, {63, 46},
                                           {80, 7}, {73, 32}, {80, 67}, {19, 71}, {90, 100}};
                     
int rekrutacja[iloscOdpowiedzi] = {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1};


void losowanieWag() {
  for (int i {0}; i<3; i++) {
      wagi[i] = ((double) random(101) / 50) - 1;
    }
}

void konfiguracjaWejsc () {
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
}
 
void setup() {
  Serial.begin(9600);
  losowanieWag();
  Serial.println((String) "Początkowe wagi: w[1]=" + wagi[0] + " w[2]=" + wagi[1] + " w[0]=" + wagi[2]);

  unsigned int start = micros(); 
  while (ileOk < iloscOdpowiedzi and epoki < limitEpok) {
    
    int liczbaBledow {0};
    for (int i {0}; i<iloscOdpowiedzi; i++) {
      suma = wagi[0] * wyniki[i][0] + wagi[1] * wyniki[i][1] + wagi[2];
      int y = suma > 0 ? 1: 0;
      if (y != rekrutacja[i]) {
        liczbaBledow++; 
      }
    }
   
    for (int i {0}; i < iloscOdpowiedzi; i++) {
      suma = wagi[0] * wyniki[i][0] + wagi[1] * wyniki[i][1] + wagi[2];
      int y = suma>0 ? 1 : 0;
      
      if (y == rekrutacja[i]) {
        ileOk ++;
      }
      else {
        for (int w {0}; w < iloscWejsc; w++) {
          wagi[w] += (rekrutacja[i] - y) * wyniki[i][w];
        }
        wagi[2] += (rekrutacja[i] - y);
        ileOk = 0;
      }
      iteracje ++;
      if (ileOk == iloscOdpowiedzi) break;
    }
    epoki ++;
  }
  unsigned int finish = micros();

  Serial.println((String) "\nUczenie trwało [epok]:       " + epoki);
  Serial.println((String) "Uczenie trwało [milisekund]: " + (finish - start));
  
  if ((iloscOdpowiedzi - ileOk) != 0) {
    Serial.println("\n ! Uczenie zakończone niepowodzeniem !");
    int liczbaBledow {0};
    for (int i; i<iloscOdpowiedzi; i++) {
      suma = wagi[0] * wyniki[i][0] + wagi[1] * wyniki[i][1] + wagi[2];
      int y = suma > 0 ? 1: 0;
      if (y != rekrutacja[i]) {
        liczbaBledow++; 
      }
    }
    Serial.println((String) "Liczba błędów: " + liczbaBledow);
  }
  else {
    Serial.println("\nUczenie zakończone sukcesem!");
    Serial.println((String) "Liczba błędów: " + (iloscOdpowiedzi - ileOk));
  }
  Serial.println((String) "Finalne wagi: w[1]=" + wagi[0] + " w[2]=" + wagi[1] + " w[0]=" + wagi[2]); 
}

void loop() {
  //procentyMatematyka = map(analogRead(matematyka), 0, 1020, 0, 100);
 // procentyFizyka = map(analogRead(fizyka), 0, 1020, 0, 100);

  //lcd.setCursor(0, 0);
  //lcd.print((String) "MATEMATYKA: " + procentyMatematyka + "%");
  //lcd.setCursor(0, 1);
  //lcd.print((String) "FIZYKA:     " + procentyFizyka + "%");
  //suma = wagi[0] * procentyFizyka + wagi[1] * procentyMatematyka + wagi[2];
  
  //if (suma > 0) {
  //  analogWrite(R, 255);
  //  analogWrite(G, 0);
  //  analogWrite(B, 255);
  //}
  //else {
  //  analogWrite(R, 0);
  //  analogWrite(G, 255);
  //  analogWrite(B, 255);
  //}
  
  //delay(400);
  //lcd.clear();
}
