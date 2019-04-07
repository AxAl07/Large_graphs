#include <stdlib.h>
#include <stdio.h>
#include <time.h>//to estimate the runing time

#define NLINKS 100000000 


typedef struct {
	unsigned long s;
	unsigned long t;
} edge;


//edge list structure:
typedef struct {
	unsigned long n;//number of nodes
	unsigned long e;//number of edges
	edge *edges;//list of edges
} edgelist;


//compute the maximum of three unsigned long
inline unsigned long max3(unsigned long a,unsigned long b,unsigned long c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}


//reading the edgelist from file
edgelist* readedgelist(char* input){
    unsigned long e1=NLINKS;
    FILE *file=fopen(input,"r");

    edgelist *g=malloc(sizeof(edgelist));
    g->n=0;
    g->e=0;
    g->edges=malloc(e1*sizeof(edge));//allocate some RAM to store edges

    while (fscanf(file,"%u %u", &(g->edges[g->e].s), &(g->edges[g->e].t))==2) {
        g->n=max3(g->n,g->edges[g->e].s,g->edges[g->e].t);
        if (g->e++==e1) {//increase allocated RAM if needed
            e1+=NLINKS;
            g->edges=realloc(g->edges,e1*sizeof(edge));
        }
    }
    fclose(file);

    g->n++;

    g->edges=realloc(g->edges,g->e*sizeof(edge));

    return g;
}


void free_edgelist(edgelist *g){
    free(g->edges);
    free(g);
}



// Exo 2 : count the number of edges and nodes without storing the graph in memory

typedef struct {
	unsigned long Nb_n; //number of nodes
	unsigned long Nb_e; //number of edges
} Size;


Size* size_graph (char* input){
    
    unsigned long max_n = 0;
    unsigned long n = 0;
    unsigned long nb_e = 0;
    unsigned long e = 0;
    Size* s = malloc (2*sizeof(unsigned int));
    FILE* file=fopen(input,"r");

    while (fscanf(file,"%lu %lu", &(n), &(e))==2) {
        nb_e++;
        if (n>max_n){
            max_n = n;}
        if (e>max_n){
            max_n = e;}
		}

    max_n++;// Le noeud numero 0 existe

    s->Nb_n = max_n;
    s->Nb_e = nb_e;

    return (s);
	}


// Exo 4 : compute the degree of each file and write it in a file

void Compute_degree_write_file (edgelist* edge_list, char* output_filename){
    
    unsigned long n = edge_list->n ;
    float* degres = malloc(n*sizeof(float));
    unsigned long e = edge_list->e ;

    for (int i=0; i<n; i++)
        degres[i] = 0.0;

    for (int j=0; j<e; j++) {
        degres[edge_list->edges[j].s] ++;
        }


    // Writing in a file 
    FILE *f = fopen(output_filename, "wb");
    if (f == NULL){
        printf("Error opening file!\n");
        exit(1);
    }

    for (int i=0; i<n; i++)
        fprintf(f, "%u %u\n",  i, (int) degres[i]);
    
    fclose(f);
    free (degres);
}


void Compute_degree_write_file2 (char* input, char* output_filename){

    Size* s;
    s = size_graph (input);
    unsigned long n = s->Nb_n;
    free(s);
    
    // unsigned long n = edge_list->n ;
    float* degres = malloc(n*sizeof(float));
    unsigned long f ;// from, node source, s was already taken
    unsigned long t ;

    for (int i=0; i<n; i++)
        degres[i] = 0.0;

    FILE* in_file=fopen(input,"r");
    while (fscanf(in_file,"%lu %lu", &(f), &(t))==2) {
        degres[f] ++;
    }

    // Writing in a file 
    FILE *out_f = fopen(output_filename, "wb");
    if (out_f == NULL){
        printf("Error opening file!\n");
        exit(1);
    }

    for (int i=0; i<n; i++)
        fprintf(out_f, "%u %u\n",  i, (int) degres[i]);
    
    fclose(out_f);
    free (degres);

}


// useful compute degree without writing it in a file
float* Compute_degree (edgelist* edge_list){
    
    unsigned long n = edge_list->n ;
    float* degres = malloc(n*sizeof(float));
    unsigned long e = edge_list->e ;


    for (int i=0; i<n; i++)
        degres[i] = 0.0;

    for (int j=0; j<e; j++) {
        degres[edge_list->edges[j].s] ++;
        }

    return (degres);
}


float* Compute_degree2 (char* input){

    Size* s;
    s = size_graph (input);
    unsigned long n = s->Nb_n;
    free(s);
    
    // unsigned long n = edge_list->n ;
    float* degres = malloc(n*sizeof(float));
    unsigned long f ;// from, node source, s was already taken
    unsigned long t ;

    for (int i=0; i<n; i++)
        degres[i] = 0.0;

    FILE* in_file=fopen(input,"r");
    while (fscanf(in_file,"%lu %lu", &(f), &(t))==2)
        degres[f] ++;

    return (degres);
}


// Exo 5 : compute a special quantity

unsigned long long Special_Quantity (edgelist* edge_list){
    
    unsigned long n = edge_list->n ;
    unsigned long e = edge_list->e ;
    float Q = 0;
    float* degres = malloc(n*sizeof(float));

    degres = Compute_degree(edge_list);


    for (int j=0; j<e; j++) {
        Q += degres[edge_list->edges[j].s] * degres[edge_list->edges[j].t];
        }

    free (degres);
    return (Q);
}


unsigned long long Special_Quantity2 (char* input){

    Size* s;
    s = size_graph (input);
    unsigned long n = s->Nb_n;
    unsigned long e = s->Nb_e;
    free(s);
    
    float Q = 0;
    float* degres = malloc(n*sizeof(float));
    unsigned long f ;// from, node source, s was already taken
    unsigned long t ;

    degres = Compute_degree2 (input);

    FILE* in_file=fopen(input,"r");
    while (fscanf(in_file,"%lu %lu", &(f), &(t))==2) {
        Q += degres[f] * degres[t];
    }

    free (degres);
    return (Q);
}


// Exo 6 : degree distribution 
void Degree_distribution (edgelist* edge_list, char* filename){
    
    unsigned long n = edge_list->n ;
    unsigned long e = edge_list->e ;
    long double Q = 0;
    float* degres = malloc(n*sizeof(float));

    degres = Compute_degree(edge_list);
    int* distrib = malloc(n*sizeof(int));

    for (int j=0; j<n; j++)
        distrib[j] = 0;

    for (int j=0; j<n; j++){
        distrib[((int) degres[j])]++;
        // printf("%u ; %u\n", (int) degres[j], distrib[((int) degres[j])]);
    }

    // Writing in a file 
    FILE *f = fopen(filename, "wb");
    if (f == NULL){
        printf("Error opening file!\n");
        exit(1);
    }

    for (int i=0; i<n; i++)
        fprintf(f, "%u %u\n", i, distrib[i]);
    
    fclose(f);

    free (degres);
    free (distrib);
}

void Degree_distribution2 (char* input, char* output_filename){
    
    Size* s;
    s = size_graph (input);
    unsigned int n = s->Nb_n;
    unsigned int e = s->Nb_e;
    free(s);

    long double Q = 0;
    float* degres = malloc(n*sizeof(float));

    degres = Compute_degree2 (input);
    int* distrib = malloc(n*sizeof(int));

    for (int j=0; j<n; j++)
        distrib[j] = 0;

    for (int j=0; j<n; j++){
        distrib[((int) degres[j])]++;
    }

    // Writing in a file 
    FILE *f = fopen(output_filename, "wb");
    if (f == NULL){
        printf("Error opening file!\n");
        exit(1);
    }

    for (int i=0; i<n; i++)
        fprintf(f, "%u %u\n", i, distrib[i]);
    
    fclose(f);
    free (degres);
    free (distrib);
}


// Partie 2 : Exo 7
// le stockage en mémoire sous la forme d'une liste of edges est faite en début de code

int** read_store_adjmat (char* input){

    // On calcule d'abord la taille de la matrice, sans la stocker
    Size* s_mat;
    unsigned int n;
    s_mat = size_graph(input);
    n = s_mat->Nb_n;
    free (s_mat);

    // Initialisation de la matrice d'adjacence 
    int** adj = (int **)calloc(n, sizeof(int*));
    for(int i = 0; i < n; i++) adj[i] = (int *)calloc(n, sizeof(int));

    unsigned int s;
    unsigned int t;

    FILE *file=fopen(input,"r");

    // On lit le fichier et on rempli la matrice si on a une arrête
    while (fscanf(file,"%u %u", &s, &t)==2) {
        adj[s][t] = 1;
    }
    fclose(file);

    return adj; 
}


void free_adj_matrix (int** mat, unsigned int N){
    for(int i = 0; i < N; i++)
        free(mat[i]);
    free(mat);
}


typedef struct {
    unsigned int n;
    unsigned long e;
    unsigned long *cd; // cumulative degree cd[0]=0 length=Nb_n+1
    unsigned int *adj; // concatenated list of neigbors
} adjarray;


void free_adjarray (adjarray* a){
    free(a->cd);
    free(a->adj);
    free(a);
}



adjarray* read_store_adjarray (char* input){

    unsigned long e1=NLINKS;
    
    // Lecture de la taille de la matrice
    adjarray *arr = malloc(sizeof(adjarray));
    arr->n =0;
    arr->e =0;
    arr->cd = calloc(e1, sizeof(unsigned long));

    unsigned int s;
    unsigned int t;

    // On lit une première fois pour completer cd : cumulative degree
    FILE *file=fopen(input,"r");

    while (fscanf(file,"%u %u", &s, &t)==2) {
        arr->cd[s+1]++;
        arr->e++;
        arr->n = max3(arr->n, s, t);
        if (s++==e1) {//increase allocated RAM if needed
            e1+=NLINKS;
            arr->cd=realloc(arr->cd,e1*sizeof(unsigned long));
        }
    }
    fclose(file);
    
    arr->n ++;

    for (int i = 2; i < arr->n+1; i++) {
        arr->cd[i] = arr->cd[i-1] + arr->cd[i];
    }

    // On fait une seconde lecture pour completer adj (vecteur d'adjacence)
    unsigned int *c = calloc((arr->n+1), sizeof(unsigned int));
    arr->adj = calloc(arr->e, sizeof(unsigned int));


    file = fopen(input,"r");

    while (fscanf(file,"%u %u", &s, &t)==2) {
        arr->adj[arr->cd[s]+c[s]] = t;
        c[s]++;
    }

    fclose(file);
    free(c);

    return arr; 
}
