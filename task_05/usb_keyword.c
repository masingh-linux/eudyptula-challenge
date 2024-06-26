#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/hid.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("MANISHA");
MODULE_DESCRIPTION("USB KEYBOARD DRIVER");
MODULE_VERSION("1.0");

#define USB_DEVICE_VENDOR_ID 0x0781
#define USB_DEVICE_PRODUCT_ID 0x558a


static struct usb_device_id kb_table[] = {
    { USB_DEVICE(USB_DEVICE_VENDOR_ID, USB_DEVICE_PRODUCT_ID) },
    { }

};
MODULE_DEVICE_TABLE (usb, kb_table);

static int kb_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    printk(KERN_INFO "USB KEYBOARD plugged in (Vendor ID : 0x%02x,\t"
             "Product ID : 0x%02x\n) plugged in", id->idVendor, id->idProduct);
    return 0;
};


static void kb_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "USB keyboard  removed\n");
};


static struct usb_driver kb_driver = {
    .name = "usb_keyboard_driver",
    .id_table = kb_table,
    .probe = kb_probe,
    .disconnect = kb_disconnect,
};

static int __init usb_keyboard_init(void) {
    int ret = usb_register(&kb_driver);
    if (ret < 0)
        printk(KERN_ERR "Failed to register\n");
    else
        printk(KERN_INFO " USB Driver registered\n");
    return ret;
}
        
       
static void __exit usb_keyboard_exit(void){
    usb_deregister(&kb_driver);
    printk(KERN_INFO "USB Driver deregistered\n");
}

module_init(usb_keyboard_init);
module_exit(usb_keyboard_exit);
