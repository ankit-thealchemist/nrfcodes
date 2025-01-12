/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/logging/log.h>	

#define SPIOP	SPI_WORD_SET(8) | SPI_TRANSFER_MSB

#define SPI_DEVICE DT_NODELABEL(bme680)


LOG_MODULE_REGISTER(app, CONFIG_LOG_DEFAULT_LEVEL);

static struct spi_dt_spec bme680 = SPI_DT_SPEC_GET(SPI_DEVICE, SPIOP, 0);



#define WHO_AM_I 0xD0
static int bme_read_reg(uint8_t reg, uint8_t *data, uint8_t size)
{
	int err;

	/* STEP 4.1 - Set the transmit and receive buffers */
	uint8_t tx_buffer = reg;
	struct spi_buf tx_spi_buf			= {.buf = (void *)&tx_buffer, .len = 1};
	struct spi_buf_set tx_spi_buf_set 	= {.buffers = &tx_spi_buf, .count = 1};
	struct spi_buf rx_spi_bufs 			= {.buf = data, .len = size};
	struct spi_buf_set rx_spi_buf_set	= {.buffers = &rx_spi_bufs, .count = 1};

	/* STEP 4.2 - Call the transceive function */
	err = spi_transceive_dt(&bme680, &tx_spi_buf_set, &rx_spi_buf_set);
	if (err < 0) {
		LOG_ERR("spi_transceive_dt() failed, err: %d", err);
		return err;
	}

	return 0;

}

int main(void)
{	
	int err;
	LOG_INF("Welcome! %s\r\n", CONFIG_BOARD_TARGET);

	err = spi_is_ready_dt(&bme680);
	if (err != true)	
	{
		LOG_ERR("SPI is not ready, with error code: %d", err);
		return -1;
	}
	
	LOG_INF("SPI is ready!");

	uint8_t data[2] = {0};
	err = bme_read_reg(WHO_AM_I, &data, sizeof(data));
	if (err != 0)
	{
		LOG_ERR("BME680 not ready, with error code: %d", err);
		return -1;
	}

	// first byte is dummy byte
	LOG_INF("BME680 returned: 0x%02x", data[1]);


	return 0;
}
