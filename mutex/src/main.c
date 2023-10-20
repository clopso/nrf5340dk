/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/random/rand32.h>
#include <string.h>

#define THREAD0_STACKSIZE       512
#define THREAD1_STACKSIZE       512

#define THREAD0_PRIORITY 4
#define THREAD1_PRIORITY 4

#define COMBINED_TOTAL   20

int32_t increment_count = 0; 
int32_t decrement_count = COMBINED_TOTAL; 

K_MUTEX_DEFINE(test_mutex);

#define USE_MUTEX 1

// Shared code run by both threads
void shared_code_section(void)
{
#if USE_MUTEX
	k_mutex_lock(&test_mutex, K_FOREVER);
#endif

	increment_count++;
	increment_count = increment_count % COMBINED_TOTAL;

	decrement_count--;
	if (decrement_count == 0)
		decrement_count = COMBINED_TOTAL;

#if USE_MUTEX
	k_mutex_unlock(&test_mutex);
#endif

	if((increment_count + decrement_count) != COMBINED_TOTAL )
	{
		printk("Race condition happend!\n");
		printk("Increment_count (%d) + Decrement_count (%d) = %d \n",
	                increment_count, decrement_count, (increment_count + decrement_count));
		k_msleep(100 * (sys_rand32_get() % 4 + 1));
	}
}

void thread0()
{
	printk("Thread 0 started\n");
	while (1) {
		shared_code_section();
	}
}

void thread1()
{
	printk("Thread 1 started\n");
	while (1) {
		shared_code_section(); 
	}
}

// Define and initialize threads
K_THREAD_DEFINE(thread0_id, THREAD0_STACKSIZE, thread0, NULL, NULL, NULL,
		THREAD0_PRIORITY, 0, 0);

K_THREAD_DEFINE(thread1_id, THREAD1_STACKSIZE, thread1, NULL, NULL, NULL,
		THREAD1_PRIORITY, 0, 0);
