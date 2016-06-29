// Based on:
// http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/#Character_Device_Drivers

#include <linux/init.h>   // Macros used to mark up functions e.g., __init __exit
#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/kernel.h> // Contains types, macros, functions for the kernel
#include <linux/device.h> // Header to support the kernel Driver Model
#include <linux/fs.h>     // Header for the Linux file system support
#include <asm/uaccess.h>  // Required for the copy to user function

#define DEVICE_NAME "ledstrip" // Will show up as /dev/ledstrip
#define CLASS_NAME "pwmledstrip"
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Liam Fraser");
MODULE_DESCRIPTION("PWM driver for LED strip. Uses DMA.");
MODULE_VERSION("0.1");

static int major_number; // Device number. Determined automatically
static struct class *pwm_ledstrip_dev_class = NULL; // Device class struct
static struct device *ledstrip_dev = NULL; // Device struct

// Define function prototypes we will implement
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

// Define file operations we will provide
// See http://www.tldp.org/LDP/lkmpg/2.4/html/c577.htm
static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

// LED strip data
static unsigned int colours[3];
static char msg_buf[256];
static int send_eof = 0;

static int __init ledstrip_init(void) {
    printk(KERN_INFO "ledstrip module loaded\n");

    // Allocate major number
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "ledstrip failed to register a major number\n");
        return major_number;
    }

    // Register device class
    pwm_ledstrip_dev_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(pwm_ledstrip_dev_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "ledstrip failed to register dev class\n");
        // PTR_ERR() retrieves the error number from the pointer
        return PTR_ERR(pwm_ledstrip_dev_class);
    }
    printk(KERN_INFO "ledstrip device class registered successfully\n");

    // Register device driver
    // See https://www.kernel.org/doc/htmldocs/device-drivers/API-device-create.html
    ledstrip_dev = device_create(pwm_ledstrip_dev_class,
                                 NULL,
                                 MKDEV(major_number, 0),
                                 NULL,
                                 DEVICE_NAME);

    if (IS_ERR(ledstrip_dev)) {
        class_destroy(pwm_ledstrip_dev_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create ledstrip device\n");
        return PTR_ERR(ledstrip_dev);
    }

    printk(KERN_INFO "ledstrip device initialised");

    // Set default colours
    for (int i = 0; i < sizeof(colours)/sizeof(int); i++) {
        colours[i] = 0;
    }

    return 0;
}
 
static void __exit ledstrip_exit(void) {
    device_destroy(pwm_ledstrip_dev_class, MKDEV(major_number, 0));
    class_unregister(pwm_ledstrip_dev_class);
    class_destroy(pwm_ledstrip_dev_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "ledstrip module removed\n");
}

static int dev_open(struct inode *inodep, struct file *filep) {
    return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int error_count = 0;
    int msg_len = 0;

    // Need to send an EOF after we've sent colours
    if (send_eof == 1) {
        send_eof = 0;
        return 0;
    } else {
        send_eof = 1;
    }

    // Output R G B values
    sprintf(msg_buf, "R: %d, G:%d, B:%d\n", colours[0], colours[1], colours[2]);
    msg_len = strlen(msg_buf);
    
    error_count = copy_to_user(buffer, msg_buf, msg_len);

    if (error_count == 0) {
        printk(KERN_INFO "ledstrip sent string of length %d", msg_len);
        return msg_len;
    } else {
        printk(KERN_INFO "ledstrip failed to send values to user\n");
        return -EFAULT; // Bad address error
    }
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    return 0;
}
 
module_init(ledstrip_init);
module_exit(ledstrip_exit);
