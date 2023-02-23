import gurobipy as gp
from gurobipy import GRB
from TSPDData import *
from math import floor

class TSPDModelSPCas2:
    def __init__(self, data):
        self.data = data
        self.list_customers = self.data.get_demand_nodes()
        self.demands = self.data.get_all_customers()
        self.demands.at[0,'amount'] = 1
        self.intersections = self.get_possible_intersections()
        self.nb_neighbors_to_visit = floor(len(self.list_customers)  /3  )
        self.nb_neighbors_to_visit_drone = floor(len(self.list_customers)  /2  )
        self.closest_neighbors, self.closest_clients_drone = self.get_closest_neighbors()
        self.nb_periods = floor(len(self.data.truck_shortest_time)  /3  )
        self.nb_periods_return_drones = 3

        self.dict_customers = {}
        for i in range(len(self.list_customers)):
            self.dict_customers[i] = self.list_customers[i]

        self.dict_intersections = {}
        for i in range(len(self.intersections)):
            self.dict_intersections[i] = self.intersections[i]
        self.inv_dict_intersections = {v: k for k, v in self.dict_intersections.items()}



    def get_closest_neighbors(self):
        compact_graph = self.data.truck_shortest_time
        closest_clients_truck = []
        for i in range(len(compact_graph)):
            closest_clients_truck.append(dict(sorted(compact_graph[i].items(), key=lambda item: item[1])[:self.nb_neighbors_to_visit+1]))

        drone_time = self.data.drone_time
        drone_time_clients = {u:{ v:
                                drone_time[u][v]
                                for v in range(len(drone_time)) if v in self.list_customers}
                                for u in range(len(drone_time))}
        closest_clients_drone = []
        
        for i in range(len(self.data.drone_time)):
            closest_clients_drone.append(dict(sorted(drone_time_clients[i].items(), key=lambda item: item[1])[:self.nb_neighbors_to_visit_drone+1]))

        return closest_clients_truck, closest_clients_drone



    def get_possible_intersections(self):
        compact_graph_path = self.data.truck_shortest_path
        return np.unique(np.concatenate(np.array([np.array(compact_graph_path[depart][arrivee]) for depart in compact_graph_path.keys() for arrivee in compact_graph_path[depart].keys()])))
        

                            

        
    def solve(self):

        def subtourelim(model, where):
            if where == GRB.Callback.MIPSOL:
                if model.cbGet(GRB.Callback.MIPSOL_OBJBST) < 4000:
                    #print(model.cbGet(GRB.Callback.MIPSOL_OBJBST))
                    truck_paths = []
                    drone_legs = []
                    waiting_times = {}
                    vals_x = model.cbGetSolution(x)
                    vals_w = model.cbGetSolution(w)
                    vals_q = model.cbGetSolution(q)
                    for key in vals_x.keys():
                        if vals_x[key] > 0.5:
                            truck_paths.append(key)
                    for key in vals_w.keys():
                        if vals_w[key] > 0.5:
                            drone_legs.append(key)
                    for key in vals_q.keys():
                        waiting_times[key] = vals_q[key]   
                    to_change = get_solution(truck_paths, drone_legs, waiting_times)   
                    for ele in to_change:
                        for a in range(2):
                            model.cbLazy(q[a,ele[1][0],ele[1][1]] >= (y[a,ele[1][0],ele[1][1]] + y[a,ele[3][0],ele[3][1]] + x[ele[2][0],ele[2][1],ele[2][2]] + x[ele[4][0],ele[4][1],ele[4][2]] - 3) * ele[0])
                
                #print(vals_x,vals_w,vals_q)

        

        def get_solution(truck_paths, drone_legs, waiting_times):
            df_truck = pd.DataFrame(truck_paths, columns = ['id_depart','id_arrivee','period'])
            df_drones = pd.DataFrame(drone_legs, columns = ['id_drone','id_depart','id_arrivee','period'])
            df_truck.sort_values('period', inplace = True)
            df_truck = df_truck.reset_index(drop=True)
            df_drones.sort_values('period', inplace = True)
            df_truck = df_truck.assign(depart = df_truck.id_depart.map(self.dict_customers), arrivee = df_truck.id_arrivee.map(self.dict_customers))
            df_drones = df_drones.assign(depart = df_drones.id_depart.map(self.dict_intersections), arrivee = df_drones.id_arrivee.map(self.dict_customers))

            #print(df_truck,"\n", df_drones)

            to_change = []           # amount,[depart intersection,periode], [x depart i,j,t], [arrivee intersection,periode], [x arrivee i,j,t] répertorie les intersections, périodes pour lesquelles le temps d'attente de 30sec des drones n'a pas été respecté
            time_since_used = [[30,30], self.data.depot_id]
            last_node_used = [[[self.data.depot_id,0],[df_truck.iloc[0]['id_depart'],df_truck.iloc[0]['id_arrivee'],0]],[[self.data.depot_id,0],[df_truck.iloc[0]['id_depart'],df_truck.iloc[0]['id_arrivee'],0]]]
            for i in range(len(df_truck.index)):
                df_truck_move = df_truck.iloc[i]
                #print('path', self.data.truck_shortest_path[df_truck_move['id_depart']][df_truck_move['id_arrivee']])
                for id in self.data.truck_shortest_path[df_truck_move['id_depart']][df_truck_move['id_arrivee']]:
                    df_drone_moves = [df_drones.loc[(df_drones['depart'] == id) & (df_drones['period'] == i) & (df_drones['id_drone'] == k)] for k in range(2)]
                    sum_time_drones_used = [max(0,len(df_drone_moves[k].index) - 1) + sum([self.data.drone_time[row['depart']][row['arrivee']] for index, row in df_drone_moves[k].iterrows()]) for k in range(2)]
                    time_to_wait = [0,0]
                    
                    if sum_time_drones_used[0] == 0:
                        time_since_used[0][0] += self.data.shortest_path(time_since_used[1],id) + sum_time_drones_used[1]
                    else:
                        time_to_wait[0] = max(0, 30 - (time_since_used[0][0] + self.data.shortest_path(time_since_used[1],id) + max(0, sum_time_drones_used[1] - sum_time_drones_used[0])))
                        if time_to_wait[0] > waiting_times[(0,self.inv_dict_intersections[id],i)]:
                            to_change.append([time_to_wait[0] + max(0, sum_time_drones_used[1] - sum_time_drones_used[0]),[self.inv_dict_intersections[id],i],[df_truck_move['id_depart'],df_truck_move['id_arrivee'],i],last_node_used[0][0],last_node_used[0][1]])
                            #print((0,id,i),time_to_wait[0] + max(0, sum_time_drones_used[1] - sum_time_drones_used[0]))
                            #print(time_since_used, self.data.shortest_path(time_since_used[1],id))
                        time_since_used[0][0] = 0
                        last_node_used[0][0] = [self.inv_dict_intersections[id], i]
                        last_node_used[0][1] = [df_truck_move['id_depart'],df_truck_move['id_arrivee'],i]

                    if sum_time_drones_used[1] == 0:
                        time_since_used[0][1] += self.data.shortest_path(time_since_used[1],id) + sum_time_drones_used[0]

                    else:
                        if time_to_wait[1] > waiting_times[(0,self.inv_dict_intersections[id],i)]:
                            to_change.append([time_to_wait[1] + max(0, sum_time_drones_used[0] - sum_time_drones_used[1]),[self.inv_dict_intersections[id],i],[df_truck_move['id_depart'],df_truck_move['id_arrivee'],i],last_node_used[1][0],last_node_used[1][1]])
                            #print((1,id,i),time_to_wait[1] + max(0, sum_time_drones_used[0] - sum_time_drones_used[1]))
                        time_since_used[0][1] = 0
                        last_node_used[1][0] = [self.inv_dict_intersections[id], i]
                        last_node_used[1][1] = [df_truck_move['id_depart'],df_truck_move['id_arrivee'],i]

                    time_since_used[1] = id
            print('TOCHANGE',to_change)
            return to_change


        compact_graph_path = self.data.truck_shortest_path
        compact_graph_values = self.data.truck_shortest_time
        nb_clients = len(compact_graph_values)
        nb_periods = self.nb_periods
        nb_intersections = len(self.intersections)
        drone_graph = self.data.drone_time
        dist_truck = {(i, j, t):
                compact_graph_values[i][j] + 60
                #math.sqrt(sum((instance.positions[instance.indices_customers[i]][k]-instance.positions[instance.indices_customers[j]][k])**2 for k in range(2)))  #remplacer ligne précédente par celle-là pour farie avec dist à vol d'oiseau
                for i in range(nb_clients) for j in range(nb_clients) for t in range(nb_periods)}
        dist_drone = {(a, i, j, t):
                2*drone_graph[self.intersections[i]][self.list_customers[j]]
                #math.sqrt(sum((instance.positions[instance.indices_customers[i]][k]-instance.positions[instance.indices_customers[j]][k])**2 for k in range(2)))  #remplacer ligne précédente par celle-là pour farie avec dist à vol d'oiseau
                for a in range(2) for i in range(nb_intersections) for j in range(nb_clients) for t in range(nb_periods)}


        model = gp.Model("mip")

        x = model.addVars(dist_truck.keys(), vtype=GRB.BINARY, name='x')
        w = model.addVars(dist_drone.keys(), vtype=GRB.BINARY, name='w')
        u = model.addVars(2, nb_intersections, nb_periods, lb = 0, vtype=GRB.CONTINUOUS, name='u')
        v = model.addVars(2, nb_intersections, nb_periods, lb = 0, vtype=GRB.INTEGER, name='v')
        y = model.addVars(2, nb_intersections, nb_periods, lb = 0, vtype=GRB.INTEGER, name='y')
        mommaU = model.addVars(nb_intersections, nb_periods, lb = 0, vtype=GRB.CONTINUOUS, name='mommaU')
        q = model.addVars(2, nb_intersections, nb_periods, lb = 0, vtype=GRB.CONTINUOUS, name='q')
        #WAZAI = model.addVars(nb_periods,vtype=GRB.CONTINUOUS, name='WAZAI')
        WAZAI = model.addVar(vtype=GRB.CONTINUOUS, obj = 1, name='WAZAI')

        
        
        print("variables done")



        #FLOT TRUCK
        
        model.addConstrs(gp.quicksum(x[j,i,t] - x[i,j,t+1] for j in range(nb_clients)) == 0 for i in range(1,nb_clients) for t in range(nb_periods-1))

        model.addConstr(gp.quicksum([x[0,i,0] for i in range(nb_clients)]) == 1)
        model.addConstr(gp.quicksum([x[i,0,t] for i in range(nb_clients) for t in range(nb_periods)]) == 1)
        model.addConstrs(gp.quicksum(x[i,j,t] for i in range(nb_clients) for j in range(nb_clients)) <= 1 for t in range(nb_periods))

        #model.addConstrs(gp.quicksum(x[i,j,t] for i in range(nb_clients) for j in range(nb_clients)) <= 1 - gp.quicksum(x[i,0,t2] for i in range(nb_clients) for t2 in range(t)) for t in range(nb_periods))

        print("flot done")

        #SATISFACTION CLIENT


        
        model.addConstrs(gp.quicksum(self.demands.iloc[i]['amount'] * x[i,j,t] for j in range(nb_clients) for t in range(nb_periods)) + gp.quicksum(w[a,j,i,t] for a in range(2) for j in range(nb_intersections) for t in range(nb_periods)) == self.demands.iloc[i]['amount'] for i in range(nb_clients))
        #model.addConstrs(gp.quicksum(x[i,j,t] for j in range(nb_clients) for t in range(nb_periods)) + gp.quicksum(w[a,j,i,t] for a in range(2) for j in range(nb_intersections) for t in range(nb_periods)) == 1 for i in range(nb_clients))

        print("satisfaction done")



        #DEPART TRAJET DRONE SUR NOEUD VISITE PAR TRUCK
        
        #model.addConstrs(gp.quicksum(x[l,k,t] for l in range(nb_clients) for k in range(nb_clients) if i in compact_graph_path[l][k]) >= w[a,i,j,t] 
        #                                    for a in range(2) 
        #                                    for i in range(nb_nodes) 
        #                                    for j in range(nb_clients) 
        #                                    for t in range(nb_clients))

        model.addConstrs(2*nb_clients*gp.quicksum(x[l,k,t] for l in range(nb_clients) for k in range(nb_clients) if (self.intersections[i] in compact_graph_path[l][k] and (self.intersections[i] != self.list_customers[l] or self.intersections[i] == self.data.depot_id))) >= gp.quicksum(w[0,i,j,t] + w[1,i,j,t] for j in range(nb_clients))
                                            for i in range(nb_intersections)
                                            for t in range(nb_periods))

        print("depart drone faisabilité done")

        #PEUT VISITER SEULEMENT nb_neighbors_to_visit CLIENTS LES PLUS PROCHES DEPUIS UN SOMMET
        model.addConstrs((x[i,j,t] == 0 for i in range(nb_clients) for j in range(nb_clients) for t in range(nb_periods) if not j in self.closest_neighbors[i]))
        model.addConstrs((w[a,i,j,t] == 0 for a in range(2) for i in range(nb_intersections) for j in range(nb_clients) for t in range(nb_periods) if not self.dict_customers[j] in self.closest_clients_drone[self.dict_intersections[i]]))

        #DEFINITION DE U V ET MOMMAU

        model.addConstrs(v[a,i,t] >= 0 for a in range(2) for i in range(nb_intersections) for t in range(nb_periods))
        model.addConstrs(v[a,i,t] >= gp.quicksum(w[a,i,j,t] for j in range(nb_clients)) - 1 for a in range(2) for i in range(nb_intersections) for t in range(nb_periods))
        model.addConstrs(v[a,i,t] >= 0 for a in range(2) for i in range(nb_intersections) for t in range(nb_periods))
        model.addConstrs(nb_clients * y[a,i,t] >= gp.quicksum(w[a,i,j,t] for j in range(nb_clients)) for a in range(2) for i in range(nb_intersections) for t in range(nb_periods))
        model.addConstrs(y[a,i,t] >= 0 for a in range(2) for i in range(nb_intersections) for t in range(nb_periods))

        model.addConstrs(u[a,i,t] >= gp.quicksum(w[a,i,j,t]*2*drone_graph[self.intersections[i]][self.list_customers[j]] for j in range(nb_clients)) + 30 * v[a,i,t] + q[a,i,t] for a in range(2) for i in range(nb_intersections) for t in range(nb_periods))
        model.addConstrs(mommaU[i,t] >= u[a,i,t] - 60 * (self.intersections[i] in [self.list_customers[k] for k in range(1,nb_clients)]) for a in range(2) for i in range(nb_intersections) for t in range(nb_periods))
        model.addConstrs(mommaU[i,t] >= 0 for i in range(nb_intersections) for t in range(nb_periods))

        model.addConstrs(q[a,i,t] >= 0 for a in range(2) for i in range(nb_intersections) for t in range(nb_periods))

        #DEFINITION WAZAI OBJECTIF
        model.addConstr(WAZAI == gp.quicksum(x[i,j,t] * (compact_graph_values[i][j]+60) for i in range(nb_clients) for j in range(nb_clients) for t in range(nb_periods)) + gp.quicksum(mommaU[i,t] for i in range(nb_intersections) for t in range(nb_periods)) - 60)
        #model.addConstrs(WAZAI[t] == WAZAI[t-1] + gp.quicksum(x[i,j,t] * compact_graph_values[i][j] for i in range(nb_clients) for j in range(nb_clients)) + gp.quicksum(mommaU[i,t] for i in range(nb_intersections)) for t in range(1,nb_periods))
        model.addConstrs(WAZAI >= gp.quicksum(x[i,j,t] * (compact_graph_values[i][j]) for i in range(nb_clients) for j in range(nb_clients) for t in range(nb_periods)) + gp.quicksum(w[a,i,j,t] * 2*drone_graph[self.intersections[i]][self.list_customers[j]] for i in range(nb_intersections) for j in range(nb_clients) for t in range(nb_periods)) for a in range(2))
        model.addConstr(WAZAI >= gp.quicksum(x[i,j,t] * (compact_graph_values[i][j]+60) for i in range(nb_clients) for j in range(nb_clients) for t in range(nb_periods)) - 60)

        #model.setObjective(WAZAI[nb_periods-1], GRB.MINIMIZE)

        model.setParam('TimeLimit', 10*60)

        #model.Params.LazyConstraints = 1
        #model.optimize(subtourelim)

        model.optimize()

        print("obj : ", model.ObjVal)

        truck_paths = []
        drone_legs = []

        vals_x = model.getAttr('X', x)
        vals_w = model.getAttr('X', w)
        vals_q = model.getAttr('X', q)
        vals_u = model.getAttr('X', u)

        print("truck:")
        for key in vals_x.keys():
            if vals_x[key] > 0.5:
                print(key)
                truck_paths.append(key)

        print("drones:")
        for key in vals_w.keys():
            if vals_w[key] > 0.5:
                print(key)
                drone_legs.append(key)

        waiting_times = {}
        for key in vals_q.keys():
            waiting_times[key] = vals_q[key]
            #if vals_q[key] > 0.5:
            #    print(vals_q[key])


        use_time = {}
        for key in vals_u.keys():
            use_time[key] = vals_u[key]
            #if vals_u[key] > 0.5:
            #    print(vals_u[key])
        
        get_solution(truck_paths, drone_legs, waiting_times)
        




