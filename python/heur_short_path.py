import gurobipy as gp
from gurobipy import GRB
from TSPDData import *

class heur_short_path:
    def __init__(self, instance):
        self.instance = instance
        
    def solve(self):
        dist = self.instance.distance_customers
        nb_clients = len(self.instance.distance_customers)
        nb_nodes = len(self.instance.positions)


        model = gp.Model("mip")

        x = []
        w = []
        u = []

        for i in range(nb_clients):
            x.append([])
            for j in range(nb_clients):
                x[i].append([])
                for t in range(nb_clients):
                    x[i][j].append(model.addVar(vtype = GRB.BINARY, name = "x" + str(i) + "," + str(j) + "," + str(t)))

        for a in range(2):
            w.append([])
            for i in range(nb_nodes):
                w[a].append([])
                for j in range(nb_clients):
                    w[a][i].append([])
                    for t in range(nb_clients):
                        w[a][i][j].append(model.addVar(vtype = GRB.BINARY, name = "w" + str(a) + "," + str(i) + "," + str(j) + "," + str(t)))

        print("variables done")

        #FLOT TRUCK
        for i in range(1, nb_clients):
            for t in range(nb_clients - 1):
                sum = 0
                for j in range(nb_clients):
                    sum += x[j][i][t]
                    sum -= x[i][j][t+1]
                model.addConstr(sum == 0, name = "(1)" + str(i) + "," + str(t))

        sum = 0
        for i in range(nb_clients):
            sum += x[0][i][0]
        model.addConstr(sum == 1, name = "(1bis)")

        sum = 0
        for i in range(nb_clients):
            for t in range(nb_clients):
                sum += x[i][0][t]
        model.addConstr(sum == 1, name = "(1ter)")

        print("flot done")

        #SATISFACTION CLIENT
        for i in range(nb_clients):
            sum = 0
            for t in range(nb_clients):
                for j in range(nb_clients):
                    sum += x[i][j][t]
                for a in range(2):
                    for j in range(len(self.instance.positions)):
                        sum += w[a][j][i][t]
            model.addConstr(sum == 1, name = "(2)" + str(i))

        print("satisfaction done")


        #DEPART TRAJET DRONE SUR NOEUD VISITE PAR TRUCK
        for i in range(len(self.instance.positions)):
            for j in range(nb_clients):
                for t in range(nb_clients):
                    for a in range(2):
                        sum = 0
                        for l in range(nb_clients):
                            for k in range(nb_clients):
                                if i in self.instance.shortest_path_customers[l][k]:
                                    sum += x[l][k][t]
                        model.addConstr(w[a][i][j][t] <= sum, name = "(3)" + str(a) + "," + str(i) + "," + str(j) + "," + str(t))

        print("depart drone faisabilité done")

        obj = 0
        for i in range(nb_clients):
            for j in range(nb_clients):
                for t in range(nb_clients):
                    obj += x[i][j][t] * dist[i][j]

        for i in range(len(self.instance.positions)):
            for j in range(nb_clients):
                id_cust = self.instance.indices_customers[j]
                lat1 = self.instance.positions[i][0]
                lon1 = self.instance.positions[i][1]
                lat2 = self.instance.positions[id_cust][0]
                lon2 = self.instance.positions[id_cust][1]
                time_to_travel_i_j = geopy.distance.geodesic((lat1, lon1), (lat2,lon2)).m / (50/3.6)
                for a in range(2):
                    for t in range(nb_clients):
                        obj += w[a][i][j][t] * time_to_travel_i_j

        print("obj done")

        model.setObjective(obj, GRB.MINIMIZE)
        model.optimize()

        val_obj = model.ObjVal
        sol_x = []
        for i in range(nb_clients):
            for j in range(nb_clients):
                for t in range(nb_clients):
                    var = model.getVarByName("x"+str(i)+","+str(j)+","+str(t))
                    if var.X == 1:
                        sol_x.append([i,j,t])
        sol_w = []
        for a in range(2):
            for i in range(len(self.instance.positions)):
                for j in range(nb_clients):
                    for t in range(nb_clients):
                        var = model.getVarByName("w"+str(a)+","+str(i)+","+str(j)+","+str(t))
                        if var.X == 1:
                            sol_w.append([a,i,j,t])

        print(val_obj, sol_x, sol_w)

