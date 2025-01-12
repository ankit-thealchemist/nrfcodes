/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>

#define PWM_LED0_NODE DT_ALIAS(pwm_led0)
#define PWM_SERVO_NODE DT_ALIAS(pwm_servo)

//Use DT_PROP() to obtain the minimum and maximum duty cycle */
#define PWM_SERVO_MIN_DUTY_CYCLE    DT_PROP(PWM_SERVO_NODE, min_pulse)
#define PWM_SERVO_MAX_DUTY_CYCLE    DT_PROP(PWM_SERVO_NODE, max_pulse)

#define PWM_PERIOD_MS 20
#define PWM_DUTY_CYCLE_MS 1
#define SLEEP_TIME_MS 20

static struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(PWM_LED0_NODE);
static struct pwm_dt_spec pwm_servo = PWM_DT_SPEC_GET(PWM_SERVO_NODE);

int main(void)
{
	int rc;
	rc = pwm_is_ready_dt(&pwm_led0);
	printf("Starting the application\r\n");
	if(rc < 0)
	{
		printf("Error: PWM device %s is not ready\n", pwm_led0.dev->name);
		return 1;
	}

	rc = pwm_is_ready_dt(&pwm_servo);
	if(rc < 0)
	{
		printf("Error: PWM device %s is not ready\n", pwm_servo.dev->name);
		return 1;
	}

	// below code will provide the glow effect on the LED
	do
	{

		int duty_cycle = 0;
		for (int index = 0; index < 100; index += 4)
		{
			duty_cycle = (index * PWM_PERIOD_MS)/100;
			rc = pwm_set_dt(&pwm_led0, PWM_MSEC(PWM_PERIOD_MS), PWM_MSEC(duty_cycle));
			if (rc < 0)
			{
				printf("Error: PWM device %s is not ready\n", pwm_led0.dev->name);
				return 1;
			}
			k_msleep(SLEEP_TIME_MS);
		}

		for (int index = 100; index > 0; index -= 4)
		{
			duty_cycle = (index * PWM_PERIOD_MS)/100;
			rc = pwm_set_dt(&pwm_led0, PWM_MSEC(PWM_PERIOD_MS), PWM_MSEC(duty_cycle));
			if (rc < 0)
			{
				printf("Error: PWM device %s is not ready\n", pwm_led0.dev->name);
				return 1;
			}
			k_msleep(SLEEP_TIME_MS);
		}
	} while (1);
	

	printf("PWM device %s is ready\n", pwm_led0.dev->name);

	return 0;
}
