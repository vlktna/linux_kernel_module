#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/vmalloc.h>
#include <linux/ioctl.h>
#include <linux/pid.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/dcache.h>
#include "driver.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Volokitina Veronika");
MODULE_DESCRIPTION("ioctl: dentry, vm_area_struct");
MODULE_VERSION("1.0");

pid_t pid = 0;
dev_t dev = 0;
int major_no;
struct class *my_class;

struct task_struct *ts;
struct dentry_struct_info *dntr;
struct vm_area_struct_info *vma;
struct message *msg;

static int device_open(struct inode *inode, struct file *file);

static int device_release(struct inode *inode, struct file *file);

static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

void init_structs(void);

static struct file_operations fops = {
        .owner          = THIS_MODULE,
        .open           = device_open,
        .release        = device_release,
        .unlocked_ioctl = device_ioctl,
};

static int device_open(struct inode *inode, struct file *file) {
    printk("open device\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    printk("release device\n");
    return 0;
}

static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    switch (cmd) {
        case WR_VALUE:
            if (copy_from_user(&pid, (pid_t *) arg, sizeof(pid))) {
                printk("driver - Error copying data from user!\n");
            } else {
                printk("driver - Update the pid to %d\n", pid);
            }
            break;
        case RD_VALUE:
            init_structs();
            if (copy_to_user((struct message *) arg, msg, sizeof(struct message))) {
                printk("driver - Error copying data to user!\n");
            } else {
                printk("driver - The pid was copied!\n");
            }
            break;
    }
    return 0;
}

void init_structs() {
    dntr = vmalloc(sizeof(struct dentry_struct_info));
    vma = vmalloc(sizeof(struct vm_area_struct_info));

    vma->isValid = false;
    ts = get_pid_task(find_get_pid(pid), PIDTYPE_PID);

    if (ts->mm) {
        vma->isValid = true;
        vma->size = ts->mm->mmap->vm_end - ts->mm->mmap->vm_start;
        vma->offset = ts->mm->mmap->vm_pgoff;

        vma->flag_read = (ts->mm->mmap->vm_flags & VM_READ) ? 'r' : '-';
        vma->flag_write = (ts->mm->mmap->vm_flags & VM_WRITE) ? 'w' : '-';
        vma->flag_exec = (ts->mm->mmap->vm_flags & VM_EXEC) ? 'x' : '-';
        vma->flag_mayshare = (ts->mm->mmap->vm_flags & VM_MAYSHARE) ? 's' : 'p';

        dntr->inode_num = ts->mm->mmap->vm_file->f_path.dentry->d_inode->i_ino;
        dntr->count = ts->mm->mmap->vm_file->f_path.dentry->d_lockref.count;
        dntr->unhashed = d_unhashed(ts->mm->mmap->vm_file->f_path.dentry);
        dntr->unlinked = d_unlinked(ts->mm->mmap->vm_file->f_path.dentry);
    }

    msg = vmalloc(sizeof(struct message));
    msg->dntr = *dntr;
    msg->vma = *vma;
}

static int __init

etx_driver_init(void) {
    major_no = register_chrdev(0, DEVICE_NAME, &fops);
    printk("\nMajor_no : %d", major_no);
    my_class = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(my_class, NULL, MKDEV(major_no, 0), NULL, DEVICE_NAME);
    printk("\nDevice Initialized in kernel\n");
    return 0;
}

static void __exit

etx_driver_exit(void) {
    printk("\nDevice is Released or closed\n");
    device_destroy(my_class, MKDEV(major_no, 0));
    class_unregister(my_class);
    class_destroy(my_class);
    unregister_chrdev(major_no, DEVICE_NAME);
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);