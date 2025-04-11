#include <bb_spi_lcd.h> // https://github.com/bitbank2/bb_spi_lcd
#include "Roboto_Black_40.h"

BB_SPI_LCD lcd;

enum State { WORK, BREAK };
State currentState = WORK;

const uint16_t bgColors[3] = {TFT_BLACK, TFT_BLUE, TFT_YELLOW};
const uint16_t fontColorsNumbers[3] = {TFT_GREEN, TFT_WHITE, TFT_BLACK};
const uint16_t fontColorsText[3] = {TFT_GREEN, TFT_WHITE, TFT_BLACK};

const unsigned long WORK_DURATION = 25 * 60 * 1000UL;
const unsigned long BREAK_DURATION = 10 * 60 * 1000UL;

unsigned long startTime;
unsigned long duration;
bool running = true;

void setup() {
  lcd.begin(DISPLAY_T_DONGLE_S3); // built-in config for LilyGO T-Dongle-S3
  lcd.setRotation(1); // Horizontal layout (160x80)
  lcd.fillScreen(bgColors[0]);
  lcd.setFreeFont(&Roboto_Black_40);
  lcd.setTextColor(fontColorsText[0]);
  // lcd.setFont(3);

  currentState = WORK;
  duration = WORK_DURATION;
  startTime = millis();
}

void loop() {
  if (!running) return;

  unsigned long elapsed = millis() - startTime;
  if (elapsed >= duration) {
    // Toggle session
    currentState = (currentState == WORK) ? BREAK : WORK;
    duration = (currentState == WORK) ? WORK_DURATION : BREAK_DURATION;
    startTime = millis();

    lcd.fillScreen(bgColors[0]);
  }

  displayTimer(duration - (millis() - startTime));
  delay(1000); // Refresh every second
}

void displayTimer(unsigned long timeLeft) {
  int minutes = (timeLeft / 1000) / 60;
  int seconds = (timeLeft / 1000) % 60;

  int colorsMode = seconds % 2 > 0 ? 1: 2;

  char buf[16];
  sprintf(buf, "%02d:%02d", minutes, seconds);

  lcd.fillScreen(minutes < 1 ? bgColors[colorsMode]: TFT_BLACK);
  lcd.setCursor(0, 40);
  lcd.setTextColor(minutes < 1 ? fontColorsNumbers[colorsMode]: ((currentState == WORK) ? TFT_GREEN : TFT_CYAN));
  lcd.print(buf);

  lcd.setCursor(0, 80);
  lcd.setTextColor(minutes < 1 ? fontColorsText[colorsMode]: TFT_WHITE);
  lcd.print((currentState == WORK) ? "Work" : "Break");
}