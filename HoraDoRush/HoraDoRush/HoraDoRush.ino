
#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define LED_COUNT 64
#define MAX_CARS 8
#define LEVEL_COUNT 5

Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

struct Car {
  byte row, col, len;
  bool horizontal;
  byte color;
};

struct Level {
  byte count;
  Car cars[MAX_CARS];
};

// Coordenadas de 0 a 5 dentro do tabuleiro 6x6.
// Carro 0 = vermelho, sempre na linha 2, horizontal.
const Level levels[LEVEL_COUNT] = {
  {2, {{2,0,2,true,0}, {1,3,2,false,1}}},
  {3, {{2,0,2,true,0}, {1,3,2,false,1}, {0,2,2,false,2}}},
  {4, {{2,0,2,true,0}, {1,3,2,false,1}, {0,2,2,false,2}, {5,2,2,true,3}}},
  {5, {{2,0,2,true,0}, {1,3,2,false,1}, {0,2,2,false,2}, {5,2,2,true,3}, {3,2,3,true,4}}},
  {7, {{2,0,2,true,0}, {1,3,2,false,1}, {0,2,2,false,2}, {5,2,2,true,3}, {3,2,3,true,4}, {0,0,2,true,5}, {0,3,2,true,6}}}
};

Car cars[MAX_CARS];

byte countCars = 0;
byte levelIndex = 0;
byte current = 0;

bool selected = false;
bool won = false;
bool completed = false;

unsigned int moves = 0;
unsigned long lastFrame = 0;

uint32_t palette(byte id) {
  switch (id) {
    case 0: return leds.Color(255, 0, 0);
    case 1: return leds.Color(0, 0, 255);
    case 2: return leds.Color(255, 150, 0);
    case 3: return leds.Color(140, 0, 255);
    case 4: return leds.Color(0, 255, 0);
    case 5: return leds.Color(0, 180, 180);
    default: return leds.Color(255, 60, 120);
  }
}

int pixel(byte row, byte col) {
  return row * 8 + col;
}

void loadLevel(byte index) {
  levelIndex = index;
  countCars = levels[index].count;

  for (byte i = 0; i < countCars; i++) {
    cars[i] = levels[index].cars[i];
  }

  current = 0;
  selected = false;
  won = false;
  completed = false;
  moves = 0;

  Serial.print(F("NIVEL "));
  Serial.print(index + 1);
  Serial.println(F("/5"));
}

bool occupied(int r, int c, byte except) {
  for (byte i = 0; i < countCars; i++) {
    if (i == except) continue;

    for (byte part = 0; part < cars[i].len; part++) {
      int rr = cars[i].row + (cars[i].horizontal ? 0 : part);
      int cc = cars[i].col + (cars[i].horizontal ? part : 0);

      if (rr == r && cc == c) {
        return true;
      }
    }
  }
  return false;
}

void moveCurrent(int delta) {
  Car &car = cars[current];

  int newRow = car.row + (car.horizontal ? 0 : delta);
  int newCol = car.col + (car.horizontal ? delta : 0);

  int tipRow = newRow +
    (car.horizontal ? 0 : (delta > 0 ? car.len - 1 : 0));

  int tipCol = newCol +
    (car.horizontal ? (delta > 0 ? car.len - 1 : 0) : 0);

  if (newRow < 0 || newCol < 0 ||
      tipRow >= 6 || tipCol >= 6) {
    return;
  }

  if (occupied(tipRow, tipCol, current)) {
    return;
  }

  car.row = newRow;
  car.col = newCol;
  moves++;

  Serial.print(F("Movimentos: "));
  Serial.println(moves);

  if (current == 0 && car.col == 4) {
    won = true;
    selected = false;

    Serial.print(F("VITORIA! Movimentos: "));
    Serial.println(moves);

    if (levelIndex < LEVEL_COUNT - 1) {
      Serial.println(F("A = PROXIMO NIVEL"));
    } else {
      Serial.println(F("A = FINALIZAR"));
    }
  }
}

// COMANDOS DO CONTROLE XBOX
void command(char cmd) {

  // Y = reiniciar o nivel atual
  if (cmd == 'Y') {
    loadLevel(levelIndex);
    return;
  }

  if (completed) {
    return;
  }

  // A = avancar quando vencer
  if (won) {
    if (cmd == 'A') {
      if (levelIndex + 1 < LEVEL_COUNT) {
        loadLevel(levelIndex + 1);
      } else {
        completed = true;
        Serial.println(
          F("PARABENS! TODOS OS 5 NIVEIS CONCLUIDOS!")
        );
      }
    }
    return;
  }

  // X = proximo carro, sem reiniciar a partida
  if (cmd == 'X') {
    current = (current + 1) % countCars;
    selected = true;

    Serial.print(F("Carro selecionado: "));
    Serial.println(current + 1);
    return;
  }

  // A = selecionar o carro atual
  if (cmd == 'A') {
    selected = true;

    Serial.print(F("Carro selecionado: "));
    Serial.println(current + 1);
    return;
  }

  // Direcional antes da selecao: escolher carro
  if (!selected) {
    if (cmd == 'D' || cmd == 'S') {
      current = (current + 1) % countCars;
    }

    if (cmd == 'E' || cmd == 'W') {
      current = (current + countCars - 1) % countCars;
    }
    return;
  }

  // Direcional depois da selecao: mover carro
  if (cars[current].horizontal) {
    if (cmd == 'D') moveCurrent(1);
    if (cmd == 'E') moveCurrent(-1);
  } else {
    if (cmd == 'S') moveCurrent(1);
    if (cmd == 'W') moveCurrent(-1);
  }
}

void render() {
  leds.clear();

  bool blink = (millis() / 350) % 2 == 0;

  // Borda branca
  uint32_t border = leds.Color(35, 35, 35);

  if (won || completed) {
    border = blink
      ? leds.Color(0, 90, 0)
      : leds.Color(0, 15, 0);
  }

  // Borda da matriz 8x8
  for (byte i = 0; i < 8; i++) {
    leds.setPixelColor(pixel(0, i), border);
    leds.setPixelColor(pixel(7, i), border);
    leds.setPixelColor(pixel(i, 0), border);
    leds.setPixelColor(pixel(i, 7), border);
  }

  // Saida verde do carro vermelho
  leds.setPixelColor(
    pixel(3, 7),
    leds.Color(0, 80, 0)
  );

  // Desenhar carros
  for (byte i = 0; i < countCars; i++) {
    uint32_t color = palette(cars[i].color);

    if (i == current && !won && !completed && !blink) {
      color = selected
        ? leds.Color(50, 50, 50)
        : leds.Color(8, 8, 8);
    }

    for (byte p = 0; p < cars[i].len; p++) {
      byte row = cars[i].row +
        (cars[i].horizontal ? 0 : p);

      byte col = cars[i].col +
        (cars[i].horizontal ? p : 0);

      leds.setPixelColor(
        pixel(row + 1, col + 1),
        color
      );
    }
  }

  leds.show();
}

void setup() {
  Serial.begin(9600);
  leds.begin();

  // Brilho reduzido para alimentacao USB
  leds.setBrightness(8);

  loadLevel(0);
  render();
}

void loop() {
  while (Serial.available()) {
    command((char)Serial.read());
  }

  if (millis() - lastFrame >= 80) {
    lastFrame = millis();
    render();
  }
}
