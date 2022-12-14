#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/usb.h>

#define IS_NEW_METHOD_USED (1)
#define USB_VENDOR_ID (0x0461)
#define USB_PRODUCT_ID (0x4de2)

#define PRINT_USB_INTERFACE_DESCRIPTOR(i)      \
{   \
    pr_info("USB_INTERFACE_DESCRIPTOR:\n"); \
    pr_info("--------------------------");  \
    pr_info("bLength:0x%x\n", i.bLength);   \
    pr_info("bDescriptorType:0x%x\n", i.bDescriptorType);   \
    pr_info("bInterfaceNumber:0x%x\n", i.bInterfaceNumber); \
    pr_info("bAlternateSetting:0x%x\n", i.bAlternateSetting);   \
    pr_info("bNumEndpoints:0x%x\n", i.bNumEndpoints); \
    pr_info("bInterfaceClass:0x%x\n", i.bInterfaceClass);   \
    pr_info("bInterfaceSubClass:0x%x\n", i.bInterfaceSubClass); \
    pr_info("bInterfaceProtocol:0x%x\n", i.bInterfaceProtocol); \
    pr_info("Interface:0x%x\n", i.iInterface);  \
    pr_info("\n");  \
}   \

#define PRINT_USB_ENDPOINT_DESCRIPTOR(e)    \
{   \
    pr_info("USB_ENDPOINT_DESCRIPTOR:\n");  \
    pr_info("--------------------------");  \
    pr_info("bLength:0x%x\n", e.bLength);   \
    pr_info("bDescriptorType:0x%x\n", e.bDescriptorType);   \
    pr_info("bEndpointAddress:0x%x\n", e.bEndpointAddress); \
    pr_info("bmAttributes:0x%x\n", e.bmAttributes);   \
    pr_info("wMaxPacketSize:0x%x\n", e.wMaxPacketSize); \
    pr_info("bInterval:0x%x\n", e.bInterval); \
    pr_info("\n");\
}   \

static int etx_usb_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    unsigned int i;
    unsigned int endpoints_count;
    struct usb_host_interface *iface_desc=interface->cur_altsetting;
    dev_info(&interface->dev, "USB Driver Probed: Vendor ID: 0x%02x, Product ID: 0x%02x\n", id->idVendor,id->idProduct);

    endpoints_count = iface_desc->desc.bNumEndpoints;
    
    PRINT_USB_INTERFACE_DESCRIPTOR(iface_desc->desc);

    for(i=0;i<endpoints_count;i++){
        PRINT_USB_ENDPOINT_DESCRIPTOR(iface_desc->endpoint[i].desc);
    }
    return 0;
}

static void etx_usb_disconnect(struct usb_interface *interface)
{
    dev_info(&interface->dev,"USB Device Disconnected!\n");
}

const struct usb_device_id etx_usb_table[]={
    {USB_DEVICE (USB_VENDOR_ID, USB_PRODUCT_ID)},
    {}
};

MODULE_DEVICE_TABLE(usb,etx_usb_table);

static struct usb_driver etx_usb_driver={
    .name = "My USB Driver",
    .probe = etx_usb_probe,
    .disconnect = etx_usb_disconnect,
    .id_table = etx_usb_table,
};

#if (IS_NEW_METHOD_USED == 0)
module_usb_driver(etx_usb_driver);

#else
static int __init etx_usb_init(void)
{
    return usb_register(&etx_usb_driver);
}

static void __exit etx_usb_exit(void)
{
    return usb_deregister(&etx_usb_driver);
}

module_init(etx_usb_init);
module_exit(etx_usb_exit);

#endif

MODULE_LICENSE("GPL");