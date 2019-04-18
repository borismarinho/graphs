#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000;

typedef struct adjListNode {
    char *id;
    struct adjListNode *next;
} t_adjListNode;

typedef struct {
    t_adjListNode *first;
} t_adjList;

typedef struct {
    char *id;
    t_adjList *adjList;
} t_graph;

char *readInput (){
    char *input;
    char buffer[1000];
    scanf("%s", buffer);
    getchar();
    input = (char *) malloc((strlen(buffer) + 1) * sizeof(char));
    strcpy(input, buffer);
    input[strlen(buffer)] = '\0';
    return input;
}

int checkString (char *str1, char *str2){
    int size1, size2, count = 0;
    size1 = strlen(str1);
    size2 = strlen(str2);
    // Return -1 indica que as strings sao iguais
    if (!strcmp(str1, str2))    return -1;
    // Return 0 indica que eh falso
    else if (size1 != size2)    return 0;
    else {
        for (int i = 0; i < size1; i++){
            if (str1[i] != str2[i]) count++;
        }
        if (count == 1) return 1;
        else            return 0;
    }
}

t_adjListNode *newAdjListNode (char *id){
    t_adjListNode *adjListNode;
    adjListNode = (t_adjListNode *) malloc(sizeof(t_adjListNode));
    adjListNode->id = id;
    adjListNode->next = NULL;
    return adjListNode;
}

t_adjList *newAdjList(){
    t_adjList *adjList;
    adjList = (t_adjList *) malloc(sizeof(t_adjList));
    adjList->first = NULL;
    return adjList;
}

t_adjList *addNodeToAdjList(t_adjList *adjList, char *id){
    t_adjListNode *adjListNode;
    if (adjList == NULL)    adjList = newAdjList();
    adjListNode = newAdjListNode(id);
    adjListNode->next = adjList->first;
    adjList->first = adjListNode;
    return adjList;
}

t_graph *newNode (t_graph *graph, char *input, int count){
    if (graph == NULL){
        graph = (t_graph *) malloc(sizeof(t_graph));
        graph[count].id = input;
        graph[count].adjList = NULL;
        } else {
            graph = (t_graph *) realloc(graph, sizeof(graph));
            graph[count].id = input;
            graph[count].adjList = NULL;
        }
    return graph;
}

void printGraph(t_graph *graph, int size){
    printf("\t--ADJACENCY LIST--\t\n");
    for (int i = 0; i < size; i++){
        printf("%s --->", graph[i].id);
        if (graph[i].adjList != NULL){
            while (graph[i].adjList->first != NULL){
                printf(" %s ->", graph[i].adjList->first->id);
                graph[i].adjList->first = graph[i].adjList->first->next;

            }
        }
        printf(" null\n");
    }
    printf("//----------//----------//\n");
}

void createAdjList(t_graph *graph, int size){
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if (checkString(graph[i].id, graph[j].id) == 1){
                graph[i].adjList = addNodeToAdjList(graph[i].adjList, graph[j].id);
            }
        }
    }
}

int **newAdjMatrix(int size){
    int **adjMatrix;
    adjMatrix = (int **) malloc (size * sizeof(int *));
    for (int i = 0; i < size; i++){
        adjMatrix[i] = (int *) malloc (size * sizeof(int));
    }
    return adjMatrix;
}

int **createAdjMatrix (t_graph *graph, int size){
    int **adjMatrix;
    adjMatrix = newAdjMatrix(size);
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if (checkString(graph[i].id, graph[j].id) == 1){
                adjMatrix[i][j] = 1;
            } else {
                adjMatrix[i][j] = 0;
            }
        }
    }
    return adjMatrix;
}

void printAdjMatrix(int **adjMatrix, int size){
    printf("\t--ADJACENCY MATRIX--\t\n");
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            printf("%d ", adjMatrix[i][j]);
        }
        printf("\n");
    }
    printf("//----------//----------//\n");
}

int **newIncMatrix (int totalVertices, int totalEdges){
    int **incMatrix;
    incMatrix = (int **) malloc (totalVertices * sizeof(int *));
    for (int i = 0; i < totalEdges; i++){
        incMatrix[i] = (int *) malloc (totalEdges * sizeof(int));
    }
    for (int i = 0; i < totalVertices; i++){
        for (int j = 0; j < totalEdges; j++){
            incMatrix[i][j] = 0;
        }
    }
    return incMatrix;
}

void printIncMatrix(int **incMatrix, int totalVertices, int totalEdges){
    printf("\t--INCIDENCE MATRIX--\t\n");
    for (int i = 0; i < totalVertices; i++){
        for (int j = 0; j < totalEdges; j++){
            printf("%d ", incMatrix[i][j]);
        }
        printf("\n");
    }
    printf("//----------//----------//\n");
}

int **createIncMatrix(int **adjMatrix, int size){
    int **incMatrix = NULL;
    int count = 0, count2 = 0;
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if (adjMatrix[i][j] == 1)   count++;
        }
    }
    printAdjMatrix(adjMatrix, size);
    incMatrix = newIncMatrix(size, count/2);
    count = 0;
    for (int i = 0; i < size; i++){
        for (int j = i; j < size; j++){
            if (adjMatrix[i][j] == 1){
                incMatrix[i][count] = 1;
                incMatrix[j][count] = 1;
                count++;
            }
        }
    }
    printIncMatrix(incMatrix, size, count);
}


void createGraph (){
    int count = 0;
    char *input;
    t_graph *graph = NULL;
    int **adjMatrix;
    printf("Insert -1 to end the input\n");
    while (strcmp("-1", input) != 0){
        printf("What's the word?\n");
        input = readInput();
        if (strcmp("-1", input) != 0){
            graph = newNode(graph, input, count);
            count++;
        }
    }
    createAdjList(graph, count);
    adjMatrix = createAdjMatrix(graph, count);
    printGraph(graph, count);
    printAdjMatrix(adjMatrix, count);
    createIncMatrix(adjMatrix, count);
}


int main(){
    createGraph();
    return 0;
}
