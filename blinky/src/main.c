/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   10 * 60 * 1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define BUTTON0_NODE DT_ALIAS(sw0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(BUTTON0_NODE, gpios);

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    gpio_pin_toggle_dt(&led);
}

int main(void)
{
	int ret;

	if (!gpio_is_ready_dt(&led)) 
	{
		return 0;
	}

	if (!gpio_is_ready_dt(&button)) 
	{
		return 0;
	}


	

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) 
	{
		return 0;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) {
		return -1;
	}

	ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret < 0) 
	{
		return 0;
	}

	static struct gpio_callback button_cb_data;
	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));

	ret = gpio_add_callback(button.port, &button_cb_data);
	if (ret < 0)
	{
		return 0;
	}

	while(1)
	{
		k_msleep(SLEEP_TIME_MS);
	}

	return 0;
}
