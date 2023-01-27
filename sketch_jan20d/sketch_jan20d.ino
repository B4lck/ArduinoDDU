#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <map>
#include <string>
#include <array>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI  23
#define OLED_CLK   18
#define OLED_DC    16
#define OLED_CS    17
#define OLED_RESET 5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void DisplayNote(char Note) {
  display.setCursor(0, 0);

  display.setTextSize(5);
  display.setCursor(10,10);
  display.println(Note);
  display.setTextSize(1);

  //Alt gør ondt
}

double GetClosestNoteFromHz(double Hz) {
  //Noter
  std::map<char, std::array<double, 8>> Notes;
  Notes['C'] = {16.35,  32.70,  65.41,  130.81, 261.63, 523.25, 1046.60, 2093.00};
  Notes['D'] = {18.35,  36.71,  73.42,  146.83, 273.66, 587.33, 1174.66, 2349.32};
  Notes['E'] = {20.60,  41.20,  82.41,  164.81, 329.53, 659.25, 1318.51, 2637.02};
  Notes['F'] = {21.83,  43.65,  87.31,  174.61, 349.23, 698.46, 1396.91, 2793.83};
  Notes['G'] = {24.50,  49.00,  98.00,  196.00, 392.00, 783.99, 1567.98, 3135.96};
  Notes['A'] = {27.50,  55.00,  110.00, 220.00, 440.00, 880.00, 1760.00, 3520.00};
  Notes['B'] = {30.87,  61.74,  123.47, 246.94, 492.88, 987.77, 1975.53, 3951.07};

  double ClosestNote = Notes['C'][4];
  char NoteLetter = 'C';

  for (auto const& note : Notes) {
    for (int i = 0; i < sizeof(note.second); i++) {
      Serial.println(note.first);
      if (abs(Hz - note.second[i]) < abs(Hz - ClosestNote)) {
        ClosestNote = note.second[i];
        NoteLetter = note.first;
      }
    }
  }
  DisplayNote(NoteLetter);
  return ClosestNote;
}


void setup() {

  Serial.begin(9600);
    if(!display.begin(SSD1306_SWITCHCAPVCC)) {

    Serial.println(F("SSD1306 allocation failed"));

    for(;;); // Don't proceed, loop forever

  }
  display.clearDisplay();
  testscrolltext();
  display.clearDisplay();
}


void loop() {

  display.clearDisplay();

  DisplayHz(324);

  display.display();
}


void testscrolltext(void) {

  display.clearDisplay();


  display.setTextSize(2); // Draw 2X-scale text

  display.setTextColor(WHITE);

  display.setCursor(0, 0);

  display.println(F("Guitar"));

  display.println(F("Tuner"));

  display.display();

  delay(1000);

}


void DisplayHz(double Hz) {
  double ClosestNote = GetClosestNoteFromHz(Hz);

  display.setCursor(0,0);

  display.fillRect(50, 30, 78, 4, WHITE); // Linjen mellem tallene

  display.setTextSize(1); // Indstil tekststørrelsen

  display.setCursor(50,10);
  display.println(Hz);
  
  display.setCursor(50,40);
  display.println(ClosestNote);
}
