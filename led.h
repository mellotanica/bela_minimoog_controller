/***** led.h *****/

#ifndef LED_H
#define LED_H

#include <component.h>

#define PWM_DC_ERROR 0.001f

class led : public output {
public:
	/** led constructor
	 * @pin - the digital pin connected to the led expressed with digital_gpio_mapping.h naming
	 */
	led(short pin);
	
	// component interface
	void setup(BelaContext *context, void *userData);
	void render(State *execState);
	
	Receiver<bool> state;
	Receiver<float> pwm_period;
	Receiver<float> pwm_duty_cycle;
protected:
	short pin;

	bool pwm_enabled;
	int pwm_duration;
	int pwm_duty_cycle_duration;
	int frames_count;
	
	/** fixup_pwm_durations recalculates the pwm cycle durations
	 * @context - the execution context
	 */
	void fixup_pwm_durations(State *execState);

	void set_state(State *execState, bool state);
};

#endif //LED_H