#include "Display.h"
#include "Game.h"

static const uint8_t matrixDataPin = 12;
static const uint8_t matrixClockPin = 11;
static const uint8_t matrixLoadPin = 10;
static const unsigned long blinkIntervalMs = 300;
static uint8_t matrixRows[8];

static void writeRegister(uint8_t address, uint8_t value) {
  digitalWrite(matrixLoadPin, LOW);
  shiftOut(matrixDataPin, matrixClockPin, MSBFIRST, address);
  shiftOut(matrixDataPin, matrixClockPin, MSBFIRST, value);
  digitalWrite(matrixLoadPin, HIGH);
}

void displayBegin() {
  pinMode(matrixDataPin, OUTPUT);
  pinMode(matrixClockPin, OUTPUT);
  pinMode(matrixLoadPin, OUTPUT);
  digitalWrite(matrixLoadPin, HIGH);

  writeRegister(0x0F, 0);
  writeRegister(0x09, 0);
  writeRegister(0x0B, 7);
  writeRegister(0x0A, 3);
  writeRegister(0x0C, 1);
  for (uint8_t row = 0; row < 8; row++) {
    writeRegister(row + 1, 0);
  }
}

void displayRender(unsigned long now) {
  for (uint8_t row = 0; row < 8; row++) matrixRows[row] = 0;

  const bool blinkOn = (now / blinkIntervalMs) % 2 == 0;
  const Vehicle *vehicles = gameGetVehicles();
  const uint8_t count = gameGetVehicleCount();
  const int8_t selected = gameGetSelectedVehicle();

  for (uint8_t index = 0; index < count; index++) {
    if ((int8_t)index == selected && !blinkOn) continue;
    const Vehicle &vehicle = vehicles[index];
    for (uint8_t offset = 0; offset < vehicle.length; offset++) {
      const uint8_t row = vehicle.row + (vehicle.horizontal ? 0 : offset);
      const uint8_t col = vehicle.col + (vehicle.horizontal ? offset : 0);
      matrixRows[row] |= (uint8_t)(1U << col);
    }
  }

  matrixRows[2] |= (uint8_t)(1U << BOARD_SIZE);

  if (blinkOn && !gameHasWon()) {
    matrixRows[gameGetCursorRow()] ^= (uint8_t)(1U << gameGetCursorCol());
  }

  if (gameHasWon() && blinkOn) {
    for (uint8_t row = 0; row < BOARD_SIZE; row++) matrixRows[row] = 0x7F;
  }

  for (uint8_t row = 0; row < 8; row++) {
    writeRegister(row + 1, matrixRows[row]);
  }
}