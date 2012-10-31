#ifndef MY_UTILS_H
#define MY_UTILS_H


/**
 * This file contains a whole bunch of things that I need all the time
 * No particular organisation, might come with time
 */


#include <stdlib.h>


static inline void * xmalloc(size_t size)
{
        void * ptr = malloc(size);
        if (ptr == NULL) {
                puts("Failed attempt to alloc %d bytes, abort.");
                abort();
                return NULL; /* Does not actually return */
        }

        return ptr;
}




#define min(a, b) (a < b) ? a : b
#define max(a, b) (a < b) ? b : a

#define pdebug(str, ...) fprintf(stderr, "%s:%d (in %s): " str, __FILE__, __LINE__, __func__, ##__VA_ARGS__)


#endif /* end of include guard: MY_UTILS_H */
