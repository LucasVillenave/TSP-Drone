import pandas as pd
import numpy as np
import folium
import networkx as nx
import math

def distance(origin, destination):
    """
    Calculate the Haversine distance
    """
    lat1, lon1 = origin
    lat2, lon2 = destination
    radius = 6372.8 * 1000  #m

    dlat = math.radians(lat2 - lat1)
    dlon = math.radians(lon2 - lon1)

    a = (math.sin(dlat / 2) * math.sin(dlat / 2) +
         math.cos(math.radians(lat1)) * math.cos(math.radians(lat2)) *
         math.sin(dlon / 2) * math.sin(dlon / 2))
    #c = math.atan2(math.sqrt(a), math.sqrt(1 - a))
    c = math.asin(math.sqrt(a))
    d = 2 * radius * c
    return d

def get_vertices_from_edges(df_edges):
    col_arrivees = pd.DataFrame({'lat' : df_edges['lat_max'], 'lon' : df_edges['lon_max']})
    col_departs = pd.DataFrame({'lat' : df_edges['lat_min'], 'lon' : df_edges['lon_min']})
    df_vertices = col_arrivees.merge(col_departs, how='outer')
    df_vertices.drop_duplicates(inplace=True)
    df_vertices.reset_index(inplace=True)
    df_vertices.drop(columns = ['index'], inplace=True)
    df_vertices.reset_index(inplace=True)
    return df_vertices

def update_edges_with_cost(df_edges, speed):
    df_edges.loc[(df_edges['type'] != 'primary') & (df_edges['type'] != 'secondary'), 'type'] = speed[3]
    df_edges.replace({'type' : { 'primary' : speed[1], 'secondary' : speed[2]}}, inplace=True)
    df_edges['costs'] = df_edges['length'] / df_edges['type']
    df_edges.drop(columns=['length','type'], inplace=True)

def get_df_edges_from_df_vertices_id(df_edges, df_vertices):
    df_edges = df_edges.merge(df_vertices, left_on=['lat_max', 'lon_max'], right_on=['lat', 'lon']).reindex(columns=['index', 'lat_min', 'lon_min', 'lat_max', 'lon_max', 'costs'])
    df_edges.rename(columns={'index': 'start_id'}, inplace = True)
    df_edges = df_edges.merge(df_vertices, left_on=['lat_min', 'lon_min'], right_on=['lat', 'lon']).reindex(columns=['start_id','index', 'costs'])
    df_edges.rename(columns={'index': 'end_id'}, inplace = True)
    return df_edges

def update_demand_without_duplicated(df_demands):
    df_demands['amount'] = df_demands.groupby(['lat', 'lon'])['amount'].transform('sum')
    df_demands.drop_duplicates(inplace=True)
    df_demands.reset_index(inplace=True)
    df_demands.rename(columns={'index':'demand_id'}, inplace=True)

def merge_vertices_with_demand(df_vertices, df_demands):
    df_vertices = df_vertices.merge(df_demands, left_on=['lat', 'lon'], right_on=['lat', 'lon'], how = 'left').reindex(columns=['index', 'lat', 'lon', 'amount', 'demand_id'])
    return df_vertices

def get_unit_vertices(df_vertices, df_customers):
    to_add_new_edge = []
    max_index = df_vertices.index.size
    for vertex in df_customers['index'].loc[df_customers['amount'] > 1].to_list():
        node_id = df_customers['node_id'].loc[vertex]
        to_add_new_edge.append([node_id])
        n = int(df_customers.loc[vertex].amount - 1)
        df_vertices.loc[node_id, 'amount'] = 1
        df_vertex = df_vertices.loc[node_id]
        df_vertices = df_vertices.append([df_vertex]*n,ignore_index=True)
        #for i in range(n):
        #    df_vertices = pd.concat([df_vertices, df_vertex], ignore_index=True)
        [to_add_new_edge[-1].append(i) for i in range(max_index, max_index+n)]
        max_index += n
    df_vertices.drop(columns = ['index'], inplace=True)
    df_vertices.reset_index(inplace=True)
    return df_vertices, to_add_new_edge

def update_edges_with_same_vertices(df_edges, to_add_new_edge):
    for i in range(len(to_add_new_edge)):
        for j in range(len(to_add_new_edge[i])):
            for k in range(j+1, len(to_add_new_edge[i])):
                list = [to_add_new_edge[i][j], to_add_new_edge[i][k], 0.0]
                df_edges.loc[len(df_edges)] = list

def get_all_customers(df_vertices, df_depot=[]):
    """
    return a DataFrame with index of demand (index), index of node (node_id) and amount (amount)
    """
    customers = df_vertices.dropna()
    customers = pd.concat([df_depot, customers], ignore_index=True)
    customers.sort_values('demand_id', ignore_index=True, inplace=True)
    customers.rename(columns={'index':'node_id'}, inplace=True)
    customers.reset_index(inplace=True)
    customers.drop(columns=['demand_id'], inplace=True)
    return customers

def keep_one_of_double_road(df_edges):
    df_edges['min'] = df_edges[['start_id', 'end_id']].min(axis=1)
    df_edges['max'] = df_edges[['start_id', 'end_id']].max(axis=1)
    df_edges.drop(columns = ['start_id', 'end_id'], inplace=True)
    df_edges.rename(columns={'min': 'start_id', 'max' : 'end_id'}, inplace = True)
    df_edges = df_edges.reindex(columns=['start_id','end_id', 'costs'])
    df_edges.sort_values(['start_id', 'end_id', 'costs'], ignore_index=True, inplace=True)
    df_edges.drop_duplicates(['start_id', 'end_id'], keep='first', inplace=True)
    return df_edges

class TSPDData:
    def __init__(self, path):
        #speed of drone (0) and truck on primary (1), secondary (2) and other (3) type road
        self.speed = [50/3.6, 60/3.6, 45/3.6, 30/3.6]
        #data, int
        self.truck_delivery_time = 60.0001
        self.drone_delivery_time = 0
        self.recharge_time = 30
        #fixed location of depot, tuble of double
        self.depot_location = (44.8500102, 0.5370699)
        #self.depot_location = (44.8578271, 0.5968475)#for test
        #ROAD GRAPH : df_edges, df_vertices, df_customers
        #UNIT GRAPH : df_edges_unit, df_vertices_unit, df_customers_unit
        self.parse_file(path)
        #ROAD GRAPH : road_graph(networkx), road_time(dict)
        #COMPACT GRAPH : truck_shortest_time(dict), truck_shortest_path(dict)
        #DRONE GRAPH : unit_graph(networkx), drone_time(dict)
        self.create_graph()

    def parse_file(self, filepath):
        """
        read graph and demand file to get information in edges and vertices DataFrame
        """
        df_edges = pd.read_json(filepath + "/Graph.txt")
        df_demands = pd.read_json(filepath + "/Demand.txt")

        df_vertices = get_vertices_from_edges(df_edges)
        update_edges_with_cost(df_edges, self.speed)
        self.df_edges_unit = df_edges.copy()
        self.df_edges = get_df_edges_from_df_vertices_id(df_edges, df_vertices)
        update_demand_without_duplicated(df_demands)
        self.df_vertices = merge_vertices_with_demand(df_vertices, df_demands)

        self.depot_id = self.find_depot()
        df_depot = pd.DataFrame({'index':self.depot_id, 'lat':self.depot_location[0], 'lon':self.depot_location[1], 'amount':0, 'demand_id':-1}, index=[0])

        self.df_customers = get_all_customers(self.df_vertices, df_depot)

        self.df_vertices_unit = self.df_vertices.copy()
        self.df_vertices_unit, to_add_new_edge = get_unit_vertices(self.df_vertices_unit, self.df_customers.copy())
        self.df_edges_unit = get_df_edges_from_df_vertices_id(self.df_edges_unit, self.df_vertices_unit)
        update_edges_with_same_vertices(self.df_edges_unit, to_add_new_edge)
        self.df_customers_unit = get_all_customers(self.df_vertices_unit, df_depot)

        self.df_edges = keep_one_of_double_road(self.df_edges)
        self.df_edges_unit = keep_one_of_double_road(self.df_edges_unit)


    def find_depot(self):
        for id in self.df_vertices.index[self.df_vertices['lat'] == self.depot_location[0]].to_list():
            if id in self.df_vertices.index[self.df_vertices['lon'] == self.depot_location[1]].to_list():
                return id
        return -1

    def create_graph(self):
        self.road_graph = nx.from_pandas_edgelist(self.df_edges, 'start_id','end_id','costs')
        nb_vertices = self.road_graph.number_of_nodes()

        self.all_paths = dict(nx.all_pairs_dijkstra_path(self.road_graph, weight='costs'))
        self.all_paths_length = dict(nx.all_pairs_dijkstra_path_length(self.road_graph, weight='costs'))
        self.road_time = {u:{ v: self.road_graph[u][v]['costs']
                                for v in range(nb_vertices) if self.road_graph.has_edge(u,v)}
                                for u in range(nb_vertices)}
        self.truck_shortest_time = None
        self.truck_shortest_path = None
        self.drone_time = None
        self.unit_graph = None

    def create_unit_graph(self):
        self.unit_graph = nx.from_pandas_edgelist(self.df_edges_unit, 'start_id','end_id','costs')

    def create_compact_graph(self):
        list = self.df_customers.index.tolist()
        self.truck_shortest_time = {u:{ v: self.all_paths_length[self.df_customers.node_id.loc[u]][self.df_customers.node_id.loc[v]]
                                for v in list}
                                for u in list}
        self.truck_shortest_path = {u:{ v: self.all_paths[self.df_customers.node_id.loc[u]][self.df_customers.node_id.loc[v]]
                                for v in list}
                                for u in list}

    def create_drone_graph(self):
        nb_vertices = self.road_graph.number_of_nodes()
        self.drone_time = {u:{ v:
                                distance((self.df_vertices.at[u,'lat'],self.df_vertices.at[u,'lon']), (self.df_vertices.at[v,'lat'],self.df_vertices.at[v,'lon'])) / self.speed[0]
                                for v in range(nb_vertices)}
                                for u in range(nb_vertices)}

    def get_road_graph(self):
        """
        return the road graph, the original one from file
        """
        return self.road_graph

    def get_unit_graph(self):
        """
        return the graph with unit demand
        """
        if self.unit_graph == None:
            self.create_unit_graph()
        return self.unit_graph

    def get_compact_graph(self):
        """
        return the graph with customer nodes and value of shortest path
        access : truck_shortest_time.get(u).get(v) = time of shortest path between u and v, customers
        truck_shortest_time.get(u).get(v) must be equal to truck_shortest_time.get(v).get(u)
        """
        if self.truck_shortest_time == None:
            self.create_compact_graph()
        return self.truck_shortest_time

    def get_shortest_path_graph(self):
        """
        return the graph with customer nodes and shortest path
        access : truck_shortest_path.get(u).get(v) = time of shortest path between u and v, customers
        truck_shortest_path.get(u).get(v) must be equal to truck_shortest_path.get(v).get(u)
        """
        if self.truck_shortest_path == None:
            self.create_compact_graph()
        return self.truck_shortest_path

    def get_drone_graph(self):
        """
        return the dictionary of a complete graph for drone
        """
        if self.drone_time == None:
            self.create_drone_graph()
        return self.drone_time

    def get_node_location(self, id):
        if id < 0 or id > self.road_graph.number_of_nodes():
            raise Exception(str(id) + " is not a node id")
        lat = self.df_vertices.loc[id].lat
        lon = self.df_vertices.loc[id].lon
        return lat, lon

    def get_edge_cost(self, id):
        if id < 0 or id > self.df_edges.index.size:
            raise Exception(str(id) + " is not an edge id")
        return self.df_edges.loc[id].costs

    def get_unit_edge_cost(self, id):
        if id < 0 or id > self.df_edges_unit.index.size:
            raise Exception(str(id) + " is not an edge id")
        return self.df_edges_unit.loc[id].costs

    def get_demand_nodes(self):
        """
        return list of nodes id with positive demand and depot at first
        """
        return self.df_customers.node_id.to_list()

    def get_unit_demand_nodes(self):
        """
        return list of nodes id with positive demand and depot at first
        """
        return self.df_customers_unit.node_id.to_list()

    def get_number_demand_nodes(self):
        """
        return number of nodes with positive demand
        """
        return self.df_customers.index.size - 1

    def get_number_unit_demand_nodes(self):
        """
        return number of nodes with positive demand
        """
        return self.df_customers_unit.index.size - 1

    def get_demand_id(self, node_id):
        """
        return demand_id of corresponding node_id
        """
        demandID = self.df_customers['index'].loc[self.df_customers['node_id']==node_id]
        if len(demandID) == 0:
            raise Exception(str(node_id) + " is not a demand node")
        if len(demandID) == 2:
            return int(self.df_customers['index'].loc[(self.df_customers['node_id']==node_id) & (self.df_customers['amount']>0)])
        return int(demandID)

    def get_demand_amount(self, id):
        """
        return amount of corresponding id
        """
        if id < 0 or id > self.df_customers.index.size :
            raise Exception(str(id) + " is not a demand id")
        return int(self.df_customers.loc[id].amount)

    def get_demand_location(self, demand_id):
        if demand_id < 0 or demand_id > self.df_customers.index.size :
            raise Exception(str(demand_id) + " is not a demand id")
        lat = self.df_customers.loc[demand_id].lat
        lon = self.df_customers.loc[demand_id].lon
        return lat, lon

    def get_total_demand(self):
        """
        return sum of all demand
        """
        return self.df_vertices['amount'].sum()

    def shortest_path(self, start, end):
        """
        return the shortest path from start to end
        if star or end aren't a node then return None
        """
        if start < 0 or start > self.road_graph.number_of_nodes() or end < 0 or end > self.road_graph.number_of_nodes() :
            return None
        path = self.all_paths[start][end]
        return path

    def shortest_path_value(self, start, end):
        """
        return the shortest path value from start to end
        if star or end aren't a node then return None
        """
        if start < 0 or start > self.road_graph.number_of_nodes() or end < 0 or end > self.road_graph.number_of_nodes() :
            return None
        value = self.all_paths_length[start][end]
        return value

    def draw_graph(self, name="graph"):
        center = (self.df_vertices.lat.mean(), self.df_vertices.lon.mean())
        map = folium.Map(location=center, zoom_start=13, control_scale=True)
        for index, vertex in self.df_vertices.iterrows():
            location = (vertex["lat"], vertex["lon"])
            folium.Circle(radius=10, location=location, popup=index, color="#0000FF", fill=False).add_to(map)
        for index, edge in self.df_edges.iterrows():
            location_start = (self.df_vertices.loc[edge.start_id].lat, self.df_vertices.loc[edge.start_id].lon)
            location_end = (self.df_vertices.loc[edge.end_id].lat, self.df_vertices.loc[edge.end_id].lon)
            folium.PolyLine([location_start, location_end]).add_to(map)

        for index, demand in self.df_customers.iterrows():
            location = (demand["lat"], demand["lon"])
            color = "red"
            if demand["amount"] == 0:
                color = "#00FFFF"
            folium.Circle(radius=10, location=location, popup=index, color=color, fill=False).add_to(map)
        map.save(name + ".html")
