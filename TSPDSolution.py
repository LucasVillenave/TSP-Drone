from TSPDData import *

class TSPDSolution:
    def __init__(self, data):
        self.data = data
        self.value = -1
        #truck tour must be in the form : depot, each node visited, depot
        self.truck_tour = []
        #drone tour must be in the form : {time : [lift off node, delivery node, pick up node]}
        self.drone_tour = [{5: [2, 42, 2], 20: [15, 42, 15]}, {}]

    def import_TSP(self, obj, tour):
        self.value = obj
        if len(tour) < 1 :
            return
        self.truck_tour.append(tour[0])
        for i in range(1, len(tour)):
            for j in self.data.truck_shortest_path.get(tour[i-1]).get(tour[i]):
                if j != self.truck_tour[len(self.truck_tour)-1]:
                    self.truck_tour.append(j)

    def import_H1(self, obj, x, y):
        return None

    def import_file(self, file):
        with open(file, "r") as fd:
            line = fd.readline()
            #...
        fd.close()

    def export(self, name="solution"):
        with open(name + ".txt", 'w') as fd:
            fd.write("TEMPS ; EVENEMENT ; LOCALISATION\n")
            #...
        fd.close()

    def display(self):
        print("Value = ", self.value, "s")
        print("Truck tour :", self.truck_tour)
        print("Drone 0 :")
        for d in self.drone_tour[0].keys():
            print("\ttime : ", d, ", tour :", self.drone_tour[0].get(d))
        print("Drone 1 :")
        for d in self.drone_tour[1].keys():
            print("\ttime : ", d, ", tour :", self.drone_tour[1].get(d))
