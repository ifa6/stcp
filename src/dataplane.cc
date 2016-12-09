

#include <stdint.h>
#include <stddef.h>
#include <stcp/dataplane.h>
#include <stcp/stcp.h>
#include <stcp/mempool.h>


namespace slank {

void dataplane::print_stat() const
{
    size_t rooty = screen.POS_PORT.y;
    size_t rootx = screen.POS_PORT.x;
    screen.move(rooty, rootx);

    screen.printwln("DataPlane ");
    screen.printwln(" Pool  : %u/%u",
            pool_use_count(mp), pool_size(mp));

    size_t i=0;
    for (const ifnet& dev : devices) {
        dev.print_stat(rootx, rooty+2+i*6);
        i++;
    }
}



} /* namespace slank */
