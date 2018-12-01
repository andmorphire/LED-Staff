/* inoise8_mover_array
 *
 * By: Andrew Tuline
 *
 * Date: March, 2017
 *
 * We've used sine waves and counting to move pixels around a strand. In this case, I'm using FastLED Noise to move an array of pixels up and down the strand.
 * 
 * The advantage is that it provides random natural movement without requiring lots of fancy math.
 *
 */
 
 
#include "FastLED.h"
 
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
 
// Fixed definitions cannot change on the fly.
#define LED_DT 3                                             // Data pin to connect to the strip.
// #define LED_CK 11                                             // Clock pin for WS2801 or APA102.
#define COLOR_ORDER GRB                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE WS2812B                                       // Using APA102, WS2812, WS2801. Don't forget to modify LEDS.addLeds to suit.
#define NUM_LEDS 90                                           // Number of LED's.


// Global variables can be changed on the fly.
uint8_t max_bright = 255;                                      // Overall brightness definition. It can be changed on the fly.
 
struct CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette=LavaColors_p;
CRGBPalette16 targetPalette=OceanColors_p;
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND 
 
static int16_t dist;                                          // A moving location for our noise generator.
uint16_t xscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint16_t yscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.

uint8_t maxChanges = 24;                                      // Value for blending between palettes.



void setup() {

  Serial.begin(57600);                                        // Initialize serial port for debugging.
  delay(1000);                                                // Soft startup to ease the flow of electrons.
 
  LEDS.addLeds<LED_TYPE,LED_DT,COLOR_ORDER>(leds,NUM_LEDS);
//  LEDS.addLeds<LED_TYPE,LED_DT,LED_CK, COLOR_ORDER>(leds,NUM_LEDS);
 
  FastLED.setBrightness(max_bright);
//  set_max_power_in_volts_and_milliamps(5, 500);               // FastLED Power management set at 5V, 500mA.
  
  dist = random16(12345);                                     // A semi-random number for our noise generator

} // setup()

 
 
void loop() {
  
  EVERY_N_MILLISECONDS(20) {
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
    
    inoise8_mover();                                    // Update the LED array with noise at the new location
    fadeToBlackBy(leds, NUM_LEDS, 16);     
  }

  EVERY_N_SECONDS(5) {                                        // Change the target palette to a random one every 5 seconds.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }
 
  FastLED.show();
  
} // loop()


 
void inoise8_mover() {

  for (int i=0; i<20; i++) {
    uint8_t locn = inoise8(xscale, dist+yscale+i*200);  // Get a new pixel location from moving noise. locn rarely goes below 48 or above 192, so let's remove those ends.
    locn = constrain(locn,48,192);                      // Ensure that the occasional value outside those limits is not used.
    uint8_t pixlen = map(locn,48,192,0,NUM_LEDS-1);     // Map doesn't constrain, so we now map locn to the the length of the strand.
    leds[pixlen] = ColorFromPalette(currentPalette, pixlen, 255, LINEARBLEND);   // Use that value for both the location as well as the palette index colour for the pixel.
  }

  dist += beatsin8(10,1,4);                                                // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.                                             

} // inoise8_mover()
