/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
/* STEP 3 - Include the header file of the I2C API */
#include <zephyr/drivers/i2c.h>

/* STEP 4.1 - Include the header file of printk() */
#include <zephyr/sys/printk.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 1000

/* STEP 8 - Define the I2C slave device address and the addresses of relevant registers */

/* STEP 6 - Get the node identifier of the sensor */
#define I2C_BME280 DT_NODELABEL(bme680)
#define BME68X_REG_CHIP_ID     (0xd0)

int main(void)
{

	int ret;

	/* STEP 7 - Retrieve the API-specific device structure and make sure that the device is
	 * ready to use  */
	static const struct i2c_dt_spec bme680 = I2C_DT_SPEC_GET(I2C_BME280);
	if(!device_is_ready(bme680.bus)) {
		printk("I2C bus %s is not ready\n", bme680.bus->name);
		return -1;
	}
	uint8_t chip_id=0;
	uint8_t reg_addr = BME68X_REG_CHIP_ID;
	ret = i2c_write_read_dt(&bme680,&reg_addr,1,&chip_id,1);
	if(ret != 0) 
	{
		printk("Error reading chip id\n");
		return -1;
	}

	printk("Chip id is 0x%02x\n",chip_id);


	/* STEP 9 - Setup the sensor by writing the value 0x8C to the Configuration register */

	while (1) {
		/* STEP 10 - Read the temperature from the sensor */

		/* STEP 11 - Convert the two bytes to a 12-bits */

		k_msleep(SLEEP_TIME_MS);
	}
}
