#include <linux/fs.h>       // file operations
#include <linux/uaccess.h>  // kernel & user space interaction
#include <linux/module.h>   // module management operations
#include <linux/slab.h>     // memory management operations
#include <linux/cdev.h>     // character device initialization
#include <linux/device.h>   // dynamically device file create
#include <linux/mm.h>

#define MAJOR_NUM 440
#define MINOR_NUM 0
#define DEVICE_NAME "lakka_character_device"
#define DEVICE_BUFF_SIZE 1024
#define IO_SUCCESS  0
#define IO_FAILURE -1

MODULE_AUTHOR("Leyen Qian");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CSCI 3753 PA2 <lakka character module>");

int  lakka_driver_init(void);
void lakka_driver_exit(void);

int     dev_open  (struct inode *file_object, struct file *file_status);
int     dev_close (struct inode *file_object, struct file *file_status);
ssize_t dev_read  (struct file *file_status, char __user *user_buff, size_t user_buff_size, loff_t *file_offset);
ssize_t dev_write (struct file *file_status, const char __user *user_buff, size_t user_buff_size, loff_t *file_offset);
loff_t  dev_seek  (struct file *file_status, loff_t file_offset, int offset_flag);

struct file_operations dev_file_operations = {
    .owner   = THIS_MODULE,
    .open    = dev_open,
    .release = dev_close,
    .read    = dev_read,
    .write   = dev_write,
    .llseek  = dev_seek
};

typedef struct _LAKKA_CDEV
{
    struct cdev device;
    unsigned int open_count;
    unsigned int close_count;
    char buff[DEVICE_BUFF_SIZE];

} LAKKA_CDEV, *PLAKKA_CDEV;

PLAKKA_CDEV plakka_cdev;
dev_t device_identity = MKDEV(MAJOR_NUM, MINOR_NUM);