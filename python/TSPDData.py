import matplotlib.pyplot as plt
import numpy as np
import json
import geopy.distance

class TSPDData:
    def __init__(self, path_graph, path_demands):
        self.positions = []  #[lat,lon,demand]
        self.edges = {}      #[ID sommet 1, ID sommet 2, duration, length, road_type]
        self.adjacency_matrix = [] #[ID neighbor, duration]
        self.max_cost = -1
        self.distance_customers = []
        self.indices_customers = []
        self.shortest_path_customers = []


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
            if (min([index_first_point, index_second_point]), max([index_first_point, index_second_point])) in self.edges : continue
            self.edges[min([index_first_point, index_second_point]), max([index_first_point, index_second_point])] =  data_roads[i]['length'] / speed
            if i == 0 or data_roads[i]['length'] / speed > self.max_cost:
                self.max_cost = data_roads[i]['length'] / speed

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

        for key in self.edges.keys():
            self.adjacency_matrix[key[0]].append([key[1], self.edges[key]])
            self.adjacency_matrix[key[1]].append([key[0], self.edges[key]])

        #DISTANCE_CUSTOMERS
        for i in range(len(self.positions)):
            if i == 0 or self.positions[i][2] > 0:
                self.distance_customers.append([])
                self.shortest_path_customers.append([])
                self.indices_customers.append(i)
                for j in range(len(self.positions)):
                    if j == 0 or self.positions[j][2] > 0:
                        dist_cus, path_cus = self.shortest_path(j,i)
                        self.distance_customers[-1].append(dist_cus)
                        self.shortest_path_customers[-1].append(path_cus)



    def shortest_path(self, point1, point2):
        distance_min = [self.max_cost * len(self.positions)] * len(self.positions)
        prec = [-1] * len(self.positions)
        distance_min[point1] = 0
        to_visit = [point1]
        dist_node_min = 0
        while to_visit:
            node_min = -1
            dist_node_min = self.max_cost * len(self.positions)
            for i in range(len(to_visit)):
                if distance_min[to_visit[i]] < dist_node_min:
                    dist_node_min = distance_min[to_visit[i]]
                    node_min = to_visit[i]
            if node_min == point2:
                break
            to_visit.remove(node_min)
            for i in range(len(self.adjacency_matrix[node_min])):
                if distance_min[node_min] + self.adjacency_matrix[node_min][i][1] < distance_min[self.adjacency_matrix[node_min][i][0]]:
                    distance_min[self.adjacency_matrix[node_min][i][0]] = distance_min[node_min] + self.adjacency_matrix[node_min][i][1]
                    prec[self.adjacency_matrix[node_min][i][0]] = node_min
                    to_visit.append(self.adjacency_matrix[node_min][i][0])
        best_path = [point2]
        while(best_path[-1] != point1):
            best_path.append(prec[best_path[-1]])
        return dist_node_min, best_path



        
