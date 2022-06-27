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

int bridge_major =   BRIDGE_MAJOR;
int bridge_minor =   0;
int bridge_nr_devs = BRIDGE_NR_DEVS;	/* number of bare bridge devices */

module_param(bridge_major, int, S_IRUGO);
module_param(bridge_minor, int, S_IRUGO);
module_param(bridge_nr_devs, int, S_IRUGO);

MODULE_AUTHOR("Jheisson Argiro Lopez Restrepo");
MODULE_LICENSE("Dual BSD/GPL");

LIST_HEAD(stack);
LIST_HEAD(linkedList);
LIST_HEAD(linkedListConcat);

static void add_element_to_stack(char *node_element_msg, struct list_head* head){
	struct string_node *tmp_element;
	tmp_element = kmalloc(sizeof(struct string_node), GFP_KERNEL);
	strcpy(tmp_element->message, node_element_msg);
	INIT_LIST_HEAD(&tmp_element->list);
	list_add(&(tmp_element->list), head);
}

void mylist_exit(void){
    struct string_node *tmp_element;
	struct list_head *watch, *next;
	list_for_each_safe(watch, next, &stack){
       	tmp_element = list_entry(watch, struct string_node, list);
       	list_del(&(tmp_element->list));
		kfree(tmp_element);
   	}
    //kfree(&stack);
}

struct bridge_dev *bridge_devices;	/* allocated in bridge_init_module */

static long bridge_ioctl(struct file *f, unsigned int cmd, unsigned long arg){
    int return_value = 0;
    int data;
    char message[100];
    struct string_node *tmp_element;
	printk("llego al kernel");
    switch(cmd){
	case BRIDGE_CREATE_Q:
        printk(KERN_INFO "message %s\n", "bla");
	    //Return a posituve value indicating the state of the queue
	    return_value = 1;
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
	    printk(KERN_INFO "Stack succesfully created\n");
	    break;
	case BRIDGE_W_S:
		printk(KERN_INFO "Creating\n");
        raw_copy_from_user(message, (char *)arg, 100);
	    add_element_to_stack(message, &stack);
        printk(KERN_INFO "Element succesfully added to the stack\n");
	    break;
	case BRIDGE_R_S:
		printk(KERN_INFO "Reading\n");
	    tmp_element = list_first_entry(&stack, struct string_node, list);
        list_del(&(tmp_element->list));
	    raw_copy_to_user((char *)arg, tmp_element->message, 100);
	    kfree(tmp_element);
	    break;
	case BRIDGE_STATE_S:
        if(list_empty(&stack) != 0){
			return_value = 0;
	    }else{
			return_value = 1;
	    }
	    printk(KERN_INFO "Stack state succesfully sended!!!\n");
	    break;
	case BRIDGE_DESTROY_S:
        printk(KERN_INFO "message %s\n", "bla12");
	    break;
	case BRIDGE_CREATE_L:
        printk(KERN_INFO "message %s\n", "bla13");
	    break;
	case BRIDGE_W_L:
		raw_copy_from_user(message, (char *)arg, 100);
		//printk(KERN_INFO "AGREGANDO ELEMENTO A LA LISTA STACK: message = %s\n", message);
	  	add_element_to_stack(message, &stack);
	    break;
	case BRIDGE_R_L:
		/*ESTO RECORRE TODOS LOS REGISTROS DE LA LISTA STACK
		struct string_node *tmp_element;
		struct list_head *watch, *next;
		list_for_each_safe(watch, next, &stack){
			tmp_element = list_entry(watch, struct string_node, list);
			char* valor = tmp_element->message;
			printk(KERN_INFO "VALOR = %s\n", valor);
		}*/
		tmp_element = list_last_entry(&stack, struct string_node, list);
        list_del(&(tmp_element->list));
	    raw_copy_to_user((char *)arg, tmp_element->message, 100);
	    kfree(tmp_element);
	    break;
	case BRIDGE_INVERT_L:
        printk(KERN_INFO "message %s\n", "bla16");
	    break;
	case BRIDGE_ROTATE_L:
		printk(KERN_INFO "------------ BRIDGE_ROTATE_L ROTAR LA LISTA \n");
		raw_copy_from_user(message, (char*)arg, 100);
		long numberRotations;
		kstrtol(message, 10, &numberRotations);
        printk(KERN_INFO "message BRIDGE_ROTATE_L numberRotations = %d\n", numberRotations);
		printk(KERN_INFO "------------------------------------------------------------------\n\n");
		int i = 0;
		while (i < numberRotations){
			struct list_head *head = &stack;
			struct list_head *first;
			if (!list_empty(head)) {
				first = head->next;
				list_move_tail(first, head);
			}
			i++;
		}
	    break;
	case BRIDGE_CLEAN_L:
        printk(KERN_INFO "message BRIDGE_CLEAN_L %s\n", "bla18");
	    break;
	case BRIDGE_GREATER_VAL_L:
	    //strcpy((char *)arg, "MensajePrueba");
        printk(KERN_INFO "message %s\n", "bla19");
	    break;
	case BRIDGE_END_L:
        printk(KERN_INFO "message %s\n", "bla21");
	    break;
	case BRIDGE_CONCAT_L:
        printk(KERN_INFO "message BRIDGE_CONCAT_L %s\n", "bla22");
	    break;
	case BRIDGE_STATE_L:
        printk(KERN_INFO "message %s\n", "bla23");
	    break;
	case BRIDGE_RANDOM_L:
		printk(KERN_INFO "------------ BRIDGE_RANDOM_L REVOLVER LA LISTA \n");
		long numberRandom;
		raw_copy_from_user(message, (char* )arg, 20);
		kstrtol(message, 10, &numberRandom);
        printk(KERN_INFO "message BRIDGE_RANDOM_L numberRandom = %d\n", numberRandom);
		printk(KERN_INFO "------------------------------------------------------------------\n\n");
		int iteratorRandom = 0;

		struct string_node *tmp_element;
		struct list_head *watch, *next;
		list_for_each_safe(watch, next, &stack){
			tmp_element = list_entry(watch, struct string_node, list);
			if(numberRandom>0 && iteratorRandom == numberRandom){
				list_swap(watch, watch->next);
				break;
			}
			iteratorRandom++;
		}
	    break;
	case BRIDGE_DESTROY_L:
        printk(KERN_INFO "message %s\n", "bla24");
    }
    return return_value;
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
	mylist_exit();
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

