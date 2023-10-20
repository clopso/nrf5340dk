#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACKSIZE 1024

#define THREAD0_PRIORITY 7
#define THREAD1_PRIORITY 7

#define SLEEP_TIME 500

void thread0(void)
{
    int i = 0;
	while (1) {
        printk("i = %d\n", i);
        i += 4;
        printk("thread0\n");
        //k_yield();
        if(i >= 60){
            i = 0;
        }
        k_msleep(SLEEP_TIME);
	}
}

void thread1(void)
{
    int j = 0;
	while (1) {
        printk("j = %d\n", j);
        j += 3;
        printk("thread1\n");	
        if(j >= 60){
            j = 0;
        }	  
        //k_yield();		  
        k_msleep(SLEEP_TIME);
	}
}

K_THREAD_DEFINE(thread0_id, STACKSIZE, thread0, NULL, NULL, NULL,
		THREAD0_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread1_id, STACKSIZE, thread1, NULL, NULL, NULL,
		THREAD1_PRIORITY, 0, 0);