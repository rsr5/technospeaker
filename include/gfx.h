
#define NUM_LEDS 128 
#define WIDTH 16
#define HEIGHT 8

CRGB leds[NUM_LEDS];

int matrix[HEIGHT][WIDTH] = {
  {0, 15, 16, 31, 32, 47, 48, 63, 64, 79, 80, 95, 96, 111, 112, 127},
  {1, 14, 17, 30, 33, 46, 49, 62, 65, 78, 81, 94, 97, 110, 113, 126},
  {2, 13, 18, 29, 34, 45, 50, 61, 66, 77, 82, 93, 98, 109, 114, 125},
  {3, 12, 19, 28, 35, 44, 51, 60, 67, 76, 83, 92, 99, 108, 115, 124},
  {4, 11, 20, 27, 36, 43, 52, 59, 68, 75, 84, 91, 100, 107, 116, 123},
  {5, 10, 21, 26, 37, 42, 53, 58, 69, 74, 85, 90, 101, 106, 117, 122},
  {6, 9, 22, 25, 38, 41, 54, 57, 70, 73, 86, 89, 102, 105, 118, 121},
  {7, 8, 23, 24, 39, 40, 55, 56, 71, 72, 87, 88, 103, 104, 119, 120}
};

/* Map an X, Y coordinate to the actual pixel ID in the array. */
#define mapp(X, Y) matrix[Y][X]

void drawpixel(int x, int y, CRGB color) {
  leds[mapp(x, y)] = color;
}

void clearScreen() {
  int i;
  for (i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
}
