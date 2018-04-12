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

#define LEDS_COUNT 5
#define POTS_COUNT 6
#define SWITCHS_COUNT 1
#define KSWITCHS_COUNT 1
#define OUTPUT_JACKS_COUNT 5
#define INPUT_JACKS_COUNT 1

#define COMPONENTS_COUNT LEDS_COUNT + POTS_COUNT //+ (SWITCHS_COUNT*2) + KSWITCHS_COUNT + INPUT_JACKS_COUNT + OUTPUT_JACKS_COUNT

int switchs[SWITCHS_COUNT][2];
int kswitchs[KSWITCHS_COUNT];
int out_jacks[OUTPUT_JACKS_COUNT];
int in_jacks[INPUT_JACKS_COUNT];

// led(short pin);
led *leds[LEDS_COUNT] = {
	new led(P8_10),
	new led(P8_12),
	new led(P8_11),
	new led(P8_08),
	new led(P8_09),
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

component *components[COMPONENTS_COUNT];

float gDigitalFramesPerAudioFrame, gAnalogFramesPerAudioFrame;

bool setup(BelaContext *context, void *userData)
{
	// Check if analog channels are enabled
	if(context->analogFrames == 0 || context->analogFrames > context->audioFrames) {
		rt_printf("Error: this example needs analog enabled, with 4 or 8 channels\n");
		return false;
	}
	
	switchs[0][0] = P8_15;
	switchs[0][1] = P8_16;
	
	kswitchs[0] = P8_07;
	
	out_jacks[0] = 0;
	out_jacks[1] = 1;
	out_jacks[2] = 2;
	out_jacks[3] = 3;
	out_jacks[4] = 4;
	
	in_jacks[0] = 7;
	
	int i, c = 0;

	// Add instances to components	
	for (i = 0; i < LEDS_COUNT; ++i) {
		components[c++] = leds[i];
	}
	
	for (i = 0; i < POTS_COUNT; ++i) {
		components[c++] = pots[i];
	}
	
	// Setup components
	for(c = 0; c < COMPONENTS_COUNT; ++c) {
		components[c]->setup(context, userData);
	}
	
	gDigitalFramesPerAudioFrame = context->digitalFrames / context->audioFrames;
	gAnalogFramesPerAudioFrame = context->analogFrames / context->audioFrames;
	
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
			components[c]->read(context, userData, audioFrame, analogFrame, digitalFrame);
			components[c]->execute(context, userData, audioFrame, analogFrame, digitalFrame);
		}
	}
}

void cleanup(BelaContext *context, void *userData)
{
	for(int c = 0; c < COMPONENTS_COUNT; ++c) {
		components[c]->cleanup(context, userData);
	}
}

