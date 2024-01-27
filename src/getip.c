/**
 * Example code for getting the IP address from hostname.
 * tidy up includes
 */


//The struct hostent (host entry) with its terms documented

#include "../headers/getip.h"

struct hostent* getHostIP(char* hostname) {

    struct hostent *h = gethostbyname(hostname);
    if (h == NULL) {
        herror("gethostbyname()");
        memset(&h, 0, sizeof(h));
        return h;
    }

    printf("Host name  : %s\n", h->h_name);
    printf("IP Address : %s\n", inet_ntoa(*((struct in_addr *)h->h_addr_list[0])));

    return h;
}
