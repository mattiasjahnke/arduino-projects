#include <Arduboy2.h>

Arduboy2 arduboy;

#define BLOCKS_COLS 7
#define BLOCKS_ROWS 5

#define BLOCK_WIDTH 15
#define BLOCK_HEIGHT 3

int level[] = { 0x7f, 0x3e, 0x1c, 0x3e, 0x7f };

int paddleWidth = 20;
float paddleX = 0;

float ballX = 0;
float ballY = 0;
float ballVelX = 0;
float ballVelY = 0;
bool idleBall = true;

void setup() {
  arduboy.boot();
  arduboy.blank();
  arduboy.setFrameRate(60);
  arduboy.display();

  paddleX = WIDTH / 2 - paddleWidth / 2;
}

void handleGameInput() {
  if (arduboy.pressed(LEFT_BUTTON)) {
    paddleX -= 2.5;
    if (paddleX < 0) {
      paddleX = 0;
    }
  } else if (arduboy.pressed(RIGHT_BUTTON)) {
    paddleX += 2.5;
    if (paddleX + paddleWidth > WIDTH) {
      paddleX = WIDTH - paddleWidth;
    }
  }

  if (idleBall) {
    ballX = paddleX + paddleWidth / 2;
    ballY = 56;
  }

  if (arduboy.pressed(A_BUTTON) && idleBall) {
    idleBall = false;
    ballVelY = -1;
    ballVelX = random(3) - 1;
  }
}

void moveBall() {
  ballX += ballVelX;
  ballY += ballVelY;

  // == CHECK FOR SCREEN BOUNDS ==
  if (ballX <= 0) {
    ballVelX *= -1;
  } else if (ballX + 4 >= WIDTH) {
    ballVelX *= -1;
  }

  if (ballY <= 0) {
    ballVelY *= -1;
  } else if (ballY > HEIGHT) {
    idleBall = true;
    return;
  }

  // == CHECK FOR PADDLE COLLISION ==
  if (ballY >= HEIGHT - 4 - 4) {
    if (ballX >= paddleX && ballX <= (paddleX + paddleWidth)) {
      ballVelY *= -1;

      float offset = (ballX + 4 - paddleWidth) / (paddleWidth + 4);
      float phi = 0.25 * PI * (2 * offset - 1);
      ballVelX = 1.25 * sin(phi);
    }
  }

  Rect ballRect = Rect { ballX, ballY, 4, 4 };

  // == CHECK FOR BLOCK COLLISION ==
  for (int y = 0; y < BLOCKS_ROWS; y++) {
    for (int x = 0; x < BLOCKS_COLS; x++) {
      if (level[y] >> x & 1 == 1) {
        Rect blockRect = Rect { x * (BLOCK_WIDTH + 2), y * (BLOCK_HEIGHT + 2), BLOCK_WIDTH, BLOCK_HEIGHT };
        if (arduboy.collide(blockRect, ballRect)) {
          level[y] &= ~(1 << x);
          ballVelY *= -1;
        }
      }
    }
  }
}

void renderBall() {
  arduboy.fillCircle(ballX, ballY, 2);
}

void renderBlocks() {
  for (int y = 0; y < BLOCKS_ROWS; y++) {
    for (int x = 0; x < BLOCKS_COLS; x++) {
      if (level[y] >> x & 1 == 1) {
        arduboy.fillRect(x * (BLOCK_WIDTH + 2), y * (BLOCK_HEIGHT + 2), BLOCK_WIDTH, BLOCK_HEIGHT);
      }
    }
  }
}

void renderPaddle() {
  arduboy.fillRect(paddleX, HEIGHT - 4, paddleWidth, 4);
}

void loop() {
  if (!arduboy.nextFrame()) return;

  arduboy.clear();

  // Place game logic here
  handleGameInput();
  moveBall();

  renderBall();
  renderBlocks();
  renderPaddle();
  // ======================

  arduboy.display();
}
