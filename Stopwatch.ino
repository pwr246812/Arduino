#include <dht11.h>
#include <time.h>
#include <stdio.h>
#define DHT11PIN 11    //przypisanie pinu 11 Arduino jako odczyt z sensora
#include <LiquidCrystal.h> //Dołączenie bilbioteki
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Informacja o podłączeniu nowego wyświetlacza

dht11 DHT11;

unsigned long start {0};
unsigned long finish {0};
unsigned long btwin {0};
float result {0.0};
unsigned long bieg {0};
int lag {150};
String tekst = "START";


void setup()
{
  lcd.begin(16, 2); //Deklaracja typu
  lcd.clear();
  pinMode(9, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  Serial.begin(1200);
  lcd.setCursor(0, 0);
  lcd.print("lewy p  -> START");
  lcd.setCursor(0, 1);
  lcd.print("prawy p -> STOP");
}

void loop()
{ 
  lcd.display();
  btwin = millis();
  
  if (digitalRead(8) == LOW) {
    delay(lag);
    if (digitalRead(8) == HIGH) {
      start = millis();
      lcd.clear();
  
    while (digitalRead(9) == HIGH) {
        lcd.setCursor(0, 0);
        lcd.print("START ");
        lcd.setCursor(5, 1); 
        unsigned long aktualny {0};
        aktualny = millis();
        bieg = aktualny - btwin - lag;
        lcd.print((String) (bieg/1000.0) + " [s]"); 
      }
    }
  }
  if (digitalRead(9) == LOW) {
     delay(lag);
     if (digitalRead(9) == HIGH) {
        lcd.clear();
        finish = millis();
        result = (finish - start - lag);
        lcd.setCursor(0, 0);
        lcd.print("STOP");
        lcd.setCursor(5, 1);
        lcd.print((String) (result/1000.0) + " [s]");

        // blinkin
        //while (digitalRead(8) == HIGH) {
        //  lcd.noDisplay();
        //  delay(lag);
        //  lcd.display();
        //  delay(2*lag);
        //}
      }
   }
}
