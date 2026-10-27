#ifndef RUSH_HOUR_LEVELS_H
#define RUSH_HOUR_LEVELS_H

#include "Game.h"

struct LevelDefinition {
  const Vehicle *vehicles;
  uint8_t vehicleCount;
  uint8_t optimalMoves;
};

const LevelDefinition &getLevelDefinition(uint8_t levelIndex);
uint8_t getLevelCount();

#endif