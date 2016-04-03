#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/types.h>

struct birthday
{
    int day;
    int month;
    int year;
    struct list_head list;
};

// Create list

static LIST_HEAD(birthday_list);

void add_person(int d, int m, int y);
void del_person(struct birthday* ptr);

/* This function is called when the module is loaded. */
int simple_init(void)
{
    struct birthday *ptr;

    printk(KERN_INFO "Loading Birthday Module\n");

    // Add 4 persons
    add_person(4,  6, 1984);
    add_person(14, 9, 1979);
    add_person(29, 8, 1954);
    add_person(1,  4, 1948);

    // Traverse list, printing items
    list_for_each_entry(ptr, &birthday_list, list)
    printk(KERN_INFO
    "FAMILY MEMBER BIRTHDAY: %d/%d/%d\n",
            ptr->day, ptr->month, ptr->year);

    return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void)
{
    struct birthday *ptr, *next;

    printk(KERN_INFO "Removing Birthday Module\n");

    // Traverse list, deleting items
    list_for_each_entry_safe(ptr, next, &birthday_list, list)
    {
        printk(KERN_INFO
        "DELETED FAMILY MEMBER WITH BIRTHDAY: %d/%d/%d\n",
                ptr->day, ptr->month, ptr->year);
        del_person(ptr);
    }
}

void add_person(int d, int m, int y)
{
    struct birthday *person;

    person = kmalloc(sizeof(*person), GFP_KERNEL);
    person->day = d;
    person->month = m;
    person->year = y;
    INIT_LIST_HEAD(&person->list);
    list_add_tail(&person->list, &birthday_list);
}

void del_person(struct birthday* ptr)
{
    list_del(&ptr->list);
    kfree(ptr);
}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module With List");
MODULE_AUTHOR("EK");
