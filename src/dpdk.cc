






#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>

#include <string>
#include <iostream>
#include <sstream>
#include <exception>
#include <vector>

#include <stcp/rte.h>
#include <stcp/net_device.h>
#include <stcp/dpdk.h>




uint32_t dpdk::num_mbufs = 8192;
uint32_t dpdk::mbuf_cache_size = 250;
