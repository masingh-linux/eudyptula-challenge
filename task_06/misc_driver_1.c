#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MANISHA");
MODULE_DESCRIPTION("A Simple Misc Char Device Driver");
MODULE_VERSION("1.0");

#define DEVICE_NAME "eudyptule101"
#define MY_ID "8630221921"
#define MY_ID_LEN (sizeof(MY_ID)-1)


static int my_open(struct inode *inode, struct file *file)
{
    pr_info("misc device open\n");
    return 0;
};
/*
** This function will be called when we close the Misc Device file
*/
static int my_close(struct inode *inodep, struct file *filp)
{
    pr_info("misc device close\n");
    return 0;
};

static ssize_t my_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    pr_info("Manisha: %s function Called\n",__func__);
    pr_info("Manisha: count=%ld\tppos=%lld\n", count, *ppos);
    // struct file *file: Pointer to the file structure representing the open file.
    // char __user *buf: Pointer to the user buffer where data will be copied.
    // size_t count: Number of bytes to read.
    // loff_t *ppos: Pointer to the file offset (position).
    size_t ret = simple_read_from_buffer(buf, count, ppos, MY_ID, MY_ID_LEN);
    // buf: Destination buffer in user space.
    // count: Number of bytes requested to read.
    // ppos: Current position in the file (offset).
    // MY_ID: Source data.
    // MY_ID_LEN: Length of the source data.
    pr_info("Manisha: %s function Exit\n",__func__);
    return ret;
};

static ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{  
    pr_info("Manisha: %s function Called\n",__func__); 
    // struct file *file: Pointer to the file structure representing the open file.
    // const char __user *buf: Pointer to the user buffer containing data to be written.
    // size_t count: Number of bytes to write.
    // loff_t *ppos: Pointer to the file offset (position).

    char kbuf[MY_ID_LEN + 1] = {0};
    size_t result;
    pr_info("Count Len = %ld\n", count); 
    pr_info("id Len = %ld\n", MY_ID_LEN);
    if (count != MY_ID_LEN) 
        return -EINVAL;
    result = simple_write_to_buffer(kbuf, MY_ID_LEN, ppos, buf, count);
    pr_info("Manisha: result=%ld", result);
    // kbuf: Destination buffer in kernel space.
    // MY_ID_LEN: Number of bytes to copy.
    // ppos: Current position in the file (offset).
    // buf: Source buffer in user space.
    // count: Number of bytes requested to write.
    if (result>0)
        return result;
    kbuf[MY_ID_LEN] = '\0';
    if (strncmp(kbuf, MY_ID, MY_ID_LEN) == 0)
        return count;
    pr_info("Manisha: %s function Exit\n",__func__);
    return -EINVAL;
}

static struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .open = my_open,
    .write = my_write,
    .release = my_close,
};


static struct miscdevice my_misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &my_fops,
};


static int my_init(void) {
    pr_info("Manisha: %s function Called\n",__func__);
    int result;
    result = misc_register(&my_misc_device);
    if (result) {
        pr_err("misc register fail for \"%s\" misc device\n", DEVICE_NAME);
    }
    pr_info("Manisha: %s function Exit\n",__func__);
    return result;
}

static void my_exit(void) {
    pr_info("Manisha: %s function Called\n",__func__);
    misc_deregister(&my_misc_device);
    pr_info("Manisha: %s function Exit\n",__func__);
}

module_init(my_init);
module_exit(my_exit);

