import folium
import python_propre.TSPDData
from python_propre.TSPDEvent import TSPDEvent

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

    def import_H1(self, x, y):
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
        return None

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

    def to_map(self, verbose=True, name="solution"):
        center = (self.data.df_vertices.lat.mean(), self.data.df_vertices.lon.mean())
        map = folium.Map(location=center, zoom_start=13, control_scale=True)
        if verbose :
            for index, vertex in self.data.df_vertices.iterrows():
                location = (vertex["lat"], vertex["lon"])
                folium.Circle(radius=10, location=location, popup=index, color="#0000FF", fill=False).add_to(map)
            for index, edge in self.data.df_edges.iterrows():
                location_start = (self.data.df_vertices.loc[edge.start_id].lat, self.data.df_vertices.loc[edge.start_id].lon)
                location_end = (self.data.df_vertices.loc[edge.end_id].lat, self.data.df_vertices.loc[edge.end_id].lon)
                folium.PolyLine([location_start, location_end]).add_to(map)
        for index, demand in self.data.df_customers.iterrows():
            location = (demand["lat"], demand["lon"])
            color = "red"
            if demand["amount"] == 0:
                color = "blue"
            if demand["amount"] == 2:
                color = "orange"
            folium.Marker(location, popup=index, icon=folium.Icon(color=color)).add_to(map)
        #truck
        location = self.data.get_node_location(self.truck_tour[0])
        for truck in self.truck_tour:
            location_pred = location
            location = self.data.get_node_location(truck)
            folium.Circle(radius=10, location=location, popup=index, color="#FF00FF", fill=False).add_to(map)
            folium.PolyLine([location_pred, location], color="#FF00FF").add_to(map)

        map.save(name + ".html")

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
