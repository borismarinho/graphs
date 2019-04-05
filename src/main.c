#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    int id;
    struct node *next;
    struct node *previous;
	struct t_node_list *adjacency_list;
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

t_node_list *create_adjacency_list (t_node_list *node_list, t_node *node, t_edge edge){
	t_node_list *adjacency_list;
	t_node *aux_node, *src_node, *tgt_node;
	aux_node = node_list->first;
	while (aux_node->id != edge.source && aux_node->next != NULL){
		aux_node = aux_node->next;
	}
	src_node = aux_node;
	aux_node = node_list->first;
	while (aux_node->id != edge.target && aux_node->next != NULL){
		aux_node = aux_node->next;
	}
}



void parser (FILE *stream){
    t_node_list *node_list;
    t_node *node;
	t_edge edge;
	char buffer[100];
    int i = 0, j, k;
	int sz, id, source, target;
    char s[100];
    char stack[1000][100];
    char aux_stack[1000][100];
    node_list = create_node_list();
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
                strcpy(buffer, (strstr(aux_stack[j-1], "id") + 3));
				sz = strlen(buffer);
				buffer[sz - 1] = '\0';
				id = atoi(buffer);
				node = create_node(id);
				add_node_to_list(node_list, node);
				node->adjacency_list = create_node_list();
            }
			if (strstr(aux_stack[j], "edge") != NULL){
				strcpy(buffer, (strstr(aux_stack[j-1], "source") + 7));
				sz = strlen(buffer);
				buffer[sz - 1] = '\0';
				source = atoi(buffer);
				strcpy(buffer, (strstr(aux_stack[j-2], "target") + 7));
				sz = strlen(buffer);
				buffer[sz - 1] = '\0';
				target = atoi(buffer);
				printf("source = %d, target = %d,\n", source, target);
				edge.source = source;
				edge.target = target;
			}
            for (k = 0; k <= j; k++){
                //printf("\nAUX\n%s\n", aux_stack[k]);
            }
        }
        else{
            strcpy(stack[i], s);
            //printf("stack[%d] = %s", i, stack[i]);
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
