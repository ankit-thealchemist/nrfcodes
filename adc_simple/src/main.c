/**
 * @file main.c
 * @author Ankit Bansal (iotdevelope@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */

/******************************************************************************
 * INCLUDES
 ******************************************************************************/
#include <stdio.h>
#include <zephyr/devicetree.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/adc.h>

/******************************************************************************
 * EXTERN VARIABLES
 ******************************************************************************/

/******************************************************************************
 * PRIVATE MACROS AND DEFINES
 ******************************************************************************/
LOG_MODULE_REGISTER(main);
/******************************************************************************
 * PRIVATE TYPEDEFS
 ******************************************************************************/

/******************************************************************************
 * STATIC VARIABLES
 ******************************************************************************/
static const struct adc_dt_spec adc_channel = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/

/******************************************************************************
 * STATIC FUNCTION PROTOTYPES
 ******************************************************************************/

/******************************************************************************
 * STATIC FUNCTIONS
 ******************************************************************************/

/******************************************************************************
 * GLOBAL FUNCTIONS
 ******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/

int main()
{
    int err;
    if (!adc_is_ready_dt(&adc_channel))
    {
        LOG_ERR("ADC controller devivce %s not ready", adc_channel.dev->name);
        return 0;
    }

    err = adc_channel_setup_dt(&adc_channel);
    if( err < 0 )
    {
        LOG_ERR("Could not setup channel #%d (%d)", 0, err);
        return 0;
    }

    int16_t buf;
    struct adc_sequence sequence = {
        .buffer = &buf,
        .buffer_size = sizeof(buf),
    };

    err = adc_sequence_init_dt(&adc_channel, &sequence);
	if (err < 0) {
		LOG_ERR("Could not initalize sequnce");
		return 0;
	}

    int adc_mv;
    while (1)    
    {
        err = adc_read_dt(&adc_channel, &sequence);
        if (err < 0)
        {
            LOG_ERR("Could not read from sequnce %d", err);
            return 0;
        }
        adc_mv = (int)buf;
        LOG_INF("ADC raw value reading: %d", adc_mv);

        err = adc_raw_to_millivolts_dt(&adc_channel, &adc_mv);
        if (err < 0)
        {
            LOG_ERR("Could not convert raw value to millivolts %d", err);
            return 0;
        }
        LOG_INF("ADC millivolts reading: %d", adc_mv);

        k_sleep(K_MSEC(1000));
    }

    return 0;
}