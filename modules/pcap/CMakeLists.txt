project(kds_pcap VERSION 1.0)
add_library(
    kds_pcap
    SHARED
    pcapmod.c
    ../../src/kds_config.c
)

message("hello")
target_link_libraries(kds_pcap PUBLIC pcap)
