/*
 ____  _____ _        _    
| __ )| ____| |      / \   
|  _ \|  _| | |     / _ \  
| |_) | |___| |___ / ___ \ 
|____/|_____|_____/_/   \_\

The platform for ultra-low latency audio and sensor processing

http://bela.io

A project of the Augmented Instruments Laboratory within the
Centre for Digital Music at Queen Mary University of London.
http://www.eecs.qmul.ac.uk/~andrewm

(c) 2016 Augmented Instruments Laboratory: Andrew McPherson,
  Astrid Bin, Liam Donovan, Christian Heinrichs, Robert Jack,
  Giulio Moro, Laurel Pardue, Victor Zappi. All rights reserved.

The Bela software is distributed under the GNU Lesser General Public License
(LGPL 3.0), available here: https://www.gnu.org/licenses/lgpl-3.0.txt
*/


#include <Bela.h>

#include <led.h>
#include <pot.h>
#include <killswitch.h>
#include <switch.h>
#include <jack.h>

/*
 *  TEST
 */
#define POT_TEST
#define SWC_TEST
#define KSW_TEST

#if defined POT_TEST or defined SWC_TEST or defined KSW_TEST
int active_led = 0;
float duty_cycle = 0.5;
float pwm_period = 0.01;
#endif
/*
 *  TEST
 */


#define LEDS_COUNT 5
#define POTS_COUNT 6
#define SWITCHS_COUNT 1
#define KSWITCHS_COUNT 1
#define OUTPUT_JACKS_COUNT 5
#define INPUT_JACKS_COUNT 1

#define COMPONENTS_COUNT LEDS_COUNT + POTS_COUNT + KSWITCHS_COUNT + SWITCHS_COUNT + INPUT_JACKS_COUNT + OUTPUT_JACKS_COUNT

// led(short pin);
led *leds[LEDS_COUNT] = {
	new led(P8_09),
	new led(P8_08),
	new led(P8_11),
	new led(P8_12),
	new led(P8_10),
};

// pot(short analogPin, float minv = 0, float maxv = 1, float error = 0);
pot *pots[POTS_COUNT] = {
	new pot(3),
	new pot(5),
	new pot(1),
	new pot(0),
	new pot(2),
	new pot(4),
};

// killswitch(short pin, bool defaultState, unsigned int debounceMsecs = 2);
killswitch *killswitches[KSWITCHS_COUNT] = {
	new killswitch(P8_07, true, 0),
};

// switch_comp(short pin0, short pin2, switch_type type);
switch_comp *switches[SWITCHS_COUNT] = {
	new switch_comp(P8_15, P8_16, ON_OFF_ON),	
};

// inputJack(short analogPin);
inputJack *inJacks[INPUT_JACKS_COUNT] = {
	new inputJack(7),
};

// outputJack(short analogPin);
outputJack *outJacks[OUTPUT_JACKS_COUNT] = {
	new outputJack(0),
	new outputJack(1),
	new outputJack(2),
	new outputJack(3),
	new outputJack(4),
};

component *components[COMPONENTS_COUNT];

float gDigitalFramesPerAudioFrame, gAnalogFramesPerAudioFrame;

// test


bool setup(BelaContext *context, void *userData)
{
	// Check if analog channels are enabled
	if(context->analogFrames == 0 || context->analogFrames > context->audioFrames) {
		rt_printf("Error: this example needs analog enabled, with 4 or 8 channels\n");
		return false;
	}
	
	int i, c = 0;

	// Add instances to components	
	for (i = 0; i < LEDS_COUNT; ++i) {
		components[c++] = leds[i];
	}
	for (i = 0; i < POTS_COUNT; ++i) {
		components[c++] = pots[i];
	}
	for (i = 0; i < KSWITCHS_COUNT; ++i) {
		components[c++] = killswitches[i];
	}
	for (i = 0; i < SWITCHS_COUNT; ++i) {
		components[c++] = switches[i];
	}
	for (i = 0; i < INPUT_JACKS_COUNT; ++i) {
		components[c++] = inJacks[i];
	}
	for (i = 0; i < OUTPUT_JACKS_COUNT; ++i) {
		components[c++] = outJacks[i];
	}
	
	// Setup components
	for(c = 0; c < COMPONENTS_COUNT; ++c) {
		components[c]->setup(context, userData);
	}
	
	gDigitalFramesPerAudioFrame = context->digitalFrames / context->audioFrames;
	gAnalogFramesPerAudioFrame = context->analogFrames / context->audioFrames;
	
/*
 *  TEST
 */
#ifdef SWC_TEST
	switches[0]->set_active(true);
	switches[0]->register_listener([&](BelaContext *context, unsigned short value, unsigned int audioFrame, unsigned int analogFrame, unsigned int digitalFrame){
		rt_printf("switching from %d to %d\n", active_led, value*2);
		leds[active_led]->set_active(false);
		active_led = value*2;
		leds[active_led]->set_active(true);
	});
#endif
	
#ifdef KSW_TEST
	killswitches[0]->set_active(true);
	killswitches[0]->register_listener([&](BelaContext *context, bool value, unsigned int audioFrame, unsigned int analogFrame, unsigned int digitalFrame){
		rt_printf("toggling led 0 %s\n", (value ? "on" : "off"));
		leds[0]->set_active(value);
	});
#endif

#ifdef POT_TEST
	pots[0]->set_active(true);
	pots[0]->set_error(0.5);
	pots[0]->set_range(0, LEDS_COUNT-0.01);
	pots[0]->register_listener([&](BelaContext *context, float value, unsigned int audioFrame, unsigned int analogFrame, unsigned int digitalFrame) {
		if((int) value != active_led) {
			rt_printf("switching from %d to %d\n", active_led, (int)value);
			leds[active_led]->set_active(false);
			active_led = (int)value;
			leds[active_led]->set_active(true);
			leds[active_led]->set_pwm_period(context, pwm_period);
			leds[active_led]->set_pwm_duty_cycle(context, duty_cycle);
		}
	});
	
	pots[1]->set_active(true);
	pots[1]->set_error(0.001);
	pots[1]->register_listener([&](BelaContext *context, float value, unsigned int audioFrame, unsigned int analogFrame, unsigned int digitalFrame) {
		rt_printf("pwm duty: %f\n", value);
		duty_cycle = value;
		leds[active_led]->set_pwm_duty_cycle(context, duty_cycle);

	});
	
	pots[2]->set_active(true);
	pots[2]->set_error(0.001);
	pots[2]->set_range(0.5, 0.01);
	pots[2]->register_listener([&](BelaContext *context, float value, unsigned int audioFrame, unsigned int analogFrame, unsigned int digitalFrame) {
		rt_printf("pwm period: %f\n", value);
		pwm_period = value;
		leds[active_led]->set_pwm_period(context, pwm_period);
	});
#endif
/*
 *  TEST
 */

	return true;
}

void render(BelaContext *context, void *userData)
{
	unsigned int analogFrame, digitalFrame;
	int c;
	
	// the audioSampleRate is the leading value, so we progress at that frequency
	for(unsigned int audioFrame = 0; audioFrame < context->audioFrames; ++audioFrame) {
		analogFrame = audioFrame * gAnalogFramesPerAudioFrame;
		digitalFrame = audioFrame * gDigitalFramesPerAudioFrame;

		for (c = 0; c < COMPONENTS_COUNT; ++c) {
			if (components[c]->is_active()) {
				components[c]->read(context, userData, audioFrame, analogFrame, digitalFrame);
				components[c]->execute(context, userData, audioFrame, analogFrame, digitalFrame);	
			}
		}
	}
}

void cleanup(BelaContext *context, void *userData)
{
	for(int c = 0; c < COMPONENTS_COUNT; ++c) {
		components[c]->cleanup(context, userData);
	}
}

