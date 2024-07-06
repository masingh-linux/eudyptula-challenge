#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>

#define BUF_SIZE PAGE_SIZE

static struct dentry *dir, *file_id, *file_jiffies, *file_foo;
static char id_buf[16] = "12345678"; // Example ID value in kernel
static char *foo_buf;
static struct mutex foo_mutex;
static unsigned long foo_len;
// extern unsigned long volatile __cacheline_aligned_in_smp __jiffy_arch_data jiffies;
// static unsigned long my_jiffies;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MANISHA");
MODULE_DESCRIPTION("creation of debugfs files and directories");
MODULE_VERSION("1.0");

static ssize_t read_jiffies(struct file *filp, char __user *buffer, size_t len, loff_t *offset){
    char buf[32];
    int my_jiffies;
    my_jiffies = jiffies; 
    // ret = snprintf(buf,sizeof(buf),"%lu\n",jiffies);
    return simple_read_from_buffer(buffer,len,offset,buf,my_jiffies);
}

static ssize_t read_id(struct file *filp, char __user *buffer, size_t len, loff_t *offset){
    return simple_read_from_buffer(buffer,len,offset,id_buf,strlen(id_buf));
}

static ssize_t write_id(struct file *filp, const char __user *buffer, size_t len, loff_t *offset)
{
    if (len > sizeof(id_buf)-1)    //len is length of data that user wants to write
        return -EINVAL;
    if(copy_from_user(id_buf,buffer,len))
        return -EFAULT;
    id_buf[len] = "\0";
    pr_info("write_id: New ID set to %s\n", id_buf);
    return len;
}

static ssize_t read_foo(struct file *filp, char __user *buffer, size_t len, loff_t *offset)
{
    ssize_t ret;
    mutex_lock(&foo_mutex);  // mutex object initialised
    ret = simple_read_from_buffer(buffer, len, offset, foo_buf, foo_len);
    mutex_unlock(&foo_mutex);
    return ret;
}

static ssize_t write_foo(struct file *filp, const char __user *buffer, size_t len, loff_t *offset)
{
    ssize_t ret;
    if (!capable(CAP_SYS_ADMIN))  //Check if the user has administrative privileges
        return -EPERM;
    if (len > BUF_SIZE - 1)
        return -EINVAL;
    mutex_lock(&foo_mutex);

    ret = simple_write_to_buffer(foo_buf, BUF_SIZE, offset, buffer, len);
    if (ret >= 0)
        // foo_len = ret;
        foo_buf[ret] = '\0'; // Ensure null-terminated string
        pr_info("write_foo: Wrote %ld bytes: %s\n", ret, foo_buf);
    mutex_unlock(&foo_mutex);  // Release the mutex
    return ret;
}


static const struct file_operations jiff_fops = {
    .owner = THIS_MODULE,
    .read = read_jiffies,
};

static const struct file_operations id_fops = {
    .owner = THIS_MODULE,
    .read = read_id,
    .write = write_id
};

static const struct file_operations foo_fops = {
    .owner = THIS_MODULE,
    .read = read_foo,
    .write = write_foo,
};

static int hello_init_module(void) {
    dir = debugfs_create_dir("eudyptula", NULL);
    if (!dir)
        goto fail;
    if(!debugfs_create_file("id",0666, dir, NULL, &id_fops))
        goto fail;
    if(!debugfs_create_file("jiffies",0444,dir, NULL,&jiff_fops))
        goto fail;
    if(!debugfs_create_file("foo",0644,dir, NULL,&foo_fops))
        goto fail;
    foo_buf = (char *)kmalloc(BUF_SIZE, GFP_KERNEL);
    if (!foo_buf)
        goto fail;

    mutex_init(&foo_mutex);
    pr_info("Module Loaded\n");
    return 0;

fail: pr_alert("Could not create devices");
    return -ENODEV;


    
}

void exit_cleanup_module(void){
    debugfs_remove_recursive(dir);
    kfree(foo_buf);
    printk(KERN_INFO "module unloaded!\n");
}

module_init(hello_init_module);
module_exit(exit_cleanup_module);
