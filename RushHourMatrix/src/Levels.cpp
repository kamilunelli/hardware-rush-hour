#include "Levels.h"

static const Vehicle level1[] = {
  {2, 1, 2, true}, {4, 1, 3, true}, {1, 5, 2, false}, {1, 1, 2, true},
  {0, 0, 2, false}, {0, 1, 2, true}, {3, 0, 3, false}
};

static const Vehicle level2[] = {
  {2, 0, 2, true}, {2, 3, 3, false}, {5, 1, 3, true}, {1, 5, 3, false},
  {0, 1, 2, true}
};

static const Vehicle level3[] = {
  {2, 1, 2, true}, {2, 0, 3, false}, {5, 4, 2, true}, {0, 3, 2, false},
  {3, 1, 3, false}, {4, 3, 3, true}, {0, 0, 2, false}, {2, 5, 2, false},
  {1, 4, 3, false}
};

static const Vehicle level4[] = {
  {2, 0, 2, true}, {4, 3, 2, false}, {5, 1, 2, true}, {1, 5, 3, false},
  {3, 1, 3, true}, {0, 4, 2, true}, {1, 0, 3, true}, {3, 0, 3, false},
  {4, 4, 2, true}, {0, 1, 2, true}
};

static const Vehicle level5[] = {
  {2, 0, 2, true}, {0, 2, 2, true}, {5, 4, 2, true}, {1, 3, 2, false},
  {2, 4, 3, false}, {2, 5, 3, false}, {3, 0, 3, true}, {4, 2, 2, false}
};

static const Vehicle level6[] = {
  {2, 3, 2, true}, {4, 3, 2, false}, {0, 5, 3, false}, {5, 0, 3, true},
  {2, 1, 3, false}, {0, 2, 3, false}, {3, 2, 2, true}, {4, 4, 2, true},
  {0, 0, 3, false}
};

static const Vehicle level7[] = {
  {2, 0, 2, true}, {0, 0, 2, true}, {0, 3, 3, false}, {4, 3, 2, true},
  {3, 2, 3, false}, {5, 4, 2, true}, {0, 5, 3, false}, {1, 4, 3, false},
  {4, 0, 2, false}, {3, 1, 2, false}
};

static const Vehicle level8[] = {
  {2, 0, 2, true}, {1, 4, 3, false}, {5, 3, 2, true}, {2, 5, 3, false},
  {0, 0, 3, true}, {1, 2, 3, false}, {0, 3, 2, false}, {4, 2, 2, true},
  {3, 0, 2, false}, {3, 1, 2, false}
};

static const Vehicle level9[] = {
  {2, 0, 2, true}, {3, 1, 3, true}, {1, 2, 2, false}, {3, 0, 3, false},
  {0, 1, 2, true}, {3, 5, 2, false}, {5, 2, 3, true}, {0, 3, 3, false}
};

static const Vehicle level10[] = {
  {2, 2, 2, true}, {3, 3, 2, false}, {1, 3, 2, true}, {3, 0, 2, true},
  {5, 3, 3, true}, {4, 4, 2, true}, {0, 5, 3, false}, {0, 1, 3, true},
  {3, 2, 3, false}, {0, 0, 3, false}
};

static const LevelDefinition levels[] = {
  {level1, sizeof(level1) / sizeof(level1[0]), 2},
  {level2, sizeof(level2) / sizeof(level2[0]), 4},
  {level3, sizeof(level3) / sizeof(level3[0]), 7},
  {level4, sizeof(level4) / sizeof(level4[0]), 9},
  {level5, sizeof(level5) / sizeof(level5[0]), 11},
  {level6, sizeof(level6) / sizeof(level6[0]), 14},
  {level7, sizeof(level7) / sizeof(level7[0]), 16},
  {level8, sizeof(level8) / sizeof(level8[0]), 18},
  {level9, sizeof(level9) / sizeof(level9[0]), 21},
  {level10, sizeof(level10) / sizeof(level10[0]), 25}
};

uint8_t getLevelCount() {
  return sizeof(levels) / sizeof(levels[0]);
}

const LevelDefinition &getLevelDefinition(uint8_t levelIndex) {
  if (levelIndex >= getLevelCount()) levelIndex = 0;
  return levels[levelIndex];
}