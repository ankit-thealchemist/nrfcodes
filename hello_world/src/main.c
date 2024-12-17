/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main,LOG_LEVEL_DBG);

int main(void)
{
	float f = 3.14;
	LOG_INF("Hello World! %s\r\n", CONFIG_BOARD);
	LOG_INF("float value is %0.2f\r\n", f);
	LOG_ERR("This is an error message");
	LOG_WRN("This is a warning message");
	LOG_INF("This is an info message");
	LOG_DBG("This is a debug message");
	return 0;
}
