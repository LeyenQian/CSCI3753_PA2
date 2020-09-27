#include <linux/module.h>   // module management operation

MODULE_AUTHOR("Leyen Qian");
MODULE_LICENSE("GPL");

int hello_init(void)
{
    printk("------------ Lakka <hello_module> ------------\n");
    printk(KERN_ALERT "inside %s function\n",__FUNCTION__);
    printk("----------------------------------------------\n");
    return 0;
}

void hello_exit(void)
{
    printk("------------ Lakka <hello_module> ------------\n");
    printk(KERN_ALERT "inside %s function\n",__FUNCTION__);
    printk("----------------------------------------------\n");
}

module_init(hello_init);
module_exit(hello_exit);