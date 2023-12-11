#include <download.h>

int main(int argc, char *argv[]) {

    int sockfd_A;    
    int sockfd_B;
    int port;

    FILE *fd;


    if (argc != 2) {
        printf("Usage: ./download ftp://[<user>:<password>@]<hotname>/<url-path>\n");
        return -1;
    }

    struct URL url;
    memset(&url, 0, sizeof(url));
    if (parse_url(argv[1], &url) != 0) {
        printf("Usage: ./download ftp://[<user>:<password>@]<hotname>/<url-path>\n");
        return -1;
    }

    printf("Target Host: %s\nFile Name: %s\nUser: %s\nPassword: %s\n", url.hostname, url.file, url.username, url.password);


    if ((sockfd_A = open_tcp_socket(url.ip, FTP_PORT)) < 0) {
        printf("Error opening socket A\n");
        return -1;
    }

    if (authenticate(sockfd_A, url.username, url.password) < 0) {
        printf("Error authenticating\n");
        return -1;
    }

    if ((setPassive(sockfd_A, &port, &url.ip)) < 0) {
        printf("Error setting passive mode\n");
        return -1;
    }

    if ((sockfd_B = open_tcp_socket(url.ip, port)) < 0) {
        printf("Error opening socket A\n");
        return -1;
    }

    if (request_file(sockfd_A, url.resource_path) < 0){
        printf("Error writing to socket A\n");
        return -1;
    }

    if (receive_file(sockfd_B, url.file) < 0) {
        printf("Error receiving file\n");
        return -1;
    }

    if (close_tcp_socket(sockfd_A) < 0){
        printf("Error closing socket A\n");
        return -1;
    }

    return 0;
}
