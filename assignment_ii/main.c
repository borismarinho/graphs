#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsing.h"

#define TRUE 1
#define FALSE 0
#define MIN 0

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
    int soften;
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

t_graph *generateNodes(t_parsedInfo info){
    t_graph *graph;
    graph = (t_graph *) malloc(info.totalNodes * sizeof(t_graph));
    for (int i = 0; i < info.totalNodes; i++){
        graph[i].id = info.nodes[i].id;
        graph[i].label = (char *) malloc((strlen(info.nodes[i].label) + 1) * sizeof(char));
        strcpy(graph[i].label, info.nodes[i].label);
        graph[i].weight = info.nodes[i].weight;
        graph[i].flagVisit = FALSE;
        graph[i].soften = MIN;
        graph[i].list = NULL;
    }
    return graph;
}

t_graph *generateGraph(t_parsedInfo info){
    t_graph *graph;
    graph = generateNodes(info);
    graph = generateEdges(graph, info);
    return graph;
}

t_list *generateStarters(t_graph *graph, t_parsedInfo info){
    t_list *starters;
    t_node *node;
    starters = (t_list *) malloc(sizeof(t_list));
    starters->first = NULL;
    for (int i = 0; i < info.totalNodes; i++){
        for (int j = 0; j < info.totalEdges; j++){
            if (graph[i].id == info.edges[j].target)    graph[i].flagVisit = TRUE;
        }
    }
    for (int i = 0; i < info.totalNodes; i++){
        if (graph[i].flagVisit == FALSE){
            node = (t_node *) malloc(sizeof(t_node));
            node->id = graph[i].id;
            node->next = starters->first;
            starters->first = node;
        }
    }
    for (int i = 0; i < info.totalNodes; i++){
        graph[i].flagVisit = FALSE;
    }
    return starters;
}

t_graph *find(t_graph *graph, t_parsedInfo info, int id){
    for (int i = 0; i < info.totalNodes; i++){
        if (graph[i].id == id) return &graph[i];
    }
}

void recursion(t_graph *graph, t_parsedInfo info){
    for (int i = 0; i < info.totalNodes; i++){
        graph[i].flagVisit
    }
}


void softenAdj(t_graph *graph, t_parsedInfo info, int id, t_longestPath *path){
    t_graph *node, *aux;
    int min = MIN, idMaior;
    node = find(graph, info, id);
    node->soften = node->weight;
//  Marca no visitado
    node->flagVisit = TRUE;
    if (node->list != NULL) {
//      Amacia nos adjacentes
        while (node->list->first != NULL) {
            aux = find(graph, info, node->list->first->id);
            if (aux->soften <= aux->weight + node->soften) {
                aux->soften = aux->weight + node->soften;
            }
            node->list->first = node->list->first->next;
        }
//  Acha o maior grau
    for (int i = 0; i < info.totalNodes; i++){
        if (graph[i].flagVisit == 0 && graph[i].soften != 0){
            if (graph[i].soften >= min){
                min = graph[i].soften;
            }
        }
    }
    for (int i = 0; i < info.totalNodes; i++){
        if (graph[i].flagVisit == 0 & graph[i].soften == min){
            printf("Maior: %s\n", graph[i].label);
            getchar();
            existeElemento = TRUE;
        }
    }


//        for (int i = 0; i < info.totalNodes; i++)
//            graph[i].soften = MIN;
//        for (int i = 0; i < info.totalNodes; i++){
//            printf("%s\n", graph[i].label);
//            printf("%d\n", graph[i].soften);
//            getchar();
//        }
    }
}

t_longestPath *longestPath (t_graph *graph, t_parsedInfo info) {
    t_list *starters;
    t_longestPath *path;
    path = (t_longestPath *) malloc(sizeof(t_longestPath));
    path->first = NULL;
    path->last = NULL;
    starters = generateStarters(graph, info);
    while (starters->first != NULL) {
        printf("%d\n", starters->first->id);
        softenAdj(graph, info, starters->first->id, path);
        getchar();
        starters->first = starters->first->next;
        //        getchar();
//        for (int i = 0; i < info.totalNodes; i++){
//            printf("%d\n", graph[i].soften);
//        }
    }
}
//  TODO Encontrei os vertices iniciais, farei uma busca a partir deles para gerar o maior caminho
//    while (starters->first != NULL){
//        printf("Code: %d\n", starters->first->id);
//        getchar();
//        starters->first = starters->first->next;
//    }



int main(){
    t_parsedInfo info;
    t_graph *graph;
    info = gmlParser();
    graph = generateGraph(info);
    longestPath(graph, info);
//    for (int i = 0; i < info.totalNodes; i++){
//        printf("Code: %d\n", graph[i].id);
//        printf("Label: %s\n", graph[i].label);
//        printf("Weight: %d\n", graph[i].weight);
//        printf("----------\n");
//        if (graph[i].list == NULL)  printf("NULO\n");
//        else {
//            while (graph[i].list->first != NULL) {
//                printf("-> %d ", graph[i].list->first->id);
//                graph[i].list->first = graph[i].list->first->next;
//            }
//        }
//        printf("\n");
//    }
    return 0;
}
