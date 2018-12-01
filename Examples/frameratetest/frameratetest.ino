#include "FastLED.h"

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    3
//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    90
#define FREQUENCY   12                // controls the interval between strikes
#define FLASHES     11 
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          255
#define FRAMES_PER_SECOND  120
#define color White;
unsigned int dimmer = 1;

CRGBPalette16 gPal;

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {POV, POV1 };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

  
void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}



const byte data[] PROGMEM = {0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0};
int pixels = 7;
int frames = 7;
int fps = 25;
int currentFrame = 0;
void POV() {
    for (int i=0; i<NUM_LEDS; i++) {
        int pixelIndex = i % pixels;
        int index = currentFrame*pixels*3 + pixelIndex*3;

        //Note: We're using pgm_read_byte_near to read bytes out of the data array stored in PROGMEM. These functions are not required for all configurations
        leds[i] = CRGB(pgm_read_byte_near(data+index),pgm_read_byte_near(data+index+1),pgm_read_byte_near(data+index+2));
   //     leds[i].setRGB(pgm_read_byte_near(data+index),pgm_read_byte_near(data+index+1),pgm_read_byte_near(data+index+2));
        
    }
    FastLED.show();
    currentFrame ++;
    if (currentFrame >= frames) currentFrame = 0;
    delay(1000/fps);
}

const byte data[] PROGMEM = {0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,0};
int pixels = 7;
int frames = 7;
int fps = 50;
int currentFrame = 0;
void POV1() {
    for (int i=0; i<NUM_LEDS; i++) {
        int pixelIndex = i % pixels;
        int index = currentFrame*pixels*3 + pixelIndex*3;

        //Note: We're using pgm_read_byte_near to read bytes out of the data array stored in PROGMEM. These functions are not required for all configurations
        leds[i] = CRGB(pgm_read_byte_near(data+index),pgm_read_byte_near(data+index+1),pgm_read_byte_near(data+index+2));
   //     leds[i].setRGB(pgm_read_byte_near(data+index),pgm_read_byte_near(data+index+1),pgm_read_byte_near(data+index+2));
        
    }
    FastLED.show();
    currentFrame ++;
    if (currentFrame >= frames) currentFrame = 0;
    delay(1000/fps);
}
