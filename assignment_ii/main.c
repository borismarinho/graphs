#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsing.h"

#define TRUE 1
#define FALSE 0
#define MIN 10000

typedef struct node{
    int id;
    struct node *next;
}t_node;

typedef struct{
    t_node *first;
}t_list;

typedef struct{
    int id;
    char *label;
    int weight;
    int flagVisit;
    int relax;
    t_list *list;
}t_graph;

typedef struct longPathNode{
    int id;
    char *label;
    int weight;
    struct longPathNode *next;
} t_longPathNode;

typedef struct{
    t_longPathNode *first;
    t_longPathNode *last;
}t_longestPath;


//Adiciona o no na lista de adjacencias
t_graph addNodeToList (t_graph graph, int target){
    t_node *node;
    if (graph.list == NULL ){
        graph.list = (t_list *) malloc(sizeof(t_list));
        graph.list->first = NULL;
    }
    node = (t_node *) malloc(sizeof(t_node));
    node->id = target;
    node->next = graph.list->first;
    graph.list->first = node;
    return graph;
}

//Localiza a o no de origem e adiciona o no de destino a lista de adjacencias
t_graph *generateEdges(t_graph *graph, t_parsedInfo info){
    for (int i = 0; i < info.totalEdges; i++){
        for (int j = 0; j < info.totalNodes; j++) {
            if (graph[j].id == info.edges[i].source){
                graph[j] = addNodeToList(graph[j], info.edges[i].target);
            }
        }
    }
    return graph;
}

//Gera os nos com todas as informacoes gml numa estrutura tipo t_graph
t_graph *generateNodes(t_parsedInfo info){
    t_graph *graph;
    graph = (t_graph *) malloc(info.totalNodes * sizeof(t_graph));
    for (int i = 0; i < info.totalNodes; i++){
        graph[i].id = info.nodes[i].id;
        graph[i].label = (char *) malloc((strlen(info.nodes[i].label) + 1) * sizeof(char));
        strcpy(graph[i].label, info.nodes[i].label);
        graph[i].weight = info.nodes[i].weight;
        graph[i].flagVisit = FALSE;
        graph[i].relax = MIN;
        graph[i].list = NULL;
    }
    return graph;
}

//Gera o grafo inteiro com as informacoes e as listas de adjacencias
t_graph *generateGraph(t_parsedInfo info){
    t_graph *graph;
    graph = generateNodes(info);
    graph = generateEdges(graph, info);
    return graph;
}


//Marca os nos que nao sao destino de nenhuma aresta
void *generateStarters(t_graph *graph, t_parsedInfo info){
    for (int i = 0; i < info.totalNodes; i++){
        for (int j = 0; j < info.totalEdges; j++){
            if (graph[i].id == info.edges[j].target)    graph[i].flagVisit = TRUE;
        }
    }
    for (int i = 0; i < info.totalNodes; i++){
        if (graph[i].flagVisit == FALSE){
            graph[i].relax = graph[i].weight;
        }
    }
    for (int i = 0; i < info.totalNodes; i++){
        graph[i].flagVisit = FALSE;
    }
}

//Encontra um no no grafo e retorna seu endereco a partir de sua ID
t_graph *find(t_graph *graph, t_parsedInfo info, int id) {
    for (int i = 0; i < info.totalNodes; i++) {
        if (graph[i].id == id) return &graph[i];
    }
}

//Multiplica os pesos dos nos por -1
void invertWeights(t_graph *graph, t_parsedInfo info){
    for (int i = 0; i < info.totalNodes; i++)
        graph[i].weight = (-1) * graph[i].weight;
}

//Multiplica os relaxamentos dos nos por -1
void invertRelax(t_graph *graph, t_parsedInfo info){
    for (int i = 0; i < info.totalNodes; i++)
        graph[i].relax = (-1) * graph[i].relax;
}

//Realiza a ordenacao topologica
void topSort(t_graph *graph, t_parsedInfo info, t_list *topoSort){
    t_graph *aux;
    t_node *node;
    graph->flagVisit = TRUE;
    if (graph->list != NULL){
        while(graph->list->first != NULL){
            aux = find(graph, info, graph->list->first->id);
            if (aux->flagVisit == FALSE)
                topSort(aux, info, topoSort);
            graph->list->first = graph->list->first->next;
        }
    }
    node = (t_node *) malloc(sizeof(t_node));
    node->id = graph->id;
    node->next = topoSort->first;
    topoSort->first = node;
}

//Percorre as listas de adjacencias fazendo relaxamento nos nos
void relaxAdj(t_graph *graph, t_parsedInfo info, int id) {
    t_graph *gNode, *adjNode;
    t_node *aux;
    gNode = find(graph, info, id);
    if (gNode->list != NULL) {
        aux = gNode->list->first;
        while (aux != NULL) {
            adjNode = find(graph, info, aux->id);
            if (adjNode->weight + gNode->relax <= adjNode->relax) {
                adjNode->relax = adjNode->weight + gNode->relax;
            }
            aux = aux->next;
        }
    }
}

//Encontra o menor caminho
void shortestPath(t_graph *graph, t_parsedInfo info, t_list *topoSort){
    t_list *aux;
    aux = topoSort;
    while(aux->first != NULL){
        relaxAdj(graph, info, aux->first->id);
        aux->first = aux->first->next;
    }
}

//Encontra a maior quantidade de creditos a ser percorrida
int maxCredits(t_graph *graph, t_parsedInfo info){
    int maxC = 0;
    for (int i = 0; i < info.totalNodes; i++)
        if (graph[i].relax >= maxC)   maxC = graph[i].relax;
    return maxC;
}

//Calcula quantos caminhos criticos existem
int totalCriticals(t_graph *graph, t_parsedInfo info, int maxC){
    int counter = 0;
    for (int i = 0; i < info.totalNodes; i++)
        if (graph[i].relax == maxC) counter++;
    return counter;
}


//Encontra os vertices terminais de todos os caminhos criticos
t_list *criticalEndPoints(t_graph *graph, t_parsedInfo info, int maxC){
    t_list *criticalPaths;
    t_node *node;
    criticalPaths = (t_list *) malloc(sizeof(t_list));
    criticalPaths->first = NULL;
    for (int i = 0; i < info.totalNodes; i++){
        if (graph[i].relax == maxC){
            node = (t_node *) malloc(sizeof(t_node));
            node->id = graph[i].id;
            node->next = criticalPaths->first;
            criticalPaths->first = node;
        }
    }
    return criticalPaths;
}

//Aloca memoria para os caminhos criticos
t_list *allocateCriticals(int totalC){
    t_list *criticalPs;
    criticalPs = (t_list *) malloc(totalC * sizeof(t_list));
    for (int i = 0; i < totalC; i++)
        criticalPs->first = NULL;
    return criticalPs;
}

//Gera o caminho critico
t_list *generateCriticalPath(t_graph *graph, t_parsedInfo info, int id, t_list *criticalPath){
    t_graph *node, *aux;
    t_node *vertex;
    node = find(graph, info, id);
    for (int i = 0; i < info.totalEdges; i++){
        if (info.edges[i].target == id){
            aux = find(graph, info, info.edges[i].source);
            if (node->relax - aux->relax == node->weight){
                vertex = (t_node *) malloc(sizeof(t_node));
                vertex->id = aux->id;
                vertex->next = criticalPath->first;
                criticalPath->first = vertex;
                generateCriticalPath(graph, info, aux->id, criticalPath);
            }
        }
    }
}

void generateGraphDotFile(t_parsedInfo info){
    FILE *fp;
    fp = fopen("graph.dot", "w+");
    fprintf(fp, "digraph CiC{\n");
    for (int i = 0; i < info.totalEdges; i++){
        fprintf(fp, "\t%d -> %d;\n", info.edges[i].source, info.edges[i].target);
    }
    fprintf(fp, "\tlabelloc=\"t\";\n");
    fprintf(fp, "\tlabel=\"Ciencia da Computacao\";\n");
    fprintf(fp, "}\n\n");
    fclose(fp);
}

int main(){
    FILE *fp;
    t_parsedInfo info;
    t_graph *graph, *aux;
    t_graph *backup;
    t_list *critical, *topoSort, *criticalPs;
    t_node *auxNode;
    int maxC, totalC;
    info = gmlParser();
    graph = generateGraph(info);
    backup = generateGraph(info);

    topoSort = (t_list *) malloc(sizeof(t_list));
    topoSort->first = NULL;
    for (int i = 0; i < info.totalNodes; i++) {
        if (!graph[i].flagVisit) {
            topSort(&graph[i], info, topoSort);
        }
    }
    invertWeights(backup, info);
    generateStarters(backup, info);

    shortestPath(backup, info, topoSort);
    invertWeights(backup, info);
    invertRelax(backup, info);
    maxC = maxCredits(backup, info);
    totalC = totalCriticals(backup, info, maxC);
    criticalPs = allocateCriticals(totalC);
    critical = criticalEndPoints(backup, info, maxC);
    for (int i = 0; i < totalC; i++){
        auxNode = (t_node *) malloc(sizeof(t_node));
        auxNode->id = critical->first->id;
        auxNode->next = criticalPs[i].first;
        criticalPs[i].first = auxNode;
        generateCriticalPath(backup, info, critical->first->id, &criticalPs[i]);
        critical->first = critical->first->next;
    }
    generateGraphDotFile(info);
    fp = fopen("graph.dot", "a+");
    for (int i = 0; i < totalC; i++){
        fprintf(fp, "digraph Critical %d\n", i + 1);
        while(criticalPs[i].first->next != NULL){
            fprintf(fp, "\t%d -> %d;\n", criticalPs[i].first->id, criticalPs[i].first->next->id);
            criticalPs[i].first = criticalPs[i].first->next;
        }
        fprintf(fp, "\tlabelloc=\"t\";\n");
        fprintf(fp, "\tlabel=\"Caminho Critico %d  ::  Peso Total = %d\";\n", i + 1, maxC);
        fprintf(fp, "}\n\n");
    }
    fclose(fp);
    return 0;
}
