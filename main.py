from python_propre.TSPDData import TSPDData
from python_propre.TSPModel import TSPModel
from python_propre.TSPDSolution import TSPDSolution
from python_propre.TSPDModelSPCas1 import TSPDModelSPCas1
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

if __name__ == "__main__":
    filename = "init"
    data = TSPDData("Data/" + filename)
    #data.display()
    print(data.df_customers)
    #model = TSPModel(data)
    #solution = TSPDSolution(data)

    cas1 = TSPDModelSPCas1(data)
    cas1.solve()

    #start = time.time()
    #obj, tour = model.solve()
    #end = time.time()-start

    #solution.import_TSP(obj, tour)
    #solution.export(name="results/sol")
    #solution.to_map(name="results/"+filename)
    #write_in_txt(data, solution.truck_tour, filename)
