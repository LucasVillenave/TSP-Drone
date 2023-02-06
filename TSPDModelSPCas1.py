import gurobipy as gp
from gurobipy import GRB
from TSPDData import *

class TSPDModelSPCas1:
    def __init__(self, data):
        self.data = data
        self.five_closest_neighbors = self.get_five_closest_neighbors()
        print(self.five_closest_neighbors)
        a



    def get_five_closest_neighbors(self):
        compact_graph = self.data.truck_shortest_path
        closest_clients = []
        for i in range(len(compact_graph)):
            closest_clients.append(dict(sorted(compact_graph[i].items(), key=lambda item: item[1])[:6]))
        
    def solve(self):
        compact_graph_path = self.data.truck_shortest_path
        compact_graph_values = self.data.truck_shortest_time
        nb_clients = len(compact_graph_values)
        drone_graph = self.data.drone_time
        nb_nodes = len(drone_graph)
        dist_truck = {(i, j, t):
                compact_graph_values[i][j]
                #math.sqrt(sum((instance.positions[instance.indices_customers[i]][k]-instance.positions[instance.indices_customers[j]][k])**2 for k in range(2)))  #remplacer ligne précédente par celle-là pour farie avec dist à vol d'oiseau
                for i in range(nb_clients) for j in range(nb_clients) for t in range(nb_clients)}
        dist_drone = {(a, i, j, t):
                drone_graph[i][j]
                #math.sqrt(sum((instance.positions[instance.indices_customers[i]][k]-instance.positions[instance.indices_customers[j]][k])**2 for k in range(2)))  #remplacer ligne précédente par celle-là pour farie avec dist à vol d'oiseau
                for a in range(2) for i in range(nb_nodes) for j in range(nb_clients) for t in range(nb_clients)}


        model = gp.Model("mip")

        x = model.addVars(dist_truck.keys(), obj=dist_truck, vtype=GRB.BINARY, name='x')
        w = model.addVars(dist_drone.keys(), obj=dist_drone, vtype=GRB.BINARY, name='w')
        #u = model.addVars(2, nb_nodes, nb_clients, vtype=GRB.CONTINUOUS, name='u')
        #v = model.addVars(2, nb_nodes, nb_clients, vtype=GRB.INTEGER, name='v')
        #mommaU = model.addVars(nb_nodes, nb_clients, obj=1, vtype=GRB.CONTINUOUS, name='mommaU')
        
        
        print("variables done")

        #FLOT TRUCK
        for i in range(1, nb_clients):
            for t in range(nb_clients - 1):
                sum = 0
                for j in range(nb_clients):
                    sum += x[j,i,t]
                    sum -= x[i,j,t+1]
                model.addConstr(sum == 0, name = "(1)" + str(i) + "," + str(t))


        sum = 0
        for i in range(nb_clients):
            sum += x[0,i,0]
        model.addConstr(sum == 1, name = "(1bis)")

        sum = 0
        for i in range(nb_clients):
            for t in range(nb_clients):
                sum += x[i,0,t]
        model.addConstr(sum == 1, name = "(1ter)")

        print("flot done")

        #SATISFACTION CLIENT
        for i in range(nb_clients):
            sum = 0
            for t in range(nb_clients):
                for j in range(nb_clients):
                    sum += x[i,j,t]
                for a in range(2):
                    for j in range(nb_nodes):
                        sum += w[a,j,i,t]
            model.addConstr(sum == 1, name = "(2)" + str(i))

        print("satisfaction done")


        #DEPART TRAJET DRONE SUR NOEUD VISITE PAR TRUCK
        for i in range(nb_nodes):
            for j in range(nb_clients):
                for t in range(nb_clients):
                    for a in range(2):
                        sum = 0
                        for l in range(nb_clients):
                            for k in range(nb_clients):
                                if i in compact_graph_path[l][k]:
                                    sum += x[l,k,t]
                        model.addConstr(w[a,i,j,t] <= sum, name = "(3)" + str(a) + "," + str(i) + "," + str(j) + "," + str(t))

        print("depart drone faisabilité done")

        #PEUT VISITER SEULEMENT 5 CLIENTS LES PLUS PROCHES DEPUIS UN SOMMET
        model.addConstrs((x[i,j,t] == 0 for i in range(nb_clients)
                            for j in range(nb_clients)
                            for t in range(nb_clients)
                            if j != j), name='c')


        model.optimize()

        print("obj : ", model.ObjVal)

        vals_x = model.getAttr('X', x)
        print("truck:")
        for key in vals_x.keys():
            if vals_x[key] > 0.5:
                print(key)

        vals_w = model.getAttr('X', w)
        print("drones:")
        for key in vals_w.keys():
            if vals_w[key] > 0.5:
                print(key)



