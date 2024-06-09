#include <linux/module.h>
#include <linux/kernel.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("MANISHA");
MODULE_DESCRIPTION("HELLO WORLD PROGRAM");
MODULE_VERSION("1.0");

int hello_init_module(void) {
    printk(KERN_INFO "cats module loaded!\n");
    pr_info("Module Loaded\n");
    return 0;
}

void exit_cleanup_module(void){
    printk(KERN_INFO "cats module unloaded!\n");
}

module_init(hello_init_module);
module_exit(exit_cleanup_module);
