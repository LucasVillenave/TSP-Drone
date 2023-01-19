import gurobipy as gp
from gurobipy import GRB
from TSPDData import *

class TSPDMIPModel:
    def __init__(self, instance):
        self.instance = instance

    def solve(self):
        dist = self.instance.distance_customers
        nb_clients = len(dist)


        model = gp.Model("mip")

        x = []
        u = []

        for i in range(nb_clients):
            x.append([])
            for j in range(nb_clients):
                x[i].append(model.addVar(vtype = GRB.BINARY, name = "x" + str(i) + "," + str(j)))

        for i in range(nb_clients):
            u.append(model.addVar(lb = 0.0, vtype = GRB.CONTINUOUS, name = "u" + str(i)))

        for i in range(nb_clients):
            sum1 = 0
            sum2 = 0
            for j in range(nb_clients):
                sum1 += x[i][j]
                sum2 += x[j][i]
            model.addConstr(sum1 == 1, name = "(1)" + str(i))
            model.addConstr(sum2 == 1, name = "(2)" + str(i))

        for i in range(nb_clients):
            for j in range(1, nb_clients):
                model.addConstr(u[i] + x[i][j] <= u[j] + (nb_clients - 1) * (1 - x[i][j]), name = "(3)" + str(i) + "," + str(j))


        obj = 0
        for i in range(nb_clients):
            for j in range(nb_clients):
                obj += x[i][j] * dist[i][j]

        model.setObjective(obj, GRB.MINIMIZE)
        model.optimize()

        val_obj = model.ObjVal
        sol_x = [-1] * nb_clients
        for i in range(nb_clients):
            for j in range(nb_clients):
                var = model.getVarByName("x"+str(i)+","+str(j))
                if var.X == 1:
                    sol_x[i] = j

        print(val_obj, sol_x)

