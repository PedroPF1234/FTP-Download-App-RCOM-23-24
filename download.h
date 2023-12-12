#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "getip.h"
#include "clientTCP.h"

#define FTP_PORT 21


struct URL {
    char* hostname;
    char* resource_path;
    char* file;
    char* username;
    char* password;
    char* ip;
};