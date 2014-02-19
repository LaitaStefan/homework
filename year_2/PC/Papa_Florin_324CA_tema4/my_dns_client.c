//Papa Florin, 324CA

#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>  
#include <unistd.h>     
#include <sys/types.h>  

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "dns_message.h"

#define MAXLEN 512
#define LIMIT 49152
#define ID_NUMBER 50

unsigned short convertType(char *type) {

	if(strcmp(type, "A") == 0) {

		return 1;
	}
	else if(strcmp(type, "NS") == 0) {

		return 2;
	}
	else if(strcmp(type, "CNAME") == 0) {

		return 5;
	}
	else if(strcmp(type, "MX") == 0) {

		return 15;
	}
	else if(strcmp(type, "SOA") == 0) {

		return 6;
	}
	else if(strcmp(type, "TXT") == 0) {

		return 16;
	}
	else {

		printf("Acest tip de interogare nu este suportat\n");
		exit(0);
	}
}

//incapsuleaza un nume de domeniu in formatul specific
void formatNameForQuery(char *name, char *format) {

	char *tok;
	char nr;
	int offset = 0;

	memset(format, 0, MAXLEN);

	tok = strtok(name, ".");

	while(tok != NULL) {

		nr = strlen(tok);

		memcpy(format + offset, &nr, 1);

		offset++;

		memcpy(format + offset, tok, strlen(tok));

		offset += strlen(tok);

		tok = strtok(NULL, ".");
	}
}

//decapsuleaza un nume de domeniu
int formatNameForPrint(char *buffer, int offset, char* name) {

	unsigned short n;
	int count = 0;
	memset(name, 0, MAXLEN);

	memcpy(&n, buffer + offset, 2);
	n = ntohs(n);

	if(n >= LIMIT) {

		char aux[MAXLEN];

		n = n - LIMIT;

		formatNameForPrint(buffer, n, aux);

		strncat(name, aux, strlen(aux));

		return 1;
	}

	else {

		if((buffer + offset)[count] != 0) {

			int size = (buffer + offset)[count];
			count++;

			strncat(name, buffer + offset + count, size);
			strncat(name, ".", 1);

			count += size;

			char aux[MAXLEN];
			int s;

			s = formatNameForPrint(buffer, offset + count, aux);

			strncat(name, aux, strlen(aux));

			return count + s;
		}
		else {

			return strlen(name);
		}	
	}	
}

//formeaza un mesaj de tip query si intoarce lungimea acestuia
int createQuery(char *name, char *type, char *query) {

	char form[MAXLEN];
	dns_header_t header;
	dns_question_t question;
	int offset = 0;

	memset(&header, 0, sizeof(dns_header_t));
	memset(&question, 0, sizeof(dns_question_t));

	header.id = ID_NUMBER;
	header.rd = 1;
	header.qdcount = htons(1);

	formatNameForQuery(name, form);

	question.qtype = htons(convertType(type));
	question.qclass = htons(1);

	memcpy(query, &header, sizeof(dns_header_t));
	offset += sizeof(dns_header_t);

	memcpy(query + offset, form, strlen(form));
	offset += strlen(form) + 1;	//pentru a lasa un 0 la sfarsitul numelui

	memcpy(query + offset, &question, sizeof(dns_question_t));
	offset += sizeof(dns_question_t);

	return offset;
}

void getServerIP(FILE *f, char *buffer) {

	memset(buffer, 0, MAXLEN);

	while(fgets(buffer, MAXLEN, f) != NULL) {

		if(buffer[0] != '#' && strlen(buffer) > 1) {

			buffer[strlen(buffer) - 1] = 0;
			return;
		}
	}

	printf("Nu s-a obtinut un raspuns de la niciun server DNS\n");
	exit(0);
}

void processMessage(char *buffer, 
					char *server_ip, 
					char *qname, 
					char *qtype,
					FILE *serv) {

	dns_header_t header;
	dns_rr_t res_rec;
	int offset= 0;
	int count, size, i, k;
	char name[MAXLEN];
	unsigned char adr[4];
	FILE *logfile;

	logfile = fopen("logfile", "a");

	memcpy(&header, buffer + offset, sizeof(dns_header_t));
	offset += sizeof(dns_header_t);

	//in caz de eroare se merge mai departe
	if(header.rcode != 0 || (ntohs(header.ancount) == 0 &&
							 ntohs(header.nscount) == 0 &&
							 ntohs(header.arcount) == 0)) return;

	//extrag numele din antetul question
	size = formatNameForPrint(buffer, offset, name);

	offset += size + sizeof(dns_question_t) + 1;

	fprintf(logfile, "; %s - %s %s\n", server_ip, qname, qtype);

	for(k = 0; k < 3; k++) {

		if(k == 0) {

			count = ntohs(header.ancount);
			if(count > 0) {
				
				fprintf(logfile, "\n;; ANSWER SECTION:\n");
			}	
		}
		else if(k == 1) {

			count = ntohs(header.nscount);
			if(count > 0) {
			
				fprintf(logfile, "\n;; AUTHORITY SECTION:\n");
			}	
		}
		else if(k == 2) {

			count = ntohs(header.arcount);
			if(count > 0) {
			
				fprintf(logfile, "\n;; ADDITIONAL SECTION:\n");
			}	
		}

		for(i = 0; i < count; i++) {

			//RR name
			size = formatNameForPrint(buffer, offset, name);
			offset += size + 1; 

			//RR struct
			memcpy(&res_rec, buffer + offset, sizeof(dns_rr_t));
			offset += sizeof(dns_rr_t) - 2;

			//raspuns de tip A
			if(ntohs(res_rec.type) == 1) {

				memcpy(adr, buffer + offset, 4);
				offset += 4;
				fprintf(logfile, "%s A IN %d.%d.%d.%d\n", qname, adr[0], adr[1], adr[2], adr[3]);
			}

			//raspuns de tip NS
			else if(ntohs(res_rec.type) == 2) {

				size = formatNameForPrint(buffer, offset, name);
				offset += size + 1;
				fprintf(logfile, "%s NS IN %s\n", qname, name);
			}

			//raspuns de tip CNAME
			else if(ntohs(res_rec.type) == 5) {

				size = formatNameForPrint(buffer, offset, name);
				offset += size + 1;
				fprintf(logfile, "%s CNAME IN %s\n", qname, name);
			}

			//raspuns de tip MX
			else if(ntohs(res_rec.type) == 15) {

				unsigned short pref;

				memcpy(&pref, buffer + offset, 2);
				offset += 2;

				size = formatNameForPrint(buffer, offset, name);
				offset += size + 1;
				fprintf(logfile, "%s MX IN %d %s\n", qname, ntohs(pref), name);
			}

			//raspuns de tip SOA
			else if(ntohs(res_rec.type) == 6) {

				dns_SOA ans;

				size = formatNameForPrint(buffer, offset, name);
				offset += size + 1;
				fprintf(logfile, "%s SOA IN %s ", qname, name);

				size = formatNameForPrint(buffer, offset, name);
				offset += size + 1;

				memcpy(&ans, buffer + offset, sizeof(dns_SOA));
				fprintf(logfile, "%s %d %d %d %d %d\n", name, ntohs(ans.serial),
															  ntohs(ans.refresh),
															  ntohs(ans.retry),
															  ntohs(ans.expire),
															  ntohs(ans.minimum));	
				offset += sizeof(dns_SOA);											  		
			}

			//raspuns de tip text
			else if(ntohs(res_rec.type) == 16) {

				int s = ntohs(res_rec.rdlength);

				memset(name, 0, MAXLEN);
				memcpy(name, buffer + offset, s);

				fprintf(logfile,"%s TXT IN %s\n", qname, name);
				offset += s;
			}
		}		
	}

	fprintf(logfile,"\n\n\n");
	fclose(logfile);
	fclose(serv);

	exit(0);
}

int main(int argc, char* argv[]) {

	int sockfd;
  	int size;
  	struct sockaddr_in servaddr;
  	char server_ip[MAXLEN];
  	char sendbuf[MAXLEN];
  	char recvbuf[MAXLEN];
  	FILE *dns_servers;
  	unsigned int x;
  	struct timeval tv;

  	//initializez timeout
  	tv.tv_sec = 1;
  	tv.tv_usec = 0;

  	//deschid fisierul dns_servers.conf si copiez prima adresa valida
  	dns_servers = fopen("dns_servers.conf", "r");

  	getServerIP(dns_servers, server_ip);

  	
  	while(server_ip != NULL) {

	  	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
			printf("Eroare la  creare socket.\n");
			exit(-1);
	  	}

	  	//setare timeout pentru receive la socket
	  	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(struct timeval));

	  	/* formarea adresei serverului */
	  	memset(&servaddr, 0, sizeof(servaddr));
	  	servaddr.sin_family = AF_INET;
	  	servaddr.sin_port = htons(53);	  	

	  	if (inet_aton(server_ip, &servaddr.sin_addr) <= 0 ) {

	    	printf("Adresa IP invalida.\n");

	    	getServerIP(dns_servers, server_ip);

	    	continue;
	  	}

	  	char aux[MAXLEN];
	  	memset(aux, 0, MAXLEN);
	  	memset(sendbuf, 0, MAXLEN);

	  	memcpy(aux, argv[1], strlen(argv[1]));
	  	size = createQuery(aux, argv[2], sendbuf);

	  	sendto(sockfd, sendbuf, size, 0, (struct sockaddr*)&servaddr, sizeof(servaddr));

	  	//primesc raspunsul si il procesez
	  	memset(recvbuf, 0, MAXLEN);
	  	size = recvfrom(sockfd, recvbuf, MAXLEN, 0, (struct sockaddr*)&servaddr, &x);

	  	if(size > 0) processMessage(recvbuf, server_ip, argv[1], argv[2], dns_servers);
	  	
	  	getServerIP(dns_servers, server_ip);

	  	close(sockfd);
    }

    return 0;
}