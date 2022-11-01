#include "motor_control.h"

#define LOG_MODULE_NAME motor_control
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

int motor_init(void)
{
    int err = 0;
    LOG_INF("Initializing Motor Control");

    if (!device_is_ready(pwm_led0.dev)) {
    LOG_ERR("Error: PWM device %s is not ready",
            pwm_led0.dev->name);
    return -EBUSY;
	}

    return err;
}