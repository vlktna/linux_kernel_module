#ifndef OS_LAB2_DRIVER_H
#define OS_LAB2_DRIVER_H

#include <stdbool.h>

struct dentry_struct_info {
    unsigned int inode_num;
    unsigned int count;
    bool unhashed;
    bool unlinked;
};

struct vm_area_struct_info {
    bool isValid;
    unsigned long size;
    unsigned long offset;
    char flag_read;
    char flag_write;
    char flag_exec;
    char flag_mayshare;
};

struct message {
    struct dentry_struct_info dntr;
    struct vm_area_struct_info vma;
};

#define DEVICE_NAME "etx_device"
#define DEVICE_PATH "/dev/etx_device"
#define MAJOR_NUM 100

#define WR_VALUE _IOW(MAJOR_NUM, 0, struct message*)
#define RD_VALUE _IOR(MAJOR_NUM, 1, struct message*)

#endif //OS_LAB2_DRIVER_H
