
/*LED patterns / animations for addressable strip*/
void LEDanimations() {
	// TODO : // keep the framerate modest

	// Call the current pattern function once, updating the 'leds' array
	gPatterns[gPatternNumber]();
	
	FastLED.show();
	// insert a delay to keep the framerate modest
	FastLED.delay(1000 / FPS);
	
	// slowly cycle the "base color" through the rainbow
	 EVERY_N_MILLISECONDS(50) {
		 gHue++;
	}
}

/*************************************************                 ANIMATIONS             *********************************************************/
/*************************************************         Don't use "Delay" in these...  *********************************************************/
/*******************************************     https://github.com/atuline/FastLED-Demos/ *********************************************************/
/*colour wheel ( gHue ) follows this from 0 - 255:

Red (0..) "HUE_RED"
Orange (32..) "HUE_ORANGE"
Yellow (64..) "HUE_YELLOW"
Green (96..) "HUE_GREEN"
Aqua (128..) "HUE_AQUA"
Blue (160..) "HUE_BLUE"
Purple (192..) "HUE_PURPLE"
Pink(224..) "HUE_PINK"

*/

/*Shimmer effect, like fire*/
void shimmer() {
	// random colored speckles that blink in and fade smoothly
	fadeToBlackBy(leds, numLEDs, 8);
	//blur1d(leds, numLEDs, 2);
	if (random8() < 250) {
		for (int i = 0; i < random8(10); i++) {
			int pos = random16(numLEDs);
		// 	leds[pos] += ColorFromPalette(OceanColors_p, random8()); // use the oceans palette
			leds[pos] += ColorFromPalette(deepWater, random8()); // use the oceans palette
		}
		
	}
}

/*some "primitive" animatiosn used in others*/
void addGlitter(fract8 chanceOfGlitter)
{
	if (random8() < chanceOfGlitter) {
		leds[random16(numLEDs)] += CRGB::White;
	}
}



void flash() {
	fill_solid(leds, numLEDs, CRGB::White);
	FastLED.show();
}

/* Rainbow */
void solidShimmer() {
	// FastLED's built-in rainbow generator
	fill_solid(leds, numLEDs, CHSV(gHue, 255, 255));
	if (random8() < 20) {
		int pos = random16(numLEDs);
		leds[pos] += CHSV(gHue, random8(200), 255);
	}
}


void fade() {
	fadeToBlackBy(leds, numLEDs, 2);
}


void breathe() {
	//float val = (exp(sin(millis() / 2000.0*PI)) - 0.36787944)*108.0;
	 uint8_t intVal = sin8(millis() >> 5);
	

	for (int i = 0; i < numLEDs; i++) {
		//leds[i] = CHSV(gHue,100, intVal);
		leds[i] = ColorFromPalette(OceanColors_p, gHue + random8(3), intVal);
	}
}


void breatheDeep() {
	//float val = (exp(sin(millis() / 2000.0*PI)) - 0.36787944)*108.0;
	uint8_t intVal = (sin8(millis() >> 6) /2 )+ (sin8(millis() >> 5)/2);


	for (int i = 0; i < numLEDs; i++) {
		leds[i] = CHSV(map(gHue,0,255,135,195),250, intVal);
		//leds[i] = ColorFromPalette(OceanColors_p, gHue + random8(3), intVal);
	}
}

/*Confetti*/
void sparkle() {
	// random colored speckles that blink in and fade smoothly
	fadeToBlackBy(leds, numLEDs, 10);
	if (random8() < 127) {
		int pos = random16(numLEDs);

		leds[pos] += CHSV(gHue + random8(64), random8(100), 255);
	}
}



void ripple() {
	EVERY_N_MILLISECONDS(100) {
		uint8_t maxChanges = 24;
		nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
	}

	EVERY_N_SECONDS(60) {
	targetPalette = CRGBPalette16(CHSV(gHue + 6, 192, random8(128, 255)), CHSV(gHue + 16, 255, random8(128, 255)), CHSV(gHue + 8, 192, random8(128, 255)), CHSV(gHue + 12, 255, random8(128, 255)));

	}

	EVERY_N_MILLISECONDS(50) {    // FastLED based non-blocking delay to update/display the sequence.
		rippleAction();
		addGlitter(120);
	}
}

void rippleAction() {

	fadeToBlackBy(leds, numLEDs, 24);                             // 8 bit, 1 = slow, 255 = fast

	switch (step) {

	case -1:                                                          // Initialize ripple variables.
		center = random(numLEDs);
		// colour = random8();
		step = 0;
		break;

	case 0:
		leds[center] = ColorFromPalette(currentPalette, gHue, 255, currentBlending);

		step++;
		break;

	case 16:   // number of steps in the ripple
		step = -1;
		break;

	default:                                                          // Middle of the ripples.
		leds[(center + step + numLEDs) % numLEDs] += ColorFromPalette(currentPalette, gHue, 255 / step * 2, currentBlending);       // Simple wrap from Marc Miller
		leds[(center - step + numLEDs) % numLEDs] += ColorFromPalette(currentPalette, gHue, 255 / step * 2, currentBlending);
		step++;                                                         // Next step.
		break;
	} // switch step

} // ripple()




  //noise rainbow
void noise() {

	EVERY_N_MILLISECONDS(100) {                                  // FastLED based non-blocking delay to update/display the sequence.
		plasma();
	}


	EVERY_N_MILLISECONDS(150) {
		uint8_t maxChanges = 24;
		nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
	}


	EVERY_N_SECONDS(60) {                                 // Change the target palette to a random one every 20 seconds.

		
	//	targetPalette = CRGBPalette16(CHSV(gHue + 6, 192, random8(128, 255)), CHSV(gHue + 16, 255, random8(128, 255)), CHSV(gHue + 8, 192, random8(128, 255)), CHSV(gHue + 12, 255, random8(128, 255)));
	}

}


void plasma() {                                                 // This is the heart of this program. Sure is short. . . and fast.

	int thisPhase = beatsin8(6, -64, 64);                           // Setting phase change for a couple of waves.
	int thatPhase = beatsin8(7, -64, 64);

	for (int k = 0; k<numLEDs; k++) {                              // For each of the LED's in the strand, set a brightness based on a wave as follows:

		int colorIndex = cubicwave8((k * 23) + thisPhase) / 2 + cos8((k * 15) + thatPhase) / 2;           // Create a wave and add a phase change and add another wave with its own phase change.. Hey, you can even change the frequencies if you wish.
		int thisBright = qsuba(colorIndex, beatsin8(7, 0, 96));                                 // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..

		leds[k] = ColorFromPalette(currentPalette, colorIndex, thisBright, currentBlending);  // Let's now add the foreground colour.
	}

} // plasma()