
// Server side C/C++ program to demonstrate Socket programming

#define TRUE 1
#define FALSE 0

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 4444
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = 1;

    while (valread != 0)
    {
        printf("reading\n");
        valread = read(new_socket, buffer, 1024);
        printf("%d %s\n", valread, buffer);
    }

    char buff[1024];
    FILE *fp = fopen("form.php", "r");
    while (fgets(buff, 1024, fp) != 0)
    {
        send(new_socket, buff, 1024, 0);
    }
    fclose(fp);
    close(new_socket);

    printf("Hello message sent\n");
    return 0;
}

typedef struct
{
    meta_line **list;
    int size;
} meta;

typedef struct
{
    char *key;
    char *value;
} meta_line;

meta *get_meta(int socket)
{

    char ch;
    int list_size = 0;

    meta *meta_data = malloc(sizeof(meta));
    meta_data->list = malloc(sizeof(meta_line *));

    while (1)
    {
        meta_data->list = realloc(meta_data->list, (list_size + 1) * sizeof(meta_line *));
        meta_line *line = malloc(sizeof(meta_line));

        read(socket, ch, 1);

        if (check_endline() == TRUE)
        {
            meta_data->list[list_size] = line;
            list_size++;
        }
    }

    meta_data->size = list_size;
    return meta_data;
}


int check_endline(char *line)
{
    char *endline = "\r\n";
    int len = strlen(line);

    if (endline[0] == line[len - 2] && endline[1] == line[len - 1])
        return TRUE;
    else
        return FALSE;
}