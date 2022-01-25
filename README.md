# Linux kernel module

A set of programs at the user level and the Linux kernel level that collects information on the kernel side, passes it to the user level and displays it in an understandable form.

The program at the user level receives the PID of the process as input, which allows identifying the path to the vm_area_struct and dentry structures, transfers it to the kernel level, receives information about these structures, and return it as standard output.

A loadable kernel module accepts a request via the ioctl interface, determines the path to the target structure based on the passed request, and returns the result to the user level.