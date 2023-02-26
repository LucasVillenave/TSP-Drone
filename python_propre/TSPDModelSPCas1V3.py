import gurobipy as gp
from gurobipy import GRB
from python_propre.TSPDData import *
from math import floor
from itertools import combinations

class TSPDModelSPCas1V3:
    def __init__(self, data, f1 = 3, f2 = 2, f3 = 6, f4 = 5):
        self.data = data
        self.list_customers = self.data.get_demand_nodes()
        self.demands = self.data.df_customers
        self.demands.at[0,'amount'] = 1
        self.nb_neighbors_to_visit = floor(len(self.list_customers)  /f1  )
        self.nb_neighbors_to_visit_drone = floor(len(self.list_customers)  /f2  )
        self.nb_periods = floor(len(self.data.get_shortest_path_graph())  /f3  )
        self.ratio_intersections =    f4
        self.closest_neighbors, self.closest_clients_drone = self.get_closest_neighbors()
        self.intersections = self.get_possible_intersections()

        self.dict_customers = {}
        for i in range(len(self.list_customers)):
            self.dict_customers[i] = self.list_customers[i]
        self.inv_dict_customers = {v: k for k, v in self.dict_customers.items()}

        self.dict_intersections = {}
        for i in range(len(self.intersections)):
            self.dict_intersections[i] = self.intersections[i]
        self.inv_dict_intersections = {v: k for k, v in self.dict_intersections.items()}



    def get_closest_neighbors(self):
        compact_graph = self.data.get_compact_graph()
        closest_clients_truck = []
        for i in range(len(compact_graph)):
            closest_clients_truck.append(dict(sorted(compact_graph[i].items(), key=lambda item: item[1])[:self.nb_neighbors_to_visit+1]))

        drone_time = self.data.get_drone_graph()
        drone_time_clients = {u:{ v:
                                drone_time[u][v]
                                for v in range(len(drone_time)) if v in self.list_customers}
                                for u in range(len(drone_time))}
        closest_clients_drone = []
        
        for i in range(len(drone_time)):
            closest_clients_drone.append(dict(sorted(drone_time_clients[i].items(), key=lambda item: item[1])[:self.nb_neighbors_to_visit_drone+1]))

        return closest_clients_truck, closest_clients_drone



    def get_possible_intersections(self):
        compact_graph_path = self.data.get_shortest_path_graph()
        #all_possible_intersections = []
        all_possible_intersections = np.unique(np.concatenate(np.array([np.array(compact_graph_path[depart][arrivee]) for depart in compact_graph_path.keys() for arrivee in compact_graph_path[depart].keys()])))
        all_possible_intersections = [all_possible_intersections[i] for i in range(0,len(all_possible_intersections),self.ratio_intersections)]
        for cust in self.list_customers:
            if not cust in all_possible_intersections:
                all_possible_intersections.append(cust)
        return all_possible_intersections
                            

        
    def solve(self, name_path = False, time_max = 30, do_callbacks = True, do_write = True):

        def subtourelim(model, where):
            is_there_cycle = False
            if where == GRB.Callback.MIPSOL:
                vals_x = model.cbGetSolution(x)
                cycles = subtour(vals_x)
                if len(cycles) != 0: 
                    is_there_cycle = True
                for cycle in cycles:
                    # add subtour elimination constr. for every pair of cities in tour
                    model.cbLazy(gp.quicksum(x[i, j] + x[j,i] for i, j in combinations(cycle, 2)) <= len(cycle)-1)
                
            if not is_there_cycle and where == GRB.Callback.MIPSOL:
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

        def subtour(vals):
            # make a list of edges selected in the solution
            edges = gp.tuplelist((i, j) for i, j in vals.keys() if vals[i, j] > 0.01)
            unvisited = []
            for e in edges:
                if not e[0] in unvisited:
                    unvisited.append(e[0])
            unvisited = list(unvisited)
            cycles = []  #list of cycles to eliminate
            while unvisited:  # true if list is non-empty
                thiscycle = []
                neighbors = unvisited
                while neighbors:
                    current = neighbors[0]
                    thiscycle.append(current)
                    unvisited.remove(current)
                    neighbors = [j for i, j in edges.select(current, '*')
                                if j in unvisited]
                if not 0 in thiscycle:
                    cycles.append(thiscycle)
            print(cycles)
            return cycles

        def get_solution(truck_paths, drone_legs, waiting_times, do_print = False):
            df_truck = pd.DataFrame(truck_paths, columns = ['id_depart','id_arrivee'])
            df_drones = pd.DataFrame(drone_legs, columns = ['id_drone','id_depart','id_arrivee','id_truck_depart','id_truck_arrivee','id_demand'])
            df_truck = df_truck.assign(depart = df_truck.id_depart.map(self.dict_customers), arrivee = df_truck.id_arrivee.map(self.dict_customers))
            df_drones = df_drones.assign(depart = df_drones.id_depart.map(self.dict_intersections), arrivee = df_drones.id_arrivee.map(self.dict_customers),truck_depart = df_drones.id_truck_depart.map(self.dict_customers), truck_arrivee = df_drones.id_truck_arrivee.map(self.dict_customers))

            #print(df_truck,"\n", df_drones)
            total_time = 0
            total_truck_time = 0
            to_change = []           # amount,[depart intersection,periode], [x depart i,j,t], [arrivee intersection,periode], [x arrivee i,j,t] répertorie les intersections, périodes pour lesquelles le temps d'attente de 30sec des drones n'a pas été respecté
            time_since_used = [[30,30], self.data.depot_id]
            last_node_used = [[[self.data.depot_id,0],[df_truck.iloc[0]['id_depart'],df_truck.iloc[0]['id_arrivee'],0]],[[self.data.depot_id,0],[df_truck.iloc[0]['id_depart'],df_truck.iloc[0]['id_arrivee'],0]]]
            to_check = self.data.depot_id
            for i in range(len(df_truck.index)):
                df_truck_move = df_truck.loc[(df_truck['depart'] == to_check)]
                to_check = int(df_truck_move['arrivee'])
                if do_print: print("truck_edge_starting")
                #print('path', self.data.truck_shortest_path[df_truck_move['id_depart']][df_truck_move['id_arrivee']])
                for id in self.data.truck_shortest_path[int(df_truck_move['id_depart'])][int(df_truck_move['id_arrivee'])]:
                    total_time += self.data.shortest_path_value(time_since_used[1],id)
                    total_truck_time += self.data.shortest_path_value(time_since_used[1],id)
                    if do_print: print(id,self.data.shortest_path_value(time_since_used[1],id),total_time)
                    df_drone_moves = [df_drones.loc[(df_drones['depart'] == id) & (df_drones['id_truck_depart'] == int(df_truck_move['id_depart'])) & (df_drones['id_truck_arrivee'] == int(df_truck_move['id_arrivee'])) & (df_drones['id_drone'] == k)] for k in range(2)]
                    sum_time_drones_used = [max(0,len(df_drone_moves[k].index) - 1)*30 + sum([2*self.data.drone_time[row['depart']][row['arrivee']] for index, row in df_drone_moves[k].iterrows()]) for k in range(2)]
                    time_to_wait = [0,0]
                    
                    if sum_time_drones_used[0] == 0:
                        time_since_used[0][0] += self.data.shortest_path_value(time_since_used[1],id) + sum_time_drones_used[1]
                    else:
                        if do_print: print("   drone 0, last time used:", time_since_used[0][0] + self.data.shortest_path_value(time_since_used[1],id), "total travel : ", sum_time_drones_used[0])
                        time_to_wait[0] = max(0, 30 - (time_since_used[0][0] + self.data.shortest_path_value(time_since_used[1],id) + max(0, sum_time_drones_used[1] - sum_time_drones_used[0])))
                        if time_to_wait[0] > waiting_times[(0,self.inv_dict_intersections[id],i)]:
                            to_change.append([time_to_wait[0] + max(0, sum_time_drones_used[1] - sum_time_drones_used[0]),[self.inv_dict_intersections[id],i],[df_truck_move['id_depart'],df_truck_move['id_arrivee'],i],last_node_used[0][0],last_node_used[0][1]])
                            if do_print: print("   ",time_to_wait[0] + max(0, sum_time_drones_used[1] - sum_time_drones_used[0]), "time to add")
                            #print((0,id,i),time_to_wait[0] + max(0, sum_time_drones_used[1] - sum_time_drones_used[0]))
                            #print(time_since_used, self.data.shortest_path(time_since_used[1],id))
                        time_since_used[0][0] = 0
                        last_node_used[0][0] = [self.inv_dict_intersections[id], i]
                        last_node_used[0][1] = [df_truck_move['id_depart'],df_truck_move['id_arrivee'],i]

                    if sum_time_drones_used[1] == 0:
                        time_since_used[0][1] += self.data.shortest_path_value(time_since_used[1],id) + sum_time_drones_used[0]

                    else:
                        if do_print: print("   drone 1, last time used:", time_since_used[0][1] + self.data.shortest_path_value(time_since_used[1],id), "total travel : ", sum_time_drones_used[1])
                        time_to_wait[1] = max(0, 30 - (time_since_used[0][1] + self.data.shortest_path_value(time_since_used[1],id) + max(0, sum_time_drones_used[0] - sum_time_drones_used[1])))
                        if time_to_wait[1] > waiting_times[(0,self.inv_dict_intersections[id],i)]:
                            to_change.append([time_to_wait[1] + max(0, sum_time_drones_used[0] - sum_time_drones_used[1]),[self.inv_dict_intersections[id],i],[df_truck_move['id_depart'],df_truck_move['id_arrivee'],i],last_node_used[1][0],last_node_used[1][1]])
                            if do_print: print("   ",time_to_wait[1] + max(0, sum_time_drones_used[0] - sum_time_drones_used[0]), "time to add")
                            #print((1,id,i),time_to_wait[1] + max(0, sum_time_drones_used[0] - sum_time_drones_used[1]))
                        time_since_used[0][1] = 0
                        last_node_used[1][0] = [self.inv_dict_intersections[id], i]
                        last_node_used[1][1] = [df_truck_move['id_depart'],df_truck_move['id_arrivee'],i]

                    time_since_used[1] = id
                    time_to_add = max(sum_time_drones_used[0] + time_to_wait[0],sum_time_drones_used[1] + time_to_wait[1])
                    if do_print and time_to_add != 0:print("   time to add: ",time_to_add)
                    total_time += time_to_add
                        
            if do_print: print('total truck time ', total_truck_time)
            print('TOCHANGE',to_change)
            return to_change


        compact_graph_path_origin = self.data.truck_shortest_path
        compact_graph_values = self.data.truck_shortest_time
        nb_clients = len(compact_graph_path_origin)
        compact_graph_path = {u:{ v: 
                                [compact_graph_path_origin[u][v][i] for i in range(0,len(compact_graph_path_origin[u][v]),max(3,floor(len(compact_graph_path_origin[u][v])/self.ratio_intersections)))]
                                for v in range(nb_clients)}
                                for u in range(nb_clients)}
        drone_graph = self.data.drone_time
        dist_truck = {(i, j):
                compact_graph_values[i][j]
                #math.sqrt(sum((instance.positions[instance.indices_customers[i]][k]-instance.positions[instance.indices_customers[j]][k])**2 for k in range(2)))  #remplacer ligne précédente par celle-là pour farie avec dist à vol d'oiseau
                for i in range(nb_clients) for j in range(nb_clients)}
        dist_drone = {(a, i, j, l,k,d):
                1
                #math.sqrt(sum((instance.positions[instance.indices_customers[i]][k]-instance.positions[instance.indices_customers[j]][k])**2 for k in range(2)))  #remplacer ligne précédente par celle-là pour farie avec dist à vol d'oiseau
                for a in range(2) for l in range(nb_clients) for k in range(nb_clients) for i in compact_graph_path[l][k] for j in range(nb_clients) for d in range(int(self.demands.iloc[j]['amount']))}
        u_v_y_w = {(a, i,l,k):
                1
                #math.sqrt(sum((instance.positions[instance.indices_customers[i]][k]-instance.positions[instance.indices_customers[j]][k])**2 for k in range(2)))  #remplacer ligne précédente par celle-là pour farie avec dist à vol d'oiseau
                for a in range(2) for l in range(nb_clients) for k in range(nb_clients) for i in compact_graph_path[l][k]}
        moU = {(i,l,k):
                1
                #math.sqrt(sum((instance.positions[instance.indices_customers[i]][k]-instance.positions[instance.indices_customers[j]][k])**2 for k in range(2)))  #remplacer ligne précédente par celle-là pour farie avec dist à vol d'oiseau
                for l in range(nb_clients) for k in range(nb_clients) for i in compact_graph_path[l][k]}

        model = gp.Model("mip")

        x = model.addVars(dist_truck.keys(), vtype=GRB.BINARY, name='x')
        w = model.addVars(dist_drone.keys(), vtype=GRB.BINARY, name='w')
        u = model.addVars(u_v_y_w.keys(), lb = 0, vtype=GRB.CONTINUOUS, name='u')
        v = model.addVars(u_v_y_w.keys(), lb = 0, vtype=GRB.INTEGER, name='v')
        y = model.addVars(u_v_y_w.keys(), lb = 0, vtype=GRB.INTEGER, name='y')
        mommaU = model.addVars(moU.keys(), lb = 0, vtype=GRB.CONTINUOUS, name='mommaU')
        q = model.addVars(u_v_y_w.keys(), lb = 0, vtype=GRB.CONTINUOUS, name='q')
        #WAZAI = model.addVars(nb_periods,vtype=GRB.CONTINUOUS, name='WAZAI')
        WAZAI = model.addVar(vtype=GRB.CONTINUOUS, obj = 1, name='WAZAI')

        
        print("variables done")


        #FLOT TRUCK
        
        model.addConstrs(gp.quicksum(x[i,j] for j in range(nb_clients)) <= 1 for i in range(nb_clients))
        model.addConstrs(gp.quicksum(x[i,j] for j in range(nb_clients)) == gp.quicksum(x[j,i] for j in range(nb_clients)) for i in range(nb_clients))

        model.addConstr(gp.quicksum(x[0,i] for i in range(nb_clients)) == 1)
        model.addConstr(gp.quicksum(x[i,0] for i in range(nb_clients)) == 1)
        model.addConstrs(x[i,i] == 0 for i in range(nb_clients))

        #model.addConstrs(gp.quicksum(x[i,j,t] for i in range(nb_clients) for j in range(nb_clients)) <= 1 - gp.quicksum(x[i,0,t2] for i in range(nb_clients) for t2 in range(t)))

        print("flot done")

        #SATISFACTION CLIENT


        model.addConstrs(gp.quicksum(w[a,j,i,l,k,d] for a in range(2) for l in range(nb_clients) for k in range(nb_clients) for j in compact_graph_path[l][k]) == 1 for i in range(nb_clients) for d in range(int(self.demands.iloc[i]['amount'])))
        #model.addConstrs(gp.quicksum(x[i,j,t] for j in range(nb_clients)) + gp.quicksum(w[a,j,i,t] for a in range(2) for j in range(nb_intersections)) == 1 for i in range(nb_clients))

        print("satisfaction done")



        #DEPART TRAJET DRONE SUR NOEUD VISITE PAR TRUCK

        #model.addConstrs(gp.quicksum(x[l,k,t] for l in range(nb_clients) for k in range(nb_clients) if i in compact_graph_path[l][k]) >= w[a,i,j,t]
        #                                    for a in range(2)
        #                                    for i in range(nb_nodes)
        #                                    for j in range(nb_clients)
        #                                    for t in range(nb_clients))

        model.addConstrs(w[a,i,j,l,k,d] <= x[l,k] for a in range(2) for l in range(nb_clients) for k in range(nb_clients) for i in compact_graph_path[l][k] for j in range(nb_clients) for d in range(int(self.demands.iloc[j]['amount'])))
        model.addConstrs(gp.quicksum(w[a,i,j,l,k,d] for a in range(2) for i in compact_graph_path[l][k] for j in range(nb_clients) for d in range(int(self.demands.iloc[j]['amount'])) if i == self.dict_customers[k]) == 0 for l in range(nb_clients) for k in range(nb_clients))

        print("depart drone faisabilité done")

        #PEUT VISITER SEULEMENT nb_neighbors_to_visit CLIENTS LES PLUS PROCHES DEPUIS UN SOMMET
        model.addConstrs((x[i,j] == 0 for i in range(nb_clients) for j in range(nb_clients) if not j in self.closest_neighbors[i]))
        model.addConstrs((w[a,i,j,l,k,d] == 0 for a in range(2) for l in range(nb_clients) for k in range(nb_clients) for i in compact_graph_path[l][k] for j in range(nb_clients) for d in range(int(self.demands.iloc[j]['amount'])) if not self.dict_customers[j] in self.closest_clients_drone[i]))

        #DEFINITION DE U V ET MOMMAU
        model.addConstrs(v[a,i,l,k] >= 0 for a in range(2) for l in range(nb_clients) for k in range(nb_clients) for i in compact_graph_path[l][k])
        model.addConstrs(v[a,i,l,k] >= gp.quicksum(w[a,i,j,l,k,d] for j in range(nb_clients) for d in range(int(self.demands.iloc[j]['amount']))) - 1 for a in range(2) for l in range(nb_clients) for k in range(nb_clients) for i in compact_graph_path[l][k])
        model.addConstrs(v[a,i,l,k] >= 0 for a in range(2) for l in range(nb_clients) for k in range(nb_clients) for i in compact_graph_path[l][k])
        model.addConstrs(nb_clients * y[a,i,l,k] >= gp.quicksum(w[a,i,j,l,k,d] for j in range(nb_clients) for d in range(int(self.demands.iloc[j]['amount']))) for a in range(2) for l in range(nb_clients) for k in range(nb_clients) for i in compact_graph_path[l][k])
        model.addConstrs(y[a,i,l,k] >= 0 for a in range(2) for l in range(nb_clients) for k in range(nb_clients) for i in compact_graph_path[l][k])

        model.addConstrs(u[a,i,l,k] >= gp.quicksum(w[a,i,j,l,k,d]*2*drone_graph[i][self.list_customers[j]] for j in range(nb_clients) for d in range(int(self.demands.iloc[j]['amount']))) + 30 * v[a,i,l,k] + q[a,i,l,k] for a in range(2) for l in range(nb_clients) for k in range(nb_clients) for i in compact_graph_path[l][k])
        model.addConstrs(mommaU[i,l,k] >= u[a,i,l,k] for a in range(2) for l in range(nb_clients) for k in range(nb_clients) for i in compact_graph_path[l][k])
        model.addConstrs(mommaU[i,l,k] >= 0 for l in range(nb_clients) for k in range(nb_clients) for i in compact_graph_path[l][k])

        model.addConstrs(q[a,i,l,k] >= 0 for a in range(2) for l in range(nb_clients) for k in range(nb_clients) for i in compact_graph_path[l][k])

        #DEFINITION WAZAI OBJECTIF
        model.addConstr(WAZAI == gp.quicksum(x[i,j] * (compact_graph_values[i][j]) for i in range(nb_clients) for j in range(nb_clients)) + gp.quicksum(mommaU[i,l,k] for l in range(nb_clients) for k in range(nb_clients) for i in compact_graph_path[l][k]))
        model.addConstrs(WAZAI >= gp.quicksum(x[i,j] * (compact_graph_values[i][j]) for i in range(nb_clients) for j in range(nb_clients)) + gp.quicksum(w[a,i,j,l,k,d] * 2*drone_graph[i][self.list_customers[j]] for l in range(nb_clients) for k in range(nb_clients) for i in compact_graph_path[l][k] for j in range(nb_clients) for d in range(int(self.demands.iloc[j]['amount']))) for a in range(2))

        #model.setObjective(WAZAI[nb_periods-1], GRB.MINIMIZE)

        model.setParam('TimeLimit', time_max*60)

        if do_callbacks :
            model.Params.LazyConstraints = 1
            model.optimize(subtourelim)

        else: model.optimize()

        print("obj : ", model.ObjVal)

        truck_paths = []
        drone_legs = []

        vals_x = model.getAttr('X', x)
        vals_w = model.getAttr('X', w)
        vals_q = model.getAttr('X', q)
        vals_u = model.getAttr('X', u)
        vals_mommaU = model.getAttr('X',mommaU)
   
        total_truck_time = 0

        if do_write : f = open(name_path, "a")
        if do_write : f.write(str(model.ObjVal))

        print("truck:")
        for key in vals_x.keys():
            if vals_x[key] > 0.5:
                print(key, self.data.truck_shortest_time[key[0]][key[1]])
                truck_paths.append(key)
                if name_path != False:
                    if do_write : f.write(str(key[0]) + " " + str(key[1]) + "\n")
                total_truck_time += self.data.truck_shortest_time[key[0]][key[1]]
        print('total truck time ', total_truck_time)
        if do_write : f.write("next\n")
        print("drones:")
        for key in vals_w.keys():
            if vals_w[key] > 0.5:
                if name_path != False:
                    if do_write : f.write(str(key[0]) + " " + str(key[1]) + " " + str(key[2]) + " " + str(key[3]) + " " + str(key[4]) + " " + str(key[5]) + "\n")
                print(key,2*drone_graph[key[1]][self.list_customers[key[2]]])
                drone_legs.append(key)

        total_waiting_time = 0
        print('time_to_wait:')
        for key in vals_mommaU.keys():
            if vals_mommaU[key] > 0:
                print(key, vals_mommaU[key])
                total_waiting_time += vals_mommaU[key]
        print('total waiting time ', total_waiting_time)

        waiting_times = {}
        actual_waiting_times = {}
        for key in vals_q.keys():
            actual_waiting_times[key] = vals_q[key]
            if vals_q[key] != 0: print(key,vals_q[key])
            waiting_times[key] = 0
            #if vals_q[key] > 0.5:
            #    print(vals_q[key])
        


        use_time = {}
        for key in vals_u.keys():
            use_time[key] = vals_u[key]
            #if vals_u[key] > 0.5:
            #    print(vals_u[key])
        
        get_solution(truck_paths, drone_legs, waiting_times, do_print = False)
        




