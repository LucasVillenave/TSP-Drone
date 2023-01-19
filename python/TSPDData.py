import matplotlib.pyplot as plt
import numpy as np
import json

class TSPDData:
    def __init__(self, path_graph, path_demands):
        self.positions = []  #[lat,lon,demand]
        self.edges = []      #[ID sommet 1, ID sommet 2, duration, length, road_type]
        self.adjacency_matrix = [] #[ID neighbor, cost]
        self.max_cost = -1
        self.distance_customers = []
        self.indices_customers = []


        with open(path_graph) as json_data:
            data_roads = json.load(json_data)
         
        #POSITIONS and EDGES
        for i in range(len(data_roads)):
            x1 = data_roads[i]['lat_min']
            y1 = data_roads[i]['lon_min']
            x2 = data_roads[i]['lat_max']
            y2 = data_roads[i]['lon_max']
            index_first_point = -1
            index_second_point = -1
            for j in range(len(self.positions)):
                if x1 == self.positions[j][0] and y1 == self.positions[j][1]:
                    index_first_point = j
                if x2 == self.positions[j][0] and y2 == self.positions[j][1]:
                    index_second_point = j
            if index_first_point == -1:
                index_first_point = len(self.positions)
                self.positions.append([x1,y1,0])
            if index_second_point == -1:
                index_second_point = len(self.positions)
                self.positions.append([x2,y2,0])
            speed = 30/3.6
            if data_roads[i]['type'] == 'primary':
                speed = 60/3.6
            if data_roads[i]['type'] == 'secondary':
                speed = 45/3.6
            self.edges.append([min([index_first_point, index_second_point]), max([index_first_point, index_second_point]), data_roads[i]['length'] / speed])
            if i == 0 or self.edges[-1][2] > self.max_cost:
                self.max_cost = self.edges[-1][2]

        #DEMANDS
        with open(path_demands) as json_data:
            data_demands = json.load(json_data)

        for i in range(len(data_demands)):
            x = data_demands[i]['lat']
            y = data_demands[i]['lon']
            for j in range(len(self.positions)):
                if self.positions[j][0] == x and self.positions[j][1] == y:
                    self.positions[j][2] += data_demands[i]['amount']
                    continue

        #ADJACENCY MATRIX
        for i in range(len(self.positions)):
            self.adjacency_matrix.append([])

        for i in range(len(self.edges)):
            self.adjacency_matrix[self.edges[i][0]].append([self.edges[i][1], self.edges[i][2]])
            self.adjacency_matrix[self.edges[i][1]].append([self.edges[i][0], self.edges[i][2]])

        #DISTANCE_CUSTOMERS
        for i in range(len(self.positions)):
            if i == 0 or self.positions[i][2] > 0:
                self.distance_customers.append([])
                self.indices_customers.append(i)
                for j in range(len(self.positions)):
                    if j == 0 or self.positions[j][2] > 0:
                        self.distance_customers[-1].append(self.shortest_path(i,j))



    def shortest_path(self, point1, point2):
        distance_min = [self.max_cost * len(self.positions)] * len(self.positions)
        distance_min[point1] = 0
        to_visit = [point1]
        while len(to_visit) != 0:
            node_min = -1
            dist_node_min = self.max_cost * len(self.positions)
            for i in range(len(to_visit)):
                if distance_min[to_visit[i]] < dist_node_min:
                    dist_node_min = distance_min[to_visit[i]]
                    node_min = to_visit[i]
            if node_min == point2:
                return dist_node_min
            to_visit.remove(node_min)
            for i in range(len(self.adjacency_matrix[node_min])):
                if distance_min[node_min] + self.adjacency_matrix[node_min][i][1] < distance_min[self.adjacency_matrix[node_min][i][0]]:
                    distance_min[self.adjacency_matrix[node_min][i][0]] = distance_min[node_min] + self.adjacency_matrix[node_min][i][1]
                    to_visit.append(self.adjacency_matrix[node_min][i][0])



        
