import sys
import folium
from math import ceil
from python_propre.TSPDData import TSPDData

def get_coordinate_from_file(line):
    line = line.split(':')[1]
    line = line.split('(')[0]
    line = line.split(')')[0]
    return float(line)

def file_to_map(solution_file, data, road_graph=True, truck_tour=True, drone_tour=[True,True]):
    center = (data.df_vertices.lat.mean(), data.df_vertices.lon.mean())
    map = folium.Map(location=center, zoom_start=13, control_scale=True)

    if road_graph:
        for index, edge in data.df_edges.iterrows():
            location_start = data.get_node_location(edge.start_id)
            location_end = data.get_node_location(edge.end_id)
            folium.PolyLine([location_start, location_end]).add_to(map)
        for index, vertex in data.df_vertices.iterrows():
            location = (vertex["lat"], vertex["lon"])
            folium.Circle(radius=10, location=location, popup=index, color="#0000FF", fill=False).add_to(map)

    #folium.Marker(data.depot_location, popup=index, icon=folium.features.CustomIcon("warehouse-icon.png", icon_size=(40,40))).add_to(map)
    for index, demand in data.df_customers.iterrows():
        location = (demand["lat"], demand["lon"])
        if demand["amount"] == 0:
            folium.Circle(radius=10, location=location, popup=index, color="#00FFFF", fill=False).add_to(map)
        for i in range(ceil(demand["amount"])):
            folium.Circle(radius=10+2*i, location=location, popup=index, color="red", fill=False).add_to(map)

    color_drone = ["green", "yellow"]
    color_truck = "#FF00FF"
    demand_amount = data.df_customers.amount.tolist()
    drone_on_truck = [True, True]

    with open(solution_file, "r") as fd:
        location_drone = [data.depot_location, data.depot_location]
        fd.readline()
        for line in fd:
            tab = line.split(';')
            size = len(tab)
            if size < 2:
                continue

            time = float(tab[0])
            location = (-1,-1)
            destination = (-1,-1)
            droneID = -1
            demandID = -1

            eventName = tab[1][1:12]
            if eventName == "LIVRAISON D":
                droneID = int(tab[1][17:18])
                demandID = int(tab[1].split(':')[1]) + 1
                location_drone[droneID] = data.get_demand_location(demandID)
                folium.Circle(radius=10-2*demand_amount[demandID], location=location_drone[droneID], popup=index, color=color_drone[droneID], fill=True).add_to(map)
                demand_amount[demandID] -= 1
            else:
                lat = get_coordinate_from_file(tab[size-2])
                lon = get_coordinate_from_file(tab[size-1])
                location = (lat, lon)
            if eventName == "ARRIVEE VEH" and truck_tour:
                folium.Circle(radius=10, location=location, popup=index, color=color_truck, fill=False).add_to(map)
            if eventName == "DEPLACEMENT":
                lat = get_coordinate_from_file(tab[1].split('(')[1])
                lon = get_coordinate_from_file(tab[2])
                destination = (lat, lon)
                if drone_tour[0] and drone_on_truck[0]:
                    folium.PolyLine([location, destination], color=color_drone[0]).add_to(map)
                if drone_tour[1] and drone_on_truck[1]:
                    folium.PolyLine([location, destination], color=color_drone[1]).add_to(map)
                if truck_tour:
                    folium.PolyLine([location, destination], color=color_truck).add_to(map)
            elif eventName == "LARGAGE DRO":
                droneID = int(tab[1][15:16])
                if drone_tour[droneID]:
                    demandID = int(tab[1].split(':')[1]) + 1
                    location_drone[droneID] = data.get_demand_location(demandID)
                    folium.PolyLine([location, location_drone[droneID]], color=color_drone[droneID], dash_array='20', dash_offset='2').add_to(map)
                    drone_on_truck[droneID] = False
            elif eventName == "RECUPERATIO":
                droneID = int(tab[1][20:21])
                if drone_tour[droneID]:
                    folium.PolyLine([location_drone[droneID], location], color=color_drone[droneID], dash_array='20', dash_offset='2').add_to(map)
                    drone_on_truck[droneID] = True
            elif eventName == "LIVRAISON C":
                demandID = int(tab[1].split(':')[1]) + 1
                folium.Circle(radius=10-2*demand_amount[demandID], location=data.get_demand_location(demandID), popup=index, color=color_truck, fill=True).add_to(map)
                demand_amount[demandID] -= 1
    fd.close()
    map.save("test.html")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 draw_solution.py <instance_file> <solution_file>")

    data = TSPDData(sys.argv[1])
    file_to_map(sys.argv[2], data, road_graph=False, truck_tour=True, drone_tour=[True, True])
