#include "predefine.h"

int dev_open (struct inode *file_object, struct file *file_status)
{
    plakka_cdev->open_count += 1;
    printk("Lakka device <character_module> is being open. <open_count %u>", plakka_cdev->open_count);
    return IO_SUCCESS;
}

int dev_close (struct inode *file_object, struct file *file_status)
{
    plakka_cdev->open_count += 1;
    printk("Lakka device <character_module> is being close. <close_count %u>", plakka_cdev->close_count);
    return IO_SUCCESS;
}

ssize_t dev_read (struct file *file_status, char __user *user_buff, size_t user_buff_size, loff_t *file_offset)
{
    ssize_t io_size = DEVICE_BUFF_SIZE - *file_offset >= user_buff_size ? user_buff_size : DEVICE_BUFF_SIZE - *file_offset;
    printk("Lakka device <character_module> received IO request <read> with offset %lld and buff size %ld.", *file_offset, io_size);
    copy_to_user(user_buff, &plakka_cdev->buff[*file_offset], io_size);
    return io_size;
}

ssize_t dev_write (struct file *file_status, const char __user *user_buff, size_t user_buff_size, loff_t *file_offset)
{
    ssize_t io_size = DEVICE_BUFF_SIZE - *file_offset >= user_buff_size ? user_buff_size : DEVICE_BUFF_SIZE - *file_offset;
    printk("Lakka device <character_module> received IO request <write> with offset %lld and buff size %ld.", *file_offset, io_size);
    copy_from_user(&plakka_cdev->buff[*file_offset], user_buff, io_size);
    return io_size;
}

loff_t dev_seek (struct file *file_status, loff_t file_offset, int offset_flag)
{
    loff_t new_file_offset = -1;
    printk("Lakka device <character_module> is being seek.");

    switch ( offset_flag )
    {
        case SEEK_SET:
            new_file_offset = file_offset;
            break;

        case SEEK_CUR:
            new_file_offset = file_status->f_pos + file_offset;
            break;

        case SEEK_END:
            new_file_offset = DEVICE_BUFF_SIZE + file_offset;
            break;
    }

    if ( new_file_offset < 0 || new_file_offset >= DEVICE_BUFF_SIZE )
    {
         return IO_FAILURE;
    }

    file_status->f_pos = new_file_offset;
    return new_file_offset;
}

int lakka_driver_init(void)
{
    printk("---------- Lakka <character_module> ----------\n");
    printk(KERN_NOTICE "inside %s function\n",__FUNCTION__);
    
    plakka_cdev = kmalloc(sizeof(LAKKA_CDEV), GFP_KERNEL);
    memset(plakka_cdev, 0, sizeof(LAKKA_CDEV));
    plakka_cdev->device.owner = THIS_MODULE;
    
    // bind device with IO dispatching methods
    cdev_init(&plakka_cdev->device, &dev_file_operations);

    // register device with predefined identity [statically or dynamically for conflict]
    // the corresponding device record is in file ( /proc/devices )
    if( register_chrdev_region(device_identity, 1, DEVICE_NAME) < 0 )
    {
        printk(KERN_WARNING "Device Identity conflict detected\n");
        alloc_chrdev_region(&device_identity, 0, 1, DEVICE_NAME);
    }
    
    // add cdev (device object) into the system
    cdev_add(&plakka_cdev->device, device_identity, 1);
    printk(KERN_NOTICE "Install %s with <major %d> and <minor %d> succeed", DEVICE_NAME, MAJOR(device_identity), MINOR(device_identity));
    printk("----------------------------------------------\n\n");
    return 0;
}

void lakka_driver_exit(void)
{
    printk("---------- Lakka <character_module> ----------\n");
    printk(KERN_NOTICE "inside %s function\n",__FUNCTION__);

    // unregister device & remove cdev (device object) from the system
    unregister_chrdev_region(device_identity, 1);
    cdev_del(&plakka_cdev->device);
    kfree(plakka_cdev);

    printk(KERN_NOTICE "Remove %s with <major %d> and <minor %d> succeed", DEVICE_NAME, MAJOR(device_identity), MINOR(device_identity));
    printk("----------------------------------------------\n\n");
}

module_init(lakka_driver_init);
module_exit(lakka_driver_exit);