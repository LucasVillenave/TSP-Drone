import matplotlib.pyplot as plt
import numpy as np
import json

def parse_graph(path_instance, path_txt):
    with open(path_instance) as json_data:
        data_roads = json.load(json_data)
    
    positions = []
    edges = []

    for i in range(len(data_roads)):
        x1 = data_roads[i]['lat_min']
        y1 = data_roads[i]['lon_min']
        x2 = data_roads[i]['lat_max']
        y2 = data_roads[i]['lon_max']
        index_first_point = -1
        index_second_point = -1
        for j in range(len(positions)):
            if x1 == positions[j][0] and y1 == positions[j][1]:
                index_first_point = i
            if x2 == positions[j][0] and y2 == positions[j][1]:
                index_second_point = i
        if index_first_point == -1:
            index_first_point = len(positions)
            positions += [[x1,y1]]
        if index_second_point == -1:
            index_second_point = len(positions)
            positions += [[x2,y2]]
        edges += [min(index_first_point, index_second_point), max(index_first_point, index_second_point)]

    print(len(positions))

parse_graph('Graph.txt', 'boop')