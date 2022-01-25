#include <sys/ioctl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <malloc.h>
#include <unistd.h>
#include "driver.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Enter pid\n");
        return 1;
    }
    pid_t pid = atoi(argv[1]);

    int fd;
    struct message msg;

    fprintf(stdout, "\nOpening Driver\n");
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "Cannot open device file\n");
        return 1;
    }

    fprintf(stdout, "Writing Pid to Driver\n");
    ioctl(fd, WR_VALUE, (pid_t * ) & pid);

    fprintf(stdout, "Reading Value from Driver\n\n");
    ioctl(fd, RD_VALUE, (struct message *) &msg);

    if (msg.vma.isValid) {
        printf("\nvm_area_struct_info for PID %d: \n", pid);
        printf("\tsize = %lu\n", msg.vma.size);
        printf("\tflags = %c%c%c%c\n", msg.vma.flag_read, msg.vma.flag_write, msg.vma.flag_exec, msg.vma.flag_mayshare);
        printf("\toffset = %lu\n", msg.vma.offset);

        printf("\ndentry_struct_info for PID %d: \n", pid);
        printf("\tinode number = %u\n", msg.dntr.inode_num);
        printf("\tusage count = %u\n", msg.dntr.count);
        printf("\tis dentry hashed = %s\n", msg.dntr.unhashed ? "true" : "false");
        printf("\tis dentry linked = %s\n", msg.dntr.unlinked ? "true" : "false");

    } else {
        fprintf(stdout, "\nprocess with PID = %d don't have virtual memory\n", pid);
    }

    fprintf(stdout, "\nClosing Driver\n");
    close(fd);

    return 0;
}
