#include <stdlib.h>
#include <stdio.h>
#include <time.h>//to estimate the runing time
#include <string.h>
#include "fifo.c"
#include "TP1.c"
#include "BFS.c"
#include "Triangles.c"
#include <sys/stat.h>
struct stat st;


// #define NLINKS 100000000 

int main(int argc, char** argv) {

	/*
	The first argument is the algorithm one wants to execute : size_graph, node_degree, special_quantity, degreedistibution, test_storing,
	The 2nd argument is the graph one wants to work on. 
	The 3rd argument is not always necessary : it the name file in which one one to write the result in
	*/

	clock_t begin = clock();

	if (strcmp(argv[1],"size_graph")==0){
		Size* s;
		printf("%s\n", argv[1]);
		s = size_graph(argv[2]);
		stat(argv[2], &st); /*give size of the file in bytes*/

		printf("Taile du fichier %u octets\n ", st.st_size);
		printf("Nombre de noeuds : %u\nNombre de connection : %u\n", s->Nb_n, s->Nb_e);
		free (s);
	}
	

	else if (strcmp(argv[1],"node_degree")==0){

		stat(argv[2], &st); 
		if (st.st_size < 1000000000){
			printf("graph entered in memory \n");
			edgelist* g;
		    g = readedgelist(argv[2]);

		    Compute_degree_write_file (g, argv[3]);
		    free_edgelist (g);
		}
		else{
			printf("graph is not putted in memory \n");
		    Compute_degree_write_file2 (argv[2], argv[3]);
		}
	} 

	else if (strcmp(argv[1],"special_quantity")==0){
		stat(argv[2], &st); 
		if (st.st_size < 1000000000){
			edgelist* g;
		    g = readedgelist(argv[2]);

			unsigned long long Q_g;
		    Q_g = Special_Quantity(g);
		    printf("Special Quantity equals %u\n", (int) Q_g);
		    free_edgelist (g);
		}
		else{

			unsigned long long Q_g;
		    Q_g = Special_Quantity2(argv[2]);
		    printf("Special Quantity equals %u\n", (int) Q_g);
		}
	}

	else if (strcmp(argv[1],"degree_distribution")==0){
		stat(argv[2], &st); 
		if (st.st_size < 1000000000){
			edgelist* g;
		    g = readedgelist(argv[2]);

			Degree_distribution (g, argv[3]);
			free_edgelist (g);
		}
		else{
			Degree_distribution2 (argv[2], argv[3]);
		}
	}


	else if (strcmp(argv[1],"test_storing")==0){

		Size* s;
		printf("%s\n", argv[1]);
		s = size_graph(argv[2]);

		adjarray* g1;
		g1 = read_store_adjarray(argv[2]);
		free_adjarray(g1);
		printf("agjacency array : storable in RAM \n");
		fflush(stdout);



		edgelist* g2;
		g2 = readedgelist(argv[2]);
		free_edgelist (g2);
		printf("edgelist : storable in RAM \n");
		fflush(stdout);


		int** g3;
		g3 = read_store_adjmat (argv[2]);
		free_adj_matrix(g3, s->Nb_n);
		printf("agjacency matrix : storable in RAM \n");
		fflush(stdout);

		free(s);
	}

	else if (strcmp(argv[1],"connected_component")==0){

		adjarray*  g;
		g = read_store_adjarray(argv[2]);

		BFS_component (g);

		free_adjarray (g);
	}

	else if (strcmp(argv[1],"diameter_bound")==0){
		adjarray*  g;
		g = read_store_adjarray(argv[2]);

		unsigned int u_init;
		unsigned int max_init = 50;//atoi(argv[3]);
		unsigned int max_bound = 0;
		unsigned int bound;

		for (int _=0; _<max_init; _++){
			u_init = rand() % g->n;
			// printf("u_init : %u\n", u_init);
			bound = lower_bound (g, u_init, 5);
			printf("bound : %u\n", bound);
			if (max_bound < bound){
				max_bound = bound;
			}
			printf("max_bound : %n", max_bound);
		}

		printf("Lower Bound to diameter : %u\n", max_bound);


		free_adjarray (g);
	}




	else if (strcmp(argv[1],"count_triangles")==0){
        adjarray*  g;
        g = read_store_adjarray(argv[2]);

        unsigned int * degrees = calloc(g->n, sizeof(unsigned int));

        for (int i = 0; i <g->n; i++) {
            degrees[i] =  g->cd[i+1] - g->cd[i];
        }

        unsigned int * indexes = malloc(g->n* sizeof(unsigned int));
        for (int i=0; i<g->n; i++)
            indexes[i]=i;

        unsigned int * sorted_indexes;

        sorted_indexes = bucketSort_indexes(degrees, indexes, g->n);


        adjarray*  g2;
        g2 = reindex(g, sorted_indexes);


        free (degrees);
        free(sorted_indexes);
        free(indexes);

        free_adjarray (g);

        g2 = sort_adjarray(g2);


        unsigned long Nb_triangles;
        Nb_triangles = count_triangles(g2);
        printf("Nb_triangles = %u\n", Nb_triangles);
        
        free_adjarray (g2);
    }



	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Execution Time %f seconds\n", time_spent);

    return(0);
}