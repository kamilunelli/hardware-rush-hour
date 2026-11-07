
#include <Adafruit_NeoPixel.h>
#include <avr/pgmspace.h>

#define LED_PIN 6
#define LED_COUNT 64
#define MAX_CARS 8
#define LEVEL_COUNT 6
#define PROTOCOL_VERSION 2

Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

struct Car {
  byte row, col, len;
  bool horizontal;
  byte color;
};

struct Level {
  byte count;
  byte optimal;  // solucao otima (deslizar N casas = 1 movimento)
  Car cars[MAX_CARS];
};

// Coordenadas de 0 a 5 dentro do tabuleiro 6x6.
// Carro 0 = vermelho, sempre na linha 2, horizontal.
// Cada veiculo do nivel tem uma cor diferente (ids 0..7, ver palette()).
// Niveis pensados para criancas: tutorial, 1 facil, 2 medios e 2 "desafios".
// Gerados e validados por busca exaustiva (BFS), sem sobreposicao - o otimo esta em "optimal".
const Level levels[LEVEL_COUNT] PROGMEM = {
  // Nivel 1 - Tutorial (2 movimentos)
  {4, 2, {
    {2, 0, 2, true, 0},  // vermelho
    {1, 4, 2, false, 1}, // azul
    {4, 2, 2, true, 2},  // verde
    {3, 1, 3, false, 4}, // roxo
  }},
  // Nivel 2 - Facil (3 movimentos)
  {5, 3, {
    {2, 0, 2, true, 0},  // vermelho
    {1, 2, 2, false, 1}, // azul
    {4, 2, 2, false, 2}, // verde
    {4, 3, 3, true, 4},  // roxo
    {0, 2, 3, true, 5},  // ciano
  }},
  // Nivel 3 - Medio (5 movimentos)
  {6, 5, {
    {2, 0, 2, true, 0},  // vermelho
    {1, 2, 2, false, 1}, // azul
    {3, 0, 3, false, 4}, // roxo
    {2, 3, 3, false, 5}, // ciano
    {2, 4, 3, false, 6}, // rosa
    {2, 5, 3, false, 7}, // laranja
  }},
  // Nivel 4 - Medio (6 movimentos)
  {6, 6, {
    {2, 0, 2, true, 0},  // vermelho
    {1, 2, 2, false, 1}, // azul
    {2, 4, 3, false, 4}, // roxo
    {0, 3, 3, false, 5}, // ciano
    {2, 5, 3, false, 6}, // rosa
    {3, 1, 3, true, 7},  // laranja
  }},
  // Nivel 5 - Desafio (7 movimentos)
  {7, 7, {
    {2, 0, 2, true, 0},  // vermelho
    {0, 3, 2, true, 1},  // azul
    {4, 0, 2, true, 2},  // verde
    {2, 2, 3, false, 4}, // roxo
    {2, 5, 3, false, 5}, // ciano
    {0, 0, 3, true, 6},  // rosa
    {5, 2, 3, true, 7},  // laranja
  }},
  // Nivel 6 - Desafio (8 movimentos)
  {7, 8, {
    {2, 0, 2, true, 0},  // vermelho
    {3, 1, 2, true, 1},  // azul
    {4, 2, 2, false, 2}, // verde
    {5, 3, 3, true, 4},  // roxo
    {3, 0, 3, false, 5}, // ciano
    {2, 4, 3, false, 6}, // rosa
    {0, 0, 3, true, 7},  // laranja
  }},
};

// Estado do jogo
enum Status : byte { IDLE = 0, PLAYING = 1, WON = 2, COMPLETED = 3 };

Car cars[MAX_CARS];

byte countCars = 0;
byte levelIndex = 0;
byte optimalMoves = 0;
byte current = 0;
Status status = IDLE;
bool selected = false;

unsigned int moves = 0;
unsigned int wins = 0;  // niveis vencidos desde que o Arduino ligou

// Passos seguidos do mesmo carro na mesma direcao contam como 1 movimento
// (igual ao jogo web, onde arrastar N casas = 1 movimento).
byte lastMovedCar = 255;
int lastDelta = 0;

unsigned long lastFrame = 0;
bool stateDirty = true;

// Comandos de 2 bytes (L<n> e C<n>) aguardam o argumento aqui.
char pendingCmd = 0;

uint32_t palette(byte id) {
  switch (id) {
    case 0: return leds.Color(255, 0, 0);    // vermelho
    case 1: return leds.Color(0, 0, 255);    // azul
    case 2: return leds.Color(0, 255, 0);    // verde
    case 3: return leds.Color(255, 170, 0);  // amarelo
    case 4: return leds.Color(130, 0, 255);  // roxo
    case 5: return leds.Color(0, 200, 200);  // ciano
    case 6: return leds.Color(255, 30, 150); // rosa
    default: return leds.Color(255, 60, 0);  // laranja
  }
}

int pixel(byte row, byte col) {
  return row * 8 + col;
}

// Linha de estado lida pelo site (Modo Arduino). Formato:
// @S <nivel> <movimentos> <otimo> <vitorias> <status> <atual> <selecionado> <carros...>
// cada carro = 5 caracteres: linha, coluna, tamanho, H/V, cor. Ex.: 202H0
void sendState() {
  Serial.print(F("@S "));
  Serial.print(status == IDLE ? 0 : levelIndex + 1);
  Serial.print(' ');
  Serial.print(moves);
  Serial.print(' ');
  Serial.print(optimalMoves);
  Serial.print(' ');
  Serial.print(wins);
  Serial.print(' ');
  Serial.print((byte)status);
  Serial.print(' ');
  Serial.print(current);
  Serial.print(' ');
  Serial.print(selected ? 1 : 0);

  if (status != IDLE) {
    for (byte i = 0; i < countCars; i++) {
      Serial.print(' ');
      Serial.print(cars[i].row);
      Serial.print(cars[i].col);
      Serial.print(cars[i].len);
      Serial.print(cars[i].horizontal ? 'H' : 'V');
      Serial.print(cars[i].color);
    }
  }
  Serial.println();
  stateDirty = false;
}

void resetTurn() {
  lastMovedCar = 255;
  lastDelta = 0;
}

void loadLevel(byte index) {
  Level lv;
  memcpy_P(&lv, &levels[index], sizeof(Level));

  levelIndex = index;
  countCars = lv.count;
  optimalMoves = lv.optimal;

  for (byte i = 0; i < countCars; i++) {
    cars[i] = lv.cars[i];
  }

  current = 0;
  selected = false;
  status = PLAYING;
  moves = 0;
  resetTurn();
  stateDirty = true;

  Serial.print(F("NIVEL "));
  Serial.print(index + 1);
  Serial.print('/');
  Serial.println(LEVEL_COUNT);
}

void cancelLevel() {
  status = IDLE;
  selected = false;
  countCars = 0;
  moves = 0;
  resetTurn();
  stateDirty = true;
  Serial.println(F("NIVEL CANCELADO"));
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

void focusCar(byte index) {
  current = index;
  selected = true;
  resetTurn();
  stateDirty = true;

  Serial.print(F("Carro selecionado: "));
  Serial.println(current + 1);
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

  if (current != lastMovedCar || delta != lastDelta) {
    moves++;
    lastMovedCar = current;
    lastDelta = delta;
  }
  stateDirty = true;

  Serial.print(F("Movimentos: "));
  Serial.println(moves);

  if (current == 0 && car.col == 4) {
    status = WON;
    selected = false;
    wins++;

    Serial.print(F("VITORIA! Movimentos: "));
    Serial.print(moves);
    Serial.print(F(" (otimo: "));
    Serial.print(optimalMoves);
    Serial.println(')');
    Serial.print(F("Vitorias: "));
    Serial.println(wins);

    if (levelIndex < LEVEL_COUNT - 1) {
      Serial.println(F("A = PROXIMO NIVEL"));
    } else {
      Serial.println(F("A = FINALIZAR"));
    }
  }
}

// Comandos recebidos pela serial (controle Xbox via controle.py ou site).
//   A X Y D E W S  -> botoes e direcional
//   L<1..6>        -> carregar nivel
//   C<0..7>        -> selecionar o carro de indice n (usado pelo Resolver)
//   Q              -> cancelar o nivel (tabuleiro vazio)
//   ?              -> pedir o estado atual
void command(char cmd) {

  // Segundo byte dos comandos L<n> e C<n>
  if (pendingCmd) {
    char op = pendingCmd;
    pendingCmd = 0;

    if (op == 'L' && cmd >= '1' && cmd < '1' + LEVEL_COUNT) {
      loadLevel(cmd - '1');
    } else if (op == 'C' && status == PLAYING &&
               cmd >= '0' && cmd < '0' + countCars) {
      focusCar(cmd - '0');
    }
    return;
  }

  if (cmd == 'L' || cmd == 'C') {
    pendingCmd = cmd;
    return;
  }

  if (cmd == '?') {
    Serial.print(F("@HELLO "));
    Serial.print(PROTOCOL_VERSION);
    Serial.print(' ');
    Serial.println(LEVEL_COUNT);
    stateDirty = true;
    return;
  }

  if (cmd == 'Q') {
    cancelLevel();
    return;
  }

  if (status == IDLE) {
    return;
  }

  // Y = reiniciar o nivel atual
  if (cmd == 'Y') {
    loadLevel(levelIndex);
    return;
  }

  if (status == COMPLETED) {
    return;
  }

  // A = avancar quando vencer
  if (status == WON) {
    if (cmd == 'A') {
      if (levelIndex + 1 < LEVEL_COUNT) {
        loadLevel(levelIndex + 1);
      } else {
        status = COMPLETED;
        stateDirty = true;
        Serial.println(
          F("PARABENS! TODOS OS NIVEIS CONCLUIDOS!")
        );
      }
    }
    return;
  }

  // X = proximo carro, sem reiniciar a partida
  if (cmd == 'X') {
    focusCar((current + 1) % countCars);
    return;
  }

  // A = selecionar o carro atual
  if (cmd == 'A') {
    focusCar(current);
    return;
  }

  // Direcional antes da selecao: escolher carro
  if (!selected) {
    if (cmd == 'D' || cmd == 'S') {
      current = (current + 1) % countCars;
      stateDirty = true;
    }

    if (cmd == 'E' || cmd == 'W') {
      current = (current + countCars - 1) % countCars;
      stateDirty = true;
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
  bool finished = status == WON || status == COMPLETED;

  // Borda branca (apagada quando nao ha nivel)
  uint32_t border = leds.Color(35, 35, 35);

  if (status == IDLE) {
    border = blink ? leds.Color(12, 12, 12) : leds.Color(4, 4, 4);
  } else if (finished) {
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

  if (status != IDLE) {
    // Saida verde do carro vermelho
    leds.setPixelColor(
      pixel(3, 7),
      leds.Color(0, 80, 0)
    );
  }

  // Desenhar carros
  for (byte i = 0; i < countCars; i++) {
    uint32_t color = palette(cars[i].color);

    if (i == current && !finished && !blink) {
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
  Serial.begin(115200);
  leds.begin();

  // Brilho reduzido para alimentacao USB. Com valores muito baixos (ex.: 8)
  // as cores parecidas (vermelho/laranja/rosa) ficam indistinguiveis.
  leds.setBrightness(20);

  Serial.print(F("@HELLO "));
  Serial.print(PROTOCOL_VERSION);
  Serial.print(' ');
  Serial.println(LEVEL_COUNT);

  loadLevel(0);
  render();
}

void loop() {
  while (Serial.available()) {
    command((char)Serial.read());
  }

  if (stateDirty) {
    sendState();
  }

  if (millis() - lastFrame >= 80) {
    lastFrame = millis();
    render();
  }
}
