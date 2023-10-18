/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

#define SLEEP_TIME_MS	50

#define USE_CALLBACK 1

/*
 * Get button configuration from the devicetree alias. This is mandatory.
 */
#define SW0_NODE	DT_ALIAS(sw0)
#define SW1_NODE	DT_ALIAS(sw1)
#define SW2_NODE	DT_ALIAS(sw2)
#define SW3_NODE	DT_ALIAS(sw3)

#define LED0_NODE	DT_ALIAS(led0)
#define LED1_NODE	DT_ALIAS(led1)
#define LED2_NODE	DT_ALIAS(led2)
#define LED3_NODE	DT_ALIAS(led3)

static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET(SW0_NODE, gpios);
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET(SW1_NODE, gpios);
static const struct gpio_dt_spec button2 = GPIO_DT_SPEC_GET(SW2_NODE, gpios);
static const struct gpio_dt_spec button3 = GPIO_DT_SPEC_GET(SW3_NODE, gpios);

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);

#if USE_CALLBACK
static struct gpio_callback button_cb_data;

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	if((pins >> button0.pin) == 1) {
		gpio_pin_toggle_dt(&led0);
	}
	if((pins >> button1.pin) == 1) {
		gpio_pin_toggle_dt(&led1);
	}
	if((pins >> button2.pin) == 1) {
		gpio_pin_toggle_dt(&led2);
	}
	if((pins >> button3.pin) == 1) {
		gpio_pin_toggle_dt(&led3);
	}
}
#endif

int main(void)
{
	gpio_pin_configure_dt(&button0, GPIO_INPUT);
	gpio_pin_configure_dt(&button1, GPIO_INPUT);
	gpio_pin_configure_dt(&button2, GPIO_INPUT);
	gpio_pin_configure_dt(&button3, GPIO_INPUT);

	gpio_pin_configure_dt(&led0, GPIO_OUTPUT);
	gpio_pin_configure_dt(&led1, GPIO_OUTPUT);
	gpio_pin_configure_dt(&led2, GPIO_OUTPUT);
	gpio_pin_configure_dt(&led3, GPIO_OUTPUT);

#if USE_CALLBACK
	gpio_pin_interrupt_configure_dt(&button0, GPIO_INT_EDGE_TO_ACTIVE);
	gpio_pin_interrupt_configure_dt(&button1, GPIO_INT_EDGE_TO_ACTIVE);
	gpio_pin_interrupt_configure_dt(&button2, GPIO_INT_EDGE_TO_ACTIVE);
	gpio_pin_interrupt_configure_dt(&button3, GPIO_INT_EDGE_TO_ACTIVE);

	gpio_port_pins_t buttonPins = BIT(button0.pin) | BIT(button1.pin) | BIT(button2.pin) | BIT(button3.pin);
	gpio_init_callback(&button_cb_data, button_pressed, buttonPins);

	gpio_add_callback(button0.port, &button_cb_data);
	gpio_add_callback(button1.port, &button_cb_data);
	gpio_add_callback(button2.port, &button_cb_data);
	gpio_add_callback(button3.port, &button_cb_data);
#else
	int val[4] = {0};

	while (1) {
		val[0] = gpio_pin_get_dt(&button0);
		val[1] = gpio_pin_get_dt(&button1);
		val[2] = gpio_pin_get_dt(&button2);
		val[3] = gpio_pin_get_dt(&button3);

		gpio_pin_set_dt(&led0, val[0]);
		gpio_pin_set_dt(&led1, val[1]);
		gpio_pin_set_dt(&led2, val[2]);
		gpio_pin_set_dt(&led3, val[3]);

		k_msleep(SLEEP_TIME_MS);
	}
#endif

	return 0;
}
