#ifndef __LIBKDS_KDS_MOD_H_
#define __LIBKDS_KDS_MOD_H_

#include "libkds.h"


#include <stdbool.h>

typedef bool (*kds_inst_fn)     (void** handle);
typedef bool (*kds_load_fn)     (void* handle);
typedef bool (*kds_unload_fn)   (void* handle);
typedef bool (*kds_recv)        (void* handle, void** data_out, unsigned int* data_length_out);
typedef bool (*kds_send)        (void* handle, void* data_in, unsigned int data_length);

typedef struct kds_module
{
    const char*                 name;
    kds_inst_fn                 instantiate;
    kds_load_fn                 load;
    kds_unload_fn               unload;
    kds_recv                    recv;
    kds_send                    send;
} kds_module_t;




#endif // __LIBKDS_KDS_MOD_H_