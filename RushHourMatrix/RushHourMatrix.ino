#include "src/Display.h"
#include "src/Game.h"
#include "src/Input.h"

void setup() {
  gameBegin();
  displayBegin();
  inputBegin();
}

void loop() {
  inputUpdate(millis());
  displayRender(millis());
}