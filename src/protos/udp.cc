

#include <stcp/protos/udp.h>
#include <stcp/stcp.h>
#include <stcp/config.h>


namespace stcp {




mbuf* stcp_udp_sock::recvfrom(stcp_sockaddr_in* src)
{
    if (state == unbind)
        throw exception("socket is not binded");

    while (rxq.size() == 0) ;

    stcp_udp_sockdata d = rxq.pop();
    *src = d.addr;
    return d.msg;
}

void stcp_udp_sock::sendto(mbuf* msg, const stcp_sockaddr_in* dst)
{
    stcp_udp_sockdata d(msg, *dst);
    txq.push(d);
}

void stcp_udp_sock::proc()
{
    while (!txq.empty()) {
        stcp_udp_sockdata d = txq.pop();
        core::udp.tx_push(d.msg, &d.addr, port);
    }
}

void stcp_udp_sock::bind(const stcp_sockaddr_in* a)
{
    addr = a->sin_addr;
    port = a->sin_port;
    state = binded;
}



/*
 * srcp: Source port as NetworkByteOrder
 * dstp: Destination port as NetworkByteOrder
 */
void udp_module::tx_push(mbuf* msg,
        const stcp_sockaddr_in* dst, uint16_t srcp)
{
    uint16_t udplen = mbuf_pkt_len(msg);

    stcp_udp_header* uh =
        reinterpret_cast<stcp_udp_header*>(mbuf_push(msg, sizeof(stcp_udp_header)));
    uh->sport = srcp;
    uh->dport = dst->sin_port;
    uh->len   = hton16(sizeof(stcp_udp_header) + udplen);
    uh->cksum = 0x0000;

    core::ip.tx_push(msg, dst, STCP_IPPROTO_UDP);
}


void udp_module::rx_push(mbuf* msg, stcp_sockaddr_in* src)
{
    stcp_udp_header* uh = mbuf_mtod<stcp_udp_header*>(msg);

    uint16_t dst_port = uh->dport;
    for (stcp_udp_sock* sock : socks) {
        src->sin_port = uh->sport;
        if (sock->port == dst_port) {
            mbuf_pull(msg, sizeof(stcp_udp_header));
            stcp_udp_sockdata d(msg, *src);
            sock->rxq.push(d);
            return ;
        }
    }

    /* Send ICMP Port Unreachable  */
    mbuf_push(msg, sizeof(stcp_ip_header));
    core::icmp.send_err(STCP_ICMP_UNREACH, STCP_ICMP_UNREACH_PORT, src, msg);
}

void udp_module::print_stat() const
{
    size_t rootx = core::screen.POS_UDP.x;
    size_t rooty = core::screen.POS_UDP.y;
    core::screen.move(rooty, rootx);

    core::screen.printwln("UDP module");

    if (!socks.empty()) {
        core::screen.printwln("");
        core::screen.printwln("\tNetStat");
    }
    size_t i = 0;
    for (const stcp_udp_sock* sock : socks) {

        core::screen.printwln("\t%u/udp rxq=%zd txq=%zd",
                hton16(sock->port),
                sock->rxq.size(),
                sock->txq.size());
        i ++;
    }
}

void udp_module::proc()
{
    for (stcp_udp_sock* sock : socks) {
        sock->proc();
    }
}





} /* namespace stcp */
