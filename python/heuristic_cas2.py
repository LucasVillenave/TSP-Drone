from TSPDData import *

def shortest_path(instance, point1, point2):
    distance_min = [instance.max_cost * len(instance.positions)] * len(instance.positions)
    prec = [-1] * len(instance.positions)
    distance_min[point1] = 0
    to_visit = [point1]
    dist_node_min = 0
    while to_visit:
        node_min = -1
        dist_node_min = instance.max_cost * len(instance.positions)
        for i in range(len(to_visit)):
            if distance_min[to_visit[i]] < dist_node_min:
                dist_node_min = distance_min[to_visit[i]]
                node_min = to_visit[i]
        if node_min == point2:
            break
        to_visit.remove(node_min)
        for i in range(len(instance.adjacency_matrix[node_min])):
            if distance_min[node_min] + instance.adjacency_matrix[node_min][i][1] < distance_min[instance.adjacency_matrix[node_min][i][0]]:
                distance_min[instance.adjacency_matrix[node_min][i][0]] = distance_min[node_min] + instance.adjacency_matrix[node_min][i][1]
                prec[instance.adjacency_matrix[node_min][i][0]] = node_min
                to_visit.append(instance.adjacency_matrix[node_min][i][0])
    best_path = [point2]
    while(best_path[-1] != point1):
        best_path.append(prec[best_path[-1]])
    return dist_node_min, best_path
        



file = open('results_tsp.txt','r')
file = file.readlines()

truck_path_cust = []
for i in range(len(file)):
    if i == 0 :continue
    line = file[i].split()
    truck_path_cust.append(int(line[0]))
print(truck_path_cust)

instance = TSPDData("../Data/init/Graph.txt", "../Data/init/Demand.txt")

truck_path = []
for i in range(len(truck_path_cust)):
    dist_this_path, this_path = shortest_path(instance, truck_path_cust[(i+1)%len(truck_path_cust)], truck_path_cust[i])
    truck_path.append(this_path)
print(truck_path)



