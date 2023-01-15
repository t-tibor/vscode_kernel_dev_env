#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>



static int __init my_module_init(void)
{
    printk(KERN_INFO "Hello, World!\n");
    return 0;
}

static void __exit my_module_exit(void)
{
    printk(KERN_INFO "Goodbye, World!\n");
}

module_init(my_module_init);
module_exit(my_module_exit);


MODULE_AUTHOR("Tusori Tibor");
MODULE_DESCRIPTION("A minimal simple kernel module.");
MODULE_VERSION("1.0");
MODULE_LICENSE("GPL");