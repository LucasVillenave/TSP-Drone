import folium
from math import ceil
from python_propre.TSPDData import *
from python_propre.TSPDEvent import TSPDEvent
from python_propre.TSPDModelSPCas1 import TSPDModelSPCas1
import matplotlib.pyplot as plt

def get_coordinate_from_file(line):
    line = line.split(':')[1]
    line = line.split('(')[0]
    line = line.split(')')[0]
    return float(line)

class TSPDSolution:
    def __init__(self, data):
        self.data = data
        self.value = -1
        #truck tour must be in the form : depot, each node visited, depot
        self.truck_tour = {} #{time : node_id, time : node_id}
        self.truck_delivery = {} #{time : node_id, time : node_id}
        #drone tour must be in the form : {time : [lift off node, delivery node, pick up node], time : ...}
        #self.drone_tour = [{}, {}]
        #self.drone_delivery = [{}, {}] #{time : delivery node, time : delivery node}
        self.list_events = []

    def import_TSP(self, obj, tour):
        self.value = obj
        if len(tour) < 1 :
            return
        time = 0
        node_id = self.data.df_customers.loc[tour[0]].node_id
        self.truck_tour[time] = node_id
        for t in range(0, len(tour)-1):
            if t != 0:
                self.truck_delivery[time] = node_id
                time += self.data.truck_delivery_time
            for j in self.data.truck_shortest_path.get(tour[t]).get(tour[t+1])[1:]:
                time += self.data.shortest_path_value(node_id, j)
                self.truck_tour[time] = j
                node_id = j
        self.update_from_x_to_event()

    def import_H1V1(self, id_instance):
        """
        DEPLACEMENT VEHICULE
        self.list_events.append(TSPDEvent(time, location, destination))

        ARRIVEE VEHICULE
        self.list_events.append(TSPDEvent(time, location))

        LARGAGE DRONE
        self.list_events.append(TSPDEvent(time, location, droneID, demandID))

        RECUPERATION DRONE
        self.list_events.append(TSPDEvent(time, location, droneID))

        LIVRAISON VEHICULE
        self.list_events.append(TSPDEvent(time, location, demandID))

        LIVRAISON DRONE
        self.list_events.append(TSPDEvent(time, droneID, demandID))
        """

        truck_paths = []
        drone_legs = []

        file = open(id_instance+'_1.txt', "r")
        lines = file.readlines()
        i = 0
        while lines[i] != "next\n":
            line = lines[i].split()
            truck_paths.append((int(line[0]),int(line[1]),int(line[2])))
            i+=1
        i+=1
        while i < len(lines):
            lines[i] = lines[i].split()
            drone_legs.append((int(lines[i][0]),int(lines[i][1]),int(lines[i][2]),int(lines[i][3])))
            i+=1

        instance = TSPDModelSPCas1(self.data)
        self.list_customers = self.data.get_demand_nodes()
        self.dict_customers = {}
        for i in range(len(self.list_customers)):
            self.dict_customers[i] = self.list_customers[i]


        df_truck = pd.DataFrame(truck_paths, columns = ['id_depart','id_arrivee','period'])
        df_drones = pd.DataFrame(drone_legs, columns = ['id_drone','id_depart','id_arrivee','period'])
        df_truck.sort_values('period', inplace = True)
        df_truck = df_truck.reset_index(drop=True)
        df_drones.sort_values('period', inplace = True)
        df_truck = df_truck.assign(depart = df_truck.id_depart.map(instance.dict_customers), arrivee = df_truck.id_arrivee.map(instance.dict_customers))
        df_drones = df_drones.assign(depart = df_drones.id_depart.map(instance.dict_intersections), arrivee = df_drones.id_arrivee.map(instance.dict_customers))
        total_time = 0
        to_change = []           # amount,[depart intersection,periode], [x depart i,j,t], [arrivee intersection,periode], [x arrivee i,j,t] répertorie les intersections, périodes pour lesquelles le temps d'attente de 30sec des drones n'a pas été respecté
        time_since_used = [[30,30], instance.data.depot_id]
        last_node_used = [[[instance.data.depot_id,0],[df_truck.iloc[0]['id_depart'],df_truck.iloc[0]['id_arrivee'],0]],[[instance.data.depot_id,0],[df_truck.iloc[0]['id_depart'],df_truck.iloc[0]['id_arrivee'],0]]]
        for i in range(len(df_truck.index)):
            df_truck_move = df_truck.iloc[i]
            for id in instance.data.truck_shortest_path[df_truck_move['id_depart']][df_truck_move['id_arrivee']]:
                self.list_events.append(TSPDEvent(time=total_time, location=self.data.get_node_location(time_since_used[1]), destination=self.data.get_node_location(id)))
                plt.plot((self.data.get_node_location(time_since_used[1])[0],self.data.get_node_location(id)[0]),(self.data.get_node_location(time_since_used[1])[1],self.data.get_node_location(id)[1]), color='black')
                total_time += instance.data.shortest_path_value(time_since_used[1],id)
                self.list_events.append(TSPDEvent(time=total_time, location=self.data.get_node_location(id)))
                if id == df_truck_move['arrivee']: self.list_events.append(TSPDEvent(time=total_time, location=self.data.get_node_location(id), demandID=df_truck_move['id_arrivee']))
                df_drone_moves = [df_drones.loc[(df_drones['depart'] == id) & (df_drones['period'] == i) & (df_drones['id_drone'] == k)] for k in range(2)]
                sum_time_drones_used = [max(0,len(df_drone_moves[k].index) - 1)*30 + sum([2*instance.data.drone_time[row['depart']][row['arrivee']] for index, row in df_drone_moves[k].iterrows()]) for k in range(2)]
                time_to_wait = [0,0]
                
                if sum_time_drones_used[0] == 0:
                    time_since_used[0][0] += instance.data.shortest_path_value(time_since_used[1],id) + sum_time_drones_used[1]
                else:
                    time_to_wait[0] = max(0, 30 - (time_since_used[0][0] + instance.data.shortest_path_value(time_since_used[1],id) + max(0, sum_time_drones_used[1] - sum_time_drones_used[0])))
                    if time_to_wait[0] > 0:
                        to_change.append([time_to_wait[0] + max(0, sum_time_drones_used[1] - sum_time_drones_used[0]),[instance.inv_dict_intersections[id],i],[df_truck_move['id_depart'],df_truck_move['id_arrivee'],i],last_node_used[0][0],last_node_used[0][1]])
                    time_since_used[0][0] = 0
                    last_node_used[0][0] = [instance.inv_dict_intersections[id], i]
                    last_node_used[0][1] = [df_truck_move['id_depart'],df_truck_move['id_arrivee'],i]
                if sum_time_drones_used[1] == 0:
                    time_since_used[0][1] += instance.data.shortest_path_value(time_since_used[1],id) + sum_time_drones_used[0]
                else:
                    time_to_wait[1] = max(0, 30 - (time_since_used[0][1] + instance.data.shortest_path_value(time_since_used[1],id) + max(0, sum_time_drones_used[0] - sum_time_drones_used[1])))
                    if time_to_wait[1] > 0:
                        to_change.append([time_to_wait[1] + max(0, sum_time_drones_used[0] - sum_time_drones_used[1]),[instance.inv_dict_intersections[id],i],[df_truck_move['id_depart'],df_truck_move['id_arrivee'],i],last_node_used[1][0],last_node_used[1][1]])
                    time_since_used[0][1] = 0
                    last_node_used[1][0] = [instance.inv_dict_intersections[id], i]
                    last_node_used[1][1] = [df_truck_move['id_depart'],df_truck_move['id_arrivee'],i]
                time_since_used[1] = id
                for a in range(2):
                    local_time = total_time + time_to_wait[a]
                    first_livraison = True
                    for index, row in df_drone_moves[a].iterrows():
                        if first_livraison == False: local_time += 30
                        self.list_events.append(TSPDEvent(time=local_time, location=self.data.get_node_location(row['depart']), droneID=a, demandID=row['id_arrivee']))
                        if a == 0:plt.plot((self.data.get_node_location(row['depart'])[0],self.data.get_node_location(row['arrivee'])[0]),(self.data.get_node_location(row['depart'])[1],self.data.get_node_location(row['arrivee'])[1]), color='red')
                        else:plt.plot((self.data.get_node_location(row['depart'])[0],self.data.get_node_location(row['arrivee'])[0]),(self.data.get_node_location(row['depart'])[1],self.data.get_node_location(row['arrivee'])[1]), color='green')
                        local_time += instance.data.drone_time[row['depart']][row['arrivee']]
                        self.list_events.append(TSPDEvent(time=local_time, droneID=a, demandID=row['id_arrivee']))
                        local_time += instance.data.drone_time[row['depart']][row['arrivee']]
                        self.list_events.append(TSPDEvent(time=local_time, location=self.data.get_node_location(row['depart']), droneID=a))
                        first_livraison = False

                if id != instance.data.truck_shortest_path[df_truck_move['id_depart']][df_truck_move['id_arrivee']][len(instance.data.truck_shortest_path[df_truck_move['id_depart']][df_truck_move['id_arrivee']])-1]:
                    time_to_add = max(sum_time_drones_used[0] + time_to_wait[0],sum_time_drones_used[1] + time_to_wait[1])
                    total_time += max(sum_time_drones_used[0] + time_to_wait[0],sum_time_drones_used[1] + time_to_wait[1])
                    
                else: 
                    time_to_add = max(max(0,sum_time_drones_used[0] + time_to_wait[0]-60),max(0,sum_time_drones_used[1] + time_to_wait[1]-60))
                    total_time += time_to_add
            total_time += 60
        total_time -= 60
        print("BOUILLA", total_time)
        plt.show()


    def import_H1V2(self, instance, truck_paths, drone_legs):
        """
        DEPLACEMENT VEHICULE
        self.list_events.append(TSPDEvent(time, location, destination))

        ARRIVEE VEHICULE
        self.list_events.append(TSPDEvent(time, location))

        LARGAGE DRONE
        self.list_events.append(TSPDEvent(time, location, droneID, demandID))

        RECUPERATION DRONE
        self.list_events.append(TSPDEvent(time, location, droneID))

        LIVRAISON VEHICULE
        self.list_events.append(TSPDEvent(time, location, demandID))

        LIVRAISON DRONE
        self.list_events.append(TSPDEvent(time, droneID, demandID))
        """

        df_truck = pd.DataFrame(truck_paths, columns = ['id_depart','id_arrivee','period'])
        df_drones = pd.DataFrame(drone_legs, columns = ['id_drone','id_depart','id_arrivee','period'])
        df_truck.sort_values('period', inplace = True)
        df_truck = df_truck.reset_index(drop=True)
        df_drones.sort_values('period', inplace = True)
        df_truck = df_truck.assign(depart = df_truck.id_depart.map(instance.dict_customers), arrivee = df_truck.id_arrivee.map(instance.dict_customers))
        df_drones = df_drones.assign(depart = df_drones.id_depart.map(instance.dict_intersections), arrivee = df_drones.id_arrivee.map(instance.dict_customers))
        total_time = 0
        to_change = []           # amount,[depart intersection,periode], [x depart i,j,t], [arrivee intersection,periode], [x arrivee i,j,t] répertorie les intersections, périodes pour lesquelles le temps d'attente de 30sec des drones n'a pas été respecté
        time_since_used = [[30,30], instance.data.depot_id]
        last_node_used = [[[instance.data.depot_id,0],[df_truck.iloc[0]['id_depart'],df_truck.iloc[0]['id_arrivee'],0]],[[instance.data.depot_id,0],[df_truck.iloc[0]['id_depart'],df_truck.iloc[0]['id_arrivee'],0]]]
        for i in range(len(df_truck.index)):
            df_truck_move = df_truck.iloc[i]
            self.list_events.append(TSPDEvent(total_time, df_truck_move['depart'], df_truck_move['arrivee']))
            for id in instance.data.truck_shortest_path[df_truck_move['id_depart']][df_truck_move['id_arrivee']]:
                total_time += instance.data.shortest_path_value(time_since_used[1],id)
                df_drone_moves = [df_drones.loc[(df_drones['depart'] == id) & (df_drones['period'] == i) & (df_drones['id_drone'] == k)] for k in range(2)]
                sum_time_drones_used = [max(0,len(df_drone_moves[k].index) - 1)*30 + sum([2*instance.data.drone_time[row['depart']][row['arrivee']] for index, row in df_drone_moves[k].iterrows()]) for k in range(2)]
                time_to_wait = [0,0]
                
                if sum_time_drones_used[0] == 0:
                    time_since_used[0][0] += instance.data.shortest_path_value(time_since_used[1],id) + sum_time_drones_used[1]
                else:
                    time_to_wait[0] = max(0, 30 - (time_since_used[0][0] + instance.data.shortest_path_value(time_since_used[1],id) + max(0, sum_time_drones_used[1] - sum_time_drones_used[0])))
                    if time_to_wait[0] > 0:
                        to_change.append([time_to_wait[0] + max(0, sum_time_drones_used[1] - sum_time_drones_used[0]),[instance.inv_dict_intersections[id],i],[df_truck_move['id_depart'],df_truck_move['id_arrivee'],i],last_node_used[0][0],last_node_used[0][1]])
                    time_since_used[0][0] = 0
                    last_node_used[0][0] = [instance.inv_dict_intersections[id], i]
                    last_node_used[0][1] = [df_truck_move['id_depart'],df_truck_move['id_arrivee'],i]
                if sum_time_drones_used[1] == 0:
                    time_since_used[0][1] += instance.data.shortest_path_value(time_since_used[1],id) + sum_time_drones_used[0]
                else:
                    time_to_wait[1] = max(0, 30 - (time_since_used[0][1] + instance.data.shortest_path_value(time_since_used[1],id) + max(0, sum_time_drones_used[0] - sum_time_drones_used[1])))
                    if time_to_wait[1] > 0:
                        to_change.append([time_to_wait[1] + max(0, sum_time_drones_used[0] - sum_time_drones_used[1]),[instance.inv_dict_intersections[id],i],[df_truck_move['id_depart'],df_truck_move['id_arrivee'],i],last_node_used[1][0],last_node_used[1][1]])
                    time_since_used[0][1] = 0
                    last_node_used[1][0] = [instance.inv_dict_intersections[id], i]
                    last_node_used[1][1] = [df_truck_move['id_depart'],df_truck_move['id_arrivee'],i]
                time_since_used[1] = id
                for a in range(2):
                    local_time = total_time + time_to_wait[a]
                    first_livraison = True
                    for index, row in df_drone_moves[a].iterrows():
                        if first_livraison == False: local_time += 30
                        self.list_events.append(TSPDEvent(local_time, row['depart'], a, row['arrivee']))
                        local_time += instance.data.drone_time[row['depart']][row['arrivee']]
                        self.list_events.append(TSPDEvent(local_time, a, row['arrivee']))
                        local_time += instance.data.drone_time[row['depart']][row['arrivee']]
                        self.list_events.append(TSPDEvent(local_time, row['depart'], a))
                        first_livraison = False

                time_to_add = max(sum_time_drones_used[0] + time_to_wait[0],sum_time_drones_used[1] + time_to_wait[1])
                total_time += time_to_add
    
    def import_file(self, file):
        with open(file, "r") as fd:
            fd.readline()
            for line in fd:
                tab = line.split(';')
                size = len(tab)
                if size < 2:
                    continue

                time = int(tab[0])
                location = (-1,-1)
                destination = (-1,-1)
                droneID = -1
                demandID = -1

                eventName = tab[1][1:12]
                if eventName == "LIVRAISON D":
                    droneID = int(tab[1][17:18])
                    demandID = int(tab[1].split(':')[1])
                else:
                    lat = get_coordinate_from_file(tab[size-2])
                    lon = get_coordinate_from_file(tab[size-1])
                    location = (lat, lon)
                #if eventName == "ARRIVEE VEH":
                if eventName == "DEPLACEMENT":
                    lat = get_coordinate_from_file(tab[1].split('(')[1])
                    lon = get_coordinate_from_file(tab[2])
                    destination = (lat, lon)
                elif eventName == "LARGAGE DRO":
                    droneID = int(tab[1][15:16])
                    demandID = int(tab[1].split(':')[1])
                elif eventName == "RECUPERATIO":
                    droneID = int(tab[1][20:21])
                elif eventName == "LIVRAISON C":
                    demandID = int(tab[1].split(':')[1])
                self.list_events.append(TSPDEvent(time, location, destination, droneID, demandID))
        fd.close()

    def update_from_x_to_event(self):
        is_delivery = [self.data.get_demand_amount(i) for i in range(self.data.get_number_demand_nodes())]
        if len(self.truck_tour) > 0:
            time = 0
            previous_vertex = self.truck_tour.get(time)
            location = self.data.get_node_location(previous_vertex)
        for key in self.truck_tour.keys():
            vertex = self.truck_tour.get(key)
            if key == 0:
                continue
            destination = self.data.get_node_location(vertex)
            #movement
            time = key - self.data.shortest_path_value(previous_vertex, vertex)
            self.list_events.append(TSPDEvent(time, location=location, destination=destination))
            #arrival
            self.list_events.append(TSPDEvent(key, location=destination))
            previous_vertex = vertex
            location = destination
            #time = key
        #delivery
        for time in self.truck_delivery.keys():
            vertex = self.truck_delivery.get(time)
            location = self.data.get_node_location(vertex)
            demandID = self.data.get_demand_id(vertex)
            for i in range(self.data.get_demand_amount(demandID)):
                self.list_events.append(TSPDEvent(time, location=location, demandID=demandID-1))
        """
        for droneID in range(2):
            dict = self.drone_tour[droneID]
            for time in dict.keys():
                tab = dict.get(time)
                start = self.data.get_node_location(tab[0])
                step = self.data.get_node_location(tab[1])
                demandID = self.data.get_demand_id(tab[1])
                end = self.data.get_node_location(tab[2])
                #lift off
                self.list_events.append(TSPDEvent(time, location=start, droneID=droneID, demandID=demandID))
                time += self.data.get_drone_graph().get(start).get(step)
                #delivery
                self.list_events.append(TSPDEvent(time, droneID=droneID, demandID=demandID))
                #pick up
                #!!! time must be greater than time
                time += self.data.get_drone_graph().get(step).get(end)
                self.list_events.append(TSPDEvent(time, location=start, droneID=droneID))
        """
        self.list_events.sort(key=lambda x: x.destination[0])
        self.list_events.sort(key=lambda x: x.time)

    def update_from_event_to_x(self):
        """
        for event in self.list_events:
            print(event)
        """

    def export(self, name="solution"):
        with open(name + ".txt", 'w') as fd:
            fd.write("TEMPS ; EVENEMENT ; LOCALISATION\n")
            for event in self.list_events:
                fd.write(event.display())
        fd.close()
        print(name + ".txt has been saved")

    def to_map(self, verbose=True, name="solution", road_graph_display=True, truck_tour_display=True, drone_tour_display=[True, True]):
        center = (self.data.df_vertices.lat.mean(), self.data.df_vertices.lon.mean())
        map = folium.Map(location=center, zoom_start=13, control_scale=True)

        if road_graph_display:
            for index, edge in self.data.df_edges.iterrows():
                location_start = self.data.get_node_location(edge.start_id)
                location_end = self.data.get_node_location(edge.end_id)
                folium.PolyLine([location_start, location_end]).add_to(map)
            for index, vertex in self.data.df_vertices.iterrows():
                location = (vertex["lat"], vertex["lon"])
                folium.Circle(radius=10, location=location, popup=index, color="#0000FF", fill=False).add_to(map)

        for index, demand in self.data.df_customers.iterrows():
            location = (demand["lat"], demand["lon"])
            if demand["amount"] == 0:
                folium.Circle(radius=11, location=location, popup=index, color="#00FFFF", fill=False).add_to(map)
            for i in range(ceil(demand["amount"])):
                folium.Circle(radius=11+2*i, location=location, popup=index, color="red", fill=False).add_to(map)
            #folium.Marker(location, popup=index, icon=folium.Icon(color="darkblue")).add_to(map)
        #folium.Marker(self.data.depot_location, popup=index, icon=folium.Icon(color="lightblue")).add_to(map)

        color_drone = ["darkgreen", "gray"]
        color_truck = "purple"
        demand_amount = self.data.df_customers.amount.tolist()
        drone_on_truck = [True, True]

        location_drone = [self.data.depot_location, self.data.depot_location]
        for event in self.list_events:
            time = event.time
            location = event.location
            destination = event.destination
            droneID = event.droneID
            demandID = event.demandID

            if location == (-1,-1) and droneID != -1 and demandID != -1:
                location_drone[droneID] = self.data.get_demand_location(demandID)
                folium.Circle(radius=10-2*demand_amount[demandID], location=location_drone[droneID], popup=index, color=color_drone[droneID], fill=True).add_to(map)
                demand_amount[demandID] -= 1
            if location != (-1,-1) and droneID == -1 and demandID == -1:
                folium.Circle(radius=10, location=location, popup=index, color=color_truck, fill=False).add_to(map)
            if location != (-1,-1) and destination != (-1,-1):
                if drone_tour_display[0] and drone_on_truck[0]:
                    folium.PolyLine([location, destination], color=color_drone[0]).add_to(map)
                if drone_tour_display[1] and drone_on_truck[1]:
                    folium.PolyLine([location, destination], color=color_drone[1]).add_to(map)
                if truck_tour_display:
                    folium.PolyLine([location, destination], color=color_truck).add_to(map)
            if location != (-1,-1) and droneID != -1 and demandID != -1:
                if drone_tour_display[droneID]:
                    location_drone[droneID] = self.data.get_demand_location(demandID)
                    folium.PolyLine([location, location_drone[droneID]], color=color_drone[droneID], dash_array='20', dash_offset='2').add_to(map)
                    drone_on_truck[droneID] = False
            if location != (-1,-1) and droneID != -1 and demandID == -1:
                if drone_tour_display[droneID]:
                    folium.PolyLine([location_drone[droneID], location], color=color_drone[droneID], dash_array='20', dash_offset='2').add_to(map)
                    drone_on_truck[droneID] = True
            if location != (-1,-1) and droneID == -1 and demandID != -1:
                folium.Circle(radius=10-2*demand_amount[demandID], location=self.data.get_demand_location(demandID), popup=index, color=color_truck, fill=True).add_to(map)
                demand_amount[demandID] -= 1
        name += ".html"
        map.save(name)
        print(name + " has been saved")

    def display(self):
        print("Value = ", self.value, "s")
        print("Truck tour :")
        for t in self.truck_tour.keys():
            print("\ttime : ", t, ", node :", self.truck_tour.get(t))
        print("Truck delivery :")
        print(self.truck_delivery)
        print("Drone 0 :")
        for d in self.drone_tour[0].keys():
            print("\ttime : ", d, ", tour :", self.drone_tour[0].get(d))
        print("Drone 1 :")
        for d in self.drone_tour[1].keys():
            print("\ttime : ", d, ", tour :", self.drone_tour[1].get(d))
