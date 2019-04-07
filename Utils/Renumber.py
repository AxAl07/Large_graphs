import csv
import sys


# use :
# pyw.exe renumber.py fichier_graphe fichier_graphe_renumbered
if __name__ == "__main__":

    for ch in sys.argv:
        print(ch)

    f = open(sys.argv[1],'r', encoding='utf-8-sig', errors='ignore');
    reader = csv.reader(f, dialect='excel', delimiter='\t')

    
    out_graph = open(sys.argv[2],'w', newline='');
    out_writer = csv.writer(out_graph, delimiter=' ',quotechar='"', quoting=csv.QUOTE_MINIMAL)

    i = 0
    ids = {}
    for row in reader:
        key_1 = int(row[0]) # new_id ancien id (lu dans le fichier)
        key_2 = int(row[1])
        if key_1 not in ids :
            ids[key_1] = i # i nouveau id
            i = i+1
        if key_2 not in ids :
            ids[key_2] = i # i nouveau id
            i = i+1

        new_edge = [ids[key_1], ids[key_2]]
        out_writer.writerow(new_edge)

    # f.close()

    # in_graph = open(sys.argv[2],'r', encoding='utf-8-sig', errors='ignore');
    # in_reader = csv.reader(in_graph, dialect='excel', delimiter='\t')


    # for row in reader:
    #     source = int(row[0])
    #     target = int(row[1])

    #     # print(ids[source])
    #     # print(ids[target])

    #     new_edge = [ids[source], ids[target]]
    #     out_writer.writerow(new_edge)

    f.close()
    out_graph.close()


    # f = open(sys.argv[1],'r', encoding='utf-8-sig', errors='ignore');
    # reader = csv.reader(f, dialect='excel', delimiter='\t')
    #
    # with open(sys.argv[1] + ".CLEAN", encoding='utf-8-sig', mode='w') as clean_file:
    #     file_writer = csv.writer(clean_file, delimiter="\t", quotechar='"', quoting=csv.QUOTE_MINIMAL)
    #
    #     for row in reader:
    #         file_writer.writerow([int(ids[row[0]]), row[1]])
