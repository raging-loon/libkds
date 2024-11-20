project(kds_pcap VERSION 1.0)
add_library(
    kds_pcap
    SHARED
    pcapmod.c
    ../../kds_config.c
)

target_link_libraries(kds_pcap PUBLIC pcap)
