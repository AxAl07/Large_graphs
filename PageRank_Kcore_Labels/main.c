#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "edgelist.c"
#include "tp2_struct.c"
#include "tp3_functions.c"
#include "tp2_functions.c"


int main(int argc, char** argv) {
  srand(time(NULL));
  if(argc <= 1) {
    printf("\n Error: No arguments given\n\n");
    printf(" Usage: ./graphs pagerank graph_input [results] [alpha]\n");
    printf("\n or \n\n");
    printf("        ./tp2.exe kcore graph_input results\n");
    printf("\n or \n\n");
    printf("        ./tp2.exe label_random graph_output results\n");
    printf("\n or \n\n");
    printf("        ./tp2.exe label_file graph_input results\n");
    printf("\n or \n\n");
    printf("        ./tp1.exe input_file\n");

  }
  if(!strcmp(argv[1], "tp1")) {
    char* input_file = argv[2];

    /*
     * TON CODE ICI
     */


  }
  if(!strcmp(argv[1], "label_random")) {
    float p = atof(argv[2]);
    float q = atof(argv[3]);
    char* out_file    = argv[4];
    char* labels_file = argv[5];

    edgelist* edge_list = randomGraph(p, q);

    printf("\n -- Random graph created -- \n");
    printf(" - %lu nodes\n", edge_list->n);
    printf(" - %lu edges\n", edge_list->e);
    fflush(stdout);

    saveGraph(edge_list, out_file);


    printf("\n -- Mutating the graph --\n");
    Graph* g = mutateEdgelist(edge_list);

    printf("\n -- Graph mutated --\n");

    int* labels = labelProp(g);

    printf("\n -- End of program -- \n");
    fflush(stdout);

    FILE* lb_file = fopen(labels_file, "w");
    fprintf(lb_file, "Node;Label\n");
    for(unsigned int i = 0; i < g->n; i++)
      fprintf(lb_file, "%lu;%lu\n", i, labels[i]);

    fclose(lb_file);

    fflush(stdout);
  } else if(!strcmp(argv[1], "label_file")) {
    char* in_file  = argv[2];
    char* out_file = argv[3];

    edgelist* edge_list = readedgelist(in_file);

    printf("\n -- Input file \"%s\" read -- \n", in_file);
    printf(" - %lu nodes\n", edge_list->n);
    printf(" - %lu edges\n", edge_list->e);
    fflush(stdout);


    printf("\n -- Mutating the graph --\n");
    Graph* g = mutateEdgelist(edge_list);

    printf("\n -- Graph mutated --\n");

    int* labels = labelProp(g);

    printf("\n -- End of program -- \n");
    fflush(stdout);

    FILE* lb_file = fopen(out_file, "w");
    fprintf(lb_file, "Node;Label\n");
    for(unsigned int i = 0; i < g->n; i++)
      fprintf(lb_file, "%lu;%lu\n", i, labels[i]);

    fclose(lb_file);

    fflush(stdout);
  }
  if(!strcmp(argv[1], "pagerank")) {
    if(argc > 2) {
      if(argc > 4)
        exercice1(argv[2], argv[3], atof(argv[4]));
      else if (argc > 3)
        exercice1(argv[2], argv[3], 0.15);
      else
        exercice1(argv[2], "NONE", 0.15);
    }
    else {
      printf("\n Error: No arguments given\n\n");
      printf(" Usage: ./tp2.exe pagerank graph_file [results_file] [alpha]\n\n");
    }
  } else if(!strcmp(argv[1], "kcore")){
      if(argc > 3) {
        kCoreAlg(argv[2], argv[3]);
      } else {
        kCoreAlg(argv[2], "NONE");
      }
  }

  return (0);
}
