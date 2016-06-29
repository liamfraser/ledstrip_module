#include <linux/init.h>   // Macros used to mark up functions e.g., __init __exit
#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/kernel.h> // Contains types, macros, functions for the kernel
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Liam Fraser");
MODULE_DESCRIPTION("PWM driver for LED strip. Uses DMA.");
MODULE_VERSION("0.1");

static int __init ledstrip_init(void) {
   printk(KERN_INFO "ledstrip module loaded\n");
   return 0;
}
 
static void __exit ledstrip_exit(void) {
   printk(KERN_INFO "ledstrip module removed\n");
}
 
module_init(ledstrip_init);
module_exit(ledstrip_exit);
