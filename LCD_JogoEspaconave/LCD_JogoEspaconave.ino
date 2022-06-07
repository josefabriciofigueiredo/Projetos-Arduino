#include <LiquidCrystal.h>

#define COLS 16
#define ROWS 2
#define VELOCIDADE 300

void atualizar();
void checar();

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

String texto= "";
int aviao = 0;
int municao[3] = {2, 0, 0};
int pedra[3] = {15, 0, 0};
int tempo300 = 0;
int tempo = millis();
int pontos = 0;
int vida = 0;

byte aviaoImg[8] = {B01000, B11100, B01110, B11111, B11111, B01110, B11100, B01000};
byte pedraImg[8] = {B00000, B01110, B10001, B10001, B10001, B10001, B01110, B00000};
byte municaoImg[8] = {B00000, B00000, B00000, B11111, B11111, B00000, B00000, B00000};

void setup() {
  pinMode(10, INPUT);
  pinMode(9, INPUT);
  pinMode(8, INPUT);
  
  lcd.createChar(0, aviaoImg);
  lcd.createChar(1, pedraImg);
  lcd.createChar(2, municaoImg);
  lcd.begin(COLS, ROWS);
  
  Serial.begin(9200);
}

void loop() {
  if (vida != 3) {
    // Botão do avião
    if (digitalRead(10) == HIGH) {
      aviao = aviao == 0 ? 1:0;
      atualizar();
    }

    // Botão da munição
    if (digitalRead(9) == HIGH && municao[2] == 0) {
      municao[0] = 3;
      municao[1] = aviao;
      municao[2] = 1;
      atualizar();
    }
    
    if ((millis() - tempo) >= 100) {
      tempo = millis();
      tempo300 += 1;
      
      // Pedra
      if (tempo300 == 3) {
        tempo300 = 0;
        if (pedra[2] == 0) {
          pedra[0] = 15;
          pedra[1] = random(2);
          pedra[2] = 1;
        } else {
          pedra[0] -= 1;
        }

        if (pedra[0] == 3) {
          pedra[2] = 0;
          vida += 1;
        }
      }
      
      // Munição
      checar();
      
      if (municao[2] == 1) {
        municao[0] += 1;
      }

      checar();
      atualizar();

      if (municao[0] == 15) {
        municao[2] = 0;
      }
    }
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pontos: ");
    lcd.print(pontos);
  }
  
  delay(100);
}

void atualizar() {
  lcd.clear();
  lcd.setCursor(0, aviao);
  lcd.write(byte(0));
  
  if (pedra[2] == 1) {
    lcd.setCursor(pedra[0], pedra[1]);
    lcd.write(byte(1));
  }
  if (municao[2] == 1) {
    lcd.setCursor(municao[0], municao[1]);
    lcd.write(byte(2));
  }
}

void checar() {
  if (pedra[0] == municao[0] && municao[1] == pedra[1]) {
    pontos += 1;
    
    pedra[2] = 0;
    pedra[0] = 15;
    municao[2] = 0;
    municao[0] = 0;
  }
}
