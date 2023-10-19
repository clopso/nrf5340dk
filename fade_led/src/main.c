/*
 * Copyright (c) 2016 Intel Corporation
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file Sample app to demonstrate PWM-based LED fade
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>

static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

#define NUM_STEPS	200
#define SLEEP_MSEC	20

int main(void)
{
	uint32_t pulse_width = 0;
	uint32_t step = pwm_led0.period / NUM_STEPS;
	uint8_t dir = 1;

	while (1) {
		pwm_set_pulse_dt(&pwm_led0, pulse_width);

		if (dir) {
			pulse_width += step;
			if (pulse_width >= pwm_led0.period) {
				pulse_width = pwm_led0.period - step;
				dir = 0;
				k_msleep(SLEEP_MSEC);
			}
		} else {
			if (pulse_width >= step) {
				pulse_width -= step;
			} else {
				pulse_width = step;
				dir = 1;
				k_msleep(SLEEP_MSEC);
			}
		}
		k_msleep(SLEEP_MSEC);
	}
	return 0;
}