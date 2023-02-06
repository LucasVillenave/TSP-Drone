from TSPDData import *

class TSPDSolution:
    def __init__(self, data):
        self.data = data
        self.value = -1
        #truck tour must be in the form : depot, each node visited, depot
        self.truck_tour = []
        #drone tour must be in the form : {time : [lift off node, delivery node, pick up node]}
        self.drone_tour = [{}, {}]

    def import_TSP(self, obj, tour):
        self.value = obj
        if len(tour) < 1 :
            return
        node_id = self.data.df_customers.loc[tour[0]].node_id
        self.truck_tour.append(node_id)
        for t in range(0, len(tour)-1):
            for j in self.data.truck_shortest_path.get(tour[t]).get(tour[t+1])[1:]:
                if j != self.truck_tour[len(self.truck_tour)-1]:
                    self.truck_tour.append(j)

    def import_H1(self, obj, x, y):
        return None

    def import_file(self, file):
        with open(file, "r") as fd:
            line = fd.readline()
            #...
        fd.close()

    def update(self):
        #tour to list_event
        return None

    def export(self, name="solution"):
        self.update()
        with open(name + ".txt", 'w') as fd:
            fd.write("TEMPS ; EVENEMENT ; LOCALISATION\n")
            #...
        fd.close()

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
        print("Truck tour :", self.truck_tour)
        print("Drone 0 :")
        for d in self.drone_tour[0].keys():
            print("\ttime : ", d, ", tour :", self.drone_tour[0].get(d))
        print("Drone 1 :")
        for d in self.drone_tour[1].keys():
            print("\ttime : ", d, ", tour :", self.drone_tour[1].get(d))
