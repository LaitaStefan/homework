//Papa Florin, 334CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

#define N 10

int adiacenta[50][50], *V;
int parent = -1, leader, second;

//numarul de vecini al unui nod
int get_neighbour(int neigh[], int dim) {
    int i, count = 0;
    for(i = 0; i < dim; i++) {
        if(neigh[i] == 1) count++;
    }
    return count;
}

//stabilirea topologiei pentru un graf
void stabilire_topologie(int rank, int size, char *file) {
    int dim, aux, neigh;
    int recv_top[50][50], nil[50][50];
    int info, nr_ecouri;
    char line[500], *tok;
    MPI_Status status;
    int i, j, k, source;
    FILE *top, *messages;

    memset(nil, 0, 50*50*sizeof(int));

    top = fopen(file, "r");
    for(i = 0; i <= rank; i++) {
        fgets(line, 500, top);
        if(i == rank) {
            tok = strtok(line, "- \n\0");            
            while(tok != NULL) {
                tok = strtok(NULL, "- \n\0");
                if(tok == NULL) break;

                neigh = atoi(tok);
                adiacenta[rank][neigh] = 1;    
            }
        }
    }
    fclose(top);

    //stabilire topologie
    if(rank == 0) {
        for(i = 0; i < 50; i++) {
            if(adiacenta[rank][i] == 1) {
                recv_top[0][0] = -1;    //sonda
                MPI_Send(recv_top, 50*50, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }

        for(i = 0; i < 50; i++) {
            if(adiacenta[rank][i] == 1) {
                MPI_Recv(recv_top, 50*50, MPI_INT, i, 0, 
                    MPI_COMM_WORLD, &status);

                for(j = 0; j < 50; j++) {
                    for(k = 0; k < 50; k++) {
                        if(recv_top[j][k] == 1) {
                            adiacenta[j][k] = recv_top[j][k];
                        }
                    }
                }
            }
        }
    }
    else {
        sleep(1);
        MPI_Recv(recv_top, 50*50, MPI_INT, MPI_ANY_SOURCE, 0, 
            MPI_COMM_WORLD, &status);
        parent = status.MPI_SOURCE;

        for(i = 0; i < 50; i++) {
            if(adiacenta[rank][i] == 1 && i != parent) {
                recv_top[0][0] = -1;    //sonda
                MPI_Send(recv_top, 50*50, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }

        nr_ecouri = get_neighbour(adiacenta[rank], 50) - 1;

        while(nr_ecouri > 0) {
            MPI_Recv(recv_top, 50*50, MPI_INT, MPI_ANY_SOURCE, 0, 
                MPI_COMM_WORLD, &status);
            source = status.MPI_SOURCE;
            
            if(recv_top[0][0] == -1) { //sonda
                //trimit ecou nul
                MPI_Send(nil, 50*50, MPI_INT, source, 0, MPI_COMM_WORLD);
            }
            else { //ecou
                
                for(i = 0; i < 50; i++) {
                    for(j = 0; j < 50; j++) {
                        if(recv_top[i][j] == 1) {
                            adiacenta[i][j] = recv_top[i][j];
                        }
                    }
                }
                nr_ecouri--;
            }
        }

        MPI_Send(adiacenta, 50*50, MPI_INT, parent, 0, MPI_COMM_WORLD);
    }


}

//transmiterea topologiei tuturor nodurilor
void difuzare_topologie(int rank, int size) {
    int flag = 0;
    int recv_top[100][100];
    MPI_Status status;
    MPI_Request request;
    int parent = -1, i, j;

    //stabilire topologie
    if(rank == 0) {
        for(i = 0; i < 50; i++) {
            if(adiacenta[rank][i] == 1) {
                MPI_Send(adiacenta, 50*50, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }
    }
    else {
        MPI_Recv(adiacenta, 50*50, MPI_INT, MPI_ANY_SOURCE, 0, 
            MPI_COMM_WORLD, &status);
        parent = status.MPI_SOURCE;
 
        for(i = 0; i < size; i++) {
            if(adiacenta[rank][i] == 1 && i != parent) {
                MPI_Send(adiacenta, 50*50, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }

        MPI_Irecv(recv_top, 50*50, MPI_INT, MPI_ANY_SOURCE, 0, 
            MPI_COMM_WORLD, &request);
        while(1) {
            sleep(1);
            MPI_Test(&request, &flag, &status);
            if(!flag || status.MPI_SOURCE < 0) {
               break;
            }
        }
    }
}

//construirea tabelei de rutare
void dijkstra(int rank, int size) {
    int D[size], S[size];
    int i, j, k, min, count = 0;

    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++) {
            if(i == j) {
                adiacenta[i][j] = 0;
            }
            else if(adiacenta[i][j] == 0) {
                adiacenta[i][j] = 1000;
            }
        }
    }

    for (i = 0; i < size; i++) {
        S[i] = 0;               // nod neselectat
        D[i] = adiacenta[rank][i]; // distantele minime de la sursa
        if (D[i] < 1000) V[i] = i; // initializeaza vecinii
        else V[i] = 0;
    }

    S[rank] = 1; // selecteaza nodul sursa
    D[rank] = 0;

    for (i = 0; i < size - 1; i++) {
        // gaseste nodul k neselectat cu D[k] minim;
        min = 1000, k = -1;
        for(j = 0; j < size; j++) {
            if(S[j] == 0 && D[j] < min) {
                min = D[j];
                k = j;
            }
        }

        S[k] = 1;
        for (j = 0; j < size; j++) {    // recalculeaza distantele
            if ((S[j] == 0) && (D[k] + adiacenta[k][j] < D[j])) {
                D[j] = D[k] + adiacenta[k][j];
                V[j] = V[k]; // modifica tabela de dirijare
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    
    while(count < size) {
        if(rank == count) {
            printf("%d\n", count);
            for(j = 0; j < size; j++) {
                printf("%d %d\n", j, V[j]);
            }
            printf("\n");
        }

        count++;
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

//trimiterea mesajelor
void send_messages(int rank, char* file) {
    int nr_mesaje, i, j, from, to, broadcast = 0, source;
    int already = 0;
    char outBuffer[1000], inBuffer[1000], line[1000];
    char *tok;
    MPI_Status status;
    FILE *in;

    in = fopen(file, "r");
    fscanf(in, "%d", &nr_mesaje);

    fgets(line, 1000, in);
    
    for(j = 0; j < nr_mesaje; j++) {
        broadcast = 0;
        fgets(line, 1000, in);

        tok = strtok(line, " ");
        from = atoi(tok);
        
        tok = strtok(NULL, " ");
        if(strcmp(tok, "B") == 0) broadcast = 1; //broadcast
        else {
            to = atoi(tok);
        }

        tok = strtok(NULL, "\n");
        strcpy(outBuffer + 4, tok);
        
        if(from == rank) { //procesul curent trimite
            
            printf("\n");
            outBuffer[3] = j;

            //broadcast initial
            outBuffer[0] = 0;
            for(i = 0; i < 50; i++) {
                if(adiacenta[rank][i] == 1) {
                    MPI_Send(outBuffer, 1000, MPI_CHAR, i, 1, MPI_COMM_WORLD);
                }
            }

            //mesaj
            if(broadcast == 1) {    //mesaj de broadcast
                outBuffer[0] = 1; //broadcast
                outBuffer[1] = rank;
                for(i = 0; i < 50; i++) {
                    if(adiacenta[rank][i] == 1) {
                        MPI_Send(outBuffer, 1000, MPI_CHAR, i, 1, 
                            MPI_COMM_WORLD);
                    }
                }
            }
            else {
                outBuffer[0] = 2; //unicast
                outBuffer[1] = rank; //sursa
                outBuffer[2] = to; //destinatia
                MPI_Send(outBuffer, 1000, MPI_CHAR, V[to], 1, MPI_COMM_WORLD);
            }

            sleep(1);

            //broadcast final
            outBuffer[0] = 3;
            for(i = 0; i < 50; i++) {
                if(adiacenta[rank][i] == 1) {
                    MPI_Send(outBuffer, 1000, MPI_CHAR, i, 1, MPI_COMM_WORLD);
                }
            }
        }
        else {
            while(1) {
                MPI_Recv(inBuffer, 1000, MPI_CHAR, MPI_ANY_SOURCE, 1, 
                    MPI_COMM_WORLD, &status);
                source = status.MPI_SOURCE;

                if(inBuffer[3] == j) {
                    if(inBuffer[0] == 0) {
                        for(i = 0; i < 50; i++) {
                            if(adiacenta[rank][i] == 1 && i != source) {
                                MPI_Send(inBuffer, 1000, MPI_CHAR, i, 
                                    1, MPI_COMM_WORLD);
                            }
                        }
                    }      
                    else if(inBuffer[0] == 3) {  //broadcast final
                        already = 0;
                        for(i = 0; i < 50; i++) {
                            if(adiacenta[rank][i] == 1 && i != source) {
                                MPI_Send(inBuffer, 1000, MPI_CHAR, i, 
                                    1, MPI_COMM_WORLD);
                            }
                        }
                        break;
                    }
                    else if(inBuffer[0] == 1) { //broadcast
                        if(already == 0) {
                            printf("rank %d de la %d broadcast \"%s\"\n", 
                                rank, inBuffer[1], inBuffer + 4);

                            for(i = 0; i < 50; i++) {
                                if(adiacenta[rank][i] == 1 && i != source) {
                                    MPI_Send(inBuffer, 1000, MPI_CHAR, i, 
                                        1, MPI_COMM_WORLD);
                                }
                            }
                            already = 1;
                        }                      
                    }
                    else if(inBuffer[0] == 2) { //unicast
                        if(inBuffer[2] == rank) {
                            printf("rank %d am primit de la %d via %d \"%s\"\n", 
                                rank, inBuffer[1], source, inBuffer + 4);
                        }
                        else {
                            printf("rank %d am primit de la %d catre %d, ", 
                                rank, source, inBuffer[2]);
                            printf("rutat catre %d \"%s\"\n", 
                                V[inBuffer[2]], inBuffer + 4);
                            MPI_Send(inBuffer, 1000, MPI_CHAR, V[inBuffer[2]], 
                                1, MPI_COMM_WORLD);
                        }
                    }
                } 
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);
    }
}

//alegerea liderului
void choose_leader(int rank, int size, char *file) {
    int dim, aux, neigh, count = 0;
    int recv_top[50][50], nil[50][50], ad[50][50];
    int info, nr_ecouri;
    int flag = 0;
    MPI_Request request;
    char line[500], *tok;
    MPI_Status status;
    int i, j, k, source;
    FILE *top, *messages;

    memset(nil, 0, 50*50*sizeof(int));
    memset(ad, 0, 50*50*sizeof(int));

    top = fopen(file, "r");
    for(i = 0; i <= rank; i++) {
        fgets(line, 500, top);
        if(i == rank) {
            tok = strtok(line, "- \n\0");            
            while(tok != NULL) {
                tok = strtok(NULL, "- \n\0");
                if(tok == NULL) break;

                neigh = atoi(tok);
                ad[rank][neigh] = 1;    
            }
        }
    }
    fclose(top);

    //stabilire topologie
    if(rank == 0) {
        for(i = 0; i < 50; i++) {
            if(ad[rank][i] == 1) {
                recv_top[0][0] = -1;    //sonda
                MPI_Send(recv_top, 50*50, MPI_INT, i, 2, MPI_COMM_WORLD);
            }
        }

        for(i = 0; i < 50; i++) {
            if(ad[rank][i] == 1) {
                MPI_Recv(recv_top, 50*50, MPI_INT, i, 2, 
                    MPI_COMM_WORLD, &status);

                for(j = 0; j < 50; j++) {
                    for(k = 0; k < 50; k++) {
                        if(recv_top[j][k] == 1) {
                            ad[j][k] = recv_top[j][k];
                        }
                    }
                }
            }
        }
    }
    else {
        sleep(1);
        MPI_Recv(recv_top, 50*50, MPI_INT, MPI_ANY_SOURCE, 2, 
            MPI_COMM_WORLD, &status);
        parent = status.MPI_SOURCE;

        for(i = 0; i < 50; i++) {
            if(ad[rank][i] == 1 && i != parent) {
                recv_top[0][0] = -1;    //sonda
                MPI_Send(recv_top, 50*50, MPI_INT, i, 2, MPI_COMM_WORLD);
            }
        }

        nr_ecouri = get_neighbour(ad[rank], 50) - 1;

        while(nr_ecouri > 0) {
            MPI_Recv(recv_top, 50*50, MPI_INT, MPI_ANY_SOURCE, 2, 
                MPI_COMM_WORLD, &status);
            source = status.MPI_SOURCE;
            
            if(recv_top[0][0] == -1) { //sonda
                MPI_Send(nil, 50*50, MPI_INT, source, 2, MPI_COMM_WORLD);
            }
            else { //ecou
                for(i = 0; i < 50; i++) {
                    for(j = 0; j < 50; j++) {
                        if(recv_top[i][j] == 1) {
                            ad[i][j] = recv_top[i][j];
                        }
                    }
                }
                nr_ecouri--;
            }
        }

        MPI_Send(ad, 50*50, MPI_INT, parent, 2, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    //difuzare topologie
    if(rank == 0) {
        for(i = 0; i < 50; i++) {
            if(ad[rank][i] == 1) {
                MPI_Send(ad, 50*50, MPI_INT, i, 2, MPI_COMM_WORLD);
            }
        }
    }
    else {
        MPI_Recv(ad, 50*50, MPI_INT, MPI_ANY_SOURCE, 2, 
            MPI_COMM_WORLD, &status);
        parent = status.MPI_SOURCE;
 
        for(i = 0; i < size; i++) {
            if(ad[rank][i] == 1 && i != parent) {
                MPI_Send(ad, 50*50, MPI_INT, i, 2, MPI_COMM_WORLD);
            }
        }

        MPI_Irecv(recv_top, 50*50, MPI_INT, MPI_ANY_SOURCE, 2, 
            MPI_COMM_WORLD, &request);
        while(1) {
            sleep(1);
            MPI_Test(&request, &flag, &status);
            if(!flag || status.MPI_SOURCE < 0) {
               break;
            }
        }
    }  

    //stabilire lider
    leader = 5000;
    second = 5000;

    for(i = 0; i < 50; i++) {
        for(j = 0; j < 50; j++) {
            if((ad[j][i] == 1 || i == j) && j < leader) {
                leader = j;
            }

        }
    }

    for(i = 0; i < 50; i++) {
        for(j = 0; j < 50; j++) {
            if((ad[j][i] == 1 || i == j) && j < second && j > leader) {
                second = j;
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    
    while(count < size) {
        if(rank == count) {
            if(rank == 0) printf("\n");
            printf("%d chose leader %d and second %d\n", count, leader, second);
        }

        count++;
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

int main(int argc, char** argv) {

    int rank, size;
    
    // init
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    V = (int*)malloc(size*sizeof(int));
    
    //descoperire topologie
    stabilire_topologie(rank, size, argv[1]);
    difuzare_topologie(rank, size);
    dijkstra(rank, size);

    //trimitere mesaje
    send_messages(rank, argv[2]);

    //alegere lider
    choose_leader(rank, size, argv[1]);

    free(V);

    MPI_Finalize();	
}
