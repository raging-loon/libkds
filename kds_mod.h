#ifndef __LIBKDS_KDS_MOD_H_
#define __LIBKDS_KDS_MOD_H_

#include "libkds.h"
#include "kds_config.h"


#include <stdbool.h>

typedef kds_error (*kds_inst_fn)        (void** handle);
typedef kds_error (*kds_load_fn)        (void* handle);
typedef kds_error (*kds_unload_fn)      (void* handle);
typedef kds_error (*kds_recv)           (void* handle, void** data_out, unsigned int* data_length_out);
typedef kds_error (*kds_send)           (void* handle, void* data_in, unsigned int data_length);
typedef kds_error (*kds_set_config)     (void* handle, kds_config_t* cfg);
typedef kds_error (*kds_parse_config)   (void* handle, const char* config_str, unsigned int config_length);

typedef enum {
    KDS_REQ_NONE            = (1 << 0),
    KDS_REQ_ADMIN           = (1 << 1),
    KDS_REQ_CONFIG          = (1 << 2)
} kds_mod_requirement_t;

typedef struct kds_module
{
    const char*                 name;
    kds_mod_requirement_t       module_requirements;
    kds_inst_fn                 instantiate;
    kds_load_fn                 load;
    kds_unload_fn               unload;
    kds_recv                    recv;
    kds_send                    send;
    kds_set_config              set_config;
    kds_parse_config            parse_config;
} kds_module_t;




#endif // __LIBKDS_KDS_MOD_H_