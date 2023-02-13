import gurobipy as gp
from gurobipy import GRB
import numpy as np
from python_propre.TSPDData import TSPDData

class TSPDModelSPCas1:
    def __init__(self, data):
        self.data = data
        self.five_closest_neighbors = self.get_five_closest_neighbors()
        self.list_customers = self.data.get_demand_nodes()
        self.intersections = self.get_possible_intersections()
        print(self.intersections)
        a

    def get_five_closest_neighbors(self):
        compact_graph = self.data.get_compact_graph()
        closest_clients = []
        for i in range(len(compact_graph)):
            closest_clients.append(dict(sorted(compact_graph[i].items(), key=lambda item: item[1])[:6]))
        return closest_clients

    def get_possible_intersections(self):
        compact_graph_path = self.data.get_shortest_path_graph()
        return np.unique(np.concatenate(np.array([np.array(compact_graph_path[set_paths][path]) for set_paths in compact_graph_path for path in compact_graph_path[set_paths]])))



    def solve(self):
        compact_graph_path = self.data.get_shortest_path_graph()
        compact_graph_values = self.data.get_compact_graph()
        nb_clients = len(compact_graph_values)
        drone_graph = self.data.drone_time
        nb_nodes = len(drone_graph)
        dist_truck = {(i, j, t):
                compact_graph_values[i][j] + 60
                #math.sqrt(sum((instance.positions[instance.indices_customers[i]][k]-instance.positions[instance.indices_customers[j]][k])**2 for k in range(2)))  #remplacer ligne précédente par celle-là pour farie avec dist à vol d'oiseau
                for i in range(nb_clients) for j in range(nb_clients) for t in range(nb_clients)}
        dist_drone = {(a, i, j, t):
                2*drone_graph[i][self.list_customers[j]]
                #math.sqrt(sum((instance.positions[instance.indices_customers[i]][k]-instance.positions[instance.indices_customers[j]][k])**2 for k in range(2)))  #remplacer ligne précédente par celle-là pour farie avec dist à vol d'oiseau
                for a in range(2) for i in range(nb_nodes) for j in range(nb_clients) for t in range(nb_clients)}


        model = gp.Model("mip")

        x = model.addVars(dist_truck.keys(), obj=dist_truck, vtype=GRB.BINARY, name='x')
        w = model.addVars(dist_drone.keys(), vtype=GRB.BINARY, name='w')
        u = model.addVars(2, nb_nodes, nb_clients, lb = 0, vtype=GRB.CONTINUOUS, name='u')
        v = model.addVars(2, nb_nodes, nb_clients, lb = 0, vtype=GRB.INTEGER, name='v')
        mommaU = model.addVars(nb_nodes, nb_clients, obj=1, lb = 0, vtype=GRB.CONTINUOUS, name='mommaU')


        print("variables done")



        #FLOT TRUCK

        model.addConstrs(gp.quicksum(x[j,i,t] - x[i,j,t+1] for j in range(nb_clients)) == 0 for i in range(1,nb_clients) for t in range(nb_clients-1))

        model.addConstr(gp.quicksum([x[0,i,0] for i in range(nb_clients)]) == 1)
        model.addConstr(gp.quicksum([x[i,0,t] for i in range(nb_clients) for t in range(nb_clients)]) == 1)

        print("flot done")

        #SATISFACTION CLIENT


        model.addConstrs(gp.quicksum(x[i,j,t] for j in range(nb_clients) for t in range(nb_clients)) + gp.quicksum(w[a,j,i,t] for a in range(2) for j in range(nb_nodes) for t in range(nb_clients)) == 1 for i in range(nb_clients))


        print("satisfaction done")



        #DEPART TRAJET DRONE SUR NOEUD VISITE PAR TRUCK

        #model.addConstrs(gp.quicksum(x[l,k,t] for l in range(nb_clients) for k in range(nb_clients) if i in compact_graph_path[l][k]) >= w[a,i,j,t]
        #                                    for a in range(2)
        #                                    for i in range(nb_nodes)
        #                                    for j in range(nb_clients)
        #                                    for t in range(nb_clients))

        model.addConstrs(2*nb_clients*gp.quicksum(x[l,k,t] for l in range(nb_clients) for k in range(nb_clients) if i in compact_graph_path[l][k]) >= gp.quicksum(w[0,i,j,t] + w[1,i,j,t] for j in range(nb_clients))
                                            for i in range(nb_nodes)
                                            for t in range(nb_clients))

        print("depart drone faisabilité done")

        #PEUT VISITER SEULEMENT 5 CLIENTS LES PLUS PROCHES DEPUIS UN SOMMET
        model.addConstrs((x[i,j,t] == 0 for i in range(nb_clients) for j in range(nb_clients) for t in range(nb_clients) if not j in self.five_closest_neighbors[i]), name='c')

        #DEFINITION DE U V ET MOMMAU

        model.addConstrs(v[a,i,t] >= 0 for a in range(2) for i in range(nb_nodes) for t in range(nb_clients))
        model.addConstrs(v[a,i,t] >= gp.quicksum(w[a,i,j,t] for j in range(nb_clients)) - 1 for a in range(2) for i in range(nb_nodes) for t in range(nb_clients))
        model.addConstrs(v[a,i,t] >= 0 for a in range(2) for i in range(nb_nodes) for t in range(nb_clients))

        model.addConstrs(u[a,i,t] >= gp.quicksum(w[a,i,j,t]*drone_graph[i][self.list_customers[j]] for j in range(nb_clients)) + 30 * v[a,i,t] for a in range(2) for i in range(nb_nodes) for t in range(nb_clients))
        model.addConstrs(mommaU[i,t] >= u[a,i,t] - 60 * (i in self.list_customers) for a in range(2) for i in range(nb_nodes) for t in range(nb_clients))
        model.addConstrs(mommaU[i,t] >= 0 for a in range(2) for i in range(nb_nodes) for t in range(nb_clients))


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

        vals_v = model.getAttr('X', v)
        mommaU = model.getAttr('X', mommaU)
        print("temps d'attente:")
        for key in mommaU.keys():
            if mommaU[key] > 0.5:
                print(key, vals_v[key], mommaU[key])
