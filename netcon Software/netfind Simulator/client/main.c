#include <stdio.h>
#include <inttypes.h>

struct netfind_request
{
        char    netfind[7];
        uint8_t version;
        uint8_t type_filter;
        uint8_t mac_filter[6];
};

struct netfind_answer
{
        char     netdiscover[11];
        uint8_t  mac_addr[6];
        uint32_t uptime;
        char     name[32];
        char     place[32];
};

int main(int argc, char *argv[])
{












        return 0;
}

