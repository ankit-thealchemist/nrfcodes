/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <nrfx_saadc.h>

#define SAADC_INPUT_PIN NRF_SAADC_INPUT_AIN0
#define BATTERY_SAMPLE_INTERVAL_MS 2000

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static void battery_sample_timer_handler(struct k_timer *timer);
K_TIMER_DEFINE(battery_sample_timer, battery_sample_timer_handler, NULL);

static nrfx_saadc_channel_t channel = NRFX_SAADC_DEFAULT_CHANNEL_SE(SAADC_INPUT_PIN, 0);
static int16_t sample;

static void configure_saadc(void)
{
	IRQ_CONNECT(DT_IRQN(DT_NODELABEL(adc)), DT_IRQ(DT_NODELABEL(adc), priority), nrfx_isr, nrfx_saadc_irq_handler, 0);

	nrfx_err_t err = nrfx_saadc_init(DT_IRQ(DT_NODELABEL(adc), priority));
	if (err != NRFX_SUCCESS)
	{
		LOG_ERR("nrfx_saadc_mode_trigger error: %08x", err);
		return;
	}

	channel.channel_config.gain = NRF_SAADC_GAIN1_6;
	err = nrfx_saadc_channels_config(&channel, 1);
	if (err != NRFX_SUCCESS)
	{
		LOG_ERR("nrfx_saadc_channels_config error: %08x", err);
		return;
	}

	err = nrfx_saadc_simple_mode_set(BIT(0),
									 NRF_SAADC_RESOLUTION_12BIT,
									 NRF_SAADC_OVERSAMPLE_DISABLED,
									 NULL);
	if (err != NRFX_SUCCESS)
	{
		LOG_ERR("nrfx_saadc_simple_mode_set error: %08x", err);
		return;
	}

	err = nrfx_saadc_buffer_set(&sample, 1);
	if (err != NRFX_SUCCESS)
	{
		LOG_ERR("nrfx_saadc_buffer_set error: %08x", err);
		return;
	}

	k_timer_start(&battery_sample_timer, K_NO_WAIT, K_MSEC(BATTERY_SAMPLE_INTERVAL_MS));
}

int main(void)
{
	LOG_INF("NRF adc example! %s\r\n", CONFIG_BOARD);
	configure_saadc();
	k_sleep(K_FOREVER);
	return 0;
}

void battery_sample_timer_handler(struct k_timer *timer)
{

	nrfx_err_t err = nrfx_saadc_mode_trigger();
	if (err != NRFX_SUCCESS)
	{
		LOG_ERR("nrfx_saadc_mode_trigger error: %08x", err);
		return;
	}

	int battery_voltage = ((600 * 6) * sample) / ((1 << 12));

	LOG_INF("SAADC sample: %d\n", sample);
	LOG_INF("Battery Voltage: %d mV\n", battery_voltage);
}