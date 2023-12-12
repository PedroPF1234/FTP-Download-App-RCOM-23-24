#pragma once

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "download.h"

int parse_url(char* host, struct URL *url);
int open_tcp_socket_with_response(char* ip, int port);
int open_tcp_socket_without_response(char* ip, int port);
int authenticate(int socket, char* username, char* password);
int setPassive(int socket, int* port, char* ip);
int readResponse(int socket, char *response);
int request_file(int socket, char* resource_path);
int receive_file(int socket, char* file);
int send_string(int socket, char *buf);
int close_tcp_socket(int socket);

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

// PASV State Machine
#define PASV_START 0
#define PASV_IP 1
#define PASV_PORT 2
#define PASV_END 3

// FTP Feedback Codes
#define SV_WELCOME 220
#define SV_PASSWORD 331
#define SV_LOGINSUCCESS 230
#define SV_PASSIVE 227
#define SV_DATACONNECTION 150
#define SV_TRANSFERCOMPLETE 226
#define SV_QUIT 221