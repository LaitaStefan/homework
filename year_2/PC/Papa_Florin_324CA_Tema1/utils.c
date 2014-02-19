//Papa Florin, 324CA

#include "utils.h"


int max(int a, int b) {
	if(a > b) return a;
	return b;
}

void sort2(pkt2* p, int size) {
	int i, j, l1, min;
	for(i=0;i<size-1;i++) {
		memcpy(&min,p[i].payload,4);
		for(j = i+1;j<size;j++) {
			memcpy(&l1,p[j].payload,4);
			if(l1 < min) {
				min = l1;
				pkt2 aux = p[i];
				p[i] = p[j];
				p[j] = aux;
			}
		}
	}
}

void sort(pkt* p, int size) {
	int i, j;
	for(i=0;i<size-1;i++) {
		int min = p[i].nr_seq;
		for(j = i+1;j<size;j++) {
			if(p[j].nr_seq < min) {
				min = p[j].nr_seq;
				pkt aux = p[i];
				p[i] = p[j];
				p[j] = aux;
			}
		}
	}
}

char getBit(char nr,int poz) {
  return (nr>>(poz-1)) & 1;
}

void setBit(char *buff, int nr_bit, char val) {
	char c = 1;
	c = c << (nr_bit - 1);
  	(*buff) = (*buff) | (c*val);
}

char get_parity(char* load, int size) {
	int i, j;
	char rez=0;
	char *c = (char*)malloc(8*sizeof(char));
	memset(c,0,8);
	for(i=0;i<size;i++) {
		for(j=1;j<=8;j++) {
			c[j-1] = c[j-1] ^ getBit(load[i],j);
		}
	}
	for(i=0;i<8;i++) {
		setBit(&rez,i+1,c[i]);
	}

	return rez;
}

void task_0_recv() {

	char *filename, *aux;
	msg r;
	int fd;

	recv_message(&r);

	aux = (char*)malloc(r.len*sizeof(char));
	memcpy(aux,r.payload,r.len);
	filename = (char*)malloc((r.len+5)*sizeof(char));
	strcpy(filename,"recv_");
	strcat(filename,aux);
	fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0666);

	while(1) {
		recv_message(&r);	
		if(r.len > 0) write(fd,r.payload,r.len);
		send_message(&r);
		if(r.len < MSGSIZE) break;
	}

	close(fd);
	free(aux);
	free(filename);
}

void task_1_recv() {
	int current=0, error, fd;
	pkt p;
	msg r,t;
	char *aux,*filename;
	while(1) {
		error = recv_message(&r);
		if(error == -1) continue;
		else {
			memcpy(&p,r.payload,MSGSIZE);
			if(p.nr_seq == current) {
				if(current == 0) {
					aux = (char*)malloc(r.len*sizeof(char));
					memcpy(&p,r.payload,MSGSIZE);
					memcpy(aux,p.payload,r.len);
					filename = (char*)malloc((r.len+5)*sizeof(char));
					strcpy(filename,"recv_");
					strcat(filename,aux);
					fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0666);
				}
				else {
					if(r.len > 0) write(fd,p.payload,r.len);
					if(r.len<PKTSIZE) {
						p.type = ACK;
						memcpy(t.payload,&p,MSGSIZE);
						send_message(&t);
						break;
					}	
				}	

				current++;
			}
		}
		p.type = ACK;
		memcpy(t.payload,&p,MSGSIZE);
		send_message(&t);
	}	
	close(fd);
	free(aux);
	free(filename);
}

void task_2_recv() {
	int error, dim = 0, i, iter = -1, fd;
	pkt *recv = (pkt*)malloc(100*sizeof(pkt)),p;
	msg r, t;
	int first, last = 0, len;
	char *aux, *filename;
	
	while(1) {
		error = recv_message(&r);
		if(error == -1) continue;
		else {
			memcpy(&p, r.payload, MSGSIZE);
			recv[dim] = p;
			if(p.nr_seq == 0) {
				first = r.len;
				aux = (char*)malloc(first*sizeof(char));
				memcpy(aux,p.payload,first);
				memcpy(&iter,p.payload+first,4);
			}	
			dim++;
			if(dim % 100 == 0 && dim != 0) {
				recv = realloc(recv, (dim+100)*sizeof(pkt));
			}
			p.type = ACK;
			memcpy(t.payload,&p,MSGSIZE);
			send_message(&t);
			if(r.len < PKTSIZE && p.nr_seq != 0) last = r.len;
		}
		if(dim > iter && iter != -1) break;
	}
	sort(recv,dim);
	filename = (char*)malloc((first+5)*sizeof(char));
	strcpy(filename,"recv_");
	strcat(filename,aux);
	fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0666);

	for(i=1;i<dim;i++) {
		len = PKTSIZE;
		if(i==dim-1) len = last;
		if(len > 0 ) write(fd,recv[i].payload,len);
	}
	close(fd);
	free(recv);
	free(filename);
	free(aux);
}	

void task_3_recv() {
	msg t, r;
	int error, dim = 0, i, iter = -1,fd;
	pkt2 *recv = (pkt2*)malloc(100*sizeof(pkt2));
	int first,last = 0,len,nr_seq;
	char parity, *aux, *filename;
	pkt2 pk;
	
	while(1) {
		error = recv_message(&r);
		if(error == -1) continue;
		else {
			memcpy(&pk,r.payload,MSGSIZE);
			parity = get_parity(pk.payload,r.len+4);
			if(parity == pk.sum) {
				recv[dim] = pk;
				memcpy(&nr_seq,pk.payload,4);
				if(nr_seq == 0) {
					first = r.len;
					aux = (char*)malloc(first*sizeof(char));
					memcpy(aux,pk.payload+4,first);
					memcpy(&iter,pk.payload+first+4,4);
				}	
				dim++;
				if(dim % 100 == 0 && dim != 0) {
					recv = realloc(recv, (dim+100)*sizeof(pkt2));
				}
				pk.sum = ACK;
				memcpy(t.payload,&pk,MSGSIZE);
				send_message(&t);
				if(r.len < PKTSIZE && nr_seq != 0) last = r.len;
			}
		}
		if(dim > iter && iter != -1) break;
	}

	sort2(recv,dim);
	filename = (char*)malloc((first+5)*sizeof(char));
	strcpy(filename,"recv_");
	strcat(filename,aux);
	fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0666);

	for(i=1;i<dim;i++) {
		len = PKTSIZE;
		if(i==dim-1) len = last;
		if(len > 0) write(fd,recv[i].payload+4,len);
	}

	close(fd);
	free(recv);
	free(filename);
	free(aux);	
}

void task_0_send(int size, int window_size, int fd, char* filename) {
	
	msg t;
	int i, complete_chunks, last_size;

	complete_chunks = 1 + size/MSGSIZE;
	last_size = size - (complete_chunks - 1)*MSGSIZE;
	if(last_size == 0) last_size--; //nu putem trimite cu t.len = 0

	memcpy(t.payload,filename,strlen(filename));
	t.len = strlen(filename);
	send_message(&t);
	
	for(i=0; i<window_size-1 && complete_chunks > 0; i++) {
		read(fd,t.payload,MSGSIZE);	
		t.len = MSGSIZE;
		if(complete_chunks == 1) t.len = last_size;
		send_message(&t);
		complete_chunks--;	
	}
	
	while(complete_chunks > 0) {
		recv_message(&t);
		read(fd,t.payload,MSGSIZE);	
		t.len = MSGSIZE;
		if(complete_chunks == 1) t.len = last_size;
		send_message(&t);
		complete_chunks--;	
	}

	close(fd);
}

void task_1_send(int size, int window_size, char* filename, int delay, int fd) {
	int dim, current = 0, wanted_ack = 0, err, in_progress=0;
	int complete_chunks, ignore, exception, i;
	pkt *packets, p;
	msg r, t;

	complete_chunks	= size/PKTSIZE;
	if(size == PKTSIZE*complete_chunks) exception = 1;

	dim = complete_chunks+1;
	packets = (pkt*)malloc((dim+1)*sizeof(pkt));

	memcpy(packets[0].payload,filename,strlen(filename));
	for(i=1;i<=dim-1;i++) {
		read(fd,packets[i].payload,PKTSIZE);
		packets[i].nr_seq = i;
	}

	if(exception == 1) {
		read(fd,packets[dim].payload,PKTSIZE);
		packets[dim].nr_seq = dim;
	}
		
	else {
		read(fd,packets[dim].payload,size - ((complete_chunks)*PKTSIZE));
		packets[dim].nr_seq = dim;
	}	

	for(i=0;i<window_size;i++) {

		t.len = PKTSIZE;
		if(i==0) t.len = strlen(filename);
		else {
			if(i==dim) {
			 	if(exception==0) t.len = size - ((complete_chunks)*PKTSIZE);
			 	if(exception==1) t.len = 0;
			}	
		}

		memcpy(t.payload, packets+i, MSGSIZE);
		send_message(&t);
		current++;
		in_progress++;
		
		if(i==dim) break;
	}
	ignore = 0;
	while(wanted_ack <= dim) {
		in_progress--;
		err = recv_message_timeout(&r, max(2*delay,1000));
		memcpy(&p,r.payload,MSGSIZE);
		if(err == -1 || (p.nr_seq != wanted_ack && ignore==0)) { 
			ignore = in_progress;
			for(i=wanted_ack;i<wanted_ack+window_size;i++) {
				if(i>dim) break;

				t.len = PKTSIZE;
				if(i==0) t.len = strlen(filename);
				else {
					if(i==dim) {
			 			if(exception==0) t.len = size - ((complete_chunks)*PKTSIZE);
			 			if(exception==1) t.len = -1;
					}	
				}		
				memcpy(t.payload, packets+i, MSGSIZE);
				send_message(&t);
				in_progress++;
			}
			current = i;
			continue;
		}

		if(p.nr_seq == wanted_ack) {			
			if(current <= dim) {
				
				t.len = PKTSIZE;
				if(current==0) t.len = strlen(filename);
				else {
					if(current==dim) {
					 	if(exception==0) t.len = size - ((complete_chunks)*PKTSIZE);
					 	if(exception==1) t.len = -1;
					}	
				}
				memcpy(t.payload, packets+current, MSGSIZE);
				send_message(&t);
				current++;
				in_progress--;
				if(ignore>0) ignore=0;
			}
			wanted_ack++;
		}
		if(ignore > 0) ignore--;
	}
	free(packets);
	close(fd);
}		

void task_2_send(int size, int window_size, char* filename, int delay, int fd) {
	int dim, wanted_ack = 0, current = 0, err, cursor=0;
	int exception, i, complete_chunks;
	char *acks;
	pkt *packets,p;
	msg r, t;

	complete_chunks	= size/PKTSIZE;
	if(size == PKTSIZE*complete_chunks) exception = 1;

	dim = complete_chunks+1;
	packets = (pkt*)malloc((dim+1)*sizeof(pkt));
	acks = (char*)malloc((dim+1)*sizeof(char));
	memset(acks,-1,dim+1);


	memcpy(packets[0].payload,filename,strlen(filename));
	memcpy(packets[0].payload+strlen(filename),&dim,4);
	for(i=1;i<=dim-1;i++) {
		read(fd,packets[i].payload,PKTSIZE);
		packets[i].nr_seq = i;
	}

	if(exception == 1) {
		read(fd,packets[dim].payload,PKTSIZE);
		packets[dim].nr_seq = dim;
	}	
	else {
		read(fd,packets[dim].payload,size - ((complete_chunks)*PKTSIZE));
		packets[dim].nr_seq = dim;
	}	

	for(i=0;i<window_size;i++) {

		t.len = PKTSIZE;
		if(i==0) t.len = strlen(filename);
		else {
			if(i==dim) {
			 	if(exception==0) t.len = size - ((complete_chunks)*PKTSIZE);
			 	if(exception==1) t.len = -1;
			}	
		}
		memcpy(t.payload, packets+i, MSGSIZE);
		send_message(&t);
		current++;
		if(i==dim) break;
	}
	while(wanted_ack <= dim) {

		err = recv_message_timeout(&r, max(2*delay,1000));
		memcpy(&p,r.payload,MSGSIZE);
		if(err == -1) {
			cursor = wanted_ack;
			for(i=0;i<window_size;i++) {
				if(cursor > dim) break;
				packets[cursor].type = DATA;
				packets[cursor].nr_seq = cursor;
				if(cursor==0) t.len = strlen(filename);
				else {
					if(cursor==dim) {
			 			if(exception==0) t.len = size - ((complete_chunks)*PKTSIZE);
			 			if(exception==1) t.len = -1;
					}	
				}
				memcpy(t.payload, packets+cursor, MSGSIZE);
				send_message(&t);
				cursor++;
				while(acks[cursor] == 1) cursor++;
			}
			if(cursor>current) current = cursor;	
			continue;
		}	
		else {
			acks[p.nr_seq] = 1;
			if(current <= dim) {
				packets[current].type = DATA;
				packets[current].nr_seq = current;
				if(current==0) t.len = strlen(filename);
				else {
					if(current==dim) {
				 		if(exception==0) t.len = size - ((complete_chunks)*PKTSIZE);
				 		if(exception==1) t.len = -1;
					}	
				}
				memcpy(t.payload, packets+current, MSGSIZE);
				send_message(&t);
				current++;
			}	
			while(acks[wanted_ack] == 1) { 
				wanted_ack++; 
				if(wanted_ack>dim) break; 
			}
			if(wanted_ack > dim) break;
		}
	}
	free(packets);
	free(acks);
	close(fd);
}

void task_3_send(int size, int window_size, char* filename, int delay, int fd) {
	msg r, t;
	int dim, wanted_ack = 0, current = 0, err, cursor=0, nr_seq;
	int complete_chunks, i, exception;
	char *acks;
	pkt2 *packets;
	pkt2 p;

	complete_chunks	= size/PKTSIZE;
	if(size == PKTSIZE*complete_chunks) exception = 1;

	dim = complete_chunks+1;
	packets = (pkt2*)malloc((dim+1)*sizeof(pkt2));

	acks = (char*)malloc((dim+1)*sizeof(char));
	memset(acks,-1,dim+1);


	memcpy(packets[0].payload+4,filename,strlen(filename));
	memcpy(packets[0].payload+strlen(filename)+4,&dim,4);
	for(i=1;i<=dim-1;i++) {
		read(fd,packets[i].payload+4,PKTSIZE);
	}

	if(exception == 1) read(fd,packets[dim].payload+4,PKTSIZE);
	else read(fd,packets[dim].payload+4,size - ((complete_chunks)*PKTSIZE));

	for(i=0;i<window_size;i++) {
		memcpy(packets[i].payload,&i,4);
		t.len = PKTSIZE;
		if(i==0) t.len = strlen(filename);
		else {
			if(i==dim) {
			 	if(exception==0) t.len = size - ((complete_chunks)*PKTSIZE);
			 	if(exception==1) t.len = 0;
			}	
		}
		packets[i].sum = get_parity(packets[i].payload, t.len+4);
		memcpy(t.payload, packets+i, MSGSIZE);			
		send_message(&t);
		current++;
		if(i==dim) break;
	}

	while(wanted_ack <= dim) {

		err = recv_message_timeout(&r, max(2*delay,1000));
		if(err == -1) {
			cursor = wanted_ack;
			for(i=0;i<window_size;i++) {
				if(cursor > dim) break;
				memcpy(packets[cursor].payload,&cursor,4);
				if(cursor==0) t.len = strlen(filename);
				else {
					if(cursor==dim) {
			 			if(exception==0) t.len = size - ((complete_chunks)*PKTSIZE);
			 			if(exception==1) t.len = 0;
					}	
				}
				packets[cursor].sum = get_parity(packets[cursor].payload, t.len+4);
				memcpy(t.payload, packets+cursor, MSGSIZE);
				send_message(&t);
				cursor++;
				while(acks[cursor] == 1) { 
					cursor++; 
					if(cursor>dim) break;
				}
			}
			if(cursor>current) current = cursor;	
			continue;
		}	
		else {
			memcpy(&p,r.payload,MSGSIZE);
			memcpy(&nr_seq,p.payload,4);
			acks[nr_seq] = 1;
			if(current <= dim) {
				memcpy(packets[current].payload,&current,4);
				if(current==0) t.len = strlen(filename);
				else {
					if(current==dim) {
				 		if(exception==0) t.len = size - ((complete_chunks)*PKTSIZE);
				 		if(exception==1) t.len = 0;
					}	
				}
				packets[current].sum = get_parity(packets[current].payload, t.len+4);
				memcpy(t.payload, packets+current, MSGSIZE);
				send_message(&t);
				current++;
			}	

			while(acks[wanted_ack] == 1) {
				wanted_ack++; 
				if(wanted_ack > dim) break; 
			}

			if(wanted_ack > dim) break;
		}
	}
	free(packets);
	free(acks);
	close(fd);
}



