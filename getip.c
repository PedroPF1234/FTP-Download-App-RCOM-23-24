/**
 * Example code for getting the IP address from hostname.
 * tidy up includes
 */


//The struct hostent (host entry) with its terms documented

#include "getip.h"

int getHostIP(char* hostname, HostDetails *h) {

    #define h_addr h_addr_list[0]	The first address in h_addr_list.

    if ((h = gethostbyname(argv[1])) == NULL) {
        herror("gethostbyname()");
        return -1;
    }

    printf("Host name  : %s\n", h->h_name);
    printf("IP Address : %s\n", inet_ntoa(*((struct in_addr *) h->h_addr)));

    return 0;
}
