#include "libkds.h"
#include "kds_mod.h"
#include "kds_err.h"
#include "kds_config.h"

#include <stdlib.h>
#include <pcap.h>
#include <string.h>
typedef struct pm_handle 
{
    char*                   dev_name;
    pcap_t*                 dev_handle;
    char                    errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr      packet_header;
    kds_config_t            config;
} pm_handle_t;

kds_error pm_instantiate(void** handle)
{
    pm_handle_t* nh = NULL;
    nh = calloc(1, sizeof(pm_handle_t));

    if(nh == NULL)
        return KDS_NO_MEM;
    
    *handle = (void*)nh;

    return KDS_OK;
}

kds_error pm_load(pm_handle_t* handle)
{
    if(!handle)
        return KDS_INVALID_PARAM;

    handle->dev_name = pcap_lookupdev(handle->errbuf);
    if(handle->dev_name == NULL)
        return KDS_INIT_FAIL;
        
    handle->dev_handle = pcap_open_live(handle->dev_name, BUFSIZ, 1, 1000, handle->errbuf);

    if(handle->dev_handle == NULL)
        return KDS_INIT_FAIL;

    return KDS_OK;
}

kds_error pm_unload(pm_handle_t* handle)
{
    if(!handle || (!handle->dev_handle))
        return KDS_INVALID_PARAM;

    pcap_close(handle->dev_handle);
    handle->dev_handle = NULL;

    return KDS_OK;
}

kds_error pm_recv(pm_handle_t* handle, void** data_out, unsigned int* length_out)
{
    if(!handle || !data_out || !length_out)
        return false;
        
    const unsigned char* packet = pcap_next(handle->dev_handle, &handle->packet_header);
    if(packet == NULL)
        return KDS_NO_VALUE;
    
    *data_out = packet;
    *length_out = handle->packet_header.len;
    memset(&handle->packet_header, 0, sizeof(struct pcap_pkthdr));
    
    return KDS_OK;
}

kds_error pm_set_config(pm_handle_t* handle, kds_config_t* cfg)
{
    if(!handle || !cfg)
        return KDS_INVALID_PARAM;

    handle->config.pool         = cfg->pool;
    handle->config.size         = cfg->size;
    handle->config.pair_count   = cfg->pair_count;
    
    return KDS_OK;
}

kds_error pm_parse_config(pm_handle_t* handle, const char* string, unsigned int length)
{
    if(!handle || !string)
        return KDS_INVALID_PARAM;
    
    kds_error err = kds_cfg_init(&handle->config, KDS_CONFIG_DEFAULT_SIZE);

    if(err != KDS_OK)
        return err;

    err = kds_parse_cfg_string(&handle->config, string, length);

    if(err != KDS_OK)
        return err;


    return KDS_OK;
}


KDS_EXPORT kds_module_t KDS_MOD_NAME = {
    .name                   = "pcap",
    .module_requirements    = KDS_REQ_ADMIN | KDS_REQ_CONFIG,
    .instantiate            = (void*)pm_instantiate,
    .load                   = (void*)pm_load,
    .unload                 = (void*)pm_unload,
    .recv                   = (void*)pm_recv,
    .send                   = NULL,
    .set_config             = (void*)pm_set_config,
    .parse_config           = (void*)pm_parse_config,
};