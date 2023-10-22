//header file initialization
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/jiffies.h>
#include<linux/cdev.h>
#include<linux/miscdevice.h>
#include<linux/uaccess.h>

#define DEVICE_NAME "Lab6"

MODULE_LICENSE("Dual BSD/GPL");

//static global variable to store major number
static unsigned long startJiffies;

//open method for misc device
static int lab6Open(struct inode *inode, struct file *file)
{
    return 0;
}
//release method for misc device
static int lab6Release(struct inode *inode, struct file *file)
{
    return 0;
}
//Read method for misc device
static ssize_t lab6Read(struct file *file, char __user *buf, size_t count, loff_t *f_pos) 
{
    // Calculate time difference between loading the module and invoking read
    unsigned long timeDiff = (jiffies - startJiffies)/HZ;
    ssize_t toCopy, remainingLength;
    // Prepare the message
    char message[64];
    snprintf(message, sizeof(message), "Hello world, the time difference is %lus.\n",timeDiff);

    //remaining length of message
    remainingLength = strlen(message) - (ssize_t)*f_pos;
    // Determine the maximum number of bytes to copy to user space
    toCopy = min((ssize_t) count, remainingLength);
    
    //if EOF reached
    if(toCopy <= 0)
        return 0;   
    // Use copy_to_user to copy data to user space
    if (copy_to_user(buf, message, toCopy) != 0) {
        return -EFAULT;  // Handle error
    }

    // Update the file position
    *f_pos += toCopy;

    return toCopy;
}

//file operations structure
static const struct file_operations lab6fops ={
    .owner = THIS_MODULE,
    .read = lab6Read,
    .open = lab6Open,
    .release = lab6Release,
};

//misc device structure
static struct miscdevice miscDeviceLab6 = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &lab6fops,
};

//Initialize module
static int __init lab6_Init(void)
{
    //variable to store major number
    int errCode = misc_register(&miscDeviceLab6);
    //check if major number assigned successfully
    if(errCode)
        printk(KERN_ALERT "Misc Device failed to register.");
    //record jiffies while loading the module
    startJiffies = jiffies;
    return 0;    
}
//Exit module
static void __exit lab6_Exit(void)
{
    misc_deregister(&miscDeviceLab6);
}
//calls to init and exit
module_init(lab6_Init);
module_exit(lab6_Exit);