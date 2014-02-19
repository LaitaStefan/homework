//Papa Florin, 324CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define BUFLEN 256
#define MAX_CLIENTS 15

typedef struct {

    int secv;
    int open;
    char filename[15];
    char name[15];
    int size;
    int fd;

} sending_file;

sending_file s[MAX_CLIENTS];
int connected[MAX_CLIENTS];
char history[50][BUFLEN];
char open_files[MAX_CLIENTS][15];
int dim;

int indexOf(in_port_t port) {
    int i;
    int p = ntohs(port);

    for(i = 0; i < MAX_CLIENTS; i++) {

        if(p == connected[i]) return i;

    }

    return -1;
}

int getFd(char *filename) {
    int i;

    for(i = 0; i < MAX_CLIENTS; i++) {

        if(strcmp(filename, open_files[i]) == 0) return i;

    }

    return -1;
}

int getSendingSock() {
    int i;

    for(i = 0; i < MAX_CLIENTS; i++) {

        if(s[i].open == 1) return i;

    }

    return -1;

}

void error(char *msg)
{
    perror(msg);
    exit(0);
}

void send_message(int sockfd, char *buffer, fd_set *read_fds, char *addr, int *fdmax, char *myName) {
    int n, i;
    struct sockaddr_in serv_addr;
    char aux[BUFLEN], *tok2;

    //aflare port client
    memset(aux, 0, BUFLEN);
    memcpy(aux, &"infoclient", 10);
    memcpy(aux + 10, &" ", 1);

    strtok(buffer, " ");
    tok2 = strtok(NULL, " ");
    memcpy(aux + 11, tok2, strlen(tok2));

    send(sockfd, aux, BUFLEN, 0);

    if ((n = recv(sockfd, aux, sizeof(aux), 0)) <= 0) {
        if (n == 0) {
            //conexiunea s-a inchis
            printf("selectserver: socket %d hung up\n", i);
        } else {
            error("ERROR in recv");
        }
        close(sockfd); 
        FD_CLR(sockfd, read_fds);
    }

    else {

        if(strncmp(aux, "Nu exista clientul cerut", 24) == 0) {
            printf("Nu exista clientul cerut\n");
        }

        else {
            in_port_t port;
            int len = strlen(myName);

            memcpy(&port, aux, sizeof(in_port_t)); //portul pe care trimit

            int sock = indexOf(port);

            if(sock == -1) {

                sock = socket(AF_INET, SOCK_STREAM, 0);

                if (sock < 0) 
                    error("ERROR opening socket");

                serv_addr.sin_family = AF_INET;
                serv_addr.sin_port = port;
                inet_aton(addr, &serv_addr.sin_addr);

                //conectare la server 
                if (connect(sock,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0) {

                    error("ERROR connecting");    

                }

                FD_SET(sock, read_fds);
                if (sock > *fdmax) { 
                    *fdmax = sock;
                }

                connected[sock] = ntohs(port);

            }

            memset(aux, 0, BUFLEN);
            memcpy(aux, &"messageR", 8);

            memcpy(aux + 8, &len, 4);
            memcpy(aux + 12, myName, strlen(myName));

            memcpy(aux + 12 + strlen(myName), buffer + 9 + strlen(tok2), BUFLEN - 9 - strlen(tok2));

            aux[strlen(aux)] = 0;

            send(sock, aux, BUFLEN, 0);
        }
    }
}

void send_file(int sockfd, char *buffer, fd_set *read_fds, char *addr, int *fdmax, char *myName, char* filename, char* dest) {
    int n, i;
    struct sockaddr_in serv_addr;
    char aux[BUFLEN], *tok2, message[1024];

    //aflare port client
    memset(aux, 0, BUFLEN);
    memcpy(aux, &"infoclient", 10);
    memcpy(aux + 10, &" ", 1);

    memcpy(aux + 11, dest, strlen(dest));

    send(sockfd, aux, BUFLEN, 0);

    if ((n = recv(sockfd, aux, sizeof(aux), 0)) <= 0) {
        if (n == 0) {
            //conexiunea s-a inchis
            printf("selectserver: socket %d hung up\n", i);
        } else {
            error("ERROR in recv");
        }
        close(sockfd); 
        FD_CLR(sockfd, read_fds);
    }

    else {

        if(strncmp(aux, "Nu exista clientul cerut", 24) == 0) {
            printf("Nu exista clientul cerut\n");
        }

        else {
            in_port_t port;
            int len = strlen(myName);

            memcpy(&port, aux, sizeof(in_port_t)); //portul pe care trimit

            int sock = indexOf(port);

            if(sock == -1) {

                sock = socket(AF_INET, SOCK_STREAM, 0);

                if (sock < 0) 
                    error("ERROR opening socket");

                serv_addr.sin_family = AF_INET;
                serv_addr.sin_port = port;
                inet_aton(addr, &serv_addr.sin_addr);

                //conectare la server 
                if (connect(sock,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0) {

                    error("ERROR connecting");    

                }

                FD_SET(sock, read_fds);
                if (sock > *fdmax) { 
                    *fdmax = sock;
                }

                connected[sock] = ntohs(port);

            }

            //trimitere fisier
            if(s[sock].open == -1) {

                s[sock].fd = open(filename, O_RDONLY);
                struct stat st;
                stat(filename, &st);
                
                s[sock].size = st.st_size;

                //printf("%d %d\n",s[sock].size, sock);
                s[sock].secv = 0;
                s[sock].open = 1;

                memset(s[sock].filename, 0, 15);
                memcpy(s[sock].filename, filename, strlen(filename));

                memset(s[sock].name, 0, 15);
                memcpy(s[sock].name, dest, strlen(dest));
            }
               
            int length = strlen(filename);

            memcpy(message, &"file", 4);
            memcpy(message + 4, &(s[sock].secv), 4);
            memcpy(message + 8, &length, 4);
            memcpy(message + 12, filename, strlen(filename));

            length = strlen(myName);
            memcpy(message + 12 + strlen(filename), &length, 4);
            memcpy(message + 16 + strlen(filename), myName, strlen(myName));

            int offset = 16 + strlen(myName) + strlen(filename);

            int sent_size;
                

            if(s[sock].size >= 1020 - offset) sent_size = 1020 - offset;
            else sent_size = s[sock].size;

            memcpy(message + offset, &sent_size, 4);

            read(s[sock].fd, message + offset + 4, 1020 - offset);
            s[sock].size = s[sock].size - sent_size;
            s[sock].secv++;

            send(sock, message, 1024, 0);

            if(s[sock].size == 0) {

                memcpy(message + 4, &(s[sock].secv), 4);

                memcpy(message + offset, &(s[sock].size), 4);

                send(sock, message, 1024, 0);

                s[sock].open = -1;

                close(s[sock].fd);
            }

        }
    }
}

int main(int argc, char *argv[])
{
    int sockfd, sockfd_in, portno, n, fdmax, i, newsockfd, clilen;
    struct timeval timeout;
    struct sockaddr_in serv_addr, cli_addr;
    struct hostent *server;
    fd_set read_fds, tmp_fds;
    in_port_t port;

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    char buffer[1024];

    dim = 0; //initializare dimensiune istoric comenzi

    for(i = 0; i < MAX_CLIENTS; i++) {

        s[i].open = -1;

    }

    if (argc < 3) {
       fprintf(stderr,"Usage %s server_address server_port\n", argv[0]);
       exit(0);
    }  
    
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[4]));
    inet_aton(argv[3], &serv_addr.sin_addr);
    
    //conectare la server 
    if (connect(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0) {

        error("ERROR connecting");    

    }

    int len = strlen(argv[1]);
    port = htons(atoi(argv[2]));
    memcpy(buffer, &len, 4);
    memcpy(buffer + 4, argv[1], strlen(argv[1]));
    memcpy(buffer + 4 + strlen(argv[1]), &port, sizeof(in_port_t));
    send(sockfd, buffer, BUFLEN, 0);

    //bind pe portul primit ca parametru
    sockfd_in = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd_in < 0) 
        error("ERROR opening socket");
     
    portno = atoi(argv[2]);

    memset((char *) &cli_addr, 0, sizeof(cli_addr));
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_addr.s_addr = INADDR_ANY;    // foloseste adresa IP a masinii
    cli_addr.sin_port = htons(portno);
     
    if (bind(sockfd_in, (struct sockaddr *) &cli_addr, sizeof(struct sockaddr)) < 0) 
              error("ERROR on binding");
     
    listen(sockfd_in, MAX_CLIENTS);

    FD_ZERO(&read_fds);
    FD_ZERO(&tmp_fds);
    FD_SET(0, &read_fds);
    FD_SET(sockfd, &read_fds);
    FD_SET(sockfd_in, &read_fds);

    fdmax = sockfd_in;

    while(1){

        tmp_fds = read_fds;

        if (select(fdmax + 1, &tmp_fds, NULL, NULL, &timeout) == -1) 
            error("ERROR in select");
    
        for(i = 0; i <= fdmax; i++) {

            if (FD_ISSET(i, &tmp_fds)) {
                
                if(i == 0) {

                    fgets(buffer, BUFLEN-1, stdin);

                    if(strncmp(buffer, "listclients", 11) == 0) {

                        send(sockfd, buffer, BUFLEN, 0);

                        if ((n = recv(sockfd, buffer, sizeof(buffer), 0)) <= 0) {
                            if (n == 0) {
                                //conexiunea s-a inchis
                                printf("selectserver: socket %d hung up\n", i);
                            } else {
                                error("ERROR in recv");
                            }
                            close(sockfd); 
                            FD_CLR(sockfd, &read_fds);
                        }

                        else {

                            int n;
                            memcpy(&n, buffer, 4);

                            char *tok = strtok(buffer + 4, " ");

                            while(n > 0) {

                                printf("%s\n", tok);
                                tok = strtok(NULL, " ");
                                n--;
                            }
                        }   
                    }

                    else if(strncmp(buffer, "infoclient", 10) == 0) {

                        buffer[strlen(buffer) - 1] = 0; 
                        send(sockfd, buffer, BUFLEN, 0);

                        if ((n = recv(sockfd, buffer, sizeof(buffer), 0)) <= 0) {
                            if (n == 0) {
                                //conexiunea s-a inchis
                                printf("selectserver: socket %d hung up\n", i);
                            } else {
                                error("ERROR in recv");
                            }
                            close(sockfd); 
                            FD_CLR(sockfd, &read_fds);
                        }

                        else {

                            if(strncmp(buffer, "Nu exista clientul cerut", 24) == 0) {
                                printf("Nu exista clientul cerut\n");
                            }

                            else {
                                in_port_t port;
                                int len, seconds;
                                char name[15];
                                memset(name, 0, 15);

                                memcpy(&port, buffer, sizeof(in_port_t));
                                memcpy(&len, buffer + sizeof(in_port_t), 4);
                                memcpy(name, buffer + sizeof(in_port_t) + 4, len);
                                memcpy(&seconds, buffer + sizeof(in_port_t) + 4 + len, 4);

                                printf("%s %d %ds\n", name, ntohs(port), seconds);
                            }
                        } 
                    }

                    //TRIMITERE MESAJ

                    else if(strncmp(buffer, "message", 7) == 0) {

                        send_message(sockfd, buffer, &read_fds, argv[3], &fdmax, argv[1]);
                        
                    }
                    //FINAL TRIMITERE MESAJ


                    //BROADCAST

                    else if(strncmp(buffer, "broadcast", 9) == 0) {

                        char aux[BUFLEN];

                        memcpy(aux, buffer + 10, BUFLEN - 10);
                        memcpy(buffer, &"listclients", 11);

                        send(sockfd, buffer, BUFLEN, 0);

                        if ((n = recv(sockfd, buffer, BUFLEN, 0)) <= 0) {
                            if (n == 0) {
                                //conexiunea s-a inchis
                                printf("selectserver: socket %d hung up\n", i);
                            } else {
                                error("ERROR in recv");
                            }
                            close(sockfd); 
                            FD_CLR(sockfd, &read_fds);
                        }

                        else {

                            int n, i;

                            memcpy(&n, buffer, 4);

                            char tokens[n][15], buffer_aux[BUFLEN];

                            char *tok = strtok(buffer + 4, " ");

                            for(i = 0; i < n; i++) {

                                memset(tokens[i], 0, 15);

                                memcpy(tokens[i], tok, strlen(tok));

                                tok = strtok(NULL, " ");
                            }

                            for(i = 0; i < n; i++) {

                                if(strcmp(tokens[i], argv[1]) != 0) {

                                    memset(buffer_aux, 0, BUFLEN);
                                    memcpy(buffer_aux, &"message ", 8);
                                    memcpy(buffer_aux + 8, tokens[i], strlen(tokens[i]));
                                    
                                    memcpy(buffer_aux + 8 + strlen(tokens[i]), &" ", 1);
                                    memcpy(buffer_aux + 9 + strlen(tokens[i]), aux, BUFLEN - 9 - strlen(tokens[i]));

                                    send_message(sockfd, buffer_aux, &read_fds, argv[3], &fdmax, argv[1]);
                                }    
                            }

                        }   
                    }
                    //FINAL BROADCAST

                    //QUIT
                    else if(strncmp(buffer, "quit", 4) == 0) {

                        memcpy(buffer, &"quitC", 5);

                        send(sockfd, buffer, BUFLEN, 0);

                        close(sockfd);
                        close(sockfd_in);

                        return 0;
                    }
                    //FINAL QUIT

                    else if(strncmp(buffer, "sendfile", 8) == 0) {

                        char *dest, *filename;
                        buffer[strlen(buffer) - 1] = 0;
                        dest = strtok(buffer + 8, " ");
                        filename = strtok(NULL, " ");

                        send_file(sockfd, buffer, &read_fds, argv[3], &fdmax, argv[1], filename, dest);

                    }

                    else if(strncmp(buffer, "history", 7) == 0) {

                        int i;

                        for(i = 0; i < dim; i++) {

                            printf("%s", history[i]);

                        }
                    }
                }

                //primesc conexiune noua
                else if(i == sockfd_in) {

                    clilen = sizeof(cli_addr);

                    if ((newsockfd = accept(i, (struct sockaddr *)&cli_addr, &clilen)) == -1) {
                        error("ERROR in accept");                   
                    } 

                    else {
                        //adaug noul socket intors de accept() la multimea descriptorilor de citire
                        FD_SET(newsockfd, &read_fds);
                        if (newsockfd > fdmax) { 
                            fdmax = newsockfd;
                        }
                    }

                }

                //primesc ceva de la clientii direct conectati
                else {

                    memset(buffer, 0, BUFLEN);

                    if ((n = recv(i, buffer, sizeof(buffer), 0)) <= 0) {
                        if (n == 0) {
                            //conexiunea s-a inchis
                            printf("selectserver: socket %d hung up\n", i);
                        } else {
                            error("ERROR in recv");
                        }
                        close(i); 
                        FD_CLR(i, &read_fds);
                    }

                    else {

                        if(strncmp(buffer, "messageR", 8) == 0) {
                            char name[15], current_time[15];
                            int n;
                            time_t now;
                            struct tm *timeinfo;

                            time(&now);
                            timeinfo = localtime(&now);
                            strftime(current_time, 15, "%T", timeinfo);

                            memset(name, 0, 15);

                            memcpy(&n, buffer + 8, 4);
                            memcpy(name, buffer + 12, n);

                            printf("[%s][%s] %s", current_time, name, buffer + 12 + n);

                            memset(history[dim], 0, BUFLEN);

                            history[dim][0] = '[';
                            strcat(history[dim], current_time);    
                            strcat(history[dim], "][");
                            strcat(history[dim], name);
                            strcat(history[dim], "] ");
                            strcat(history[dim], buffer + 12 + n);

                            dim++;
                        }

                        if(strncmp(buffer, "quit", 4) == 0) {

                            memcpy(buffer, &"quitC", 5);

                            send(sockfd, buffer, BUFLEN, 0);

                            close(sockfd);
                            close(sockfd_in);

                            return 0;
                        }

                        if(strncmp(buffer, "nameConflict", 12) == 0) {

                            printf("Exista deja un client cu acest nume\n");

                            close(sockfd_in);
                            close(sockfd);

                            return 0;
                        }

                        if(strncmp(buffer, "file", 4) == 0) {

                            int secv, name_len, fd, file_len, offset, recv_dim;
                            char myName[15], filename[20];

                            memset(myName, 0, 15);
                            memset(filename, 0, 20);

                            memcpy(&secv, buffer + 4, 4);
                            memcpy(&file_len, buffer + 8, 4);
                            memcpy(filename, buffer + 12, file_len);
                            memcpy(&name_len, buffer + 12 + file_len, 4);
                            memcpy(myName, buffer + 16 + file_len, name_len);
                            memcpy(&recv_dim, buffer + 16 + file_len + name_len, 4);

                            offset = 20 + file_len + name_len;

                            if(secv == 0) {

                                strcat(filename, "_primit");

                                fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0666);
                                memset(open_files[fd], 0, 15);
                                memcpy(open_files[fd], filename, file_len);

                                memset(history[dim], 0, BUFLEN);

                                time_t now;
                                struct tm *timeinfo;
                                char current_time[15];

                                time(&now);
                                timeinfo = localtime(&now);
                                strftime(current_time, 15, "%T", timeinfo);

                                history[dim][0] = '[';
                                strcat(history[dim], current_time);
                                strcat(history[dim], "][");    
                                strcat(history[dim], myName);    
                                strcat(history[dim], "] sendfile ");
                                strcat(history[dim], filename);
                                strcat(history[dim], "\n");

                                dim++;

                            }

                            else fd = getFd(filename);

                            if(fd == -1) continue;

                            if(recv_dim == 0) {

                                close(fd);
                                memset(open_files[fd], 0, 15);
                                continue;

                            }

                            write(fd, buffer + offset, recv_dim);

                        }
                    }
                }
            }
        }

        int sck = getSendingSock();

        if(sck != -1) {

            memset(buffer, 0, 1024);

            strcat(buffer, "sendfile ");
            strcat(buffer, s[sck].name);
            strcat(buffer, " ");
            strcat(buffer, s[sck].filename);

            send_file(sockfd, buffer, &read_fds, argv[3], &fdmax, argv[1], s[sck].filename, s[sck].name);
        }            
    }

    return 0;

}