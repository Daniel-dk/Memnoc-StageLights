
/* FastLED LED control library*/
#include <FastLED.h>	

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)                              // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0


/*where the LEDs are connected*/
#define DATA_PIN_1   A0 // left "border"
#define DATA_PIN_2   A1 // left Goal line
#define DATA_PIN_3   A2 // Right "border"
#define DATA_PIN_4   A4 // right goal line

#define FPS 100 // framerate

#define LED_TYPE    WS2812B // use WS2811_400 for the real field ( like the  ones BELOW) 
#define COLOR_ORDER GRB

//#define LED_TYPE     WS2811_400
//#define COLOR_ORDER RGB

#define LONGLED 60 // Half number of LEDs on the long side of the field 
#define SHORTLED 30 // number of LEDs in the width of the field ( goal line length )

#define NUMLEDBORDER (LONGLED+SHORTLED+LONGLED) // number of LEDs on the really long LED strips ( the ~75m ones )
#define NUMLEDSGOAL (SHORTLED) // number of LEDs on the short strips ( like the 20m goal line )
#define BRIGHTNESS 255

// LED arrays
CRGB leds[NUMLEDBORDER*2]; // the long ( 75 m ) piece that goes around the  left side

int numLEDs = NUMLEDBORDER * 2;

int gPatternNumber = 0;

typedef void(*SimplePatternList[])();

// Palette definitions
CRGBPalette16 currentPalette = OceanColors_p;                               
CRGBPalette16 targetPalette = OceanColors_p;
TBlendType currentBlending = LINEARBLEND;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

CRGBPalette16 deepWater = CRGBPalette16(CRGB::Black, CRGB::Purple, CRGB::BlueViolet, CRGB::DarkMagenta,
	CRGB::DarkMagenta, CRGB::DarkSlateBlue, CRGB::Indigo, CRGB::DeepSkyBlue,
	CRGB::Amethyst, CRGB::CadetBlue, CRGB::Fuchsia, CRGB::DeepSkyBlue,
	CRGB::DarkCyan, CRGB::Aquamarine, CRGB::LightBlue, CRGB::DeepSkyBlue
);



int center = 0;
int step = -1;
int maxSteps = 9;
float fadeRate = 0.8;
int diff;
/* initialise the patter list */
SimplePatternList gPatterns = { shimmer ,ripple ,noise, breathe ,breatheDeep,sparkle, solidShimmer };


void setup()
{
  /*  setup code here */
  /*set LED controllers*/
	FastLED.addLeds<LED_TYPE, DATA_PIN_1, COLOR_ORDER>(leds,0, NUMLEDBORDER).setCorrection(TypicalLEDStrip); // long strip 1
	FastLED.addLeds<LED_TYPE, DATA_PIN_2, COLOR_ORDER>(leds, NUMLEDBORDER,NUMLEDBORDER).setCorrection(TypicalLEDStrip); //long strip 2

	// set master brightness control
	FastLED.setBrightness(BRIGHTNESS);
	Serial.begin(115200);
}

void loop()
{
  /* main program code here */
  /*handle Serial ( to emulate the keypad )*/
	handleSerial();
	/*handle LED animations*/
	LEDanimations();

}

/* Handle Seril messages */

void handleSerial() {
	char c = 0;
	if (Serial.available() > 0)
		c = Serial.read();

	switch (c) {
	case '0':
		setAnimation( 0 );
		break;
	case '1':
		setAnimation( 1 );
		break;
	case '2':
		setAnimation( 2 );
		break;
	case '3':
		setAnimation(3);
		break;
	case '4':
		setAnimation(4);
		break;
	case '5':
		setAnimation(5);
		break;
	case '6':
		setAnimation(6);
		break;
	case '7':

		break;
	case '8':

		break;
	case '9':
		
		break;
	case '*':
		nextAnimation();
		break;
	case '#':
		flash();
		break;
	default:
		break;
	}
}

/*go to next animation*/
void nextAnimation() {
	// add one to the current pattern number, and wrap around at the end
	//gPatternNumber = (gPatternNumber++) % MAX_PATTERN_IDX;
	gPatternNumber++;
	gPatternNumber = gPatternNumber % (ARRAY_SIZE(gPatterns));
	Serial.print(F("pattern is now : "));
	Serial.println(gPatternNumber);
}

/*set a specific animation*/
void setAnimation(int animation ){
	// add one to the current pattern number, and wrap around at the end

	gPatternNumber = animation;
	// safety modulo, so we don't set to an illegal pattern
	gPatternNumber = gPatternNumber % (ARRAY_SIZE(gPatterns));

	Serial.print(F("pattern is now : "));
	Serial.println(gPatternNumber);
}