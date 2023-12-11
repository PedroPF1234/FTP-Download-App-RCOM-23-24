#pragma once

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "download.h"


// Parser State Machine
#define START 0
#define USERNAME_FOUND 1
#define PATH_STATE 2
#define HOSTNAME 3

// Response State Machine
#define RESPONSE_START 0
#define RESPONSE_MESSAGE 2
#define RESPONSE_SINGLELINE 3
#define RESPONSE_MULTILINE 4
#define RESPONSE_END 5

// FTP Feedback Codes
#define SV_WELCOME 220
#define SV_PASSWORD 331
#define SV_LOGINSUCCESS 230
#define SV_PASSIVE 227
#define SV_DATACONNECTION 150
#define SV_TRANFERCOMPLETE 226
#define SV_QUIT 221