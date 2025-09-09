#include <LedControl.h>
#include <Encoder.h>

// Pins for SPI communication
int DIN = 8;
int CLK = 10;

// Chip Select pins
int CS_MATRIX = 9; // For 8x8 LED matrix
int CS_7SEG = 7;   // For 7-segment displays

// Rotary encoder pins for spikeyness and block count
#define ENCODER_SPIKEYNESS_A 2
#define ENCODER_SPIKEYNESS_B 3
#define ENCODER_BLOCK_A 4
#define ENCODER_BLOCK_B 5

Encoder spikeynessEncoder(ENCODER_SPIKEYNESS_A, ENCODER_SPIKEYNESS_B);
Encoder blockEncoder(ENCODER_BLOCK_A, ENCODER_BLOCK_B);

int BUTTON_PIN = 6;

int LED_Matrix[64];

LedControl lcMatrix(DIN, CLK, CS_MATRIX, 1); // Control for 8x8 LED matrix
LedControl lc7Seg(DIN, CLK, CS_7SEG, 1);     // Control for 7-segment displays

int spikeyness = 5; // Initial spikeyness
int max_size = 20;  // Initial block count

void setup() {
  // Initialize 8x8 LED matrix
  lcMatrix.shutdown(0, false);
  lcMatrix.setIntensity(0, 1);
  lcMatrix.clearDisplay(0);

  // Initialize 7-segment displays
  lc7Seg.shutdown(0, false);
  lc7Seg.setIntensity(0, 1);
  lc7Seg.clearDisplay(0);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  randomSeed(analogRead(0));

  spikeynessEncoder.write(spikeyness);
  blockEncoder.write(max_size);
}

void loop() {
  // Read spikeyness encoder
  int newSpikeyness = constrain(spikeynessEncoder.read() / 2, 0, 9);
  if (newSpikeyness != spikeyness) {
    spikeyness = newSpikeyness;
    update7SegDisplay(0, spikeyness);
  }

  // Read block count encoder
  int newMaxSize = constrain(blockEncoder.read() / 2, 10, 40);
  if (newMaxSize != max_size) {
    max_size = newMaxSize;
    update7SegDisplay(1, max_size / 10); // Tens place
    update7SegDisplay(2, max_size % 10); // Units place
  }

  // Button press to generate and display the board
  if (digitalRead(BUTTON_PIN) == LOW) {
    generateBoard(spikeyness, max_size);
    displayBoard();
    while (digitalRead(BUTTON_PIN) == LOW); // Wait for button release
    delay(500);
  }
}

void update7SegDisplay(int digit, int value) {
  // Update specific digit on the 7-segment display
  lc7Seg.setDigit(0, digit, value, false);
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
    lcMatrix.setRow(0, i, rowData);
  }
}
