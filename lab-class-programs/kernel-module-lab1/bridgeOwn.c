#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include "bridgeOwn.h"

/*
 * Our parameters which can be set at load time.
 */

#define BRIDGE_CREATE_Q _IO('p', 1)                     //Create a queue
#define BRIDGE_W_HIGH_PRIOR_Q _IOW('p', 2, char *)
#define BRIDGE_W_MIDDLE_PRIOR_Q _IOW('p', 3, char *)
#define BRIDGE_W_LOW_PRIOR_Q _IOW('p', 4, char *)
#define BRIDGE_R_HIGH_PRIOR_Q _IOR('p', 5, char *)
#define BRIDGE_R_MIDDLE_PRIOR_Q _IOR('p', 6, char *)
#define BRIDGE_R_LOW_PRIOR_Q _IOR('p', 7, char *)
#define BRIDGE_STATE_Q _IO('p', 8)
#define BRIDGE_DESTROY_Q _IO('p', 9)

#define BRIDGE_CREATE_S _IO('p', 10)                    //Create a stack
#define BRIDGE_W_S _IOW('p', 11, char *)
#define BRIDGE_R_S _IOR('p', 12, char *)
#define BRIDGE_STATE_S _IO('p', 13)
#define BRIDGE_DESTROY_S _IO('p', 14)

#define BRIDGE_CREATE_L _IO('p', 15)                    //Create a list
#define BRIDGE_W_L _IOW('p', 16, char *)
#define BRIDGE_R_L _IOR('p', 17, char *)
#define BRIDGE_INVERT_L _IO('p', 18)
#define BRIDGE_ROTATE_L _IOW('p', 19, int *)
#define BRIDGE_CLEAN_L _IO('p', 20)
#define BRIDGE_GREATER_VAL_L _IOR('p', 21, char *)
#define BRIDGE_END_L _IO('p', 22)
#define BRIDGE_CONCAT_L _IO('p', 23)
#define BRIDGE_STATE_L _IO('p', 24)
#define BRIDGE_DESTROY_L _IO('p', 25)

int bridge_major =   BRIDGE_MAJOR;
int bridge_minor =   0;
int bridge_nr_devs = BRIDGE_NR_DEVS;	/* number of bare bridge devices */

module_param(bridge_major, int, S_IRUGO);
module_param(bridge_minor, int, S_IRUGO);
module_param(bridge_nr_devs, int, S_IRUGO);

MODULE_AUTHOR("Jheisson Argiro Lopez Restrepo");
MODULE_LICENSE("Dual BSD/GPL");

struct bridge_dev *bridge_devices;	/* allocated in bridge_init_module */

static long bridge_ioctl(struct file *f, unsigned int cmd, unsigned long arg){
    int data;
    char message[100];
    switch(cmd){
	case BRIDGE_CREATE_Q:
            printk(KERN_INFO "message %s\n", "bla");
	    //Return a posituve value indicating the state of the queue
	    return 1;
	    break;
	case BRIDGE_W_HIGH_PRIOR_Q:
    	    raw_copy_from_user(message, (char *)arg, 100);
	    printk(KERN_INFO "message %s\n", message);
	    break;
	case BRIDGE_W_MIDDLE_PRIOR_Q:
	    printk(KERN_INFO "message %s\n", "bla1");
	    break;
	case BRIDGE_W_LOW_PRIOR_Q:
	    printk(KERN_INFO "message %s\n", "bla2");
	    break;
	case BRIDGE_R_HIGH_PRIOR_Q:
            printk(KERN_INFO "message %s\n", "bla3");
	    break;
	case BRIDGE_R_MIDDLE_PRIOR_Q:
            printk(KERN_INFO "message %s\n", "bla4");
	    break;
	case BRIDGE_R_LOW_PRIOR_Q:
            printk(KERN_INFO "message %s\n", "bla5");
	    break;
	case BRIDGE_STATE_Q:
            printk(KERN_INFO "message %s\n", "bla6");
	    break;
	case BRIDGE_DESTROY_Q:
            printk(KERN_INFO "message %s\n", "bla7");
	    break;

	case BRIDGE_CREATE_S:
	    printk(KERN_INFO "message %s\n", "bla8");
	    break;
	case BRIDGE_W_S:
            printk(KERN_INFO "message %s\n", "bla9");
	    break;
	case BRIDGE_R_S:
            printk(KERN_INFO "message %s\n", "bla10");
	    break;
	case BRIDGE_STATE_S:
            printk(KERN_INFO "message %s\n", "bla11");
	    break;
	case BRIDGE_DESTROY_S:
            printk(KERN_INFO "message %s\n", "bla12");
	    break;

	case BRIDGE_CREATE_L:
             printk(KERN_INFO "message %s\n", "bla13");
	     break;
	case BRIDGE_W_L:
             printk(KERN_INFO "message %s\n", "bla14");
	     break;
	case BRIDGE_R_L:
             printk(KERN_INFO "message %s\n", "bla15");
	     break;
	case BRIDGE_INVERT_L:
             printk(KERN_INFO "message %s\n", "bla16");
	     break;
	case BRIDGE_ROTATE_L:
             get_user(data, (int *)arg);
             printk(KERN_INFO "message %d\n", data);
	     break;
	case BRIDGE_CLEAN_L:
             printk(KERN_INFO "message %s\n", "bla18");
	     break;
	case BRIDGE_GREATER_VAL_L:
	     strcpy((char *)arg, "MensajePrueba");
             printk(KERN_INFO "message %s\n", "bla19");
	     break;
	case BRIDGE_END_L:
             printk(KERN_INFO "message %s\n", "bla21");
	     break;
	case BRIDGE_CONCAT_L:
             printk(KERN_INFO "message %s\n", "bla22");
	     break;
	case BRIDGE_STATE_L:
             printk(KERN_INFO "message %s\n", "bla23");
	     break;
	case BRIDGE_DESTROY_L:
             printk(KERN_INFO "message %s\n", "bla24");
    }
    return 0;
}

struct file_operations bridge_fops = {
	.owner =    THIS_MODULE,
	.unlocked_ioctl = bridge_ioctl
};

/*
 * Finally, the module stuff
 */

/*
 * The cleanup function is used to handle initialization failures as well.
 * Thefore, it must be careful to work correctly even if some of the items
 * have not been initialized
 */
void bridge_cleanup_module(void)
{
	int i;
	dev_t devno = MKDEV(bridge_major, bridge_minor);

	/* Get rid of our char dev entries */
	if (bridge_devices) {
		for (i = 0; i < bridge_nr_devs; i++) {
			cdev_del(&bridge_devices[i].cdev);
		}
		//Be aware of clean memory completely
		kfree(bridge_devices);
	}

	/* cleanup_module is never called if registering failed */
	unregister_chrdev_region(devno, bridge_nr_devs);
}

/*
 * Set up the char_dev structure for this device.
 */
static void bridge_setup_cdev(struct bridge_dev *dev, int index)
{
	int err, devno;

	devno = MKDEV(bridge_major, bridge_minor + index);
	cdev_init(&dev->cdev, &bridge_fops);
	dev->cdev.owner = THIS_MODULE;
	err = cdev_add(&dev->cdev, devno, 1);
	/* Fail gracefully if need be */
	if (err)
		printk(KERN_NOTICE "Error %d adding bridge%d", err, index);
}


int bridge_init_module(void)
{
	int result, i;
	dev_t dev = 0;

/*
 * Get a range of minor numbers to work with, asking for a dynamic
 * major unless directed otherwise at load time.
 */
	if (bridge_major) {
		dev = MKDEV(bridge_major, bridge_minor);
		result = register_chrdev_region(dev, bridge_nr_devs, "bridgeOwn");
	} else {
		result = alloc_chrdev_region(&dev, bridge_minor, bridge_nr_devs, "bridgeOwn");
		bridge_major = MAJOR(dev);
        	bridge_devices = kmalloc(bridge_nr_devs * sizeof(struct bridge_dev), GFP_KERNEL);
	}

	if (result < 0) {
		printk(KERN_WARNING "bridge: can't get major %d\n", bridge_major);
		return result;
	}

		/*
	 * allocate the devices -- we can't have them static, as the number
	 * can be specified at load time
	 */
	bridge_devices = kmalloc(bridge_nr_devs * sizeof(struct bridge_dev), GFP_KERNEL);

	if (!bridge_devices) {
		result = -ENOMEM;
		goto fail;  /* Make this more graceful */
	}

	memset(bridge_devices, 0, bridge_nr_devs * sizeof(struct bridge_dev));

		/* Initialize each device. */
	for (i = 0; i < bridge_nr_devs; i++) {
		bridge_setup_cdev(&bridge_devices[i], i);
	}

	return 0; /* succeed */

  fail:
	bridge_cleanup_module();
	return result;
}

module_init(bridge_init_module);
module_exit(bridge_cleanup_module);
