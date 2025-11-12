#ifndef _KERNEL_MISC_H
#define _KERNEL_MISC_H

#define internal static
#define global_variable static
#define local_persist static

#define unused __attribute__((__unused__))
#define ignore(i) (void)i

#endif
