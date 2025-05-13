#include <LiquidCrystal.h> //Dołączenie bilbioteki
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Informacja o podłączeniu nowego wyświetlacza
#define przycisk 12

int odczytPotencjometru {0};
int wybor {0};
int wybor_prev {0};

int stan {0};

String slowa[] = {
  "MARYNARZ", "LOKATOR", "GENERATOR", "SKLEPIK", 
  "POJAZD", "MAKARON", "FRYTKI", "PIZZA", 
  "PORCELANA", "APETYT", "MARATON", "KOLEGA",
  "MONITOR", "ANALIZA", "HARMONOGRAM", "WATAHA",
  "ROWER", "POGODA", "KLAUN", "KARAT", "KOALICJA"
  };

int wybrane[12] = {};

String slowo;
String pomieszany;
int numerSlowa {-1};

byte strzalka[8] = {
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00000,
  B00000,
};


void zamienLitery (int index1, int index2) {
  char temp = pomieszany[index1];
  pomieszany[index1] = pomieszany[index2];
  pomieszany[index2] = temp;
  //lcd.setCursor(0, 0);
  //lcd.print(pomieszany);
}

String wybierzSlowo () {
  int iloscSlow = sizeof(slowa)/sizeof(String);
  Serial.println(iloscSlow);
  int indexSlowa = random(iloscSlow);
  return slowa[indexSlowa];
}

String podajNastepne () {
  numerSlowa++;
  return slowa[numerSlowa];
}

String pomieszaj () {
  pomieszany = slowo;
  int dlugosc = pomieszany.length();
  for (int i = 0; i < dlugosc; i++) {
    int losowyIndex = random(dlugosc);
    char temp = pomieszany[i];
    pomieszany[i] = pomieszany[losowyIndex];
    pomieszany[losowyIndex] = temp;
  }
  //lcd.print(pomieszany);
  return(pomieszany);
}

void odNowa () {
  lcd.clear();
  slowo = wybierzSlowo();
  pomieszaj();
}


void setup() {
  pinMode(przycisk, INPUT_PULLUP);
  lcd.begin(16, 2); //Deklaracja typu
  lcd.createChar(0, strzalka);
  lcd.setCursor(0, 0); //Ustawienie kursora
  lcd.print("Zapraszam"); //Wyświetlenie tekstu
  lcd.setCursor(0, 1); //Ustawienie kursora
  lcd.print("do super gry!"); //Wyświetlenie tekstu
  delay(3000);
  lcd.clear();
  Serial.begin(1200);
}
 
void loop() {
  switch (stan) {
    case 0:
      //slowo = wybierzSlowo();
      slowo = podajNastepne();
      pomieszany = pomieszaj();
      lcd.setCursor(0, 0);
      lcd.print(pomieszany);
      stan++;
      break;

    case 1:
      lcd.setCursor(0, 0);
      lcd.print(pomieszany);
      wybor_prev = map(analogRead(A0), 0, 1024, 0, slowo.length());
      lcd.setCursor(wybor_prev, 1);
      lcd.write(byte(0));
      delay(100);
      lcd.clear();
      if (digitalRead(przycisk) == LOW) {
        delay(200);
        if (digitalRead(przycisk) == HIGH) {
          stan++;
        }
      }
      break;

    case 2:
      lcd.setCursor(0, 0);
      lcd.print(pomieszany);
      wybor = map(analogRead(A0), 0, 1024, 0, slowo.length());
      lcd.setCursor(wybor, 1);
      lcd.write(byte(0));
      delay(100);
      lcd.clear();
      if (digitalRead(przycisk) == LOW) {
        delay(200);
        if (digitalRead(przycisk) == HIGH) {
          stan++;
        }
      }
      break;

    case 3:
      lcd.setCursor(0, 0);
      lcd.print(pomieszany);
      zamienLitery(wybor_prev, wybor);
      stan++;
      break;

    case 4:
      if (pomieszany == slowo) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(pomieszany);
        lcd.setCursor(0, 1);
        lcd.print("WYGRANKO!");
        delay(3000);
        stan = 0;
        break;
        }
      else {
        stan = 1;
        break;
      }
    default:
      lcd.setCursor(0, 0);
      lcd.print(pomieszany);
  }
}
