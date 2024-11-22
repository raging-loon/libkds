#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "libkds/kds_config.h"
#include "libkds/kds_err.h"

static uint32_t kds_fnv1a(const char* string, unsigned int length)
{
    if(!string || length == 0)
        return 0;

#define KDS_FNV1A_PRIME     0x01000193
#define KDS_FNV1A_SEED      0x811C9DC5

    uint32_t hash = KDS_FNV1A_SEED;

    for(unsigned int i = 0; i < length; i++)
        hash = (string[i] ^ hash) * KDS_FNV1A_PRIME;

    return hash;
}

kds_error kds_cfg_init(kds_config_t* cfg, unsigned int initial_size)
{
    if(!cfg)
        return KDS_INVALID_PARAM;
    
    cfg->pool = NULL;
    
    cfg->pool = (kds_pair_t*)calloc(initial_size, sizeof(kds_pair_t));
    
    if(!cfg->pool)
        return KDS_NO_MEM;
    
    cfg->pair_count = 0;
    cfg->size       = initial_size;

    return KDS_OK;
}

///
/// @brief 
///     Insert a key based on hash
///
/// @param[in] cfg 
/// @param[in] hash 
/// @param[in] key 
/// @param[in] value 
///
/// @return NULL    if we cannot find a place for the key
///
static kds_pair_t* __int_kds_cfg_insert(kds_config_t* cfg, uint32_t hash, const char* key, char* value)
{
    uint32_t index = hash % cfg->size;

    int current = 0;
    while(current != cfg->size)
    {
        if(cfg->pool[index].free == KDS_PAIR_FREE)
        {
            cfg->pool[index].key    = key;
            cfg->pool[index].value  = value;
            cfg->pool[index].hash   = hash;
            cfg->pool[index].free   = KDS_PAIR_OCCUPIED;
            cfg->pair_count++;
            return &cfg->pool[index];
        }
        index = (index + 1) % cfg->size;
        ++current;
    }

    return NULL;
}


kds_pair_t* kds_cfg_insert(kds_config_t* cfg, const char* key, char* value)
{
    if(!cfg || !key || !value || !cfg->pool)
        return NULL;

    if(cfg->pair_count >= cfg->size)
    {
        if(kds_cfg_resize(cfg, cfg->size * 2) != KDS_OK)
            return false;
    }

    uint32_t hash = kds_fnv1a(key, strlen(key));
    
    return __int_kds_cfg_insert(cfg, hash, key, value);
}

kds_pair_t* kds_cfg_find(kds_config_t* cfg, const char* key)
{
    if(!cfg || !cfg->pool || cfg->pair_count == 0 || !key)
        return NULL;

    uint32_t index = kds_fnv1a(key, strlen(key)) % cfg->size;

    for(unsigned int i = 0; i < cfg->size; i++)
    {
        if(cfg->pool[index].free == KDS_PAIR_OCCUPIED && !strcmp(key, cfg->pool[index].key))
            return &cfg->pool[index];

        index = (index + 1) % cfg->size;
        
    }

    return NULL;
}

char* kds_cfg_find_val(kds_config_t* cfg, const char* key)
{
    kds_pair_t* pair = kds_cfg_find(cfg, key);
    
    if(pair)
        return pair->value;
    
    return NULL;
}


kds_error kds_cfg_resize(kds_config_t* cfg, unsigned int new_size)
{
    if(!cfg)
        return KDS_INVALID_PARAM;

    kds_pair_t* new_pool = calloc(new_size, sizeof(kds_pair_t));
    kds_pair_t* old_pool = cfg->pool;

    unsigned int old_size = cfg->size;

    cfg->pool = new_pool;
    cfg->size = new_size;

    for(unsigned int i = 0; i < old_size; i++)
    {
        if(old_pool[i].free == KDS_PAIR_OCCUPIED)
        {
            __int_kds_cfg_insert(
                cfg, 
                old_pool[i].hash, 
                old_pool[i].key, 
                old_pool[i].value
            );

        }
    }


    return KDS_OK;
}


kds_error kds_cfg_destroy(kds_config_t* cfg)
{
    if(!cfg || !cfg->pool)
        return KDS_INVALID_PARAM;

    free(cfg->pool);

    return KDS_OK;
}

///
/// @brief 
///     Parse a key-value config, e.g. "k=v" or "k"
///
/// @details
///     This will make ONE allocation and store @str inside of it
///     It will then look for index of '=' or ':'. 
///     This will be the NULL terminator.
///     KEY will point to the beginning of beginning of the new string
///     VALUE will point to 
///         - Just after the NULL terminator
///         - or NULL if there is no value
///
/// @param[in] cfg 
/// @param[in] str 
/// @param[in] len 
///
/// @return KDS_NO_MEM      if we cannot allocate memory
///
static kds_error kds_parse_cfg_value(kds_config_t* cfg, const char* str, unsigned int len)
{

    char* kv_mem = (char*)malloc(len + 1);

    if(!kv_mem)
        return KDS_NO_MEM;

    strncpy(kv_mem, str, len+1);
    printf("%s\n",str);
    unsigned int split_loc = 0;

    for(unsigned int i = 0; i < len; i++)
    {
        if(str[i] == '=' || str[i] == ':')
        {
            split_loc = i;
            break;
        }
    }

    char* key_ptr = kv_mem;
    char* val_ptr = NULL;
    if(split_loc != 0)
    {
        // mark end of key
        kv_mem[split_loc] = 0;
        val_ptr = kv_mem + split_loc + 1;
    }

    kds_cfg_insert(cfg, key_ptr, val_ptr);

    return KDS_OK;
}

kds_error kds_parse_cfg_string(kds_config_t* cfg, const char* string, unsigned int length)
{
    if(!cfg || !cfg->pool || !string)
        return KDS_INVALID_PARAM;

    unsigned int cur_key_idx = 0;
    unsigned int str_idx = 0;

    char buffer[500];
    memset(buffer, 0, sizeof(buffer));
    cur_key_idx = str_idx;

    while(str_idx < length)
    {
        
        if(string[str_idx] == ',' || string[str_idx] == '\0')
        {
            unsigned int length = str_idx - cur_key_idx;
            strncat(buffer, string + cur_key_idx, str_idx);
            // mark end of string
            buffer[length] = 0;

            kds_parse_cfg_value(cfg, buffer, length);
            // so strncat will think the string is empty
            buffer[0] = 0;
            cur_key_idx = str_idx + 1;
        }

        str_idx++;

    }    

    return KDS_OK;
}

