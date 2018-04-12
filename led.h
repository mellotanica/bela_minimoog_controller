/***** led.h *****/

#ifndef LED_H
#define LED_H

#include <component.h>

class led : public component {
public:
	/** led constructor
	 * @pin - the digital pin connected to the led expressed with digital_gpio_mapping.h naming
	 */
	led(short pin);
	
	// component interface
	void setup(BelaContext *context, void *userData);
	void read(BelaContext *context, void *userData, unsigned int audioFrameCount, unsigned int analogFrameCount, unsigned int digitalFrameCount);
	void execute(BelaContext *context, void *userData, unsigned int audioFrameCount, unsigned int analogFrameCount, unsigned int digitalFrameCount);
	void cleanup(BelaContext *context, void *userData);
	
	/** set_active enables and disables the led
	 * @active - the new state of the led
	 */
	void set_active(bool active);

	/** set_pwm_period set the period of the pulse width modulation in seconds
	 * @context - the execution context
	 * @period - the pwm period in seconds
	 */
	void set_pwm_period(BelaContext *context, float period);
	
	/** set_pwm_duty_cycle set the pulse width modulation duty cycle
	 * @context - the execution context
	 * @period - the pwm duty cycle (value between 0 and 1)
	 */
	void set_pwm_duty_cycle(BelaContext *context, float dc);
	
protected:
	short pin;
	
	bool is_active;
	bool state;
	
	float pwm_period;
	float pwm_duty_cycle;
	int pwm_duration;
	int pwm_duty_cycle_duration;
	int frames_count;
	
	/** fixup_pwm_durations recalculates the pwm cycle durations
	 * @context - the execution context
	 */
	void fixup_pwm_durations(BelaContext *context);
	
	/** set_state - toggles the led state
	 * @context - the execution context
	 * @reference_frame - the first frame when to switch state
	 * @state - true == GPIO_HIGH, false == GPIO_LOW
	 */
	void set_state(BelaContext *context, int reference_frame, bool state);
};

#endif //LED_H