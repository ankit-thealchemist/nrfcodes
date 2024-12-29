/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>

#define PWM_LED0_NODE DT_ALIAS(pwm_led0)

#define PWM_PERIOD_NS 200000000
#define PWM_DUTY_CYCLE 14000000

static struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(PWM_LED0_NODE);

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

	rc = pwm_set_dt(&pwm_led0, PWM_MSEC(40), PWM_MSEC(1));
	if(rc < 0)
	{
		printf("Error: PWM device %s is not ready\n", pwm_led0.dev->name);
		return 1;
	}

	printf("PWM device %s is ready\n", pwm_led0.dev->name);

	return 0;
}
