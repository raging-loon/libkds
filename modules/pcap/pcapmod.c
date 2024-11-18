#include "libkds.h"
#include "kds_mod.h"
#include <stdlib.h>
#include <pcap.h>
#include <string.h>
typedef struct pm_handle 
{
    char*                   dev_name;
    pcap_t*                 dev_handle;
    char                    errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr      packet_header;
} pm_handle_t;

bool pm_instantiate(void** handle)
{
    pm_handle_t* nh = NULL;
    nh = calloc(1, sizeof(pm_handle_t));

    if(nh == NULL)
        return false;
    
    *handle = (void*)nh;

    return true;
}

bool pm_load(pm_handle_t* handle)
{
    if(!handle)
        return false;

    handle->dev_name = pcap_lookupdev(handle->errbuf);
    if(handle->dev_name == NULL)
        return false;
        
    handle->dev_handle = pcap_open_live(handle->dev_name, BUFSIZ, 1, 1000, handle->errbuf);

    if(handle->dev_handle == NULL)
        return false;

    return true;
}

bool pm_unload(pm_handle_t* handle)
{
    if(!handle || (!handle->dev_handle))
        return false;

    pcap_close(handle->dev_handle);
    handle->dev_handle = NULL;

    return true;
}

bool pm_recv(pm_handle_t* handle, void** data_out, unsigned int* length_out)
{
    if(!handle || !data_out || !length_out)
        return false;
        
    const unsigned char* packet = pcap_next(handle->dev_handle, &handle->packet_header);
    if(packet == NULL)
        return false;
    *data_out = packet;
    *length_out = handle->packet_header.len;
    memset(&handle->packet_header, 0, sizeof(struct pcap_pkthdr));
    return true;
}

KDS_EXPORT kds_module_t KDS_MOD_NAME = {
    .name           = "pcap",
    .instantiate    = (void*)pm_instantiate,
    .load           = (void*)pm_load,
    .unload         = (void*)pm_unload,
    .recv           = (void*)pm_recv,
    .send           = NULL
};