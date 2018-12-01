#include "FastLED.h"

// Addapted from Mark Kriegsman "FastLED "100-lines-of-code" demo reel", showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED. This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate."
//
// Fast noise and pallettes, fast noise and movement, lightning and two sin pal adpated from Andrew Tualine
//
// Andrew Morton 2018


#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    3
//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    90
#define BRIGHTNESS          255
#define FRAMES_PER_SECOND  120

unsigned int dimmer = 1;

CRGB leds[NUM_LEDS];

//for noise and pallette
// struct CRGB leds[NUM_LEDS];

static uint16_t dist;         // A random number for our noise generator. Also for noise movement
uint16_t scale = 30;          // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint8_t maxChanges = 48;      // Value for blending between palettes.

CRGBPalette16 currentPalette(CRGB::Black);
CRGBPalette16 targetPalette(OceanColors_p);

//for noise for movement
CRGBPalette16 currentPalette2=LavaColors_p;

TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND 
 
uint16_t xscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint16_t yscale = 30;                                         // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint8_t maxChanges2 = 24;                                      // Value for blending between palettes.


//for 2sinpal
#define qsubd(x, b)  ((x>b)?b:0)                     // A digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Unsigned subtraction macro. if result <0, then => 0
unsigned long previousMillis;                                 // Store last time the strip was updated.


// Most of these variables can be mucked around with. Better yet, add some form of variable input or routine to change them on the fly. 1970's here I come. . 

uint8_t thishue;                                              // You can change the starting hue value for the first wave.
uint8_t thathue;                                              // You can change the starting hue for other wave.
uint8_t thisrot;                                              // You can change how quickly the hue rotates for this wave. Currently 0.
uint8_t thatrot;                                              // You can change how quickly the hue rotates for the other wave. Currently 0.
uint8_t allsat;                                               // I like 'em fully saturated with colour.
uint8_t thisdir;
uint8_t thatdir;
uint8_t alldir;                                               // You can change direction.
int8_t thisspeed;                                             // You can change the speed.
int8_t thatspeed;                                             // You can change the speed.
uint8_t allfreq;                                              // You can change the frequency, thus overall width of bars.
int thisphase;                                                // Phase change value gets calculated.
int thatphase;                                                // Phase change value gets calculated.
uint8_t thiscutoff;                                           // You can change the cutoff value to display this wave. Lower value = longer wave.
uint8_t thatcutoff;                                           // You can change the cutoff value to display that wave. Lower value = longer wave.
int thisdelay;                                                // Standard delay. . 
uint8_t fadeval;                                              // Use to fade the led's of course.

CRGBPalette16 thisPalette;
CRGBPalette16 thatPalette;
// TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND






void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  
  //fornoise and pallette
  dist = random16(12345);          // A semi-random number for our noise generator

//for 2sinpal
// Also, RainbowColors_p, RainbowStripeColors_p, OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p
  thisPalette = RainbowColors_p;
  thatPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  resetvars();                                                  // Initialize the variables

}




// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {sinelon, juggle, confetti, rainbow, lightning, lightning, juggle, twosinpal, POV, juggle, POV, noisepallette, lightning, lightning, juggle, rainbow, rainbowWithGlitter, POV, lightning, bpm, lightning, noisepallette, colorfade};

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
  EVERY_N_SECONDS( 9 ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy (leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

#define FREQUENCY   5   // controls the interval between strikes
#define FLASHES     11 
#define color White;
void lightning() {
  for (int flashCounter = 0; flashCounter < random8(3,FLASHES); flashCounter++)
  {
    if(flashCounter == 0) dimmer = 5;     // the brightness of the leader is scaled down by a factor of 5
    else dimmer = random8(1,3);           // return strokes are brighter than the leader
    
    FastLED.showColor(CHSV(255, 0, 255/dimmer));
    delay(random8(4,10));                 // each flash only lasts 4-10 milliseconds
    FastLED.showColor(CHSV(255, 0, 0));
    
    if (flashCounter == 0) delay (150);   // longer delay until next flash after the leader
    delay(50+random8(100));               // shorter delay between strokes  
  }
  delay(random8(FREQUENCY)*100);          // delay between strikes
}

const byte data[] PROGMEM = {0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255,0,0,255};
int pixels = 16;
int frames = 16;
int fps = 30;
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

void colorfade(){

#define wait_time 5

//start from red
  for( int colorStep=0; colorStep <= 255; colorStep++ ) {

  int r = 255;
  int g = 0;
  int b = colorStep;

  // Now loop though each of the LEDs and set each one to the current color
  for(int x = 0; x < NUM_LEDS; x++){
      leds[x] = CRGB(r,g,b);
  }

  // Display the colors we just set on the actual LEDs
      delay(10); 
      FastLED.show();
      }

    //into blue
      for( int colorStep=255; colorStep >= 0; colorStep-- ) {

      int r = colorStep;
      int g = 0;
      int b = 255;

      // Now loop though each of the LEDs and set each one to the current color
      for(int x = 0; x < NUM_LEDS; x++){
          leds[x] = CRGB(r,g,b);
      }

      // Display the colors we just set on the actual LEDs
  delay(10); 
  FastLED.show();
  }

//start from blue
  for( int colorStep=0; colorStep <= 255; colorStep++ ) {

      int r = 0;
      int g = colorStep;
      int b = 255; 

      // Now loop though each of the LEDs and set each one to the current color
      for(int x = 0; x < NUM_LEDS; x++){
          leds[x] = CRGB(r,g,b);
      }

      // Display the colors we just set on the actual LEDs
  delay(10); 
  FastLED.show();
  }

//into green
  for( int colorStep=255; colorStep >= 0; colorStep-- ) {

      int r = 0;
      int g = 255;
      int b = colorStep; 

      // Now loop though each of the LEDs and set each one to the current color
      for(int x = 0; x < NUM_LEDS; x++){
          leds[x] = CRGB(r,g,b);
      }

      // Display the colors we just set on the actual LEDs
  delay(wait_time); 
  LEDS.show();
  }

//start from green
  for( int colorStep=0; colorStep <= 255; colorStep++ ) {

      int r = colorStep;
      int g = 255;
      int b = 0;

      // Now loop though each of the LEDs and set each one to the current color
      for(int x = 0; x < NUM_LEDS; x++){
          leds[x] = CRGB(r,g,b);
      }

      // Display the colors we just set on the actual LEDs
  delay(wait_time); 
  LEDS.show();
  }

//into yellow
  for( int colorStep=255; colorStep >= 0; colorStep-- ) {

      int r = 255;
      int g = colorStep;
      int b = 0;

      // Now loop though each of the LEDs and set each one to the current color
      for(int x = 0; x < NUM_LEDS; x++){
          leds[x] = CRGB(r,g,b);
      }

      // Display the colors we just set on the actual LEDs
  delay(wait_time); 
  LEDS.show();
  }


} //end main loop




void noisepallette() {

  
  EVERY_N_MILLISECONDS(10) {
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);  // Blend towards the target palette
    fillnoise8();                                                           // Update the LED array with noise at the new location
  }

  EVERY_N_SECONDS(5) {             // Change the target palette to a random one every 5 seconds.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }

  LEDS.show();                    // Display the LED's at every loop cycle.
} // loop()


void fillnoise8() {
  for(int i = 0; i < NUM_LEDS; i++) {                                      // Just ONE loop to fill up the LED array as all of the pixels change.
    uint8_t index = inoise8(i*scale, dist+i*scale) % 255;                  // Get a value from the noise function. I'm using both x and y axis.
    leds[i] = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }
  dist += beatsin8(10,1, 4);                                               // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.
                                                                           // In some sketches, I've used millis() instead of an incremented counter. Works a treat.
} // fillnoise8()


void noisemovement() {
  
  EVERY_N_MILLISECONDS(20) {
    nblendPaletteTowardPalette(currentPalette2, targetPalette, maxChanges2);   // AWESOME palette blending capability.
    
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

void twosinpal() {
  
  ChangeMe();

  EVERY_N_MILLISECONDS(thisdelay) {
    two_sin();                                                // Routine is still delay based, but at least it's now a non-blocking day.
  }

  FastLED.show();

} // loop()



void two_sin() {

    thisdir ? thisphase += beatsin8(thisspeed, 2, 10) : thisphase -= beatsin8(thisspeed, 2, 10);
    thatdir ? thatphase += beatsin8(thisspeed, 2, 10) : thatphase -= beatsin8(thatspeed, 2, 10);
    thishue += thisrot;                                        // Hue rotation is fun for thiswave.
    thathue += thatrot;                                        // It's also fun for thatwave.
  
  for (int k=0; k<NUM_LEDS-1; k++) {
    int thisbright = qsuba(cubicwave8((k*allfreq)+thisphase), thiscutoff);      // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    int thatbright = qsuba(cubicwave8((k*allfreq)+128+thatphase), thatcutoff);  // This wave is 180 degrees out of phase (with the value of 128).

    leds[k] = ColorFromPalette(thisPalette, thishue, thisbright, currentBlending);
    leds[k] += ColorFromPalette(thatPalette, thathue, thatbright, currentBlending);
  }
     nscale8(leds,NUM_LEDS,fadeval);
     
} // two_sin()



// RainbowColors_p, RainbowStripeColors_p, OceanColors_p, CloudColors_p, ForestColors_p, and PartyColors_p.
void ChangeMe() {
  
  uint8_t secondHand = (millis() / 1000) % 60;                // Increase this if you want a longer demo.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    switch (secondHand) {
      case  0: thisrot = 1; thatrot = 1; thisPalette=PartyColors_p; thatPalette=PartyColors_p; break;
      case  5: thisrot = 0; thatdir = 1; thatspeed = -4; thisPalette=ForestColors_p; thatPalette=OceanColors_p; break;
      case 10: thatrot = 0; thisPalette=PartyColors_p; thatPalette=RainbowColors_p; break;
      case 15: allfreq = 16; thisdir = 1; thathue = 128; break;
      case 20: thiscutoff = 96; thatcutoff = 240; break;
      case 25: thiscutoff = 96; thatdir = 0; thatcutoff = 96; thisrot = 1; break;
      case 30: thisspeed= -4; thisdir = 0; thatspeed= -4; break;
      case 35: thiscutoff = 128; thatcutoff = 128; break;
      case 40: thisspeed = 3; break;
      case 45: thisspeed = 3; thatspeed = -3; break;
      case 50: thisspeed = 2; thatcutoff = 96; thiscutoff = 224; thatspeed = 3; break;
      case 55: resetvars(); break;
      case 60: break;
    }
  }
  
} // ChangeMe()



void resetvars() {                       // Reset the variable back to the beginning.

  thishue = 0;                          // You can change the starting hue value for the first wave.
  thathue = 140;                        // You can change the starting hue for other wave.
  thisrot = 1;                          // You can change how quickly the hue rotates for this wave. Currently 0.
  thatrot = 1;                          // You can change how quickly the hue rotates for the other wave. Currently 0.
  allsat = 255;                         // I like 'em fully saturated with colour.
  thisdir = 0;                          // Change the direction of the first wave.
  thatdir = 0;                          // Change the direction of the other wave.
  alldir = 0;                           // You can change direction.
  thisspeed = 4;                        // You can change the speed, and use negative values.
  thatspeed = 4;                        // You can change the speed, and use negative values.
  allfreq = 32;                         // You can change the frequency, thus overall width of bars.
  thisphase = 0;                        // Phase change value gets calculated.
  thatphase = 0;                        // Phase change value gets calculated.
  thiscutoff = 192;                     // You can change the cutoff value to display this wave. Lower value = longer wave.
  thatcutoff = 192;                     // You can change the cutoff value to display that wave. Lower value = longer wave.
  thisdelay = 10;                       // You can change the delay. Also you can change the allspeed variable above.
  fadeval = 192;                        // How quickly we fade.
  
} // resetvars()
