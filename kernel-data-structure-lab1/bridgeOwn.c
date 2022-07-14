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

MODULE_AUTHOR("2K+1");
MODULE_LICENSE("Dual BSD/GPL");

LIST_HEAD(stack);
LIST_HEAD(linkedList);// TODO:no se usa ?
LIST_HEAD(linkedListConcat);//TODO:no se usa ?

LIST_HEAD(highPriorityQueue);
LIST_HEAD(middlePriorityQueue);
LIST_HEAD(lowPriorityQueue);



static void add_element_to_stack(char *node_element_msg, struct list_head* head){
	struct string_node *tmp_element;
	tmp_element = kmalloc(sizeof(struct string_node), GFP_KERNEL);
	strcpy(tmp_element->message, node_element_msg);
	INIT_LIST_HEAD(&tmp_element->list);
	list_add(&(tmp_element->list), head);
}

static void read_all_list(struct list_head* head, char* nameList){
	struct string_node *tmp_element;
	struct list_head *watch, *next;
	list_for_each_safe(watch, next, head){
		tmp_element = list_entry(watch, struct string_node, list);
		char* valor = tmp_element->message;
		printk(KERN_INFO "%s : VALOR = %s", nameList, valor);
	}
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

void cleanAllListElements(struct list_head* head){
 	struct string_node *tmp_element;
	struct list_head *watch, *next;
	list_for_each_safe(watch, next, head){
       	tmp_element = list_entry(watch, struct string_node, list);
       	list_del(&(tmp_element->list));
		kfree(tmp_element);
   	}
	printk(KERN_INFO "Stack destroyed successfuly ");
}

int listLength(struct list_head* head){
	struct string_node *tmp_element;
	struct list_head *watch, *next;
	int cantidadRegistros = 0;
	list_for_each_safe(watch, next, head){
       	cantidadRegistros++;
   	}
	return cantidadRegistros;
}

struct bridge_dev *bridge_devices;	/* allocated in bridge_init_module */

static long bridge_ioctl(struct file *f, unsigned int cmd, unsigned long arg){
    int return_value = 0;
    int data;
    char message[100];
    struct string_node *tmp_element;
	struct list_head *watch, *next;

    switch(cmd){
	case BRIDGE_CREATE_Q:
        printk(KERN_INFO "message %s\n", "bla");
	    //Return a posituve value indicating the state of the queue
	    return_value = 1;
	    break;
	case BRIDGE_W_HIGH_PRIOR_Q:
    	printk(KERN_INFO "Inserting value in a high priority queue\n");
        raw_copy_from_user(message, (char *)arg, 100);
	    add_element_to_stack(message, &highPriorityQueue);
        printk(KERN_INFO "Element succesfully added to the queue\n");
	    break;
	case BRIDGE_W_MIDDLE_PRIOR_Q:
	    printk(KERN_INFO "Inserting value in a middle priority queue\n");
        raw_copy_from_user(message, (char *)arg, 100);
	    add_element_to_stack(message, &middlePriorityQueue);
        printk(KERN_INFO "Element succesfully added to the queue\n");
	    break;
	case BRIDGE_W_LOW_PRIOR_Q:
	    printk(KERN_INFO "Inserting value in a low priority queue\n");
        raw_copy_from_user(message, (char *)arg, 100);
	    add_element_to_stack(message, &lowPriorityQueue);
        printk(KERN_INFO "Element succesfully added to the queue\n");
	    break;
	case BRIDGE_R_HIGH_PRIOR_Q:
        printk(KERN_INFO "Reading\n");
	    tmp_element = list_last_entry(&highPriorityQueue, struct string_node, list);
        list_del(&(tmp_element->list));
	    raw_copy_to_user((char *)arg, tmp_element->message, 100);
	    kfree(tmp_element);
	    break;
	case BRIDGE_R_MIDDLE_PRIOR_Q:
        printk(KERN_INFO "Reading\n");
	    tmp_element = list_last_entry(&middlePriorityQueue, struct string_node, list);
        list_del(&(tmp_element->list));
	    raw_copy_to_user((char *)arg, tmp_element->message, 100);
	    kfree(tmp_element);
	    break;
	case BRIDGE_R_LOW_PRIOR_Q:
         printk(KERN_INFO "Reading\n");
	    tmp_element = list_last_entry(&lowPriorityQueue, struct string_node, list);
        list_del(&(tmp_element->list));
	    raw_copy_to_user((char *)arg, tmp_element->message, 100);
	    kfree(tmp_element);
	    break;
	case BRIDGE_STATE_Q:
        printk(KERN_INFO "message %s\n", "bla6");
	    break;
	case BRIDGE_STATE_HP_Q:
		return_value=listLength(&highPriorityQueue);
	    printk(KERN_INFO "Queue state succesfully sended !!! entero: %d\n", return_value);
	    break;
	case BRIDGE_STATE_MP_Q:
		return_value=listLength(&middlePriorityQueue);
	    printk(KERN_INFO "Queue state succesfully sended !!!\n");
	    break;
	case BRIDGE_STATE_LP_Q:
		return_value=listLength(&lowPriorityQueue);
	    printk(KERN_INFO "Queue state succesfully sended !!!\n");
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
		//return_value=list_empty(&stack);
		return_value=listLength(&stack);
	    printk(KERN_INFO "Stack state succesfully sended !!!\n");
	    break;
	case BRIDGE_DESTROY_S:
		printk(KERN_INFO "Destroying stack"); 
		cleanAllListElements(&stack);
	    break;
	case BRIDGE_CREATE_L:
        printk(KERN_INFO "message %s\n", "bla13");
	    break;
	case BRIDGE_W_L:
		raw_copy_from_user(message, (char *)arg, 100);
	  	add_element_to_stack(message, &stack);
	    break;
	case BRIDGE_R_L:
		if(list_empty(&stack) != 0){
			printk(KERN_INFO "LISTA VACIA \n");
			raw_copy_to_user((char *)arg, "void", 100);
	    }else{
			tmp_element = list_last_entry(&stack, struct string_node, list);
			list_del(&(tmp_element->list));
			raw_copy_to_user((char *)arg, tmp_element->message, 100);
			kfree(tmp_element);
	    }
	    break;
	case BRIDGE_INVERT_L:
        printk(KERN_INFO "message %s\n", "bla16");
	    break;
	case BRIDGE_ROTATE_L:
		printk(KERN_INFO "ROTAR LA LISTA \n");
		raw_copy_from_user(message, (char*)arg, 100);
		long numberRotations;
		kstrtol(message, 10, &numberRotations);
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
        printk(KERN_INFO "LIMPIAR LA LISTA \n");
		read_all_list(&stack, "stack");
		printk(KERN_INFO "--------------------------\n");
		
		list_for_each_safe(watch, next, &stack){
			tmp_element = list_entry(watch, struct string_node, list);
			char* valor = tmp_element->message;
			struct string_node *tmpElementLinkedList;
			struct list_head *watchLinkedList, *nextLinkedList;
			int esDuplicado = 0;
			list_for_each_safe(watchLinkedList, nextLinkedList, &stack){
				tmpElementLinkedList = list_entry(watchLinkedList, struct string_node, list);
				char* valorLinkedList = tmpElementLinkedList->message;
				if(&(tmp_element->message) != &(tmpElementLinkedList->message)){
					if(strcmp(valor, valorLinkedList) == 0){
						esDuplicado = 1;
					}
				}
			}
			if(esDuplicado == 1){
				list_del(&(tmp_element->list));
				kfree(tmp_element);
			}
		}
		read_all_list(&stack, "stack");
		int n = listLength(&stack);
		char cantidadRegistros[100];
		sprintf(cantidadRegistros, "%d", n);
		raw_copy_to_user((char *)arg, cantidadRegistros, 100);
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
        printk(KERN_INFO "DESTRIUR LISTA");
		cleanAllListElements(&stack);		
		printk(KERN_INFO "--------------------------\n");
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

