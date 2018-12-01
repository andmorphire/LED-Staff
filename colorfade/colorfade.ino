void loop(){

//start from red
  for( int colorStep=0; colorStep <= 255; colorStep++ ) {

  int r = 255;
  int g = 0;
  int b = colorStep;

  // Now loop though each of the LEDs and set each one to the current color
  for(int x = 0; x < LED_COUNT; x++){
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
      for(int x = 0; x < LED_COUNT; x++){
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
      for(int x = 0; x < LED_COUNT; x++){
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
      for(int x = 0; x < LED_COUNT; x++){
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
      for(int x = 0; x < LED_COUNT; x++){
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
      for(int x = 0; x < LED_COUNT; x++){
          leds[x] = CRGB(r,g,b);
      }

      // Display the colors we just set on the actual LEDs
  delay(wait_time); 
  LEDS.show();
  }


} //end main loop
