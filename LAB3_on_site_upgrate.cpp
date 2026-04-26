#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

// Definicje pinów i parametrów
#define OLED_ADDR 0x3C
#define OLED_5DA A5
#define OLED_4DA A4
#define JOY_X A0
#define JOY_Y A1
#define JOY_PIN 2
#define BUZZER_PIN 8
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SNAKE_SIZE 4
#define MAX_SNAKE_SIZE 100

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
RTC_DS1307 rtc;

// Zmienne gry
int snakeX[MAX_SNAKE_SIZE];
int snakeY[MAX_SNAKE_SIZE];
int snakeSize;
int snakeDir; // 0: góra, 1: prawo, 2: dół, 3: lewo
int foodX, foodY;
bool gameOver = false;
bool gamePaused = true;
int score = 0;
int gameSpeed = 100;
bool buttonReleased = true;

// ==================== DŹWIĘKI ====================

void playEatSound() {
  tone(BUZZER_PIN, 440, 30);
  delay(40);
  tone(BUZZER_PIN, 523, 40);
  delay(50);
  noTone(BUZZER_PIN);
}

void playPauseSound() {
  tone(BUZZER_PIN, 440, 35);
  delay(45);
  noTone(BUZZER_PIN);
}

void playResumeSound() {
  tone(BUZZER_PIN, 494, 35);
  delay(45);
  noTone(BUZZER_PIN);
}

void playGameOverSound() {
  tone(BUZZER_PIN, 330, 70);
  delay(85);
  tone(BUZZER_PIN, 294, 90);
  delay(105);
  tone(BUZZER_PIN, 262, 120);
  delay(140);
  noTone(BUZZER_PIN);
}

// ==================== SETUP ====================

void setup() {
  pinMode(JOY_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    for (;;);
  }

  if (!rtc.begin()) {
    // RTC nie znalezione
  }

  randomSeed(analogRead(A3));

  initSnake();
  spawnFood();

  display.clearDisplay();
  display.setTextColor(WHITE);
}

// ==================== GRA ====================

void initSnake() {
  snakeSize = SNAKE_SIZE;
  int startX = (SCREEN_WIDTH / 2 / 4) * 4;
  int startY = ((SCREEN_HEIGHT / 2) / 4) * 4;

  for (int i = 0; i < snakeSize; i++) {
    snakeX[i] = startX - (i * 4);
    snakeY[i] = startY;
  }

  snakeDir = 1;
  score = 0;
}

void spawnFood() {
  foodX = random(0, (SCREEN_WIDTH - 4) / 4) * 4;
  foodY = random(3, SCREEN_HEIGHT / 4) * 4; // omija górny pasek z RTC
}

void handleInput() {
  int xVal = analogRead(JOY_X);
  int yVal = analogRead(JOY_Y);

  // Lewo / Prawo
  if (xVal < 200 && snakeDir != 1) {
    snakeDir = 3;
  }
  else if (xVal > 800 && snakeDir != 3) {
    snakeDir = 1;
  }
  // Góra / Dół
  else if (yVal > 800 && snakeDir != 2) {
    snakeDir = 2;
  }
  else if (yVal < 200 && snakeDir != 0) {
    snakeDir = 0;
  }

  // Pauza / wznowienie przyciskiem joysticka
  if (digitalRead(JOY_PIN) == LOW && buttonReleased) {
    gamePaused = !gamePaused;
    buttonReleased = false;

    if (gamePaused) playPauseSound();
    else playResumeSound();

    delay(50);
  }

  if (digitalRead(JOY_PIN) == HIGH) {
    buttonReleased = true;
  }
}

void updateLogic() {
  if (gamePaused || gameOver) return;

  // Przesunięcie ogona
  for (int i = snakeSize - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  // Ruch głowy
  if (snakeDir == 0) snakeY[0] -= 4;
  if (snakeDir == 1) snakeX[0] += 4;
  if (snakeDir == 2) snakeY[0] += 4;
  if (snakeDir == 3) snakeX[0] -= 4;

  // Kolizja ze ścianami dla X
  if (snakeX[0] < 0) {
    snakeX[0] = SCREEN_WIDTH - 4;
  }
  else if  (snakeX[0] >= SCREEN_WIDTH) {
    snakeX[0] = 0;
  }

  // Kolizja ze ścianami dla Y
  if (snakeY[0] < 10) {
    snakeY[0] = SCREEN_HEIGHT - 4;
  }
  else if (snakeY[0] >= SCREEN_HEIGHT) {
    snakeY[0] = 10;
  }

  // Kolizja z samym sobą
  for (int i = 1; i < snakeSize; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      endGame();
      return;
    }
  }

  // Jedzenie
  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    score += 10;

    if (snakeSize < MAX_SNAKE_SIZE) {
      snakeX[snakeSize] = snakeX[snakeSize - 1];
      snakeY[snakeSize] = snakeY[snakeSize - 1];
      snakeSize++;
    }

    playEatSound();
    spawnFood();
  }
}

void endGame() {
  gameOver = true;
  playGameOverSound();
}

// ==================== RYSOWANIE ====================

void draw() {
  display.clearDisplay();

  // Zegar z RTC
  DateTime now = rtc.now();
  display.setTextSize(1);
  display.setCursor(0, 0);

  display.print(now.year(), DEC);
  display.print('/');

  if (now.month() < 10) display.print('0');
  display.print(now.month(), DEC);
  display.print('/');

  if (now.day() < 10) display.print('0');
  display.print(now.day(), DEC);
  display.print(' ');

  if (now.hour() < 10) display.print('0');
  display.print(now.hour(), DEC);
  display.print(':');

  if (now.minute() < 10) display.print('0');
  display.print(now.minute(), DEC);

  if (gamePaused && !gameOver) {
    display.setCursor(28, 30);
    display.print("GAME PAUSED");
  }
  else if (gameOver) {
    display.setCursor(32, 24);
    display.print("GAME OVER!");
    display.setCursor(36, 40);
    display.print("Score: ");
    display.print(score);
  }
  else {
    // Wąż
    for (int i = 0; i < snakeSize; i++) {
      display.fillRect(snakeX[i], snakeY[i], 4, 4, WHITE);
    }

    // Jedzenie
    display.fillCircle(foodX + 2, foodY + 2, 2, WHITE);
  }

  display.display();
}

// ==================== LOOP ====================

void loop() {
  handleInput();
  updateLogic();
  draw();
  delay(gameSpeed);

  if (gameOver && digitalRead(JOY_PIN) == LOW) {
    gameOver = false;
    initSnake();
    spawnFood();
    gamePaused = false;
    playResumeSound();
    delay(500);
  }
}
