// Partie 3 : Exo 8 :
// Implémentation de la structure FIFO faite dans un ficher séparé : fifo.c

// Algorithme BFS :

unsigned int* BFS (adjarray *G, unsigned int n, unsigned int* marked_nodes, int n_conex){
    /* Inputs : G : ajacency array 
                n : node number : the node algorrithm start from
                marked_nodes : array of size (G->n, sizeof(unsigned int)) containing 0 if the node is not marked yet, an integer if it is marked
                n_conex : intger : the number of conex component that has already been gone trough
        Outputs : updates marked_nodes array.
    */

    // On verifie que n appartienne à G:
    if (G->n<n){
        fprintf(stderr, "node %u does not belong to graph! Exiting...\n", n);
        exit(-1);
    }
    
    // unsigned int* marked_nodes = calloc(G->n, sizeof(unsigned int));
    FIFO *fifo = initialize_fifo();
    add (fifo, n);
    marked_nodes[n] = n_conex;
    int N_passed_nodes = 1;

    int max_i;
    int u;

    while (fifo->first != NULL){
        u = pop(fifo);

        for (int j=G->cd[u]; j<G->cd[u+1]; j++){

            if  (marked_nodes[G->adj[j]]==0){
                add(fifo,G->adj[j]);
                marked_nodes[G->adj[j]] = n_conex;
                N_passed_nodes += 1;
            }   
        }     
    }


    free_fifo(fifo);
    return (marked_nodes);
}



unsigned int * max_array_2 (unsigned int * array, int len_array){

    unsigned int* a = malloc(2*sizeof(unsigned int));

    if (array[0]>array[1]){
        a[0] = array[0];
        a[1] = array[1];
    }
    else{
        a[0] = array[1];
        a[1] = array[0];
    }


    for (int i=2; i<len_array; i++){
        if (a[0]<array[i]){
            a[1] = a[0];
            a[0] = array[i];
        }
        if (a[0]>=array[i] && a[1]<array[i]){
            a[1] = array[i];
        }
    }

    return(a);
}


unsigned int max_array_1 (unsigned int * array, int len_array){

    unsigned int a = array[0];

    for (int i=1; i<len_array; i++){
        if (a<array[i])
            a = array[i];
    }

    return(a);
}


unsigned int min_array_1 (unsigned int * array, int len_array){

    unsigned int a = array[0];

    for (int i=1; i<len_array; i++){
        if (a>array[i])
            a = array[i];
    }

    return(a);
}




// Algorithme Largest_Conexe_Component : BFF_component
void BFS_component (adjarray *G){
    /* Inputs : adjacency array G
        Ouputs : no output : print largest connex componant size, number of connex componants ans number of connex componants of size 1
        Can print the components themself if one un-comment the lines inside.
    */

    // initialization of the list marked_nodes
    unsigned int* marked_nodes = calloc(G->n, sizeof(unsigned int));

    // Pour la boucle : on commence au noeud numéroté 1 puis tant que le noeud n'est pas marqué on continue
    // On attribue un entier strictement positif à chaque composante connexe
    int i = 0;
    int n_conex = 1;
    while (i <= G->n){
        if (marked_nodes[i] == 0){
            marked_nodes = BFS (G, i, marked_nodes, n_conex);
            i++;
            n_conex++;
        }
        else{
            i++;
        }
    }

    n_conex -=2; 

    printf("Nombre de composantes connexes = %u\n", n_conex);
    unsigned int* i_conex = calloc(n_conex, sizeof(unsigned int));


    for (int j=0; j<G->n; j++){
        i_conex[marked_nodes[j]-1]++;
    }

    unsigned int* max = max_array_2 (i_conex, n_conex);
    // unsigned int** passed_nodes = (unsigned int **)calloc(n_conex, sizeof(unsigned int*));

    // for (int j=0; j<n_conex; j++){
    //     passed_nodes[j] = (unsigned int*) malloc(max[0]+1 * sizeof(unsigned int));
    // }

    // for (int j=0; j<G->n; j++){
    //     passed_nodes[marked_nodes[j]-1][i_conex[marked_nodes[j]-1]] = j;
    // }


    // If one want to display all passed nodes
    // for (int j=0; j<n_conex; j++){
    //     printf("%u th connected component : size : %u\n Nodes : ", j+1, i_conex[j]);
    //     for (int k=0; k<i_conex[j]-1; k++){
    //         printf("%u, ", passed_nodes[j][k]);
    //     }
    //     printf("%u\n", passed_nodes[j][i_conex[j]]);
    // }

    unsigned int Nb_comp_1 = 0;

    for (int j=0; j<n_conex; j++){
        if (i_conex[j] ==1)
            Nb_comp_1++;
    }

    printf("Taille de la plus grande composente connexte : %u\n", max[0]);
    printf("Taille de la seconde plus grande composente connexte : %u\n", max[1]);
    printf("Nombre de composantes de taille 1 : %u\n", Nb_comp_1);


    // for (int j=0; j<n_conex; j++){
    //     free(passed_nodes[j]);
    // }
    // free(passed_nodes);
    free(marked_nodes);
    free(i_conex);

}




unsigned int lower_bound (adjarray *G, unsigned int n, unsigned int max_iterations){
    /* Modified version od BFS to cound the distance from the initial node to all other nodes.
        Inputs : G : ajacency array 
                n : node number : the node algorrithm start from
                max_iteration: the number of iteration to do : an iteratio is a restart of the algorithm from the last visited nodes of the previous one.
        Output : the maximum distance encountered
    */   

    // On verifie que n appartienne à G:
    if (G->n<n){
        fprintf(stderr, "node %u does not belong to graph! Exiting...\n", n);
        exit(-1);
    }

    unsigned int dist = 0;
    unsigned int count = 1;
    unsigned int new_count =0;

    unsigned int* marked_nodes = calloc(G->n, sizeof(unsigned int));
    // unsigned int* list_distances = calloc(G->n, sizeof(unsigned int));
    
    // unsigned int* marked_nodes = calloc(G->n, sizeof(unsigned int));
    FIFO *fifo = initialize_fifo();
    add (fifo, n);

    // int max_i;
    int u;
    unsigned int max_dist =0;

    for (int _=0; _<max_iterations; _++){

        while (fifo->first != NULL){
            u = pop(fifo);

            for (int j=G->cd[u]; j<G->cd[u+1]; j++){

                if  (marked_nodes[G->adj[j]]==0){
                    add(fifo,G->adj[j]);
                    marked_nodes[G->adj[j]] = 1;
                    new_count ++;
                }   
            }
            
            count --;
            if (count==0){
                dist ++;
                count = new_count;
                new_count = 0;
            } 
        }

        max_dist = (max_dist>dist) ? max_dist : dist; 
        add (fifo, u);


        dist = 0;
        count = 1;
        new_count =0;

        for (int i=0; i<G->n; i++){
            marked_nodes[i] = 0;
        }

    }
 

    free_fifo(fifo);
    free(marked_nodes);
    return (max_dist);
}