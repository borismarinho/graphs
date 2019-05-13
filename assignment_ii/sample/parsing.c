#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define ID 3
#define LABEL 6
#define WEIGHT 7
#define SOURCE 7
#define TARGET 7

typedef struct{
    int id;
    char label[MAX];
    int weight;
}t_parsedNodes;

typedef struct{
    int source;
    int target;
}t_parsedEdges;

typedef struct{
    t_parsedNodes *nodes;
    t_parsedEdges *edges;
    int totalNodes;
    int totalEdges;
    int stackSize;
}t_parsedInfo;

//Analisa o texto e coloca em uma estrutura de dados
t_parsedInfo textParser(t_parsedInfo info){
    FILE *fp;
    int stackTop = 0, nodesTop = 0, edgesTop = 0;
    int flagNode = 0, flagEdge = 0;
    char buffer[MAX];
    char stack[info.stackSize][MAX];
    char *aux;

    fp = fopen("cic.txt", "r");
    while(fgets(buffer, MAX, fp) != NULL){
        if (strstr(buffer, "]") == NULL){
            strcpy(stack[stackTop], buffer);
            if (strstr(stack[stackTop], "node") != NULL)    flagNode = 1;
            if (strstr(stack[stackTop], "edge") != NULL)    flagEdge = 1;
            stackTop++;
        }
        else{
            if (flagNode == 1) {
                aux = strstr(stack[stackTop - 3], "id") + ID;
                aux[strlen(aux) - 1] = '\0';
                info.nodes[nodesTop].id = atoi(aux);
                aux = strstr(stack[stackTop - 2], "label") + LABEL;
                aux[strlen(aux) - 1] = '\0';
                strcpy(info.nodes[nodesTop].label, aux);
                aux = strstr(stack[stackTop - 1], "weight") + WEIGHT;
                aux[strlen(aux) - 1] = '\0';
                info.nodes[nodesTop].weight = atoi(aux);
                nodesTop++;
                flagNode = 0;
            } else if (flagEdge == 1){
                aux = strstr(stack[stackTop - 2], "source") + SOURCE;
                aux[strlen(aux) - 1] = '\0';
                info.edges[edgesTop].source = atoi(aux);
                aux = strstr(stack[stackTop - 1], "target") + TARGET;
                aux[strlen(aux) - 1] = '\0';
                info.edges[edgesTop].target = atoi(aux);
                edgesTop++;
                flagEdge = 0;
            }
        }
    }
    fclose(fp);
    return info;
}

//Descobre quantos nos e arestas existem, alem do total de linhas para fazer a pilha de parsing
t_parsedInfo getMaxInfo(){
    FILE *fp;
    t_parsedInfo info;
    char buffer[MAX];

    info.totalNodes = 0;
    info.totalEdges = 0;
    info.stackSize = 0;

    fp = fopen("cic.txt", "r");

    while(fgets(buffer, MAX, fp)) {
        if (strstr(buffer, "node") != NULL) info.totalNodes++;
        if (strstr(buffer, "edge") != NULL) info.totalEdges++;
        info.stackSize++;
    }

    fclose(fp);

    info.nodes = (t_parsedNodes *) malloc(info.totalNodes * sizeof(t_parsedNodes));
    info.edges = (t_parsedEdges *) malloc(info.totalEdges * sizeof(t_parsedEdges));

    return info;
}

t_parsedInfo gmlParser (){
    t_parsedInfo info;
    info = getMaxInfo();
    info = textParser(info);
    return info;
}
