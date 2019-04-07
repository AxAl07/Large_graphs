# Large_graphs

Nodes of all graphs have to be renamed before launching algorithms. You'll find a python code for this in Utils.
Graphs can be dowload at the following links :
http://snap.stanford.edu/data/email-Eu-core.html ; 
http://snap.stanford.edu/data/com-Amazon.html ; 
http://snap.stanford.edu/data/com-LiveJournal.html ; 
http://snap.stanford.edu/data/com-Orkut.html ; 


Algorithms from TP1 (Handling a large graph) can be found in folder handling_large_graphs.
To call a function : ./TP1 function_name graph_name (file_name)

- graph_name is a .txt file that includes the graph edges
- file_name is not necessary of all function : it is the name of .txt file where will be printed outputs of the algorithm. Functions with an (*) are the one that requieres a third parameter.
- function_name : name of the function you need to call, among the following ones :

      - size_graph
      - node_degree     (*)
      - special_quantity
      - degree_distribution    (*)
      - test_storing
      - connected_component
      - diameter_bound
      - count_triangles   (this one does not work well)
     
     
Algotrithms from TP2(PageRank and k-core) and TP3(Community detection) can be found in folder PageRank_Kcore_Labels.
To call a function, compile main.c then execute with the following arguments : name_function arg2 (arg3) (arg4) (arg5)

- graph_name is a .txt file that includes the graph edges
- function_name : name of the function you need to call, among the following ones :

      - pagerank
      - kcore
      - label_random
      - label_file
      
- For pagerank :  

              - arg2 = graph_name (.txt file that includes the graph edges)\\
              - arg3 = file_name  (.txt file where outputs will be printed)
              - arg4 = alpha
                 
- For kcore : 

              - arg2 = graph_name (.txt file that includes the graph edges)
              - arg3 = file_name
              
- For label_random (which create a random graph with communities) : 

              - arg2 = p
              - arg3 = q
              - arg4 = output_file (.txt)
              - arg5 = labels_file (.txt)
                                                          
- For label_file :  

              - arg2 = in_file (.txt)
              - arg3 = out_file (.txt)
                                                                  
                                                                    
                   

      
      
