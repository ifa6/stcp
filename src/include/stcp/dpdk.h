#pragma once

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

#include <rte_config.h>
#include <rte_version.h>
#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_ether.h>
#include <rte_cycles.h>
#include <rte_lcore.h>
#include <rte_mbuf.h>
#include <rte_hexdump.h>





namespace dpdk {


class core {
private:
    static uint32_t num_mbufs;        /* num of mbuf that allocated in a mempool */
    static uint32_t mbuf_cache_size;  /* packet cache size in each mbufs */
    static uint16_t rx_ring_size;     /* rx ring size */
    static uint16_t tx_ring_size;     /* tx ring size */
    static uint16_t num_rx_rings;     /* num of rx_rings per port */
    static uint16_t num_tx_rings;     /* num of tx_rings per port */

    struct rte_mempool* mempool;
    void port_init(uint8_t port);


private:
    core();
    core(const core&) = delete;
    core& operator=(const core&) =delete;
    ~core();

public:
    static core& instance();

    void init(int argc, char** argv);
    size_t num_ports();
    uint16_t io_rx(uint16_t port, struct rte_mbuf** bufs, size_t num_bufs);
    uint16_t io_tx(uint16_t port, struct rte_mbuf** bufs, size_t num_bufs);
};





class pkt_queue {
protected:
    struct rte_mbuf* head;
    void enq(struct rte_mbuf* buf);
    struct rte_mbuf* deq();

public:
    static struct rte_mbuf* array2llist(
            struct rte_mbuf** bufs, size_t num_bufs);

    pkt_queue();
    ~pkt_queue();
    void print_info();
    size_t size();

    virtual void input(struct rte_mbuf* mbuf);
    virtual void output();
};




    
} /* namespace dpdk */




