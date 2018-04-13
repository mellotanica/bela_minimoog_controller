/***** led.h *****/

#ifndef LED_H
#define LED_H

#include <component.h>

#define PWM_DC_ERROR 0.001f

class led : public component {
public:
	/** led constructor
	 * @pin - the digital pin connected to the led expressed with digital_gpio_mapping.h naming
	 */
	led(short pin, float pwm_period = 0.01, float pwm_duty_cycle = 1);
	
	// component interface
	void setup(BelaContext *context, void *userData);
	void execute(BelaContext *context, void *userData, unsigned int audioFrameCount, unsigned int analogFrameCount, unsigned int digitalFrameCount);
	
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
	
	bool state;
	bool required_state;
	
	bool pwm_enabled;
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