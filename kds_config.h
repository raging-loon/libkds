#ifndef KDS_CONFIG_H_
#define KDS_CONFIG_H_

#include <stdint.h>
#include <stdbool.h>
#include "kds_err.h"
#include "libkds.h"

#define KDS_PAIR_FREE       0
#define KDS_PAIR_OCCUPIED   1

#define KDS_CONFIG_DEFAULT_SIZE     5

///
/// @brief 
///     Key Value Pair
///     Stores its hash for faster reinsertions
///
typedef struct kds_pair 
{
    const char* key;
    char*       value;
    uint32_t    hash;
    /// This is used by kds_config to keep track of free pairs
    uint8_t     free;
} kds_pair_t;

///
/// @brief
///     Config. Hash Map
///     Uses open address + linear probing
///
typedef struct kds_config
{
    uint16_t            pair_count;
    /// Total size of the pool
    uint16_t            size;
    kds_pair_t*         pool;
} kds_config_t;

///
/// @brief 
///     Initialize a configuration's memory pool
/// 
/// @param[in] cfg                  Input configuratoin
/// @param[in] initial_size         Initial allocation size. 
///                                 Use KDS_CONFIG_DEFAULT_SIZE for the...default size
/// 
/// @return KDS_NO_MEM              if we can't allocate memory
///
KDS_EXPORT kds_error kds_cfg_init(kds_config_t* cfg, unsigned int initial_size);

///
/// @brief 
///     Destroy the configuration's memory pool
///     Destroys each allocated key and then the pool
///
/// @param[in] cfg    the configuration 
///
/// @return KDS_INVALID_PARAM       if cfg is NULL
///
KDS_EXPORT kds_error kds_cfg_destroy(kds_config_t* cfg);

///
/// @brief 
///     Parse a configuration string, i.e. k1=v1,k2=v1,...,kN=vN
///     Also supports keys with no value
///
/// @param[in] cfg              the config 
/// @param[in] string           config string
/// @param[in] length           length of the string
///
/// @return KDS_INVALID_PARAM   if cfg or string is NULL
///
KDS_EXPORT kds_error kds_parse_cfg_string(kds_config_t* cfg, const char* string, unsigned int length);

///
/// @brief 
///     Insert a new key-value pair
///
/// @param[in] cfg       
/// @param[in] key       
/// @param[in] value         
///
/// @return NULL        if any inputs are NULL or we can't allocate memory
///
KDS_EXPORT kds_pair_t* kds_cfg_insert(kds_config_t* cfg, const char* key, char* value);

///
/// @brief
///     Find a pair based on the key
///
/// @param[in] cfg
/// @param[in] key
///
/// @return NULL        if the key cannot be found 
///
KDS_EXPORT kds_pair_t* kds_cfg_find(kds_config_t* cfg, const char* key);

///
/// @brief 
///     Find a pair, return the key's value
/// 
/// @param cfg 
/// @param key 
///
/// @return NULL        if the key cannot be found
///
KDS_EXPORT char* kds_cfg_find_val(kds_config_t* cfg, const char* key);

///
/// @brief 
///     Resize the memory pool
///
/// @param[in] cfg 
/// @param[in] new_size 
///
/// @return KDS_INVALID_PARAM   if cfg is NULL
///
KDS_EXPORT kds_error kds_cfg_resize(kds_config_t* cfg, unsigned int new_size);

#endif // KDS_CONFIG_H_
