/*
    16/0114195 - Boris Marinho Ramos Silva Araujo
    19/0055511 - Ignacio Sanabria Alonso de Caso

    Os dados estao projetados para serem impressos no terminal.
    O codigo esta baguncado, nao esta dividio em bibliotecas, esta 
    pouco modularizado, varias variaveis estao mal nomeadas e algumas nao sao nem usadas,
    mas o codigo funciona.
    
    Compile com
    $ gcc -o main main.c
    
    Compilado com Sistema operacional de 32 bits Ubuntu  16.04.11
    Versão do compilador:
    $ gcc --version
    gcc (Ubuntu 5.4.0-6ubuntu1~16.04.11) 5.4.0 20160609
    Copyright (C) 2015 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_list;
struct node;

//Tipo No da lista de adjacencias

typedef struct node {
    int id;
    struct node *next;
    struct node *previous;
	struct node_list *adjacency_list;
} t_node;

//Tipo no de grafo

typedef struct node_list {
    int id;
    int outdegree;
    int indegree;
    double clus_coef;
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


//Aloca no de grafo
t_node_list *create_node_list (){
    t_node_list *node_list;
    node_list = (t_node_list *) malloc (sizeof(t_node_list));
    node_list->first = NULL;
    node_list->last = NULL;
    node_list->next = NULL;
    node_list->indegree = 0;
    node_list->outdegree = 0;
    node_list->clus_coef = 0;
    return node_list;
}


//Aloca no de lista de adjacencia
t_node *create_node(int id){
    t_node *node;
    node = (t_node *) malloc (sizeof(t_node));
    node->id = id;
    node->next = NULL;
    node->previous = NULL;
    node->adjacency_list = create_node_list();
    return node;
}


//Adiciona no na lista de adjacencia
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


//Encontra o endereco de memoria do no do grafo
t_node_list *find_vertex (t_node_list *node_list, int id){
    t_node_list *vertex;
    vertex = node_list;
    while(vertex->id != id){
        vertex = vertex->next;
    }
    return vertex;
}


//A partir da informacao parseada no arquivo gml se cria uma estrutura dinamica de grafos
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


//Le o arquivo gml utilizando uma pilha e salva em uma estrutura estatica
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

//Cria lista de adjacencias
void create_adjacency(t_node_list *node_list, t_parsed_info graph){
    t_node_list *src;
    t_node *node;
    for (int i = 0; i < graph.count_edge; i++){
        src = find_vertex(node_list, graph.edge[i].source);
        node = create_node(graph.edge[i].target);
        add_node_to_list(src, node);
        src = find_vertex(node_list, graph.edge[i].target);
        node = create_node(graph.edge[i].source);
        add_node_to_list(src, node);

    }

}

//Imprime o grafo
void print_graph (t_node_list *node_list){
    t_node_list *aux;
    t_node *node;
    aux = node_list;
    node = aux->first;
    while (aux != NULL){
        printf("%d --> ", aux->id);
        while(node != NULL){
            printf("%d ", node->id);
            node = node->next;

        }
        printf("\n");
        printf("Degree = %d,\n", aux->outdegree);
        printf("Clustering coeficient = %g,\n", aux->clus_coef);
    aux = aux->next;
    if (aux != NULL)    node = aux->first;
    }
}


//Calcula o grau do grafo
void node_outdegree(t_node_list *graph){
    int count;
    t_node_list *aux;
    t_node *node;
    aux = graph;
    node = aux->first;
    while(aux != NULL){
        count = 0;
        while (node != NULL){
            count++;
            node = node->next;
        }
        aux->outdegree = count;
        printf("Vertex %d, degree: %d\n", aux->id, aux->outdegree);
        aux = aux->next;
        if (aux != NULL)    node = aux->first;
    }
}

//Implementacao para grafos direcionados
void node_indegree(t_node_list *graph){
    int count;
    t_node_list *aux;
    t_node *node;
    t_node_list *indegree;
    indegree = graph;
    aux = graph;
    node = aux->first;
    for (int i = 1; i < 35; i++){
        count = 0;
        while(aux != NULL){
            while (node != NULL){
                if (node->id == i)  count++;
                node = node->next;
            }
            aux = aux->next;
            if (aux != NULL)    node = aux->first;
        }
        indegree->indegree = count;
        printf("Vertex %d, indegree: %d\n", indegree->id, count);
        indegree = indegree->next;
        aux = graph;
    }
}


//Esta funcao nao eh chamada
void undirected_graph(t_node_list *graph){
    int count;
    t_node_list *aux, *reverse;
    t_node *node, *tmp;
    t_node_list *indegree;
    indegree = graph;
    aux = graph;
    node = aux->first;
    for (int i = 1; i < 35; i++){
        while(aux != NULL){
            while (node != NULL){
                if (node->id == i){
                    reverse = find_vertex(graph, i);
                    tmp = create_node(aux->id);
                    add_node_to_list(reverse, tmp);
                }
                node = node->next;
            }
            aux = aux->next;
            if (aux != NULL)    node = aux->first;
        }
        

        aux = graph;
    }
}
/*
int clustering_count(t_node *adjacent, t_node *neighbour, int count, t_node *repeat){
    t_node *node;
    //printf("BEFORE: %d\n", count);
    while(adjacent != NULL){
        node = neighbour;
        printf("VIZINHO: %d\n", adjacent->id);
        while(node != NULL){
            if (adjacent->id == node->id) {
                count++;
                printf("\nIGUAS: %d IGUAIS: %d\n", adjacent->id, node->id);
                
            }
            //printf("ID %d\n", node->id);
                    
            //    getchar();
            printf("%d ", node->id);
            node = node->next;
        }
        getchar();
        adjacent = adjacent->next;
                
    }
    printf("COUNT: %d\n", count);
    return count;
}

void clustering_coeficient(t_node_list *graph){
    t_node_list *aux, *tmp1;
    t_node *node, *tmp2;
    int n_vertex, i;
    aux = graph;
    node = aux->first;
    while (aux != NULL){
        n_vertex = 0;
        //printf("NODO %d ADJACENTE %d\n", aux->id, node->id);
        while (node != NULL){
            tmp1 = find_vertex(graph, node->id);
            //printf("ADJACENTE %d\n", node->id);
            n_vertex = clustering_count(tmp1->first, node, n_vertex, aux->first);
            node = node->next;
        }
        printf("%d. n_vertex = %d\n", i, n_vertex);
        i++;
        aux = aux->next;
        if (aux != NULL)    node = aux->first;
    }
}
*/

//Comparando duas listas a funcao conta quantos elementos formam triangulos
int verify(t_node_list *list, int id1, int id2, int count){
    t_node_list *n1, *n2;
    t_node *v1, *v2;
    n2 = find_vertex(list, id2);
    v2 = n2->first;
    while(v2 != NULL){
        n1 = find_vertex(list, id1);
        v1 = n1->first;
        //printf("LISTA %d\n", v2->id);
        while(v1 != NULL){
            if(v1->id == v2->id)  count++;
          //  printf("%d ", v1->id);
            v1 = v1->next;
        }
        //printf("\n");
        //getchar();    
        v2 = v2->next;
    }
    return count;
}

//Calcula quantos elementos tem a lista de adjacencias
int list_size(t_node_list *list){
    int count = 0;
    t_node *node;
    node = list->first;
    while (node != NULL){
        count++;
        node = node->next;
    }

    return count;
}

//Calcula o coeficiente de aglomeracao
void clustering_coeficient(t_node_list *list){
    int count, k;
    t_node_list *graph, *neighbour;
    t_node *adjacent;
    graph = list;
    while (graph != NULL){
        count = 0;
        adjacent = graph->first;
        //printf("Grafo: %d, Adjacente: %d\n", graph->id, adjacent->id);
        while (adjacent != NULL){
            count = verify(list, graph->id, adjacent->id, count);
            adjacent = adjacent->next;
        }       
        k = list_size(graph);
        if (count == 0) graph->clus_coef = 0;
        else graph->clus_coef = (double) count/(k*(k-1));
        //printf("Count = %d\n", count);
        //getchar();
        graph = graph->next;
    }
}

//Calcula a aglomeracao media
void avg_clustering(t_node_list *list){
    double avg = 0;
    int aux, tmp = 0;
    while (list != NULL){
        aux = (int) (list->clus_coef * 100000);
        tmp += aux;
        list = list->next;
    }
    avg = (double) tmp/100000;
    avg = avg/34;
    printf("Average clustering: %g\n", avg);
}

int main(){
    t_parsed_info graph;
    t_node_list *list;
    FILE *fp;
    fp = fopen("karate.gml", "r");
    graph = parser(fp);
    list = create_graph(graph);
    create_adjacency(list, graph);
    //undirected_graph(list);
    node_outdegree(list);
    node_indegree(list);
    /*for (int i = 0; i < graph.count_edge; i++){
        printf("source = %d, target = %d,\n", graph.edge[i].source, graph.edge[i].target);
    }*/
    //node_degree(list);
    //node_degree(list);
    clustering_coeficient(list);
    print_graph(list);
    avg_clustering(list);
    return 0;
}
