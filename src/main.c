#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    int id;
    struct node *next;
    struct node *previous;
} t_node;

typedef struct {
    t_node *first;
    t_node *last;
} t_node_list;

typedef struct {
    int source;
    int target;
} t_edge;

t_node_list *create_node_list (){
    t_node_list *node_list;
    node_list = (t_node_list *) malloc (sizeof(t_node_list));
    node_list->first = NULL;
    node_list->last = NULL;
    return node_list;
}

t_node *create_node(int id){
    t_node *node;
    node = (t_node *) malloc (sizeof(t_node));
    node->id = id;
    node->next = NULL;
    node->previous = NULL;
    return node;
}

void add_node_to_list(t_node_list* node_list, t_node *node){
    if (node_list->first == NULL && node_list->last == NULL){
        node_list->first = node;
        node_list->last = node;
    }
    else {
        node_list->last->next = node;
        node->previous = node_list->last;
        node_list->last = node;
    }
}

void parser (FILE *stream){
    t_node_list *node_list;
    t_node *node;
    int i = 0, j, k;
    char s[100];
    char stack[1000][100];
    char aux_stack[1000][100];
    node_list = create_node_list();
    node = create_node(0);
    while(fgets(s, 100, stream) != NULL){
        j = 0;
        if (strstr(s, "]") != NULL){
            strcpy(stack[i], s);
            while(strstr(stack[i], "[") == NULL){
                i--;
                if (strstr(stack[i], "id") != NULL || strstr(stack[i], "node") != NULL || strstr(stack[i], "edge") != NULL || strstr(stack[i], "source") != NULL || strstr(stack[i], "target") != NULL || strstr(stack[i], "graph") != NULL){
                    strcpy(aux_stack[j], stack[i]);
                    j++;
                }
            }
            //printf("stack[%d] = %s", i, stack[i]);
            i--;
            strcpy(aux_stack[j], stack[i]);
            if (strstr(aux_stack[j], "node") != NULL){
                printf("SANDUBA %c", *(strstr(aux_stack[j-1], "id") + 3));
            }
            for (k = 0; k <= j; k++){
                printf("\nAUX\n%s\n", aux_stack[k]);
            }
        }
        else{
            strcpy(stack[i], s);
            printf("stack[%d] = %s", i, stack[i]);
            i++;
        }
    }
}

int main(){
    FILE *fp;
    fp = fopen("karate.gml", "r");
    parser(fp);
    return 0;
}
