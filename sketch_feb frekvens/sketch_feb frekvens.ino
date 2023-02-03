#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <arduinoFFT.h>

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

#define MIK_IN 0

void DisplayNote(char Note) {
  display.setTextSize(5); // Indstil skriftstørrelse til 5px
  display.setCursor(0,10); // Indstil skriv positionen
  display.println(Note); // Skriv hvilken
}

double GetClosestNoteFromHz(double Hz) {
  //Noter
  std::map<char, std::array<double, 8>> Notes; // Et map over alle noder og deres frekvens
  Notes['C'] = {16.35,  32.70,  65.41,  130.81, 261.63, 523.25, 1046.60, 2093.00};
  Notes['c'] = {17.32,  34.65,  69.30,  138.59, 277.18, 554.37, 1108.73, 2217.46};
  Notes['D'] = {18.35,  36.71,  73.42,  146.83, 273.66, 587.33, 1174.66, 2349.32};
  Notes['d'] = {19.45,  38.89,  77.78,  155.56, 311.13, 622.25, 1244.51, 2217.46};
  Notes['E'] = {20.60,  41.20,  82.41,  164.81, 329.53, 659.25, 1318.51, 2637.02};
  Notes['F'] = {21.83,  43.65,  87.31,  174.61, 349.23, 698.46, 1396.91, 2793.83};
  Notes['f'] = {23.12,  46.25,  92.50,  185.00, 369.99, 739.99, 1479.98, 2959.96};
  Notes['G'] = {24.50,  49.00,  98.00,  196.00, 392.00, 783.99, 1567.98, 3135.96};
  Notes['g'] = {25.96,  51.91,  103.83, 207.65, 415.30, 830.61, 1661.22, 3322.44};
  Notes['A'] = {27.50,  55.00,  110.00, 220.00, 440.00, 880.00, 1760.00, 3520.00};
  Notes['a'] = {29.14,  58.27,  116.54, 233.08, 466.16, 932.33, 1864.66, 3729.31};
  Notes['B'] = {30.87,  61.74,  123.47, 246.94, 492.88, 987.77, 1975.53, 3951.07};

  double ClosestNote = Notes['C'][4]; // Standarden bliver sat til noden C4
  char NoteLetter;                    // Nodens bogstav. 

  for (auto const& note : Notes) { // Loop igennem mappet.
    for (int i = 0; i < sizeof(note.second) / sizeof(double); i++) { // Loop igennem alle frekvenserne under alle tonerne
      if (abs(Hz - note.second[i]) < abs(Hz - ClosestNote)) { // Hvis den loopede frekvens er tættere på den tidligere tætteste frekvens
        ClosestNote = note.second[i]; // Ændre den tætteste frekvens til den nye
        NoteLetter = note.first;      // Angiv hvilken node det er 
      }
    }
  }
  DisplayNote(NoteLetter); // Vis hvilken node der er tættest
  return ClosestNote;      // Returner den tætteste frekvens
}


void setup() {

  Serial.begin(9600);
  
  //Skærm
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  testscrolltext();
  display.clearDisplay();

  //Lyd
  pinMode(MIK_IN, INPUT);
}

double hz = 50;
void loop() {
  hz += 1;
  display.clearDisplay();
  DisplayHz(hz);
  display.display();

  Serial.println(analogRead(MIK_IN), DEC);
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
  double ClosestNote = GetClosestNoteFromHz(Hz); // Hent den tætteste node
  display.setCursor(0,0); // Reset tegne positionen

  display.fillRect(35, 30, 78, 4, WHITE); // Linjen mellem tallene
  //               X,   Y, W,  H, C  

  display.setTextSize(2); // Indstil tekststørrelsen

  display.setCursor(40,10);
  display.println(Hz); // Skriv hvor hvilken frekvens guitaren spiller
  
  display.setCursor(40,40);
  display.println(ClosestNote); // Skriv hvilken frekvens er den tætteste
}
