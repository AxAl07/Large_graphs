#define N_NODES 400

float uRand(void) {
  return (float) rand() / (float) RAND_MAX;
}

void createEdge(edgelist* edge_list, int i, int j, float e_prior) {
//  printf(" -- New edge : %lu -> %lu\n", i, j);
//  fflush(stdout);
  unsigned int e = edge_list->e++;

  edge_list->edges[e].t = i;
  edge_list->edges[e].s = j;

  if(edge_list->e > e_prior) {
    printf(" - Reallocating memory for the edge list\n");
    fflush(stdout);
    e_prior *= 2;
    edge_list->edges = realloc(edge_list->edges, e_prior * sizeof(edge));
  }
}

edgelist* randomGraph(float p, float q) {
  printf("\n -- Creating random graph --\n");
  printf(" - p = %f\n", p);
  printf(" - q = %f\n", q);
  fflush(stdout);

  unsigned int e_prior = (int) (40000.0 * p + 300000.0 * 4.0 * q);

  edgelist* edge_list = malloc(sizeof(edgelist));
  edge_list->n = N_NODES;
  edge_list->e = 0;
  edge_list->edges = malloc(e_prior * sizeof(edge));

  printf("\n - Creating the edges\n");
  fflush(stdout);

  for(unsigned int i = 0; i < N_NODES; i++) {
    for(unsigned int j = 0; j < i; j++) {
      if(i % 4 == j % 4) { // in the same cluster
        if(uRand() < p)
          createEdge(edge_list, i, j, e_prior);
      } else {
        if(uRand() < q)
          createEdge(edge_list, i, j, e_prior);
      }
    }
  }
  edge_list->edges = realloc(edge_list->edges, edge_list->e* sizeof(edge));
  return(edge_list);
}


void saveGraph(edgelist* edge_list, char* out) {
  printf("\n -- Saving graph to %s \n\n", out);

  FILE* out_file = fopen(out, "w");

  for(unsigned int i = 0; i < edge_list->e; i++) {
    unsigned int s = edge_list->edges[i].s;
    unsigned int t = edge_list->edges[i].t;
    fprintf(out_file, "%lu %lu\n", s, t);
  //  printf(" - Printing edge : %lu -> %lu\n", s, t);
  //  fflush(stdout);
  }
  printf("\n - Done printing file\n");
  fflush(stdout);

  fclose(out_file);
}

int* shuffle(int size) {
  printf(" - Shuffling the array\n");
  fflush(stdout);
  int* pos = malloc(size * sizeof(int));


  int* used = calloc(size, sizeof(int));

  int untouched = size;
  for(int i = 0; i < size; i++) {

    int k = rand() % untouched;

    int taken = 0;
    int free  = 0;

    while(free < k || used[free + taken]) {
      if(used[free + taken])
        taken++;
      else
        free++;
    }

    pos[i] = free + taken;
    used[free + taken] = 1;
    untouched--;

  }
  for(unsigned int i = 0; i < N_NODES; i++) {
    printf("%lu - ", pos[i]);
  }
  printf("\n - End of shuffle\n");
  fflush(stdout);

  return pos;
}

int randomMaximalIndex(int n, int* arr) {
  printf(" -- randomMaximalIndex\n");
  fflush(stdout);

  // finding the max in arr
  int max = 0;
  for(int i = 0; i < n; i++)
    if(arr[i] > max)
      max = arr[i];

  printf(" - Max foud : %lu\n", max);
  fflush(stdout);

  // all the possible indexes for the max
  int* maximas = malloc(n*sizeof(int));
  int number = 0;
  for(unsigned int i = 0; i < n; i++)
    if(arr[i] == max)
      maximas[number++] = i;

  printf(" - Number of max: %lu\n", number);
  fflush(stdout);

  int max_i = maximas[rand() % number];

  free(maximas);

  return max_i;

}
int dominantLabel(Graph* g, int* labels, int v_index) {
  int* n_per_label = calloc(g->n, sizeof(int));

  // writing the number of neighbors with each label
  Vertice* v = &g->v[v_index];
  for(int i = 0; i < v->n; i++)
    n_per_label[labels[v->neigh[i]]]++;

  int r =randomMaximalIndex(g->n, n_per_label);
  printf(" - dominant label for %lu : %lu", v_index, r);
  fflush(stdout);

  return r;
}

bool stopLabelProp(Graph* g, int* labels) {
  for(unsigned int i = 0; i < g->n; i++)
    if(dominantLabel(g, labels, i) != labels[i])
      return 0;

  return 1;
}

int* labelProp(Graph* g) {
  printf(" -- Running the label propagation algorithm --\n");
  printf(" - Initialization of the labels\n");
  fflush(stdout);

  // initialization of the labels
  int* labels = malloc(g->n * sizeof(int));
  for(int i = 0; i < g->n; i++) {
    labels[i] = i;
  }

  int iter = 0;


  while(!stopLabelProp(g, labels) && iter++ < 10) {
    printf(" - Shuffling shuffling\n");
    fflush(stdout);

    int* order = shuffle(g->n);

    printf(" - Computing the dominant label\n");
    fflush(stdout);

    for(int i = 0; i < g->n; i++)
      labels[order[i]] = dominantLabel(g, labels, order[i]);
/*
    tmp = labels;
    labels = new_labels;
    new_labels = tmp;
*/
    printf(" -- Iteration %lu\n ", iter);
    for(unsigned int i = 0; i < g->n; i++)
      printf(" - Node %lu has label : %lu\n", i, labels[i]);
    fflush(stdout);
  }
  printf(" - Number of iterations : %lu\n", iter);

  return(labels);

}
