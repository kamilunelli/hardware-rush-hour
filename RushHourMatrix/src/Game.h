#ifndef RUSH_HOUR_GAME_H
#define RUSH_HOUR_GAME_H

#include <Arduino.h>

const uint8_t BOARD_SIZE = 6;
const uint8_t MAX_VEHICLES = 10;

enum Direction {
  DIRECTION_NONE,
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_RIGHT
};

struct Vehicle {
  uint8_t row;
  uint8_t col;
  uint8_t length;
  bool horizontal;
};

void gameBegin();
void gameResetLevel();
void gameAdvanceLevel();
void gameMoveCursor(Direction direction);
void gameMoveSelected(Direction direction);
void gameButtonReleased();

uint8_t gameGetLevelNumber();
uint8_t gameGetLevelCount();
uint8_t gameGetVehicleCount();
uint16_t gameGetMoveCount();
int8_t gameGetSelectedVehicle();
uint8_t gameGetCursorRow();
uint8_t gameGetCursorCol();
bool gameHasWon();
const Vehicle *gameGetVehicles();
int8_t gameVehicleAt(uint8_t row, uint8_t col);

#endif