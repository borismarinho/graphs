#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_list;
struct node;

typedef struct node {
    int id;
    struct node *next;
    struct node *previous;
	struct node_list *adjacency_list;
} t_node;

typedef struct node_list {
    int id;
    t_node *first;
    t_node *last;
    struct node_list *next;
} t_node_list;

typedef struct {
    int source;
    int target;
} t_edge;

typedef struct {
    int id[50];
    t_edge edge[500];
    int count_node;
    int count_edge;
} t_parsed_info;

typedef struct{
    t_node_list *node_list;
} t_graph;

t_node_list *create_node_list (){
    t_node_list *node_list;
    node_list = (t_node_list *) malloc (sizeof(t_node_list));
    node_list->first = NULL;
    node_list->last = NULL;
    node_list->next = NULL;
    return node_list;
}

t_node *create_node(int id){
    t_node *node;
    node = (t_node *) malloc (sizeof(t_node));
    node->id = id;
    node->next = NULL;
    node->previous = NULL;
    node->adjacency_list = create_node_list();
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

t_node *find_vertex (t_node_list *node_list, int id){
    t_node_list *vertex;
    vertex = node_list;
    while(vertex->id != id){
        vertex = vertex->next;
    }
    return vertex;
}

t_node_list *create_graph(t_parsed_info info){
    t_node_list *node_list;
    t_node_list *header, *aux;
    node_list = create_node_list();
    header = node_list;
    aux = node_list;
    header->id = info.id[0];
    for (int i = 1; i < info.count_node; i++){
        node_list = create_node_list();
        node_list->id = info.id[i];
        aux->next = node_list;
        aux = aux->next;
    }
    return header;
}

void add_to_adjacency_list (t_node_list *node_list, t_edge edge){
	/*t_node *aux_node;
	t_node *src_node;
	t_node *tgt_node;
	src_node = find_node(node_list, edge.source);
	//tgt_node = find_node(node_list, edge.target);
	//printf("src %d\n", src_node->id);
	//printf("tgt %d\n", tgt_node->id);
	aux_node = node_list->first;
	printf("source %d, target %d,\n", edge.source, edge.target);

	while (aux_node->id != edge.source && aux_node != NULL){
		printf("SOURCE id = %d\n", aux_node->id);
		aux_node = aux_node->next;
	}
	src_node = aux_node;
	aux_node = node_list->first;
	while (aux_node->id != edge.target && aux_node != NULL){
		printf("target id = %d\n", aux_node->id);
		aux_node = aux_node->next;
	}
	tgt_node = aux_node;
	add_node_to_list(src_node->adjacency_list, tgt_node);*/
}



t_parsed_info parser (FILE *stream){
    t_parsed_info graph;
    t_node_list *node_list;
    t_node *node;
	t_edge edge;
	char buffer[100];
    int i = 0, j, k;
	int sz, id, source, target, count_node = 0, count_edge = 0;
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
				graph.id[count_node] = id;
				count_node++;

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
				edge.source = source;
				edge.target = target;
				graph.edge[count_edge].source = source;
				graph.edge[count_edge].target = target;
				count_edge++;
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
    node = node_list->first;
    while(node != NULL){
        printf("%d\n", node->id);
        node = node->next;
    }
    graph.count_edge = count_edge;
    graph.count_node = count_node;
    return graph;
}

/*t_node_list *create_graph(t_parsed_info graph){
    t_node_list *node_list;
    t_node *node, *aux;
    for (int i = 0; i < graph.count_node; i++){
        node_list = create_node_list(graph.id[i]);
        add_node_to_list(node_list, node);
        //printf("id %d\n", node_list->last->id);
    }
    aux = node_list->first;
    for (int i = 0; i < graph.count_edge; i++){
        add_to_adjacency_list(node_list, graph.edge[i]);
    }
    while (aux != NULL){
        printf("%d\n", aux->id);
        aux = aux->next;
    }*/


void create_adjacency(t_node_list *node_list, t_parsed_info graph){
    t_node_list *src;
    t_node *node;
    for (int i = 0; i < graph.count_edge; i++){
        src = find_vertex(node_list, graph.edge[i].source);
        node = create_node(graph.edge[i].target);
        add_node_to_list(src, node);

    }

}

void print_graph (t_node_list *node_list){
    while (node_list != NULL){
        printf("%d --> ", node_list->id);
        while(node_list->first != NULL){
            printf("%d ", node_list->first->id);
            node_list->first = node_list->first->next;

        }
        printf("\n");
    node_list = node_list->next;
    getchar();
    }
}

int main(){
    t_parsed_info graph;
    t_node_list *list;
    FILE *fp;
    fp = fopen("karate.gml", "r");
    graph = parser(fp);
    list = create_graph(graph);
    create_adjacency(list, graph);
    print_graph(list);
    /*for (int i = 0; i < graph.count_edge; i++){
        printf("source = %d, target = %d,\n", graph.edge[i].source, graph.edge[i].target);
    }*/
    return 0;
}
