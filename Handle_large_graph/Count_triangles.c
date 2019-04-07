// Function to sort arr of size n using bucket sort 
unsigned int * bucketSort(unsigned int * array, int s) { 
/* This algorithm sort an array of intergers using th e bin sort mechanism
    Inputs : array : array of integers
            s : integer : size of array and number of buckets
    Output : sorted_array : a sorted version of the input array
*/
    
    unsigned int min = min_array_1(array, s);
    unsigned int max = max_array_1(array, s);

    unsigned int * sorted_array = malloc (s* sizeof(unsigned int));

    unsigned int** buckets = (unsigned int **)calloc(s, sizeof(unsigned int*));
    for(int i = 0; i < s; i++) {buckets[i] = (unsigned int *)calloc(s, sizeof(int));}

    unsigned int * size_buckets = calloc(s, sizeof(unsigned int));

    int bi;
    for (int i=0; i<s; i++){
        bi = s*(array[i]-min)/(max-min);
        bi=(bi>1) ? bi : 1;
        buckets[bi-1][size_buckets[bi-1]] = array[i];
        size_buckets[bi-1]++ ;
    }
    

    for (int i=0; i<s; i++){
        if (size_buckets[i] > 1){
            buckets[i] = bucketSort(buckets[i], size_buckets[i]);
        }
    }

    int k = 0;
    for (int i=0; i<s; i++){
        for (int j=0; j<size_buckets[i]; j++){
            sorted_array[k] = buckets[i][j];
            k++;
            // printf("%u\n", sorted_array[k-1]);
        }
    }

    for (int i=0; i<s; i++){
        free(buckets[i]);
    }
    free(buckets);
    free(size_buckets);

    return (sorted_array);
}



adjarray* sort_adjarray(adjarray *G){
    /* This algorithm use bucket_sort to sort the list of neighbours for each node of an adjacency array
        Input : G adjacency array
        Output : G adjacency array, in which each list of neighbours is sorted
    */

    unsigned int n = G->n;
    printf("G->n : %u\n", n);

    unsigned int* array_u = malloc(100*sizeof(unsigned int));

    for(int u=0; u<n; u++){
        array_u = realloc(array_u, (G->cd[u+1]-G->cd[u]+1) *sizeof(unsigned int));
        for (int v=G->cd[u]; v<G->cd[u+1]; v++){
            array_u[v-G->cd[u]] = G->adj[v];
        }

        if (G->cd[u+1]-G->cd[u]>1) {
            array_u = bucketSort (array_u, G->cd[u+1]-G->cd[u]);
        }

        for (int v=G->cd[u]; v<G->cd[u+1]; v++){
            G->adj[v] = array_u[v-G->cd[u]];
        }
    }

    free(array_u);

    return(G);
}


unsigned int * bucketSort_indexes(unsigned int * array, unsigned int * indexes, int s) {
    /* 
    This function return the indexes to make the correspondance between an array and the same sorted array
    The indexes are sorted in the decreasing order.
    */
    unsigned int min = min_array_1(array, s);
    unsigned int max = max_array_1(array, s);

    unsigned int * sorted_indexes = malloc (s* sizeof(unsigned int));

    unsigned int** buckets = (unsigned int **)calloc(s, sizeof(unsigned int*));
    for(int i = 0; i < s; i++) {buckets[i] = (unsigned int *)calloc(s, sizeof(int));}

    unsigned int** bucket_index = (unsigned int **)calloc(s, sizeof(unsigned int*));
    for(int i = 0; i < s; i++) {bucket_index[i] = (unsigned int *)calloc(s, sizeof(int));}

    unsigned int * size_buckets = calloc(s, sizeof(unsigned int));


    if (max==min){
        return(indexes);
    }


    int bi;
    for (int i=0; i<s; i++){
        bi = s*(array[i]-min)/(max-min);
        bi=(bi>1) ? bi : 1;
        buckets[bi-1][size_buckets[bi-1]] = array[i];
        bucket_index[bi-1][size_buckets[bi-1]] = indexes[i];
        size_buckets[bi-1]++ ;    
    }
    


    for (int i=0; i<s; i++){
        if (size_buckets[i] > 1){
            bucket_index[i] = bucketSort_indexes(buckets[i], bucket_index[i], size_buckets[i]);
        }
    }

    // Reprint in deacreasing order !
    int k = s-1;
    for (int i=0; i<s; i++){
        for (int j=0; j<size_buckets[i]; j++){
            sorted_indexes[k] = bucket_index[i][j];
            k--;
            fflush(stdout);
        }
    }

    for (int i=0; i<s; i++){
        free(buckets[i]);
        free(bucket_index[i]);
    }
    free(buckets);
    free(bucket_index);
    free(size_buckets);

    return (sorted_indexes);
}


adjarray * reindex(adjarray* G, unsigned int * sorted_indexes){

    adjarray *arr = malloc(sizeof(adjarray));
    arr->n = G->n;
    arr->e = G->e;
    arr->cd = calloc(G->n +1, sizeof(unsigned long));
    arr->adj = calloc(arr->e, sizeof(unsigned int));
        
    for (int i = 0; i <G->n; i++) {
        arr->cd[i+1] =  G->cd[sorted_indexes[i]+1]-G->cd[sorted_indexes[i]];
    }  


    unsigned int k = 0;
    for (int i=0; i<arr->n; i++){
        for (int j=0; j<arr->cd[i+1]; j++){
            arr->adj[k] = G->adj[G->cd[sorted_indexes[i]]+j];
            k++;
        }
    }


    for (int i=1; i<arr->n+1; i++){
        arr->cd[i] = arr->cd[i-1] + arr->cd[i];
    }

    return (arr);

}


unsigned long count_triangles(adjarray *G){
    /* G->adj has to be reindexed and sorted before entering the algorithm
    */

    unsigned int n = G->n;
    unsigned long * cd = G->cd;
    // c : couting of triangles
    unsigned long c = 0;
    unsigned int u;
    unsigned int v;
    unsigned int trunc_u_index = 0;
    unsigned int trunc_v_index = 0;

    for (unsigned int u=0; u<n; u++){
      
        for (unsigned int j=cd[u]; j<cd[u+1]; j++){
                v = G->adj[j];
               
                trunc_u_index = 0;
                trunc_v_index = 0;
                while (G->adj[cd[v]+trunc_v_index]<=v && trunc_v_index<cd[v+1]-cd[v]){
                    trunc_v_index ++;
                }
                while(G->adj[cd[u]+trunc_u_index]<u && trunc_u_index<cd[u+1]-cd[u] && trunc_v_index<cd[v+1]-cd[v]){
                    if (G->adj[cd[u]+trunc_u_index] < G->adj[cd[v]+trunc_v_index]){
                        trunc_u_index ++;
                    }
                    else if (G->adj[cd[u]+trunc_u_index] > G->adj[cd[v]+trunc_v_index]){
                        trunc_v_index ++;
                    }
                    else {c ++; trunc_u_index++; trunc_v_index++;}
                }
        }

    }

    free(cd);
    return (c);
}