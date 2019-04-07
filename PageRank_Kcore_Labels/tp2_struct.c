typedef struct {
  unsigned int n; // number of neighbors (can change in the algo)
  unsigned int *neigh;
  unsigned int i; // index of last added neighbor in neigh
  int deleted;
} Vertice;

typedef struct {
  unsigned int n; // number of nodes
  unsigned int e; // number of edges
  Vertice *v;     // all vertex
} Graph;



void printGraph(Graph* g) {
  printf("\n -- Printing Graph --\n\n");
  for(unsigned int i = 0; i < g->n; i++) {
    printf(" - Node %lu : ", i);

    Vertice v = g->v[i];

    for(unsigned int j = 0; j < v.n; j++) {
      printf("%lu ; ", v.neigh[j]);
    }
    printf("\n");
  }
  fflush(stdout);
}

unsigned int* nNeighbors(edgelist *edges) {
  unsigned int n = edges->n;
  unsigned int e = edges->e;
  unsigned int* neighbors = malloc(n * sizeof(unsigned int));

  // initializing to zero
  for(unsigned int i = 0; i < n; i++)
    neighbors[i] = 0;

  // looping on edges, adding 1 to each vertice of the edge
  for(unsigned int i = 0; i < e; i++) {
    unsigned int s = edges->edges[i].s;
    unsigned int t = edges->edges[i].t;

    neighbors[s] ++;
    neighbors[t] ++;
  }

  return neighbors;
}

void fillNeighbors(Graph* g, edgelist* edges) {
  unsigned int e = edges->e;

  for(unsigned int i = 0; i < e; i++) {
    unsigned int s = edges->edges[i].s;
    unsigned int t = edges->edges[i].t;

    Vertice* source = &g->v[s];
    Vertice* target = &g->v[t];

    source->neigh[(source->i)++] = t;
    target->neigh[(target->i)++] = s;
  }
}

Graph* mutateEdgelist(edgelist* edges) {
  unsigned int n = edges->n;
  unsigned int e = edges->e;

  printf("\n -- Mutating the EdgeList to a Graph --\n\n");
  printf(" /!\\ The Graph must be non-directed with no doubled edges\n");
  fflush(stdout);

  // computing the number of neighbors for each node
  unsigned int* neighbors = nNeighbors(edges);

  printf("WOLALALA\n\n");
  fflush(stdout);

  printf("sizeof(Graph) : %lu \n", sizeof(Graph));
  fflush(stdout);
  // initializing the Vertice elements of Graph
  Graph *g = malloc(sizeof(Graph));


  printf("neigh computed\n######\n");
  fflush(stdout);
  g->n = n;
  g->e = e;
  g->v = malloc(n * sizeof(Vertice));

  printf("\nneigh computed");
  fflush(stdout);

  printf("so far so good\n");
  fflush(stdout);
  // initialization of the neighbors arrays of the vertices
  for(unsigned int i = 0; i < n; i++) {
    Vertice* v = &g->v[i];
    v->n = neighbors[i];
    v->neigh = malloc(v->n * sizeof(unsigned int));
    v->i = 0;
    v->deleted = 0;
    for(unsigned int j = 0; j < v->n; j++)
      v->neigh[j] = 0;
  }

  // filling the neighbors arrays
  fillNeighbors(g, edges);

  printf (" -- Graph mutated -- \n");
  fflush(stdout);

  //printGraph(g);

  return(g);
}

unsigned int minimalDegree(Graph* g) {
  unsigned int m_index = 0;
  unsigned int m_neigh = -2;

  for(unsigned int i = 0; i < g->n; i++) {
    Vertice* v = &g->v[i];
  //  printf("- Vertice %lu - %lu neighbors\n", i, v->n);
    //fflush(stdout);
    if(v->n < m_neigh && v->n > 0) {
      m_neigh = v->n;
      m_index = i;
    }

  }

  return m_index;
}

void printVertice(Graph* g, unsigned int v) {

  Vertice* ver = &g->v[v];


  printf("   - State of the vertice %lu - %lu neighbors\n", v, ver->n);
  for(unsigned int i = 0; i < ver->n; i++) {
    printf("    -> Neighbor %lu\n", ver->neigh[i]);
  }
  fflush(stdout);
}
Graph* removeVertice(Graph* g, unsigned int v) {

  Vertice* ver = &g->v[v];
  ver->deleted = 1;
  g->e -= ver->n;
// printf("Number of neighbors : %lu\n", ver->n);
// fflush(stdout);
//  printVertice(g, v);


  unsigned int* neigh = ver->neigh;

  // removing the vertex from the neighbors arrays

  for(unsigned int i = 0; i < ver->n; i++) {
   // printf("--- Dealing with vertice %lu\n", neigh[i]);
   // printVertice(g, neigh[i]);
    Vertice* v_nei = &g->v[neigh[i]]; // i-th neighbor of v
    // printf("neighbor %lu has %lu neighbors\n", neigh[i], v_nei->n);
    // fflush(stdout);
    v_nei->n--;

    unsigned int* new_neighbors = malloc(v_nei->n * sizeof(unsigned int));
    unsigned int found = 0;
    for(unsigned int j = 0; j <= v_nei->n; j++) { // <= bc we skip v
      if(v_nei->neigh[j] != v) {
        // printf("j = %lu ; n = %lu\n", j, v_nei->n);
        // fflush(stdout);
        new_neighbors[j - found] = v_nei->neigh[j];
        // printf("j = %lu ; n = %lu\n", j, v_nei->n);
        // fflush(stdout);
        // printf("My new neighbor is : %lu\n",  v_nei->neigh[j]);
      } else {
        found = 1;
      }
    }
    if(found == 0) {
      printf("not found for %lu : %lu\n", v, neigh[i]);
      fflush(stdout);
      scanf("%lu", &found);
    }

    free(v_nei->neigh);
    v_nei->neigh = new_neighbors;


  //  printVertice(g,neigh[i]);

  }



  free(neigh);

  ver->n = -1; // so it's never scanned again

  return(g);
}
