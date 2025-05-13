#define x1 13
#define x2 12
#define led_x1 5
#define led_x2 4
#define led_y 3


void konfiguracjaWejsc () {
  pinMode(led_x1, OUTPUT);
  pinMode(led_x2, OUTPUT);
  pinMode(led_y, OUTPUT);
  pinMode(x1, INPUT_PULLUP);
  pinMode(x2, INPUT_PULLUP);
}

int x1_val {0};
int x2_val {0};


int wejscia[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
//Ciąg uczący do uzupełnienia dla konkretnej funkcji logicznej
int odpowiedzi[4] = {0, 1, 1, 0};

double wagi[3][3] = {{0.1, 0.1, 0.1}, {0.2, 0.2, 0.2}, {-0.4, -0.4, -0.4}};

void losowanieWag() {
  for (int i {0}; i<3; i++) {
    for (int j {0}; j<3; j++) {
      wagi[i][j] = ((double) random(101) / 50) - 1;
    }
  }
}

double f(double s) {
  return (1/(1+ exp(-s)));
}

long epoki {0};
unsigned int limitEpok {2000};
double gamma {0.3};
double sumy[3] = {0.0, 0.0, 0.0};
int blad {0};

void setup() {
  konfiguracjaWejsc();
  losowanieWag();
  Serial.begin(9600);
  Serial.println((String) "Rozpoczynam uczenie dla funkcji o odpowiedziach:   " + odpowiedzi[0] + ", " + odpowiedzi[1] + ", " + odpowiedzi[2] + ", " + odpowiedzi[3]);
  Serial.println((String) "\nWagi początkowe:  w[1][1]=" + wagi[0][0] + "   w[1][2]=" + wagi[0][1] + "   w[1][0]=" + wagi[0][2]);
  Serial.println((String)   "                  w[2][1]=" + wagi[1][0] + "   w[2][2]=" + wagi[1][1] + "   w[2][0]=" + wagi[1][2]);
  Serial.println((String)   "                  w[3][1]=" + wagi[2][0] + "   w[3][2]=" + wagi[2][1] + "   w[3][0]=" + wagi[2][2]);

  double bladCalkowity {1.0};
  unsigned long start = millis();

  while (epoki < limitEpok) {
    bladCalkowity = 0.0;
    for (int i {0}; i<4; i++) {
      
      sumy[0] = wagi[0][0] * wejscia[i][0] + wagi[0][1] * wejscia[i][1] + wagi[0][2];
      sumy[1] = wagi[1][0] * wejscia[i][0] + wagi[1][1] * wejscia[i][1] + wagi[1][2];
      sumy[2] = wagi[2][0] * f(sumy[0]) + wagi[2][1] * f(sumy[1]) + wagi[2][2];

      double blad = sq(odpowiedzi[i] - f(sumy[2]));
      bladCalkowity += blad;
            
      //Pierwszy perceptron pierwszej warstwy
      wagi[0][0] -= gamma * 2 * (odpowiedzi[i] - f(sumy[2])) * (-1) * f(sumy[2]) * (1-f(sumy[2])) * wagi[2][0] * f(sumy[0]) * (1-f(sumy[0])) * wejscia[i][0];
      wagi[0][1] -= gamma * 2 * (odpowiedzi[i] - f(sumy[2])) * (-1) * f(sumy[2]) * (1-f(sumy[2])) * wagi[2][0] * f(sumy[0]) * (1-f(sumy[0])) * wejscia[i][1];
      wagi[0][2] -= gamma * 2 * (odpowiedzi[i] - f(sumy[2])) * (-1) * f(sumy[2]) * (1-f(sumy[2])) * wagi[2][0] * f(sumy[0]) * (1-f(sumy[0]));

      
      //Drugi perceptron pierwszej warstwy
      wagi[1][0] -= gamma * 2 * (odpowiedzi[i] - f(sumy[2])) * (-1) * f(sumy[2]) * (1-f(sumy[2])) * wagi[2][1] * f(sumy[1]) * (1-f(sumy[1])) * wejscia[i][0];
      wagi[1][1] -= gamma * 2 * (odpowiedzi[i] - f(sumy[2])) * (-1) * f(sumy[2]) * (1-f(sumy[2])) * wagi[2][1] * f(sumy[1]) * (1-f(sumy[1])) * wejscia[i][1];  
      wagi[1][2] -= gamma * 2 * (odpowiedzi[i] - f(sumy[2])) * (-1) * f(sumy[2]) * (1-f(sumy[2])) * wagi[2][1] * f(sumy[1]) * (1-f(sumy[1]));

      //Perceptron drugiej warstwy
      wagi[2][0] -= gamma * 2 *(odpowiedzi[i] - f(sumy[2])) * (-1) * f(sumy[2]) * (1-f(sumy[2])) * f(sumy[0]);
      wagi[2][1] -= gamma * 2 *(odpowiedzi[i] - f(sumy[2])) * (-1) * f(sumy[2]) * (1-f(sumy[2])) * f(sumy[1]);
      wagi[2][2] -= gamma * 2 *(odpowiedzi[i] - f(sumy[2])) * (-1) * f(sumy[2]) * (1-f(sumy[2]));
    }
    
    int blad {0};
    for (int i {0}; i<4; i++) { 
      sumy[0] = wagi[0][0] * wejscia[i][0] + wagi[0][1] * wejscia[i][1] + wagi[0][2];
      sumy[1] = wagi[1][0] * wejscia[i][0] + wagi[1][1] * wejscia[i][1] + wagi[1][2];    
      double suma4 = wagi[2][0] * f(sumy[0]) + wagi[2][1] * f(sumy[1]) + wagi[2][2]; 
      bool y = f(suma4) >= 0.5 ? 1 : 0; 
      blad += (y!=odpowiedzi[i]);
    }
    epoki++;
    if (blad == 0) {
      Serial.println("\nUczenie zakończone sukcesem!");
      break;
    }
  }  
  unsigned long finish = millis();
  Serial.println((String) "\nUczenie trwało [epok]:       " + epoki);
  Serial.println((String) "Uczenie trwało [milisekund]: " + (finish - start)); 
  Serial.println((String) "\nWagi końcowe:   w[1][1]=" + wagi[0][0] + "  w[1][2]=" + wagi[0][1] + "  w[1][0]= " + wagi[0][2]);
  Serial.println((String)   "                w[2][1]=" + wagi[1][0] + "  w[2][2]=" + wagi[1][1] + "  w[2][0]= " + wagi[1][2]);
  Serial.println((String)   "                w[3][1]=" + wagi[2][0] + "  w[3][2]=" + wagi[2][1] + "  w[3][0]= " + wagi[2][2]);
  Serial.println((String) "\nFinalny, sumaryczny dla całego ciągu uczącego, błąd:   " + bladCalkowity); 
//  Serial.println(wagi[0][0], 4);
//  Serial.println(wagi[0][1], 4);
//  Serial.println(wagi[0][2], 4);
//  Serial.println(wagi[1][0], 4);
//  Serial.println(wagi[1][1], 4);
//  Serial.println(wagi[1][2], 4);
//  Serial.println(wagi[2][0], 4);
//  Serial.println(wagi[2][1], 4);
//  Serial.println(wagi[2][2], 4);
}


void loop() {
  if (digitalRead(x1) == LOW) {
      digitalWrite(led_x1, HIGH);
      x1_val = 1;
  }
  else {
      digitalWrite(led_x1, LOW);
      x1_val = 0;
  }
  if (digitalRead(x2) == LOW) {
      digitalWrite(led_x2, HIGH);
      x2_val = 1;
  }
  else {
      digitalWrite(led_x2, LOW);
      x2_val = 0;
  }

  double suma1 = wagi[0][0] * x1_val + wagi[0][1] * x2_val + wagi[0][2];
  double suma2 = wagi[1][0] * x1_val + wagi[1][1] * x2_val + wagi[1][2];
  double suma3 = wagi[2][0] * f(suma1) + wagi[2][1] * f(suma2) + wagi[2][2]; 
  double y = f(suma3); 
  int result = y > 0.5 ? 1: 0;
  
  if (result == 1) {
    digitalWrite(led_y, HIGH);
  }
  else {
    digitalWrite(led_y, LOW);
  }
}
