#include <TFT_eSPI.h>
#include <SPI.h>
#include <math.h>

TFT_eSPI tft = TFT_eSPI();
const int numOfArms = 6;
const int drawDelay = 35;
const int objectDelay = 500;
const int groupDisplayTime = 1500;

// defining the colors - finding a random blue color (for the snowflakes), green (for spring leaf), and defining colors to return for the autumn leaves.
uint16_t randomBlue() {
  int r = random(0, 50);
  int g = random(80, 220);
  int b = random(180, 256);
  return tft.color565(r, g, b);
}

uint16_t randomGreen() {
  return tft.color565(random(30, 80), random(130, 210), random(30, 80));
}

uint16_t randomLeafColor() {
  int choice = random(0, 5);
  if (choice == 0) {
    return tft.color565(random(220, 256), random(140, 200), random(10, 50)); // yellow
  }
  else if (choice == 1) {
    return tft.color565(random(220, 256), random(70, 140), random(10, 40)); // orange
  }
  else if (choice == 2) {
    return tft.color565(random(180, 256), random(30, 80), random(20, 60)); // red
  }
  else if (choice == 3) {
    return tft.color565(random(120, 190), random(60, 110), random(20, 50)); // brown
  }
  else {
    return tft.color565(random(200, 256), random(70, 130), random(20, 50)); // reddish orange
  }
}

// rotate a horizontal line to be a certain angle - helper function used in drawing
void rotatedLine(int cx, int cy, float x1, float y1, float x2, float y2, float angle, uint16_t color) {
  float cosA = cos(angle);
  float sinA = sin(angle);
  int rx1 = cx + x1 * cosA - y1 * sinA;
  int ry1 = cy + x1 * sinA + y1 * cosA;
  int rx2 = cx + x2 * cosA - y2 * sinA;
  int ry2 = cy + x2 * sinA + y2 * cosA;
  tft.drawLine(rx1, ry1, rx2, ry2, color);
}

// fill leaf - created with horizontal lines that are rotated to fit the leaf based on the chosen angle and size (via rotatedLine)
void fillLeaf(int cx, int cy, float sideX[], float sideY[], int N, float scale, float angle, uint16_t fillColor) {
  for (int i = 0; i < N - 1; i++) {
    float y1 = sideY[i] * scale;
    float y2 = sideY[i + 1] * scale;
    float x1 = sideX[i] * scale;
    float x2 = sideX[i + 1] * scale;
    int steps = abs((int)(y2 - y1));
    if (steps < 1) { steps = 1; }
    for (int j = 0; j <= steps; j++) {
      float t = (float)j / steps;
      float y = y1 + (y2 - y1) * t;
      float edgeX = x1 + (x2 - x1) * t;
      rotatedLine(cx, cy, -edgeX, y, edgeX, y, angle, fillColor);
    }
  }
}

// drawing the leaf veins
void drawLeafVeins(int cx, int cy, float sideX[], float sideY[], int N, float scale, float angle, uint16_t color) {
  for (int i = 3; i < N - 3; i += 2) {
    float veinY = sideY[i] * scale;
    float veinX = sideX[i] * scale * 0.55;
    float tipDirection = -4 * scale;
    rotatedLine(cx, cy, 0, veinY, veinX, veinY + tipDirection, angle, color);  // right side
    rotatedLine(cx, cy, 0, veinY, -veinX, veinY + tipDirection, angle, color); // left side
  }
}

// drawing the leaf based on the inputted coordinates (the leaf shape)
void drawLeafShape(int cx, int cy, float sideX[], float sideY[], int N, float scale, float angle, uint16_t color) {
  fillLeaf(cx, cy, sideX, sideY, N, scale, angle, color);
  for (int i = 0; i < N - 1; i++) {
    rotatedLine(cx, cy, sideX[i] * scale, sideY[i] * scale, sideX[i + 1] * scale, sideY[i + 1] * scale, angle, color);
    rotatedLine(cx, cy, -sideX[i] * scale, sideY[i] * scale, -sideX[i + 1] * scale, sideY[i + 1] * scale, angle, color);
  }
  rotatedLine(cx, cy, 0, sideY[1] * scale, 0, sideY[N - 2] * scale, angle, TFT_WHITE);
  drawLeafVeins(cx, cy, sideX, sideY, N, scale, angle, TFT_WHITE);
}

// maple leaf
void drawMapleLeaf(int cx, int cy, float scale, float angle, uint16_t color) {
  float sideX[] = {0, 9, 7, 20, 14, 32, 19, 37, 16, 21, 8, 5, 0};
  float sideY[] = {-42, -23, -27, -19, -14, -10, -4, 5, 7, 18, 17, 34, 45};
  const int N = 13;
  drawLeafShape(cx, cy, sideX, sideY, N, scale, angle, color);
}

// oak leaf
void drawOakLeaf(int cx, int cy, float scale, float angle, uint16_t color) {
  float sideX[] = {0, 8, 14, 11, 21, 23, 15, 26, 27, 17, 23, 16, 10, 0};
  float sideY[] = {-43, -37, -31, -26, -21, -14, -8, -2, 7, 11, 17, 23, 31, 42};
  const int N = 14;
  drawLeafShape(cx, cy, sideX, sideY, N, scale, angle, color);
}

// lilac leaf
void drawLilacLeaf(int cx, int cy, float scale, float angle, uint16_t color) {
  float sideX[] = {0, 8, 15, 21, 26, 29, 28, 25, 20, 14, 9, 5, 0};
  float sideY[] = {-43, -37, -30, -22, -12, 0, 11, 21, 29, 34, 30, 24, 38};
  const int N = 13;
  drawLeafShape(cx, cy, sideX, sideY, N, scale, angle, color);
}

// poplar leaf
void drawPoplarLeaf(int cx, int cy, float scale, float angle, uint16_t color) {
  float sideX[] = {0, 8, 17, 26, 33, 36, 34, 29, 22, 14, 7, 0};
  float sideY[] = {-42, -36, -29, -19, -7, 5, 16, 25, 31, 34, 32, 43};
  const int N = 12;
  drawLeafShape(cx, cy, sideX, sideY, N, scale, angle, color);
}

// choose a random leaf
void drawLeafType(int cx, int cy, float scale, float angle, uint16_t color) {
  int leafType = random(0, 4);
  if (leafType == 0) {
    drawMapleLeaf(cx, cy, scale, angle, color);
  } else if (leafType == 1) {
    drawOakLeaf(cx, cy, scale, angle, color);
  } else if (leafType == 2) {
    drawLilacLeaf(cx, cy, scale, angle, color);
  } else {
    drawPoplarLeaf(cx, cy, scale, angle, color);
  }
}

// drawing one centered green leaf in the beginning in the center
void drawGreenLeaf() {
  int cx = tft.width() / 2; 
  int cy = tft.height() / 2;
  float scale = 1.0;
  float angle = 0;
  uint16_t color = randomGreen();
  drawLeafType(cx, cy, scale, angle, color);
}

// drawing random fall leaf
void drawRandomFallLeaf() {
  float scale = random(60, 101) / 100.0;
  int cx = random(10, tft.width() - 10);
  int cy = random(10, tft.height() - 10);
  float angle = random(0, 360) * PI / 180.0;
  uint16_t color = randomLeafColor();
  drawLeafType(cx, cy, scale, angle, color);
}

// drawing snowflake
void drawSnowflake(int cx, int cy, int radius, uint16_t color) {
  int branches = random(2, 5);
  float branchPositions[4];
  float branchLengths[4];
  float branchAngles[4];
  for (int i = 0; i < branches; i++) {
    branchPositions[i] = random(35, 85) / 100.0;
    branchLengths[i] = random(radius / 5, radius / 2);
    branchAngles[i] = random(25, 60) * PI / 180.0;
  }
  for (int r = 2; r <= radius; r += 2) {
    for (int arm = 0; arm < numOfArms; arm++) {
      float angle = arm * TWO_PI / numOfArms;
      rotatedLine(cx, cy, 0, 0, r, 0, angle, color);
      for (int b = 0; b < branches; b++) {
        float branchStart = radius * branchPositions[b];
        if (r >= branchStart) {
          float currentLength = min((float)branchLengths[b], (float)(r - branchStart));
          float x2 = branchStart + currentLength * cos(branchAngles[b]);
          float y2 = currentLength * sin(branchAngles[b]);
          rotatedLine(cx, cy, branchStart, 0, x2, y2, angle, color);
          rotatedLine(cx, cy, branchStart, 0, x2, -y2, angle, color);
        }
      }
    }
    delay(drawDelay);
  }
}

// generating dimensions for the snowflake
void drawRandomSnowflake() {
  int radius = random(25, 55);
  int cx = random(5, tft.width() - 5);
  int cy = random(5, tft.height() - 5);
  uint16_t color = randomBlue();
  drawSnowflake(cx, cy, radius, color);
}

// setup
void setup() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  randomSeed(analogRead(0));
}

void loop() {
  // creating one single green leaf at the beginning
  tft.fillScreen(TFT_BLACK);
  drawGreenLeaf();
  delay(1500);
  tft.fillScreen(TFT_BLACK);
  delay(300);

  // fall leaves
  int numberOfFallLeaves = random(7, 11);
  for (int i = 0; i < numberOfFallLeaves; i++) {
    drawRandomFallLeaf();
    delay(objectDelay);
  }
  delay(groupDisplayTime);
  tft.fillScreen(TFT_BLACK);
  delay(300);

  // snowflakes
  int numberOfSnowflakes = random(7, 8);
  for (int i = 0; i < numberOfSnowflakes; i++) {
    drawRandomSnowflake();
    delay(objectDelay);
  }
  delay(groupDisplayTime);
  tft.fillScreen(TFT_BLACK);
  delay(300);
}
