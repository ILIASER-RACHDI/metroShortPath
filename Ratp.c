//
// Created by ER-RACHDI ILIAS .
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATIONS 400
#define INF 99999
#define connexion_directe_entre_deux_stations 1
#define CHANGEMENT_DE_LIGNE 5

struct station {
    char name[100];
    int id;
};

struct arcs {
    struct station origine;
    struct station destination;
    int ligne;
};

int start, end_;

int nbStations, nbArcs;
struct station stations[MAX_STATIONS];
struct arcs arcs[MAX_STATIONS * MAX_STATIONS];
//table des d
int d[MAX_STATIONS];
//table des predecesseurs
int p[MAX_STATIONS];
//table des stations visitées
int visited[MAX_STATIONS];
//table des lignes

//fonction qui retourne l'indice de la station la plus proche
int findMin() {
    int i, minV = -1;
    for (i = 1; i < nbStations; i++) {
        if (!visited[stations[i].id] && (minV == -1 || d[stations[i].id] < d[minV])) {
            minV = stations[i].id;
        }
    }
    return minV;
}

//fonction to get the name of the station from the id
char *getStationName(int id) {
    int i;
    for (i = 0; i < nbStations; i++) {
        if (stations[i].id == id) {
            return stations[i].name;
        }
    }
    return NULL;
}

//fonction qui calcule les d et les predecesseurs
void dijkstra(int start) {
    int i, j;
    for (i = 1; i < nbStations; i++) {
        d[stations[i].id] = INF;
        p[stations[i].id] = -1;
        visited[stations[i].id] = 0;
    }
    d[start] = 0;

    for (i = 1; i < nbStations; i++) {
        int u = findMin();
        if (u == -1) {
            return;
        }
        visited[u] = 1;
        for (j = 1; j < nbArcs; j++) {
            if (arcs[j].origine.id == u) {
                int v = arcs[j].destination.id;
                int previous_ligne = -1;
                for (int k = 1; k < nbArcs; k++) {
                    if (arcs[k].origine.id == p[u] && arcs[k].destination.id == u) {
                        previous_ligne = arcs[k].ligne;
                        break;
                    }
                }
                int cost = connexion_directe_entre_deux_stations;
                if (previous_ligne != -1 && arcs[j].ligne != previous_ligne)
                    cost = CHANGEMENT_DE_LIGNE;

                if (d[v] > d[u] + cost) {
                    d[v] = d[u] + cost;
                    p[v] = u;
                }
            }
        }
    }
}
//fonction qui affiche le chemin le plus court
void printPath(int start, int end, int previous_ligne) {
    if (start == end) {
        printf("%s  \n", stations[start].name);
    } else if (p[end] == -1) {
        printf("No path from %s to %s.  ", stations[start].name, stations[end].name);
    }
    else {
        int current_ligne = -1;
        for (int i = 1; i < nbArcs; i++) {
            if (arcs[i].origine.id == p[end] && arcs[i].destination.id == end) {
                current_ligne = arcs[i].ligne;
                break;
            }
        }
        printPath(start, p[end], current_ligne);
        printf(" Ligne : %d || %d  %s  \n", current_ligne , stations[end].id , stations[end].name);
        if(previous_ligne!= current_ligne && stations[end].id!=end_){
            printf("\n\t\tAttention changement !!\n\n");
            }
    }
}

//fonction qui affiche le trajet le plus court entre deux stations et le temps de trajet en minutes et id de stations parcourues
void printShortestPath(int start, int end) {
    dijkstra(start);
    printf("Shortest path from %s to %s: ", getStationName(start), getStationName(end));
    printPath(start, end, -1);
    printf("    \n");
    printf("Temps de trajet : %d minutes  \n", d[end]);
}

//fonction to read the file stations and fill the table stations
void readStations() {
    FILE *f = fopen("FichierCSV/Stations.csv", "r");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    char line[1024];
    int i = 1;
    //skip the first line
    fgets(line, 1024, f);
    //la structure de fichier est : id,nom and store
    while (fgets(line, 1024, f)) {

        char *token = strtok(line, ",");
        int j = 0;
        while (token) {
            if (j == 1) {
                stations[i].id = atoi(token);
            } else if (j == 0) {
                strcpy(stations[i].name, token);
            }
            token = strtok(NULL, ",");
            j++;
        }
        i++;
    }
    nbStations = i;
    //print the table to check
    printf("id name \n");
    for (i = 1; i < nbStations; i++) {
        printf("id : %d name : %s \n", stations[i].id, stations[i].name);
    }
    fclose(f);
}

//fonction to read the file aretes and fill the table arcs
void readArcs() {
    FILE *f = fopen("FichierCSV/Aretes.csv", "r");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    char line[1024];
    int i = 1;
    //skip the first line
    fgets(line, 1024, f);
    //la structure de fichier est : origine,destination,ligne and store
    while (fgets(line, 1024, f)) {

        char *token = strtok(line, ",");
        int j = 0;
        while (token) {
            if (j == 0) {
                arcs[i].origine.id = atoi(token);
                strcpy(arcs[i].origine.name, getStationName(arcs[i].origine.id));
            } else if (j == 1) {
                arcs[i].destination.id = atoi(token);
                strcpy(arcs[i].destination.name, getStationName(arcs[i].destination.id));
            } else if (j == 2) {
                //get the just numiric part of the line convert into int
                char *token2 = strtok(token, "b");
                arcs[i].ligne = atoi(token2);
            }
            token = strtok(NULL, ",");
            j++;
        }
        i++;
    }
    nbArcs = i;
    fclose(f);
}
//afficher arcs
void printArcs() {
    int i;
    for (i = 1; i < nbArcs; i++) {
        printf("id origine : %d name origine : %s id destination : %d name destination : %s ligne :  %d \n",
               arcs[i].origine.id, arcs[i].origine.name, arcs[i].destination.id, arcs[i].destination.name,
               arcs[i].ligne);
    }
}

// fonction display a menu manage the user choice
void menu() {
    int choix;
    
    do {
        printf("1.Afficher les stations    2. Afficher le chemin le plus court    3. Quitter  \n ");
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                readStations();
                break;
            case 2:
                readStations();
                readArcs();
                printf("Entrer l'id de la station de depart : ");
                scanf("%d", &start);
                printf("Entrer l'id de la station d'arrivee : ");
                scanf("%d", &end_);
                //afficher les noms des stations correspondant aux id
                printf("Station de depart : %s  \n", getStationName(start));
                printf("Station d'arrivee : %s  \n", getStationName(end_));
                printShortestPath(start, end_);
                break;
            case 3:
                printf("Au revoir");
                break;
            default:
                printf("Choix invalide");
                break;
        }
    } while (choix != 4);
}

void logoRATP() {
    FILE *fichier;
    char ch;

    fichier = fopen("fun.txt", "r"); // Ouverture du fichier en mode lecture
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier fun.txt\n");
        exit(1); // Quitter le programme en cas d'erreur d'ouverture
    }
    while ((ch = fgetc(fichier)) != EOF) { // Lire chaque caractère jusqu'à la fin du fichier
        putchar(ch); // Afficher le caractère lu
    }

    fclose(fichier); // Fermer le fichier après avoir terminé de lire
}



//menu to calculate the shortest path or to quit return struct cotient the id of the start and end_ stations

int main() {
    logoRATP();
    menu();
    return 0;
}