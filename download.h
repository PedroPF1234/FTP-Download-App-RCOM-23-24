#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define MAX_CHARS 1000


#define FTP_PORT 21


struct URL {
    char hostname[MAX_CHARS];
    char resource_path[MAX_CHARS];
    char file[MAX_CHARS];
    char username[MAX_CHARS];
    char password[MAX_CHARS];
    char ip[MAX_CHARS];
}