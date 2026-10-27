#include "Game.h"
#include "Levels.h"

static Vehicle currentVehicles[MAX_VEHICLES];
static uint8_t currentVehicleCount = 0;
static uint8_t currentLevelIndex = 0;
static uint8_t cursorRow = 2;
static uint8_t cursorCol = 0;
static int8_t selectedVehicle = -1;
static uint16_t moveCount = 0;
static bool won = false;

static void loadLevel(uint8_t levelIndex) {
  if (levelIndex >= getLevelCount()) levelIndex = 0;
  currentLevelIndex = levelIndex;

  const LevelDefinition &level = getLevelDefinition(currentLevelIndex);
  currentVehicleCount = level.vehicleCount;
  for (uint8_t index = 0; index < currentVehicleCount; index++) {
    currentVehicles[index] = level.vehicles[index];
  }

  cursorRow = 2;
  cursorCol = currentVehicles[0].col;
  selectedVehicle = -1;
  moveCount = 0;
  won = false;
}

void gameBegin() {
  loadLevel(0);
}

void gameResetLevel() {
  loadLevel(currentLevelIndex);
}

void gameAdvanceLevel() {
  loadLevel((currentLevelIndex + 1) % getLevelCount());
}

int8_t gameVehicleAt(uint8_t row, uint8_t col) {
  for (uint8_t index = 0; index < currentVehicleCount; index++) {
    const Vehicle &vehicle = currentVehicles[index];
    for (uint8_t offset = 0; offset < vehicle.length; offset++) {
      const uint8_t vehicleRow = vehicle.row + (vehicle.horizontal ? 0 : offset);
      const uint8_t vehicleCol = vehicle.col + (vehicle.horizontal ? offset : 0);
      if (vehicleRow == row && vehicleCol == col) return index;
    }
  }
  return -1;
}

static bool canMove(uint8_t index, int8_t rowStep, int8_t colStep) {
  const Vehicle &vehicle = currentVehicles[index];
  if ((vehicle.horizontal && rowStep != 0) || (!vehicle.horizontal && colStep != 0)) {
    return false;
  }

  const int16_t nextRow = (int16_t)vehicle.row + rowStep;
  const int16_t nextCol = (int16_t)vehicle.col + colStep;
  if (nextRow < 0 || nextCol < 0) return false;

  const int16_t lastRow = nextRow + (vehicle.horizontal ? 0 : vehicle.length - 1);
  const int16_t lastCol = nextCol + (vehicle.horizontal ? vehicle.length - 1 : 0);
  if (lastRow >= BOARD_SIZE || lastCol >= BOARD_SIZE) return false;

  for (uint8_t offset = 0; offset < vehicle.length; offset++) {
    const uint8_t row = nextRow + (vehicle.horizontal ? 0 : offset);
    const uint8_t col = nextCol + (vehicle.horizontal ? offset : 0);
    const int8_t occupant = gameVehicleAt(row, col);
    if (occupant >= 0 && occupant != (int8_t)index) return false;
  }

  return true;
}

void gameMoveCursor(Direction direction) {
  if (direction == DIRECTION_UP && cursorRow > 0) cursorRow--;
  if (direction == DIRECTION_DOWN && cursorRow < BOARD_SIZE - 1) cursorRow++;
  if (direction == DIRECTION_LEFT && cursorCol > 0) cursorCol--;
  if (direction == DIRECTION_RIGHT && cursorCol < BOARD_SIZE - 1) cursorCol++;
}

void gameMoveSelected(Direction direction) {
  if (selectedVehicle < 0 || won) return;

  int8_t rowStep = 0;
  int8_t colStep = 0;
  if (direction == DIRECTION_UP) rowStep = -1;
  if (direction == DIRECTION_DOWN) rowStep = 1;
  if (direction == DIRECTION_LEFT) colStep = -1;
  if (direction == DIRECTION_RIGHT) colStep = 1;
  if (rowStep == 0 && colStep == 0) return;

  const uint8_t index = selectedVehicle;
  if (!canMove(index, rowStep, colStep)) return;

  currentVehicles[index].row += rowStep;
  currentVehicles[index].col += colStep;
  moveCount++;

  const Vehicle &redCar = currentVehicles[0];
  won = redCar.horizontal && redCar.row == 2 &&
        redCar.col + redCar.length - 1 == BOARD_SIZE - 1;
}

void gameButtonReleased() {
  if (won) {
    gameAdvanceLevel();
  } else if (selectedVehicle >= 0) {
    selectedVehicle = -1;
  } else {
    selectedVehicle = gameVehicleAt(cursorRow, cursorCol);
  }
}

uint8_t gameGetLevelNumber() { return currentLevelIndex + 1; }
uint8_t gameGetLevelCount() { return getLevelCount(); }
uint8_t gameGetVehicleCount() { return currentVehicleCount; }
uint16_t gameGetMoveCount() { return moveCount; }
int8_t gameGetSelectedVehicle() { return selectedVehicle; }
uint8_t gameGetCursorRow() { return cursorRow; }
uint8_t gameGetCursorCol() { return cursorCol; }
bool gameHasWon() { return won; }
const Vehicle *gameGetVehicles() { return currentVehicles; }