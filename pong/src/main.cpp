#include <Arduino.h>
#include <TVout.h>
#include <fontALL.h>

TVout TV;

const int screenWidth = 128;
const int screenHeight = 96;
const int paddleHeight = 16;

int p1y, p2y; // Stores "last" position
int p1s, p2s; // Scores

int ballX, ballY, ballDx, ballDy; // Stores the balls position

void drawPaddles();
void drawBall();
void drawField();
void resetGame();
void resetBall();

void setup() {
  TV.begin(_PAL);
  TV.clear_screen();
  TV.select_font(font8x8);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(6, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);

  randomSeed(analogRead(2));

  resetGame();
}

void loop() {

  drawField();
  drawBall();
  drawPaddles();

  if (ballX < 0) { p2s++; resetBall(); }
  if (ballX > screenWidth) { p1s++; resetBall(); }
  if (ballY <= 0 || ballY >= screenHeight) { ballDy *= -1; }

  // Player 1 collision detection
  if (ballX == 1 && ballY > p1y && ballY < p1y + paddleHeight) {
    ballDx *= -1;
  }

  if (ballX == screenWidth - 1 && ballY > p2y && ballY < p2y + paddleHeight) {
    ballDx *= -1;
  }

  // Check for game over
  if (p1s >= 10 || p2s >= 10) {
    TV.set_cursor(10, 10);
    TV.print("The winner is:");
    TV.set_cursor(10, 18);
    TV.print(p1s > p2s ? "Player 1" : "Player 2");
    delay(5000);
    resetGame();
  }

  /*if (digitalRead(6) == LOW) {
    TV.set_cursor(10, 20 + 8);
    TV.print("Button 1 down");
  }

  if (digitalRead(5) == LOW) {
    TV.set_cursor(10, 20 + 8 + 8);
    TV.print("Button 2 down");
  }*/

  delay(25);
}

void resetGame() {
  TV.clear_screen();
  p1s = 0;
  p2s = 0;
  resetBall();
}

void resetBall() {
  ballX = screenWidth / 2;
  ballY = screenHeight / 2;
  ballDx = random(2) == 1 ? 1 : -1;
  ballDy = random(2) == 1 ? 1 : -1;
}

void drawField() {
  // Center line
  TV.draw_rect(screenWidth / 2, 0, 1, screenHeight, 1, 1);

  // Scores
  TV.set_cursor(screenWidth / 2 - 2 * 8, 0);
  TV.print(p1s);
  TV.set_cursor(screenWidth / 2 + 8, 0);
  TV.print(p2s);
}

void drawBall() {
  TV.set_pixel(ballX, ballY, 0);
  ballX += ballDx;
  ballY += ballDy;
  TV.set_pixel(ballX, ballY, 1);
}

void drawPaddles() {
  // First - draw black over the current paddles on screen
  TV.draw_rect(0, p1y, 1, paddleHeight, 0, 0);
  TV.draw_rect(screenWidth - 2, p2y, 1, paddleHeight, 0, 0);

  p1y = map(analogRead(A0), 0, 1023, screenHeight - paddleHeight, 0);
  TV.draw_rect(0, p1y, 1, paddleHeight, 1, 1);

  p2y = map(analogRead(A1), 0, 1023, screenHeight - paddleHeight, 0);
  TV.draw_rect(screenWidth - 2, p2y, 1, paddleHeight, 1, 1);
}
