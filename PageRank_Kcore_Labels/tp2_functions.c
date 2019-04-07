#include <stdlib.h>
#include <stdio.h>

/*
 * returns the (outward) degree of each node of the graph
 */
double* nodesDegree(edgelist* edges) {
  double* nodes_degrees = malloc(edges->n * sizeof(double));

  // initialization of the array (null array)
  for(unsigned int i = 0; i < edges->n; i++)
    nodes_degrees[i] = 0.0;

  for(unsigned int i = 0; i < edges->e ; i++)
    nodes_degrees[edges->edges[i].s]++;

  return nodes_degrees;
}

double* inNodesDegree(edgelist* edges) {
  double* nodes_degrees = malloc(edges->n * sizeof(double));

  // initialization of the array (null array)
  for(unsigned int i = 0; i < edges->n; i++)
    nodes_degrees[i] = 0.0;

  for(unsigned int i = 0; i < edges->e ; i++)
    nodes_degrees[edges->edges[i].t]++;

  return nodes_degrees;
}

/*
 * Implementation of the powerIteration algorithm for PageRank
 * alpha : random jump probability
 * t     : number of iterations of the algorithm
 */
double* powerIteration (edgelist* edge_list, double alpha, unsigned int t){

  unsigned long n = edge_list->n; // number of nodes
  unsigned long e = edge_list->e; // number of edges
  double* deg = nodesDegree(edge_list); // degree for all nodes
  double* P   = malloc(n*sizeof(double)); // probability density vector
  double* J   = malloc(n*sizeof(double)); // unit vector div. by n
  double* TP  = malloc(n*sizeof(double)); // T * P (T : trans. matrix)

  // initialization of P and J (unit vector)
  for (int i = 0; i < n; i++) {
      P[i] = 1.0 / ((double) n);
      J[i] = 1.0 / ((double) n);
  }

  // loop on iterations of the algorithm
  for (int j = 0 ; j < t; j++){
    printf(" - Running iteration %lu\n", j);
    fflush(stdout);

    // resetting T*P to zero, then computing TP
    for (int i = 0; i < n; i++)
        TP[i] = 0.0;

    for (int i=0; i<e; i++) {
      unsigned int source = edge_list->edges[i].s;
      unsigned int target = edge_list->edges[i].t;
      double deg_source   = deg[source];

      // if the degree is non-zero, T(u,v) = 1/deg(v), else 1/n
      if(deg_source > 0.1)
        TP[target] += P[source]/deg_source;
      //else
      //  TP[source] += P[target]/((double) n);
    }

    // computing P and its norm P_1
    double P_1 = 0.0;
    for (int i = 0; i < n; i++) {
        P[i] = (1.0 - alpha) * TP[i] + alpha * J[i];
        P_1 = P_1 + P[i];
    }
    // normalization of P
    for (int i = 0; i < n; i++)
      P[i] += (1.0 - P_1) / ((double) n);
  }

  free(J);
  free(TP);
  free(deg);

  return(P);
}


int* kCoreDec(Graph* g, edgelist* edges) {
  int* k_cores = malloc(g->n * sizeof(int));
  int c = 0;
  int i = g->n;

  while(i > 0) {
    unsigned int m_index = minimalDegree(g);
    Vertice* v = &g->v[m_index]; // vertice of minimal degree
    c = v->n > c ? v->n : c;     // c = max(c, deg(v))

    // printf("before removing DELETED : %lu vertice %lu\n", v->deleted, m_index);
    // fflush(stdout);
    // printf("vertice has %lu neighbors\n", v->n);
    // fflush(stdout);
    if(v->deleted == 1) {
      printf("PROBLEM LOL\n");
      for(unsigned int k = 0; k < g->n; k++) {
        if(!(g->v[k].n == -1 || g->v[k].n == 0)) {
          printf("vertice %lu has  neighbord\n", k);
          fflush(stdout);
        }
      }
    }
    // printf("end of check\n");
    // fflush(stdout);
    if(!v->deleted) {
      removeVertice(g, m_index);
      // printf("after rremoving vertice\n");
      // fflush(stdout);

      k_cores[m_index] = c;

    } else {
      i = 0;
    }
    i--;
    if(i % 10000 == 0) {
      printf("%ld\n", i);
      fflush(stdout);
    }
    if(i < 10000) {
      printf("%ld\n", i);
      fflush(stdout);
    }
  }

  return(k_cores);
}


void exercice1(char* filename, char* results_file, double alpha) {

  printf("\n -- Starting PageRank algorithm --\n\n");

  // loading the graph
  printf(" Reading graph \"%s\"\n", filename);
  fflush(stdout);

  edgelist* edges = readedgelist(filename);
  unsigned int n_nodes = edges->n;

  printf(" -- %lu nodes in the graph\n", n_nodes);

  // running the algorithm
  printf("\n Edgelist read, running the powerIteration algorithm\n");
  printf("\n - alpha = %f\n\n", alpha);
  fflush(stdout);

  double* prob_density = powerIteration(edges, alpha, 20);

  for(int i = 990; i < 1000; i++) {
    printf(" -- Prob for node %lu : %.10e \n", i, prob_density[i]);
  }
  fflush(stdout);

  // saving the results
  if(strcmp(results_file, "NONE")) {
    printf("\n Printing the results in \"%s\"\n", results_file);
    fflush(stdout);
    FILE* results = fopen(results_file, "w");

    /*
     * we save :
     *  - node id
     *  - pageRank
     *  - in-degree
     *  - out-degree
     */
    fprintf(results, "Node.ID;PageRank;In.Degree;Out.Degree\n");

    // computing the degrees
    double* in_degree  = inNodesDegree(edges);
    double* out_degree = nodesDegree(edges);

    // printing in the file
    for(int i = 0; i < n_nodes; i++) {
      fprintf(results, "%lu;", i);
      fprintf(results, "%.10e;", prob_density[i]);
      fprintf(results, "%f;", in_degree[i]);
      fprintf(results, "%f\n", out_degree[i]);
    }

    free(in_degree);
    free(out_degree);

    fclose(results);
  }

  // TODO : free the memory :)

}

void kCoreAlg(char* filename, char* output) {
  printf("\n -- Starting k-Core Algorithm --\n\n");

  // loading the graph
  printf(" Reading graph \"%s\"\n", filename);
  fflush(stdout);

  edgelist* edges = readedgelist(filename);
  unsigned int n_nodes = edges->n;

  printf(" -- %lu nodes in the graph\n", n_nodes);

  // running the algorithm
  printf("\n - Edgelist read, changing representation...\n");
  fflush(stdout);

  Graph* g = mutateEdgelist(edges);

  printf("\n -- Starting the k-Core Algorithm --\n");
  fflush(stdout);

  int* k_cores = kCoreDec(g, edges);

  printf("\n -- Algorithm terminated --\n");
  for(unsigned int i = 0; i < 100; i++) {
    printf(" -- K-Core for node %lu : %lu\n", i, k_cores[i]);
  }
  fflush(stdout);

  if(strcmp(output, "NONE")) {
    printf("\n Printing the results in \"%s\"\n", output);
    fflush(stdout);
    FILE* results = fopen(output, "w");

    /*
     * we save :
     *  - node id
     *  - core
     *  - degree
     */
    fprintf(results, "Node.ID;Degree;Core;N.Neigh\n");

    // computing the degrees
    double* in_degree  = inNodesDegree(edges);
    double* out_degree = nodesDegree(edges);

    g = mutateEdgelist(edges);

    // printing in the file
    for(int i = 0; i < n_nodes; i++) {
      fprintf(results, "%lu;", i);
      fprintf(results, "%f;", out_degree[i] + in_degree[i]);
      fprintf(results, "%lu;", k_cores[i]);
      fprintf(results, "%lu\n", g->v[i].n);
    }

    free(in_degree);
    free(out_degree);

    fclose(results);
  }
  return;
}
