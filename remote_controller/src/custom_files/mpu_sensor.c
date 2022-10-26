#include "mpu_sensor.h"
#include "nrfx_twim.h"
#include <drivers/i2c.h>
#include <nrf52840.h>

#define LOG_MODULE_NAME mpu_sensor
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

//static const struct nrfx_twim_t m_twi_instance 


void my_twim_handler(nrfx_twim_evt_t const * p_event, void * p_context)
{
    LOG_INF("TWIM callback");
}

int mpu_sensor_init(void)
{
    int err = 0;

    LOG_INF("Initializing MPU Sensor");

    const nrfx_twim_t m_twim_instance       = NRFX_TWIM_INSTANCE(0);
    const nrfx_twim_config_t twim_config    = NRFX_TWIM_DEFAULT_CONFIG(4,3);

    err = nrfx_twim_init(&m_twim_instance,
                          &twim_config,
                          my_twim_handler,
                          NULL);
                          
    if (err != NRFX_SUCCESS) {
        LOG_ERR("twim_init failed. (err %x)", err);
        return err;
    }

    nrfx_twim_enable(&m_twim_instance);

    return 0;
}