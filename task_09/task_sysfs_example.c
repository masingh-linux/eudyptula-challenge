#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/init.h>

static int attr_1;
static int attr_2;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MANISHA");
MODULE_DESCRIPTION("kobject and sysfs driver module example");
MODULE_VERSION("1.0");


static ssize_t attr_show(struct kobject *kobj,
                      struct kobj_attribute *attr,
                     char *buf) {
    int var;
    if (strcmp(attr->attr.name,"attr_1")==0)
        var = attr_1;
    else
	var = attr_2;

    pr_info("attr_show: reading done value is  %d\n", var);
    return sprintf(buf,"%d\n",var);
}

static ssize_t attr_store(struct kobject *kobj,
                       struct kobj_attribute *attr,
               const char *buf, size_t count)
{
	int ret, var;
	ret = kstrtoint(buf,10,&var);
	if (ret < 0)
		return ret;
	if (strcmp(attr->attr.name,"attr_1") == 0)
		attr_1 = var;
	else
		attr_2 = var;
	pr_info("attr_store: written in buffer as %d\n",var);
	return count;
}

static struct kobj_attribute attr_1_attribute =
    __ATTR(attr_1, 0664, attr_show, attr_store);
static struct kobj_attribute attr_2_attribute =
    __ATTR(attr_2,0444,attr_show,NULL);

// This is an array of pointers to struct attribute, which represents the individual attributes that will be associated with the kobject    
static struct attribute *attrs[] = {
    &attr_1_attribute.attr,
    &attr_2_attribute.attr,
    NULL,   
      /* need to NULL terminate the list of attributes */
};
 
// structure groups the attributes together and provides additional information needed by the sysfs interface 
static struct attribute_group attr_group = {
    .attrs = attrs,
};

static struct kobject *Driver_Module_kobj;
 
static int __init Driver_Module_init(void)
{
    int retval;
 
    
    /* Create a directory with name of eudyptula    */ 
    Driver_Module_kobj = kobject_create_and_add("Eudyptula", 
                                                    kernel_kobj);
    if (!Driver_Module_kobj)
        return -ENOMEM;
 
    /* 
     * Create the files associated with this kobject under 
         * directory /sys/kernel/Driver_Module 
     */
    retval = sysfs_create_group(Driver_Module_kobj,
                                    &attr_group);
    if (retval)
        kobject_put(Driver_Module_kobj);
 
    printk(KERN_INFO "Driver Module Initialization Done!!!\n");
 
    return retval;
}
 
static void __exit Driver_Module_exit(void)
{
    kobject_put(Driver_Module_kobj);
    printk(KERN_INFO "Driver Module Removed!!!\n");
}
 
module_init(Driver_Module_init);
module_exit(Driver_Module_exit);






