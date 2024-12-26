/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>


/* STEP 2 - Define stack size and scheduling priority used by each thread */
#define STACKSIZE 1024
#define THREAD0_PRIORITY 7
#define THREAD1_PRIORITY 6


LOG_MODULE_REGISTER(main,LOG_LEVEL_DBG);

typedef struct {
    uint32_t x_reading;
    uint32_t y_reading;
    uint32_t z_reading;
} SensorReading;

int8_t test_var = 0;

static struct gpio_dt_spec gpio_led_0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);


static void timer0_handler(struct k_timer *dummy)
{
	gpio_pin_toggle_dt(&gpio_led_0);
}

K_TIMER_DEFINE(timer0, timer0_handler, NULL);
K_MSGQ_DEFINE(device_message_queue, sizeof(SensorReading), 5, 4);

// this task will start the timer 
void thread0(void);
void thread0(void)
{	
	if (!gpio_is_ready_dt(&gpio_led_0)) {
		LOG_ERR("LED0 device is not ready");
		return;
	}
	if (gpio_pin_configure_dt(&gpio_led_0, GPIO_OUTPUT_ACTIVE)) {
		LOG_ERR("LED0 pin configuration failed");
		return;
	}

	k_timer_start(&timer0, K_MSEC(500), K_MSEC(500));
	SensorReading reading = {0};
	int ret;
	while (1) {
		/* STEP 3 - Call printk() to display a simple string "Hello, I am thread0" */
		LOG_INF("Reading sensor data\n");
		reading.x_reading += 1;
		reading.y_reading += 2;
		reading.z_reading += 3;
		ret = k_msgq_put(&device_message_queue, &reading, K_MSEC(5000));
		if(ret)
		{
			LOG_ERR("Error sending message to queue: %d", ret);
		}
		k_msleep(2200);
		
	}
}

void thread1(void)
{
	int ret;
	SensorReading temp;
	while (1)
	{

		ret = k_msgq_get(&device_message_queue, &temp, K_FOREVER);
		if (ret)
		{
			LOG_ERR("Return value from k_msgq_get = %d", ret);
		}
		else
		{
			LOG_INF("Sensor data: x = %d, y = %d, z = %d", temp.x_reading, temp.y_reading, temp.z_reading);
		}
		k_yield();
	}
}

int main(void)
{
	LOG_INF("Hello from my_module!");
	for(;test_var < 127; test_var++)
	{
		LOG_INF("test_var = %d\r\n", test_var);
		test_var++;
	}

}



// K_THREAD_DEFINE(thread0_id, STACKSIZE, thread0, NULL, NULL, NULL,
// 		THREAD0_PRIORITY, 0, 0);
// K_THREAD_DEFINE(thread1_id, STACKSIZE, thread1, NULL, NULL, NULL,
// 		THREAD1_PRIORITY, 0, 0);