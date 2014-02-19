//Papa Florin, 324CA

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "helpers.h"

//structura in care tin vecinii unui nod
typedef struct {

	int neighbors[KIDS];

} packet;

int out, in, nod_id, seq_no = 0;
int timp = -1;
int gata = FALSE;

//nu modificati numele, modalitatea de alocare si initializare a tabelei de rutare 
//se foloseste la mesajele de tip 8/10 (deja implementate aici) si la logare (implementata in simulator.c)
int tab_rutare [KIDS][2];
msg LSADatabase[KIDS];
int topologie[KIDS][KIDS];

void update_topologie() {
	int i,j;
	packet neighbors, aux_neigh;
	
	if(nod_id == 4) {
 	for(i=0;i<KIDS;i++) printf("%d ",topologie[nod_id][i]);
 	printf("\n");	
	 }
	for(i = 0; i < KIDS; i++) {

		memcpy(&neighbors, LSADatabase[i].payload, sizeof(packet));
		for(j = i; j < KIDS; j++) {
			if(LSADatabase[i].time >= LSADatabase[j].time) { 
				topologie[i][j] = neighbors.neighbors[j];
				topologie[j][i] = neighbors.neighbors[j];
			}
			else {
				memcpy(&aux_neigh, LSADatabase[j].payload, sizeof(packet));
				topologie[i][j] = aux_neigh.neighbors[i];
				topologie[j][i] = aux_neigh.neighbors[i];
			}
		}
	}
}

void calcul_tabela_rutare() { //calcul tabela de rutare folosind algoritmul lui Dijkstra
	int S[KIDS], V[KIDS], D[KIDS];
	int i, j, k;

	for (i=0; i<KIDS; i++)
	{
		S[i] = 0;
		D[i] = topologie[nod_id][i];
		if (D[i] < DRUMAX)	V[i] = i;
		else V[i] = -1;
	}

	S[nod_id] = 1;
	D[nod_id] = 0;

	for (k = 0; k < KIDS; k++)
	{
		int min = DRUMAX + 1;
		for (j = 0; j < KIDS; j++)
		{
			if (S[j] == 0 && D[j] < min)
			{
				k=j;
				min=D[j];
			}
		}

		S[k] = 1;
		for (j=0; j<KIDS; j++) {
			if ((S[j] == 0) && (D[k] + topologie[k][j] <= D[j]))
			{
				if((D[k] + topologie[k][j] == D[j]) && V[k] > V[j]) continue; //nu iau in considerare cai de cost
				D[j] = D[k] + topologie[k][j];								  //egal, dar cu next-hop mai mare	
				V[j] = V[k];
			}
		}
	}
	for (i=0; i<KIDS; i++)
	{
		tab_rutare[i][0] = D[i];
		tab_rutare[i][1] = V[i];
	}
}

void procesare_eveniment(msg mevent) {

	if (mevent.add == TRUE) {
		printf ("Nod %d, msg tip eveniment - am aderat la topologie la pasul %d\n", nod_id, timp); 
	}
	else
		printf ("Timp %d, Nod %d, procesare eveniment\n", timp, nod_id);

	//aveti de implementat tratarea evenimentelor si trimiterea mesajelor ce tin de protocolul de rutare
	//in campul payload al mesajului de tip 7 e linia de fisier corespunzatoare respectivului eveniment
	//(optional) vezi simulator.c, liniile 93-119 (trimitere mes tip 7) si liniile 209-219 (parsare fisier evenimente) 
					 
	//rutere direct implicate in evenimente, care vor primi mesaje de tip 7 de la simulatorul central:
	//eveniment tip 1: ruterul nou adaugat la retea  (ev.d1  - vezi liniile indicate)
	//eveniment tip 2: capetele noului link (ev.d1 si ev.d2)
	//eveniment tip 3: capetele linkului suprimat (ev.d1 si ev.d2)
	//evenimet tip 4:  ruterul sursa al pachetului (ev.d1)
	int i;
	msg message;
	int type = atoi(strtok(mevent.payload, " "));

	if(type == 1) {	//trimit database request
		nod_id = atoi(strtok(NULL, " ")); //nu stiu daca e bine
		int nr_vec = atoi(strtok(NULL, " "));

		message.type = 2;
		message.creator = nod_id;
		message.sender = nod_id;
		message.time = timp;
		message.seq_no = seq_no;
		seq_no++;

		for(i = 0; i < nr_vec; i++) {
			
			message.next_hop = atoi(strtok(NULL, " "));
			int cost = atoi(strtok(NULL, " "));
			memcpy(message.payload, &cost, sizeof(int));

			write (out, &message, sizeof(msg)); 
		}
	}

	if(type == 2) { //aparitia unui nou link
		int id_1 = atoi(strtok(NULL, " "));
		int id_2 = atoi(strtok(NULL, " "));
		int cost = atoi(strtok(NULL, " "));

		//actualizare topologie
		topologie[id_1][id_2] = cost;
		topologie[id_2][id_1] = cost;

		if(nod_id == id_1) {
			message.type = 2;
			message.creator = nod_id;
			message.sender = nod_id;
			message.time = timp;

			message.next_hop = id_2;
			memcpy(message.payload, &cost, sizeof(int));

			message.seq_no = seq_no;
			seq_no++;
			write (out, &message, sizeof(msg));
		}
		else if(nod_id == id_2) {
			message.type = 2;
			message.creator = nod_id;
			message.sender = nod_id;
			message.time = timp;

			message.next_hop = id_1;
			memcpy(message.payload, &cost, sizeof(int));

			message.seq_no = seq_no;
			seq_no++;
			write (out, &message, sizeof(msg));
		}		
	}

	if(type == 3) {
		int id_1 = atoi(strtok(NULL, " "));
		int id_2 = atoi(strtok(NULL, " "));
		packet p;

		topologie[id_1][id_2] = DRUMAX;
		topologie[id_2][id_1] = DRUMAX;

		message.type = 1;
		message.creator = nod_id;
		message.sender = nod_id;
		message.time = timp;
		message.seq_no = seq_no;
		seq_no++;

		memcpy(p.neighbors, topologie[nod_id], KIDS*sizeof(int));
		memcpy(message.payload, &p, sizeof(packet));

		for(i = 0; i < KIDS; i++) {
			if(topologie[nod_id][i] < DRUMAX && i != nod_id) {
				message.next_hop = i;
				write (out, &message, sizeof(msg));
			}
		}
	}

	if(type == 4) {
		int id_sursa = atoi(strtok(NULL, " "));
		int id_dest = atoi(strtok(NULL, " "));

		message.type = 4;
		message.creator = id_sursa;
		message.sender = id_sursa;
		if(tab_rutare[id_dest][1] != -1) {
			message.next_hop = tab_rutare[id_dest][1];
			memcpy(message.payload, &id_dest, sizeof(int));
			message.seq_no = seq_no;
			seq_no++;
			message.time = timp;

			write (out, &message, sizeof(msg));
		}
	}	

}

int main (int argc, char ** argv)
{
	msg mesaj, mesaj_event;
	msg old_queue[BUFSIZE], new_queue[BUFSIZE];	//cozile de mesaje
	int o_q_size, n_q_size;						//dimensiunile cozilor
	int cit, k, i, j;
	int event = FALSE;
	packet p;
	
	out = atoi(argv[1]);  //legatura pe care se trimit mesaje catre simulatorul central (toate mesajele se trimit pe aici)
	in = atoi(argv[2]);   //legatura de pe care se citesc mesajele

	nod_id = atoi(argv[3]); //procesul curent participa la simulare numai dupa ce nodul cu id-ul lui este adaugat in topologie 
				
	//tab_rutare[k][0] reprezinta costul drumului minim de la ruterul curent (nod_id) la ruterul k 
	//tab_rutare[k][1] reprezinta next_hop pe drumul minim de la ruterul curent (nod_id) la ruterul k 							
	for (k = 0; k < KIDS; k++) {
		tab_rutare[k][0] = DRUMAX;  // drum =DRUMAX daca ruterul k nu e in retea sau informatiile despre el nu au ajuns la ruterul curent
		tab_rutare[k][1] = -1; //in cadrul protocolului(pe care il veti implementa), next_hop =-1 inseamna ca ruterul k nu e (inca) cunoscut de ruterul nod_id (vezi mai sus) 
	}

	if(nod_id == 0) {
		tab_rutare[0][0] = 0;
		tab_rutare[0][1] = -1;
	}

	for(i = 0; i < KIDS; i++) {
		p.neighbors[i] = DRUMAX;
	}	

	//initializare topologie
	for(k = 0; k < KIDS; k++) {
		for(j = 0; j < KIDS; j++) {
			topologie[k][j] = DRUMAX;
		}
		LSADatabase[k].creator = k;
		LSADatabase[k].time = -1;		//este -1 cand nu s-a primit nimic de la routerul cu indicele k
		LSADatabase[k].seq_no= -1;
		memcpy(LSADatabase[k].payload, &p, sizeof(packet));
	}
																
	printf ("Nod %d, pid %u alive & kicking\n", nod_id, getpid());

	if (nod_id == 0) { //sunt deja in topologie
		timp = -1; //la momentul 0 are loc primul eveniment
		mesaj.type = 5; //finish procesare mesaje timp -1
		mesaj.sender = nod_id;
		write (out, &mesaj, sizeof(msg)); 
		printf ("TRIMIS Timp %d, Nod %d, msg tip 5 - terminare procesare mesaje vechi din coada\n", timp, nod_id);
	
	}

	//initializare dimensiuni cozi de mesaje
	o_q_size = 0;
	n_q_size = 0;

	while (!gata) {
		cit = read(in, &mesaj, sizeof(msg));
		if (cit <= 0) {
			printf ("Adio, lume cruda. Timp %d, Nod %d, msg tip %d cit %d\n", timp, nod_id, mesaj.type, cit);
			exit (-1);
		}
		
		switch (mesaj.type) {
			
			//1,2,3,4 sunt mesaje din protocolul link state; 
			//actiunea imediata corecta la primirea unui pachet de tip 1,2,3,4 este buffer-area 
			//(punerea in coada /coada new daca sunt 2 cozi - vezi enunt)
			
			case 1:
				{
					new_queue[n_q_size] = mesaj;
					n_q_size++;

					printf ("Timp %d, Nod %d, msg tip 1 - LSA\n", timp, nod_id);
				}

				break;
				
			case 2:
				{
					new_queue[n_q_size] = mesaj;
					n_q_size++;

					printf ("Timp %d, Nod %d, msg tip 2 - Database Request\n", timp, nod_id);
				}
				break;
				
			case 3:
				{
					new_queue[n_q_size] = mesaj;
					n_q_size++;

					printf ("Timp %d, Nod %d, msg tip 3 - Database Reply\n", timp, nod_id);
				}
				break;
				
			case 4: 
				{
					new_queue[n_q_size] = mesaj;
					n_q_size++;

					printf ("Timp %d, Nod %d, msg tip 4 - pachet de date (de rutat)\n", timp, nod_id);
				}
				break; 
			
			case 6:
				{
					printf ("Timp %d, Nod %d, msg tip 6 - incepe procesarea mesajelor puse din coada la timpul anterior (%d)\n", timp, nod_id, timp-1);
					for(i = 0; i < n_q_size; i++) {
					 	old_queue[i] = new_queue[i];
					 }				
					
					 o_q_size = n_q_size;
					 n_q_size = 0;

					for(i = 0; i < o_q_size; i++) {

						if(old_queue[i].type == 1) { //primesc LSA
							int index = old_queue[i].creator;

							if(old_queue[i].time > LSADatabase[index].time || LSADatabase[index].time == -1) {

								memcpy(&LSADatabase[index], &old_queue[i], sizeof(msg)); //actualizare LSADatabase
								
								update_topologie();		//actualizare topologie
								
								old_queue[i].sender = nod_id;
								for(j = 0; j < KIDS; j++) {
									if(topologie[nod_id][j] < DRUMAX && j != index && j != nod_id) {
										old_queue[i].next_hop = j;
										write (out, &old_queue[i], sizeof(msg));
									}
								}
							}
						}

						if(old_queue[i].type == 2) {//primesc database request
							int cost, j;
							msg message;
							packet p;

							memcpy(&cost, old_queue[i].payload, sizeof(int));

							topologie[old_queue[i].creator][nod_id] = cost;
							topologie[nod_id][old_queue[i].creator] = cost;

							message.type = 3;
							message.sender = nod_id;
							message.next_hop = old_queue[i].creator;
							message.time = timp;
							message.seq_no = seq_no;
							seq_no++;

							//trimit database reply
							for(j = 0; j < KIDS; j++) {
								if(LSADatabase[j].time >= 0) {
									message.creator = j;
									memcpy(message.payload, LSADatabase[j].payload, 1400);
									write (out, &message, sizeof(msg));
								}
							}

							//trimit LSA vecinilor
							for(j = 0; j < KIDS; j++) {
								p.neighbors[j] = topologie[nod_id][j];
							}

							message.type = 1;
							message.creator = nod_id;
							message.sender = nod_id;
							message.time = timp;
							memcpy(message.payload, &p, sizeof(packet));

							memcpy(&LSADatabase[nod_id], &message, sizeof(msg));

							for(j = 0; j < KIDS; j++) {
								if(topologie[nod_id][j] < DRUMAX && j != nod_id) {
									message.next_hop = j;
									write (out, &message, sizeof(msg));
								}
							}
						}

						if(old_queue[i].type == 3) { //primesc database reply
							int index = old_queue[i].creator;
							int v[KIDS];
							memcpy(v,old_queue[i].payload, KIDS*sizeof(int));
							
							if(old_queue[i].time > LSADatabase[index].time || LSADatabase[index].time == -1) {

								memcpy(LSADatabase[index].payload, old_queue[i].payload, 1400); //actualizare LSADatabase

								update_topologie();		//actualizare topologie
								
							}
						}

						if(old_queue[i].type == 4) {
							int dest;

							memcpy(&dest, old_queue[i].payload, sizeof(int));

							old_queue[i].sender = nod_id;
							old_queue[i].time = timp;

							//fac forward la mesaj, nu trimit mai departe daca eu sunt destinatarul
							if(tab_rutare[dest][1] != -1 && dest != nod_id) {
								old_queue[i].next_hop = tab_rutare[dest][1];
								write (out, &old_queue[i], sizeof(msg));
							}
						}
					}
					o_q_size = 0;
					
					//procesez mesaj eveniment
					if (event) {
						procesare_eveniment(mesaj_event);
						event = FALSE;
					}	
					//calculez tabela de rutare
					calcul_tabela_rutare();
					
					//nu mai sunt mesaje vechi, am procesat evenimentul(daca exista), am calculat tabela de rutare(in aceasta ordine)
					//trimit mesaj de tip 5 (terminare pas de timp)
					mesaj.type = 5; 
					mesaj.sender = nod_id;
					write (out, &mesaj, sizeof(msg)); 

				}
				break;
			
			case 7: //complet implementat - nu modificati! (exceptie afisari on/off)
					//aici se trateaza numai salvarea mesajului de tip eveniment(acest mesaj nu se pune in coada), pentru a fi procesat la finalul acestui pas de timp
					//procesarea o veti implementa in functia procesare_eveniment(), apelata in case 6
				{
				event = TRUE;
				memcpy (&mesaj_event, &mesaj, sizeof(msg));
				
				if (mesaj.add == TRUE) 
					timp = mesaj.time+1; //initializam timpul pentru un ruter nou 
				}
				break;
			
			case 8: //complet implementat - nu modificati! (exceptie afisari on/off)
					//aici doar se apeleaza calcul_tabela_rutare() trebuie sa completati voi implementarea in corpul functiei
				{
				//printf ("Timp %d, Nod %d, msg tip 8 - cerere tabela de rutare\n", timp, nod_id);
				
				mesaj.type = 10;  //trimitere tabela de rutare
				mesaj.sender = nod_id;
				mesaj.time = timp;
				memcpy (mesaj.payload, &tab_rutare, sizeof (tab_rutare));
				//Observati ca acest tip de mesaj (8) se proceseaza imediat - nu se pune in nicio coada (vezi enunt)
				write (out, &mesaj, sizeof(msg)); 
				timp++;
				}
				break;
				
			case 9: //complet implementat - nu modificati! (exceptie afisari on/off)
				{
				//Aici poate sa apara timp -1 la unele "noduri"
				//E ok, e vorba de procesele care nu reprezentau rutere in retea, deci nu au de unde sa ia valoarea corecta de timp
				//Alternativa ar fi fost ca procesele neparticipante la simularea propriu-zisa sa ramana blocate intr-un apel de read()
				printf ("Timp %d, Nod %d, msg tip 9 - terminare simulare\n", timp, nod_id);
				gata = TRUE;
				}
				break;
				

			default:
				printf ("\nEROARE: Timp %d, Nod %d, msg tip %d - NU PROCESEZ ACEST TIP DE MESAJ\n", timp, nod_id, mesaj.type);
				exit (-1);
		}			
	}

	return 0;

}
