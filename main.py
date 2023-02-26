from python_propre.TSPDData import TSPDData
from python_propre.TSPModel import TSPModel
from python_propre.TSPDSolution import TSPDSolution
from python_propre.TSPDModelSPCas1 import TSPDModelSPCas1
from python_propre.TSPDModelSPCas1V2 import TSPDModelSPCas1V2
from python_propre.TSPDModelSPCas1V3 import TSPDModelSPCas1V3
import time
import collections

def write_in_txt(data, order, name):
    with open("results_tsp_" + name + ".txt", 'w') as file:
        file.write("#indice client, latitude, longitude\n")
        for i in range(len(order)):
            lat, lon = data.get_node_location(order[i])
            lat = round(lat, 7)
            lon = round(lon, 7)
            file.write(str(order[i]) + " " + str(lat) + " " + str(lon) + "\n")

def tsp(data, filename="solTSP"):
    model = TSPModel(data)
    solution = TSPDSolution(data)
    start = time.time()
    obj, tour = model.solve()
    end = time.time()-start
    print("Time : " + str(end))
    solution.import_TSP(obj, tour)
    solution.export(name="results/"+filename)

if __name__ == "__main__":
    filename = "100"
    data = TSPDData("Data/" + filename)
    solution = TSPDSolution(data)
    solution.import_H1V1(filename)
    solution.to_map()
    filename = "50"
    data = TSPDData("Data/" + filename)
    instance = TSPDModelSPCas1V2(data,2, 2, 4, 1)
    instance.solve()#name_path = "50_1V2.txt", time_max = 30)
    a
    filename = "init"
    data = TSPDData("Data/" + filename)
    instance = TSPDModelSPCas1V3(data,2, 2, 2, 1)
    instance.solve()#name_path = "50_1V2.txt", time_max = 30)
    filename = "50"
    data = TSPDData("Data/" + filename)
    instance = TSPDModelSPCas1V2(data,2, 2, 2, 1)
    instance.solve(name_path = "50_1V2.txt", time_max = 30)
    filename = "30"
    data = TSPDData("Data/" + filename)
    instance = TSPDModelSPCas1V2(data,2, 2, 2, 1)
    instance.solve(name_path = "30_1V2.txt", time_max = 30)
    filename = "80"
    data = TSPDData("Data/" + filename)
    instance = TSPDModelSPCas1V2(data)
    instance.solve(name_path = "80_1V2.txt", time_max = 30, do_callbacks = False)
    filename = "100"
    data = TSPDData("Data/" + filename)
    instance = TSPDModelSPCas1V2(data)
    instance.solve(name_path = "100_1V2.txt", time_max = 30, do_callbacks = False)
    
    filename = "30"
    data = TSPDData("Data/" + filename)
    instance = TSPDModelSPCas1V2(data,2, 2, 4, 1)
    instance.solve(name_path = "30_2V2.txt", time_max = 30)
    filename = "50"
    data = TSPDData("Data/" + filename)
    instance = TSPDModelSPCas1V2(data,2, 2, 5, 1)
    instance.solve(name_path = "50_2V2.txt", time_max = 30)
    filename = "80"
    data = TSPDData("Data/" + filename)
    instance = TSPDModelSPCas1V2(data)
    instance.solve(name_path = "80_2V2.txt", time_max = 60)
    filename = "100"
    data = TSPDData("Data/" + filename)
    instance = TSPDModelSPCas1V2(data)
    instance.solve(name_path = "100_2V2.txt", time_max = 60)
    #data.draw_graph()
    #print(data.df_edges.loc[(data.df_edges['start_id']==2) | (data.df_edges['end_id']==2)])

    #tsp(data, "solTSP"+filename)


    #solution = TSPDSolution(data)

    #cas1 = TSPDModelSPCas1(data)
    #cas1.solve()
    #solution.display()
    #solution.export(name="results/sol")
    #solution.to_map(name="results/"+filename)
