#ifndef LED_H
#define LED_H

#include <base/component.h>

#include <components/constant.h>

static auto led_pwm_period = constant<float>::make(0.01);

class led : public output {
public:
	/** led constructor
	 * @pin - the digital pin connected to the led expressed with digital_gpio_mapping.h naming
	 */
	led(short pin, 
		EmitterP<float> period = led_pwm_period,
		EmitterP<float> duty_cylcle = OneF,
		EmitterP<bool> statet = False);
	
	// component interface
	void setup(BelaContext *context, void *userData);
	void reset();
	void render(State *execState);
	
	ReceiverP<bool> state;
	ReceiverP<float> pwm_period;
	ReceiverP<float> pwm_duty_cycle;
protected:
	short pin;

	bool pwm_enabled;
	int pwm_duration;
	int pwm_duty_cycle_duration;
	int frames_count;
	
	/** fixup_pwm_durations recalculates the pwm cycle durations
	 * @execState - the execution state
	 * returns the led state in case pwm is disabled
	 */
	bool fixup_pwm_durations(State *execState);

	void set_state(State *execState, bool state);
};

#endif //LED_H
