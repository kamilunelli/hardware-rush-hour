#include "Input.h"
#include "Game.h"

static const uint8_t joystickXPin = A0;
static const uint8_t joystickYPin = A1;
static const uint8_t joystickButtonPin = 2;
static const int axisLowThreshold = 350;
static const int axisHighThreshold = 650;
static const unsigned long directionRepeatMs = 220;
static const unsigned long buttonDebounceMs = 35;
static const unsigned long longPressMs = 1000;

static Direction activeDirection = DIRECTION_NONE;
static unsigned long lastDirectionAt = 0;
static int lastButtonReading = HIGH;
static int stableButtonState = HIGH;
static unsigned long lastButtonChangeAt = 0;
static unsigned long buttonPressedAt = 0;
static bool longPressHandled = false;

static Direction readDirection() {
  const int xValue = analogRead(joystickXPin);
  const int yValue = analogRead(joystickYPin);
  const int xOffset = abs(xValue - 512);
  const int yOffset = abs(yValue - 512);

  if (xOffset < 160 && yOffset < 160) return DIRECTION_NONE;

  if (xOffset >= yOffset) {
    if (xValue < axisLowThreshold) return DIRECTION_LEFT;
    if (xValue > axisHighThreshold) return DIRECTION_RIGHT;
  } else {
    if (yValue < axisLowThreshold) return DIRECTION_UP;
    if (yValue > axisHighThreshold) return DIRECTION_DOWN;
  }
  return DIRECTION_NONE;
}

static void updateJoystick(unsigned long now) {
  const Direction direction = readDirection();
  if (direction == DIRECTION_NONE) {
    activeDirection = DIRECTION_NONE;
    return;
  }

  if (direction != activeDirection || now - lastDirectionAt >= directionRepeatMs) {
    if (gameGetSelectedVehicle() < 0) {
      gameMoveCursor(direction);
    } else {
      gameMoveSelected(direction);
    }
    activeDirection = direction;
    lastDirectionAt = now;
  }
}

static void updateButton(unsigned long now) {
  const int reading = digitalRead(joystickButtonPin);
  if (reading != lastButtonReading) {
    lastButtonChangeAt = now;
    lastButtonReading = reading;
  }

  if (now - lastButtonChangeAt >= buttonDebounceMs && reading != stableButtonState) {
    stableButtonState = reading;
    if (stableButtonState == LOW) {
      buttonPressedAt = now;
      longPressHandled = false;
    } else if (!longPressHandled) {
      gameButtonReleased();
    }
  }

  if (stableButtonState == LOW && !longPressHandled && now - buttonPressedAt >= longPressMs) {
    gameResetLevel();
    longPressHandled = true;
  }
}

void inputBegin() {
  pinMode(joystickButtonPin, INPUT_PULLUP);
}

void inputUpdate(unsigned long now) {
  updateJoystick(now);
  updateButton(now);
}