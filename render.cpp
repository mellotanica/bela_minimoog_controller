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

#include <hardware.h>
#include <coordinator.h>

#include <vector>

float gDigitalFramesPerAudioFrame, gAnalogFramesPerAudioFrame;
State gCurrentState;


bool setup(BelaContext *context, void *userData)
{
	// Check if analog channels are enabled
	if(context->analogFrames == 0 || context->analogFrames > context->audioFrames) {
		rt_printf("Error: this example needs analog enabled, with 4 or 8 channels\n");
		return false;
	}
	
	// Setup components
	for(auto c : hardware::getInstance().hw_components) {
		c->setup(context, userData);
	}

	coordinator::getInstance().initialize();
	
	gDigitalFramesPerAudioFrame = context->digitalFrames / context->audioFrames;
	gAnalogFramesPerAudioFrame = context->analogFrames / context->audioFrames;
	gCurrentState.inverse_sample_rate = 1.0 / context->audioSampleRate;
	
	return true;
}

void render(BelaContext *context, void *userData)
{
	hardware& hw = hardware::getInstance();

	gCurrentState.context = context;
	gCurrentState.userData = userData;
	
	unsigned int audioFrame = 0;
	gCurrentState.audioFrame = audioFrame;
	gCurrentState.analogFrame = audioFrame * gAnalogFramesPerAudioFrame;
	gCurrentState.digitalFrame = audioFrame * gDigitalFramesPerAudioFrame;
	gCurrentState.totalFramesElapsed = context->audioFramesElapsed + audioFrame;

	coordinator::getInstance().mode_switch_handler->render(&gCurrentState);

	if(hw.bypass) {
		return;
	}
	// the audioSampleRate is the leading value, so we progress at that frequency
	while(audioFrame < context->audioFrames) {

		for (auto o : hw.hw_outputs) {
			o->render(&gCurrentState);
		}

		for (auto o : hw.active_outputs) {
			o->render(&gCurrentState);
		}
		
		++audioFrame;

		gCurrentState.audioFrame = audioFrame;
		gCurrentState.analogFrame = audioFrame * gAnalogFramesPerAudioFrame;
		gCurrentState.digitalFrame = audioFrame * gDigitalFramesPerAudioFrame;
		gCurrentState.totalFramesElapsed = context->audioFramesElapsed + audioFrame;
	}
}

void cleanup(BelaContext *context, void *userData)
{
	for(auto c : hardware::getInstance().hw_components) {
		c->cleanup(context, userData);
	}
}

