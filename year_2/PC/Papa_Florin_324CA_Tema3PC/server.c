//Papa Florin, 324CA

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define MAX_CLIENTS	15
#define BUFLEN 256

void error(char *msg)
{
    perror(msg);
    exit(1);
}

struct in_addr address[MAX_CLIENTS];	//adresele IP ale clientilor conectati
char clients_names[MAX_CLIENTS][15];	//numele clientilor
in_port_t ports[MAX_CLIENTS]; 			//porturile pe care asculta clientii
char exists[MAX_CLIENTS];				//ia valoarea 1 daca un client exista si -1 in caz contrar 
time_t beginnings[MAX_CLIENTS];

//intoarce indexul unui client
int indexOf(char target[]) {
	int i;

	for(i = 0; i < MAX_CLIENTS; i++) {

		if(strcmp(target, clients_names[i]) == 0) {
			return i;
		}

	}

	return -1;
}

//intoarce 1 daca un client exista si 0 in caz contrar
int present(char target[], int len) {
	int i;

	for(i = 0; i < MAX_CLIENTS; i++) {

		if(strncmp(target, clients_names[i], len) == 0 && len == strlen(clients_names[i])) {
			return 1;
		}

	}

	return 0;	
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     char buffer[BUFLEN];
     struct sockaddr_in serv_addr, cli_addr;
     int n, i, j;

     memset(exists, -1, MAX_CLIENTS);

     fd_set read_fds;	//multimea de citire folosita in select()
     fd_set tmp_fds;	//multime folosita temporar 
     int fdmax;			//valoare maxima file descriptor din multimea read_fds

     if (argc < 2) {
         fprintf(stderr,"Usage : %s port\n", argv[0]);
         exit(1);
     }

     //golim multimea de descriptori de citire (read_fds) si multimea tmp_fds 
     FD_ZERO(&read_fds);
     FD_ZERO(&tmp_fds);
     
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     
     portno = atoi(argv[1]);

     memset((char *) &serv_addr, 0, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;	// foloseste adresa IP a masinii
     serv_addr.sin_port = htons(portno);
     
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) 
              error("ERROR on binding");
     
     listen(sockfd, MAX_CLIENTS);

     //adaugam noul file descriptor (socketul pe care se asculta conexiuni) in multimea read_fds
     FD_SET(0, &read_fds);
     FD_SET(sockfd, &read_fds);
     fdmax = sockfd;


	while (1) {

		tmp_fds = read_fds; 

		if (select(fdmax + 1, &tmp_fds, NULL, NULL, NULL) == -1) 
			error("ERROR in select");
	
		for(i = 0; i <= fdmax; i++) {

			if (FD_ISSET(i, &tmp_fds)) {
				
				if(i == 0) {

					fgets(buffer, BUFLEN-1, stdin);

            		if(strncmp(buffer, "status", 6) == 0) {

						for(j = 0; j < MAX_CLIENTS; j++) {
							if(exists[j] == 1) {
								printf("%s %s %d\n", clients_names[j], inet_ntoa(address[j]), ntohs(ports[j]));
							}
						}
					}

					else if(strncmp(buffer, "kick", 4) == 0) {
						char *tok;
						int index;

						buffer[strlen(buffer) - 1] = 0;

						strtok(buffer, " ");
						tok = strtok(NULL, " ");

						index = indexOf(tok);

						if(index == -1) printf("Clientul nu exista\n");

						else {

							memcpy(buffer, "quit", 4);
							send(index, buffer, BUFLEN, 0);
							close(index);
							FD_CLR(index, &read_fds);

							exists[index] = -1;
							memset(clients_names[index], 0, 15);

						}
					}

					else if(strncmp(buffer, "quit", 4) == 0) {

						memcpy(buffer, &"quit", 4);
						for(j = 0; j < MAX_CLIENTS; j++) {

							if(exists[j] == 1) {

								send(j, buffer, BUFLEN, 0);
								close(j);
								FD_CLR(j, &read_fds);

							}
						}

						close(sockfd);
						return 0;
					}
				}

				else if (i == sockfd) {
					// a venit ceva pe socketul inactiv(cel cu listen) = o noua conexiune
					// actiunea serverului: accept()
					clilen = sizeof(cli_addr);

					if ((newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen)) == -1) {
						error("ERROR in accept");					
					} 

					else {
						//adaug noul socket intors de accept() la multimea descriptorilor de citire
						FD_SET(newsockfd, &read_fds);
						if (newsockfd > fdmax) { 
							fdmax = newsockfd;
						}
					}

					//primesc informatiile clientului nou
					memcpy(&address[newsockfd], &cli_addr.sin_addr, sizeof(struct in_addr));

					n = recv(newsockfd, buffer, sizeof(buffer), 0);
					if (n <= 0) {
						if (n == 0) {
							//conexiunea s-a inchis
							printf("selectserver: socket %d hung up\n", newsockfd);
						} else {
							error("ERROR in recv");
						}
						close(newsockfd); 
						FD_CLR(newsockfd, &read_fds);
						
						exists[newsockfd] = -1;
						memset(clients_names[newsockfd], 0, 15);

					}

					else {
						int len;
						memcpy(&len, buffer, 4);
						memset(clients_names[newsockfd], 0, 15);

						//conflict de nume
						if(present(buffer + 4, len)) {

							memcpy(buffer, &"nameConflict", 12);
							send(newsockfd, buffer, BUFLEN, 0);

							close(newsockfd);
							FD_CLR(newsockfd, &read_fds);

						}

						else {

							time(&beginnings[newsockfd]);
							memcpy(&clients_names[newsockfd], buffer + 4, len);
							memcpy(&ports[newsockfd], buffer + 4 + len, sizeof(in_port_t));
							exists[newsockfd] = 1;

						}	
					}

				}
					
				else {
					// am primit date pe unul din socketii cu care vorbesc cu clientii
					//actiunea serverului: recv()
					memset(buffer, 0, BUFLEN);

					if ((n = recv(i, buffer, sizeof(buffer), 0)) <= 0) {
						if (n == 0) {
							//conexiunea s-a inchis
							printf("selectserver: socket %d hung up\n", i);
						} else {
							error("ERROR in recv");
						}
						close(i); 
						FD_CLR(i, &read_fds); // scoatem din multimea de citire socketul pe care 
						exists[i] = -1;
						memset(clients_names[i], 0, 15);
					} 
					
					//recv intoarce > 0
					else { 

						//primesc comanda listclients
						if(strncmp(buffer, "listclients", 11) == 0) {
							int count = 0;
							int number = 0;

							for(j = 0; j < MAX_CLIENTS; j++) {
								if(exists[j] == 1) {

									memcpy(buffer + count + 4, &clients_names[j], strlen(clients_names[j]));
									count += strlen(clients_names[j]);

									memcpy(buffer + count + 4, &" ", 1);
									count ++;
									number++;
								}
							}

							memcpy(buffer, &number, 4);
							send(i, buffer, BUFLEN, 0);
						}

						if(strncmp(buffer, "infoclient", 10) == 0) {
							char *name;
							int index, len;
							time_t now;
							int seconds;

							strtok(buffer, " ");
							name = strtok(NULL, " ");
							len = strlen(name);
							
							index = indexOf(name);
							if(index == -1) {
								memcpy(buffer, &"Nu exista clientul cerut", 24);
								send(i, buffer, BUFLEN, 0);
							}

							else {

								time(&now);
								seconds = difftime(now, beginnings[index]);

								memcpy(buffer, &ports[index], sizeof(in_port_t));
								memcpy(buffer + sizeof(in_port_t), &len, 4);
								memcpy(buffer + 4 + sizeof(in_port_t), name, len);
								memcpy(buffer + 4 + sizeof(in_port_t) + len, &seconds, 4);

								send(i, buffer, BUFLEN, 0);
							}
						}

						else if(strncmp(buffer, "quitC", 5) == 0) {
							close(i); 
							FD_CLR(i, &read_fds);

							exists[i] = -1;
							memset(clients_names[i], 0, 15);
						}

					}
				} 
			}
		}
     }

     close(sockfd);
   
     return 0; 
}


