from TSPDData import *
from parser import *
from TSPModel import *
from TSPDSolution import *

def write_in_txt(data, order, name):
    with open("results_tsp_" + name + ".txt", 'w') as file:
        file.write("#indice client, latitude, longitude\n")
        for i in range(len(order)):
            lat, lon = data.get_node_location(order[i])
            lat = round(lat, 7)
            lon = round(lon, 7)
            file.write(str(order[i]) + " " + str(lat) + " " + str(lon) + "\n")

if __name__ == "__main__":
    for filepath in ["30"]: #["init", "30", "50", "80", "100"]:
        data = TSPDData("Data/" + filepath)
        print(data.df_customers)
        model = TSPModel(data)
        solution = TSPDSolution(data)
        obj, tour = model.solve()
        solution.import_TSP(obj, tour)
        solution.to_map()
        #write_in_txt(data, solution.truck_tour, filepath)
