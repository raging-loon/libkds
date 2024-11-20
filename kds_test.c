#include "kds_config.h"
#include <stddef.h>
#include <stdio.h>
int main()
{
    kds_config_t cfg;

    if(kds_cfg_init(&cfg, KDS_CONFIG_DEFAULT_SIZE) != KDS_OK)
    {
        printf("faol;\n");
        return -1;
    }

    char str[] = "interface=eth0";

    kds_parse_cfg_string(&cfg, str, sizeof(str));
    printf("%s\n", kds_cfg_find_val(&cfg, "interface"));

    // kds_cfg_insert(&cfg, "hello", "1");
    // kds_cfg_insert(&cfg, "hello2","2");
    // kds_cfg_insert(&cfg, "hello3","3");
    // kds_cfg_insert(&cfg, "hello4","4");
    // kds_cfg_insert(&cfg, "hello5","5");

    // printf("%s\n", kds_cfg_find(&cfg, "hello")->value);
    // printf("%s\n", kds_cfg_find(&cfg, "hello2")->value);
    // printf("%s\n", kds_cfg_find(&cfg, "hello3")->value);
    // printf("%s\n", kds_cfg_find(&cfg, "hello4")->value);
    // printf("%s\n", kds_cfg_find(&cfg, "hello5")->value);
    // printf("---------------------\n");
    // kds_cfg_insert(&cfg, "hello6","6");

    // printf("%s\n", kds_cfg_find(&cfg, "hello")->value);
    // printf("%s\n", kds_cfg_find(&cfg, "hello2")->value);
    // printf("%s\n", kds_cfg_find(&cfg, "hello3")->value);
    // printf("%s\n", kds_cfg_find(&cfg, "hello4")->value);
    // printf("%s\n", kds_cfg_find(&cfg, "hello5")->value);
    // printf("%s\n", kds_cfg_find(&cfg, "hello6")->value);

    kds_cfg_destroy(&cfg);
    return 0;
}