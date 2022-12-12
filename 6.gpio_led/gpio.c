#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/err.h>

#define GPIO_21 (21)

dev_t dev=0;
static struct class *dev_class;
static struct cdev  etx_cdev;

static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);

static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off);

static struct file_operations fops = 
{
        .owner =    THIS_MODULE,
        .read  =    etx_read,
        .write =    etx_write,
        .open  =    etx_open,
        .release =  etx_release, 
};

static  int etx_open(struct inode *inode, struct file *file)
{
    pr_info("Device File Opened\n");
    return 0;
}

static int etx_release(struct inode *inode, struct file *file)
{
    pr_info("Device File Closed\n");
    return 0;
}

static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    uint8_t gpio_state=0;
    gpio_state=gpio_get_value(GPIO_21);

    len=1;
    if(copy_to_user(buf,&gpio_state,len)>0){
        pr_err("ERROR: Not all the bytes have been copied to users\n");
    }
    pr_info("Read function: GPIO 21 =%d \n", gpio_state);
    return 0;
}

static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    uint8_t rec_buf[10]={0};
    if(copy_from_user(rec_buf,buf,len)>0){
        pr_err("ERROR: Not all the bytes have been copied from users\n");
    }
    pr_info("Write function: GPIO 21 Set =%c \n", rec_buf[0]);

    if(rec_buf[0]=='1'){
        gpio_set_value(GPIO_21,1);
    }
    else if (rec_buf[0]=='0'){
        gpio_set_value(GPIO_21,0);
    }
    else{
        pr_err("Unknown command. Please enter either 1 or 0\n");

    }
    return len;   
}

static int __init etx_driver_init(void)
{
    if((alloc_chrdev_region(&dev,0,1,"etx_Dev")) < 0) 
    {
        pr_info("Cannot allocate major number!\n");
        goto r_unreg;
    }    
    pr_info("Major = %d, Minor= %d \n", MAJOR(dev), MINOR(dev));

    cdev_init(&etx_cdev, &fops);

    if((cdev_add(&etx_cdev,dev,1)) < 0) 
    {
        pr_info("Cannot add device to the system!\n");
        goto r_del;
    }      

    if(IS_ERR(dev_class = class_create(THIS_MODULE, "etx_class"))) 
    {
        pr_info("Cannot create structure class!\n");
        goto r_class;
    }   

    if(IS_ERR(device_create(dev_class, NULL, dev, NULL,"etx_device"))) 
    {
        pr_info("Cannot create device!\n");
        goto r_device;
    }   

    if(gpio_is_valid(GPIO_21)==false){
        pr_err("GPIO %d is no valid\n", GPIO_21);
        goto r_device;
    }

    if(gpio_request(GPIO_21, "GPIO_21")<0){
        pr_err("GPIO %d request\n", GPIO_21);
        goto r_gpio;
    }    

    gpio_direction_output(GPIO_21,0);

    gpio_export(GPIO_21,false);

    pr_info("Device Driver Insert ... Done\n");
    return 0;

r_gpio:
    gpio_free(GPIO_21);
r_device:
    device_destroy(dev_class,dev);
r_class:
    class_destroy(dev_class);
r_del:
    cdev_del(&etx_cdev);
r_unreg:    
    unregister_chrdev_region(dev,1);
    
    return -1;
}       

static void __exit etx_driver_exit(void)
{
    gpio_unexport(GPIO_21);
    gpio_free(GPIO_21);
    device_destroy(dev_class,dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev,1);
    pr_info("Device Driver Remove ... Done\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");