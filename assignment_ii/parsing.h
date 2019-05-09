#ifndef GRAPHS_II_PARSING_H
#define GRAPHS_II_PARSING_H

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

t_parsedInfo gmlParser ();

#endif
