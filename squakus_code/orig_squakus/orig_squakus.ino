#include <LedControl.h>
#include <LiquidCrystal.h>
#include <Encoder.h>


int DIN = 8;
int CS = 9;
int CLK = 10;
int BUTTON_PIN = 0;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

LedControl lc(DIN, CLK, CS, 1);

//const int max_size = 20;
int LED_Matrix[64];

int POTENTIOMETER_SPIKEYNESS = A0;
int POTENTIOMETER_BLOCK = A1;

void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 1);
  lc.clearDisplay(0);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  randomSeed(analogRead(0));
 
  lcd.begin(24, 2);
}

void loop() {
  int spikeyValue = analogRead(POTENTIOMETER_SPIKEYNESS);
  int spikeyness = map(spikeyValue, 0, 1023, 1, 10);
  int blockValue = analogRead(POTENTIOMETER_BLOCK);
  int max_size = map(blockValue, 0, 1023, 2, 41);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Spikeyness: " + String(spikeyness));
  lcd.setCursor(0, 1);
  lcd.print("Block Count: " + String(max_size));
  delay(250);

  if (digitalRead(BUTTON_PIN) == LOW) {
    generateBoard(spikeyness, max_size);
    displayBoard();
    while (digitalRead(BUTTON_PIN) == LOW);
    delay(500);
  }
}

void generateBoard(int spikeyness, int max_size) {
  memset(LED_Matrix, 0, sizeof(LED_Matrix));

  int current_size = 0;
  int random_initiate = random(0, 63);
  LED_Matrix[random_initiate] = 1;

  while (current_size < max_size) {
    int random_choice = random(0, 63);
    int random_below = random_choice - 8;
    int random_above = random_choice + 8;
    int random_left = (random_choice % 8 == 0) ? -1 : random_choice - 1;
    int random_right = (random_choice % 8 == 7) ? -1 : random_choice + 1;

    int adjacent = 0;

    if (random_below >= 0 && random_below < 64 && random_choice % 8 == random_below % 8) {
      if (LED_Matrix[random_below] == 1) {
        adjacent += 1;
      }
    }

    if (random_above >= 0 && random_above < 64 && random_choice % 8 == random_above % 8) {
      if (LED_Matrix[random_above] == 1) {
        adjacent += 1;
      }
    }

    if (random_left >= 0 && random_left < 64) {
      if (LED_Matrix[random_left] == 1) {
        adjacent += 1;
      }
    }

    if (random_right >= 0 && random_right < 64) {
      if (LED_Matrix[random_right] == 1) {
        adjacent += 1;
      }
    }

    if (adjacent > 0) {
      if (adjacent > 1) {
        if (adjacent == 3) {
          int random_value = random(0, 10);
          if (random_value > spikeyness * 1) {
            LED_Matrix[random_choice] = 1;
          }
        }

        if (adjacent == 2) {
          int random_value = random(0, 10);
          if (random_value > spikeyness * 0.75) {
            LED_Matrix[random_choice] = 1;
          }
        }

        if (adjacent == 4) {
          int random_value = random(0, 10);
          if (random_value > spikeyness * 1.25) {
            LED_Matrix[random_choice] = 1;
          }
        }

        current_size = 0;
        for (int i = 0; i < 64; i++) {
          current_size += LED_Matrix[i];
        }

        displayBoard();
      } else {
        LED_Matrix[random_choice] = 1;
        current_size = 0;
        for (int i = 0; i < 64; i++) {
          current_size += LED_Matrix[i];
        }

        delay(25);
        displayBoard();
      }
    }
  }
}

void displayBoard() {
  for (int i = 0; i < 8; i++) {
    byte rowData = 0;
    for (int j = 0; j < 8; j++) {
      rowData |= (LED_Matrix[i * 8 + j] << j);
    }
    lc.setRow(0, i, rowData);
  }
}