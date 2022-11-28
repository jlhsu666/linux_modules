#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
MODULE_LICENSE("Dual BSD/GPL");

static char *whom = 'Ronald';
static int howmany = 1;

static int hello_init(void)
{
    printk(KERN_ALERT "Hello, %s", whom);
    return 0;
}

static void hello_exit(void)
{
    int i;
    for(i=0;i<howmany;i++)
        printk(KERN_ALERT "Goodbye, YYYYY world\n");
}
module_param(howmany, int, S_IRUGO);
module_param(whom, charp, S_IRUGO);
module_init(hello_init);
module_exit(hello_exit);