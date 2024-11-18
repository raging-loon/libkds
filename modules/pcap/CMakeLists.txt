project(kds_pcap VERSION 1.0)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
add_library(
    kds_pcap
    SHARED
    pcapmod.c
)

target_link_libraries(kds_pcap PUBLIC pcap)
