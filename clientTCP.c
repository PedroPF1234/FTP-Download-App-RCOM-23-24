/**      (C)2000-2021 FEUP
 *       tidy up some includes and parameters
 * */

#include "clientTCP.h"

//#define SERVER_PORT 6000
//#define SERVER_ADDR "192.168.28.96"


int parse_url(char* host, struct URL *url) {
    
    char defaultFTP[] = "ftp://";
    char cmp[7]; 

    for (int i=0; i<6; i++){
        cmp[i] = host[i];
    }
    cmp[6] = '\0';

    if(strcmp(cmp, defaultFTP) != 0){
        printf("Invalid URL\n");
        return -1;
    }

    int index = 6;
    int tempIndex = 0;

    int state = START;

    int indexNAME = 0;

    char* tempString = malloc(1000*sizeof(char));
    char* nameString = malloc(1000*sizeof(char));
    int nameIndex = 0;
    
    while(host[index]!='\0'){
        switch(state){
            case START:
                if(host[index] == ':'){
                    state = USERNAME_FOUND;
                    index++;
                    url->username = malloc(1000*sizeof(char));
                    strcpy(url->username, tempString);
                    memset(tempString, 0, 1000);
                    tempIndex = 0;
                }
                else if(host[index] == '/'){
                    state = HOSTNAME;
                    index++;
                    url->hostname = malloc(1000*sizeof(char));
                    strcpy(url->hostname, tempString);
                    memset(tempString, 0, 1000);
                    tempIndex = 0;
                    tempString[tempIndex++] = host[index];
                }
                else{
                    tempString[tempIndex++] = host[index];
                    index++;
                }
                break;
            case USERNAME_FOUND:
                if(host[index] == '@'){
                    state = HOSTNAME;
                    index++;
                    url->password = malloc(1000*sizeof(char));
                    strcpy(url->password, tempString);
                    memset(tempString, 0, 1000);
                    tempIndex = 0;
                }
                else{
                    tempString[tempIndex++] = host[index];
                    index++;
                }
                break;

            case HOSTNAME:
                if(host[index] == '/'){
                    state = PATH_STATE;
                    url->hostname = malloc(1000*sizeof(char));
                    strcpy(url->hostname, tempString);
                    memset(tempString, 0, 1000);
                    tempIndex = 0;
                    tempString[tempIndex++] = host[index];
                    index++;
                }
                else{
                    tempString[tempIndex++] = host[index];
                    index++;
                }
                break;

            case PATH_STATE:
                url->path = malloc(1000*sizeof(char));
                url->filename = malloc(1000*sizeof(char));
                if (host[index] == '/') {
                    memset(nameString, 0, 1000);
                    nameIndex = 0;
                    tempString[tempIndex++] = host[index++];
                } else {
                    nameString[nameIndex++] = host[index];
                    tempString[tempIndex++] = host[index];
                    index++;
                }
                break;
        }
    }

    strcpy(url->path, tempString);
    strcpy(url->filename, nameString);

    free(tempString);
    free(nameString);

    return 0;
}

int open_tcp_socket(char* ip, int port){

    int sockfd;
    struct sockaddr_in server_addr;

    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);  
    server_addr.sin_port = htons(port); 


    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        return -1;
    }

    if (connect(sockfd,
                (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect()");
        return -1;
    }

    if (readResponse(sockfd, response) != SV_WELCOME)
        return -1;

    return sockfd;
}

int authenticate(int socket, char* username, char* password){
    
        char* buf = malloc(1000*sizeof(char));
        char* response = malloc(1000*sizeof(char));
        size_t bytes;
    
        sprintf(buf, "USER %s\n", username);
        if (send_string(socket, buf) < 0)
            return -1;
    
        if (readResponse(socket, response) != SV_PASSWORD)
            return -1;
    
        printf("%s\n", response);

        memset(response, 0, 1000);
        sprintf(buf, "PASS %s\n", password);
        if (send_string(socket, buf) < 0)
            return -1;
    
       if (readResponse(socket, response) != SV_LOGINSUCCESS)
            return -1;
    
        printf("%s\n", response);
    
        free(buf);
        free(response);
    
        return 0;
}

int setPassive(int socket, int* port, char* ip){

    char* buf = malloc(1000*sizeof(char));
    char* response = malloc(1000*sizeof(char));
    size_t bytes;

    sprintf(buf, "pasv\n");
    bytes = write(socket, buf, strlen(buf));
    if (bytes > 0)
        printf("Bytes written %ld\n", bytes);
    else {
        perror("write()");
        return -1;
    }

    if (readResponse(socket, response) != SV_PASSIVE)
        return -1;

    printf("%s\n", response);

    int index = 0;
    int state = START;
    int tempIndex = 0;

    char* tempString = malloc(1000*sizeof(char));
    char* portString = malloc(1000*sizeof(char));
    int portIndex = 0;

    while(response[index]!='\0'){
        switch(state){
            case START:
                if(response[index] == '('){
                    state = IP_FOUND;
                    index++;
                }
                else{
                    index++;
                }
                break;
            case IP_FOUND:
                if(response[index] == ','){
                    state = PORT_FOUND;
                    index++;
                    ip = malloc(1000*sizeof(char));
                    strcpy(ip, tempString);
                    memset(tempString, 0, 1000);
                    tempIndex = 0;
                }
                else{
                    tempString[tempIndex++] = response[index];
                    index++;
                }
                break;

            case PORT_FOUND:
                if(response[index] == ')'){
                    state = END;
                    index++;
                    port = malloc(1000*sizeof(char));
                    strcpy(port, tempString);
                    memset(tempString, 0, 1000);
                    tempIndex = 0;
                }
                else{
                    tempString[tempIndex++] = response[index];
                    index++;
                }
                break;
        }
    }

    strcpy(port, tempString);

    free(tempString);
    free(portString);

    return 0;

}

int readResponse(int socket, char *buf){    

    int state = RESPONSE_START;
    int bytes;

    char* code = malloc(4*sizeof(char));
    int code_index = 0;
    char* response_line = malloc(1000*sizeof(char));
    int response_line_index = 0;

    while (state != RESPONSE_END)
    {
        bytes = read(socket, buf, 1);
        switch (state)
        {
        case RESPONSE_START:
            if (buf[0] == ' ')
                code[code_index++] = '\0';
                printf("%s ", code);
                state = RESPONSE_SINGLELINE;
            else if (buf[0] == '-')
                code[code_index++] = '\0';
                printf("%s-", code);
                state = RESPONSE_MULTILINE;
            else if (buf[0] == '\n')
                code[code_index++] = '\0';
                printf("%s\n", code);
                state = RESPONSE_END;
            else
                code[code_index] = buf[0];
            break;
        
        case RESPONSE_SINGLELINE:
            if (buf[0] == '\n')
                response_line[response_line_index++] = '\0';
                printf("%s\n", response_line);
                state = RESPONSE_END;
            else
                response_line[response_line_index++] = buf[0];
            break;

        case RESPONSE_MULTILINE:
            if (buf[0] == '\n')
                response_line[response_line_index++] = '\0';
                printf("%s\n", response_line);
                memset(response_line, 0, 1000);
                memset(code, 0, 4);
                response_line_index = 0;
                code_index = 0;
                state = RESPONSE_START;
            else
                response_line[response_line_index++] = buf[0];
            break;

        default:
            break;
        }
    }

    free(code);
    free(response_line);

    int ret;
    sscanf(code, "%d", &ret);
    
    return ret;
}


int request_file(int socket, char* resource_path){
    char cmd[5+strlen(resource_path)+1];
    sprintf(cmd, "retr %s\n", resource_path);
    write(socket, cmd, sizeof(cmd));
    return 0;
}

int receive_file(int socket, char* file) {
    
    FILE *fd = fopen(file, "wb");

    if(fd == NULL){
        perror("fopen()");
        return -1;
    }

    char* buf = malloc(1000*sizeof(char));
    int bytes = read(socket, buf, 1000);

    while(bytes > 0){
        fwrite(buf, bytes, 1, fd);
        bytes = read(socket, buf, 1000);
    }

    fclose(fd);

    if(readResponse(socket, response) != SV_TRANSFERCOMPLETE) 
        return -1;

    return 0;
}



int send_string(int socket, char *buf){
    bytes = write(socket, buf, strlen(buf));
    if (bytes > 0)
        printf("Bytes written %ld\n", bytes);
    else {
        perror("write()");
        return -1;
    }
}


int close_tcp_socket(int socket){
    if (close(socket)<0) {
        perror("close()");
        return -1;
    }
}

/*
int main(int argc, char **argv) {

    if (argc > 1)
        printf("**** No arguments needed. They will be ignored. Carrying ON.\n");
    int sockfd;
    struct sockaddr_in server_addr;
    char buf[] = "Mensagem de teste na travessia da pilha TCP/IP\n";
    size_t bytes;

    //server address handling
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);    //32 bit Internet address network byte ordered
    server_addr.sin_port = htons(SERVER_PORT);        //server TCP port must be network byte ordered

    //open a TCP socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(-1);
    }
    //connect to the server
    if (connect(sockfd,
                (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect()");
        exit(-1);
    }
    //send a string to the server
    bytes = write(sockfd, buf, strlen(buf));
    if (bytes > 0)
        printf("Bytes escritos %ld\n", bytes);
    else {
        perror("write()");
        exit(-1);
    }

    if (close(sockfd)<0) {
        perror("close()");
        exit(-1);
    }
    return 0;
}

*/


