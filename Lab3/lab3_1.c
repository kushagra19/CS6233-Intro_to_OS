#include<linux/init.h>
#include<linux/module.h>
#include<linux/timekeeping.h>
#include<linux/jiffies.h>

MODULE_LICENSE("Dual BSD/GPL");

typedef unsigned long ul;

// decalring global variables for start time
static ul start_jiffies;
static ktime_t start_time;

static int hello_init(void)
{	
	ul tick_time_ms;
	//start jiffies initialized for method 1
	start_jiffies = jiffies;
	//start time initialized for method 2
	start_time = ktime_get_boottime();
	
	//Calculating tick time in mili seconds using HZ
	tick_time_ms = (10e3/HZ);
	
	//printing statements
	printk(KERN_ALERT "Hello, World\n");
	printk(KERN_ALERT "Tick Time in msecs: %lu ms \n", tick_time_ms);
	return 0;
}

void hello_exit(void)
{	
	//declaring and initializing end time for both methods
	ktime_t end_time = ktime_get_boottime();
	ul end_jiffies = jiffies;
	//calculating the time elapsed from start using both methods
	ul time_interval_ms = ktime_to_ms(ktime_sub(end_time,start_time));
	ul time_interval_jiffies = jiffies_to_msecs(end_jiffies - start_jiffies);
	
	//printing statements
	printk(KERN_ALERT "Goodbye, cruel world\n");
	printk(KERN_ALERT "Time difference between the insertion and removal using method 1 : %lu ms\n", time_interval_jiffies);
	printk(KERN_ALERT "Time difference between the insertion and removal using method2 : %lu ms\n", time_interval_ms);
}

module_init(hello_init);
module_exit(hello_exit);


