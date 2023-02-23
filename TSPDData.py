import pandas as pd
import numpy as np
import folium
import networkx as nx
import geopy.distance 

class TSPDData:
    def __init__(self, path):
        #speed of drone (0) and truck on primary (1), secondary (2) and other (3) type road
        self.speed = [50/3.6, 65/3.6, 45/3.6, 30/3.6]
        #data, int
        self.truck_delivery_time = 60
        self.drone_delivery_time = 0
        self.recharge_time = 30
        #fixed location of depot, tuble of double
        self.depot_location = (44.8500102, 0.5370699)
        #self.depot_location = (44.8578271, 0.5968475)#for test

        #DataFrame with attributes
        self.df_edges, self.df_vertices = self.parse_file(path)
        # id of depot, int
        self.depot_id = self.find_depot()
        #DataFrame with customers and depot
        self.df_customers = self.get_all_customers()
        self.create_all_graphs()

    def parse_file(self, filepath):
        """
        read graph and demand file to get information in edges and vertices DataFrame
        """
        df_edges = pd.read_json(filepath + "/Graph.txt")
        df_demands = pd.read_json(filepath + "/Demand.txt")

        #Get vertices from edges
        col_arrivees = pd.DataFrame({'lat' : df_edges['lat_max'], 'lon' : df_edges['lon_max']})
        col_departs = pd.DataFrame({'lat' : df_edges['lat_min'], 'lon' : df_edges['lon_min']})
        df_vertices = col_arrivees.merge(col_departs, how='outer')
        df_vertices.drop_duplicates(inplace=True)
        df_vertices.reset_index(inplace=True)
        df_vertices.drop(columns = ['index'], inplace=True)
        df_vertices.reset_index(inplace=True)

        #Get edges from start_id to end_id corresponding to vertices id
        df_edges = df_edges.merge(df_vertices, left_on=['lat_max', 'lon_max'], right_on=['lat', 'lon']).reindex(columns=['index', 'lat_min', 'lon_min', 'lat_max', 'lon_max', 'length', 'type'])
        df_edges.rename(columns={'index': 'start_id'}, inplace = True)
        df_edges = df_edges.merge(df_vertices, left_on=['lat_min', 'lon_min'], right_on=['lat', 'lon']).reindex(columns=['start_id','index', 'length', 'type'])
        df_edges.rename(columns={'index': 'end_id'}, inplace = True)
        df_edges['type'].loc[(df_edges['type'] != 'primary') & (df_edges['type'] != 'secondary')] = self.speed[3]
        df_edges.replace({'type' : { 'primary' : self.speed[1], 'secondary' : self.speed[2]}}, inplace=True)
        df_edges['costs'] = df_edges['length'] / df_edges['type']
        df_edges.drop(columns=['length','type'], inplace=True)

        #Get demands without duplicated
        df_demands['amount'] = df_demands.groupby(['lat', 'lon'])['amount'].transform('sum')
        df_demands.drop_duplicates(inplace=True)
        df_demands.reset_index(inplace=True)
        df_demands.drop(columns = ['index'], inplace=True)
        #Matching demands with vertices
        df_vertices = df_vertices.merge(df_demands, left_on=['lat', 'lon'], right_on=['lat', 'lon'], how = 'left').reindex(columns=['index', 'lat', 'lon', 'amount'])

        return df_edges, df_vertices

    def find_depot(self):
        for id in self.df_vertices.index[self.df_vertices['lat'] == self.depot_location[0]].to_list():
            if id in self.df_vertices.index[self.df_vertices['lon'] == self.depot_location[1]].to_list():
                return id
        return -1

    def get_all_customers(self):
        """
        return a DataFrame with index of demand (index), index of node (node_id) and amount (amount)
        """
        depot = pd.DataFrame({'index':self.depot_id, 'lat':self.depot_location[0], 'lon':self.depot_location[1], 'amount':0}, index=[0])
        customers = self.df_vertices.dropna()
        customers = customers.append(depot)
        customers.sort_index(inplace=True)
        customers.reset_index(drop=True, inplace=True)
        customers.rename(columns={'index':'node_id'}, inplace=True)
        customers.reset_index(inplace=True)
        return customers

    def create_all_graphs(self):
        self.road_graph = nx.from_pandas_edgelist(self.df_edges, 'start_id','end_id','costs')
        nb_vertices = self.road_graph.number_of_nodes()

        self.all_paths = dict(nx.all_pairs_dijkstra_path(self.road_graph, weight='costs'))
        self.all_paths_length = dict(nx.all_pairs_dijkstra_path_length(self.road_graph, weight='costs'))
        self.drone_time = {u:{ v:
                                geopy.distance.geodesic((self.df_vertices.at[u,'lat'],self.df_vertices.at[u,'lon']), (self.df_vertices.at[v,'lat'],self.df_vertices.at[v,'lon'])).m / self.speed[0]
                                for v in range(nb_vertices)}
                                for u in range(nb_vertices)}
        self.truck_shortest_time = {u:{ v: self.all_paths_length[self.df_customers.node_id.loc[u]][self.df_customers.node_id.loc[v]]
                                for v in self.df_customers.index.tolist()}
                                for u in self.df_customers.index.tolist()}
        self.truck_shortest_path = {u:{ v: self.all_paths[self.df_customers.node_id.loc[u]][self.df_customers.node_id.loc[v]]
                                for v in self.df_customers.index.tolist()}
                                for u in self.df_customers.index.tolist()}
        self.road_time = {u:{ v: self.road_graph[u][v]['costs']
                                for v in range(nb_vertices) if self.road_graph.has_edge(u,v)}
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
        return None

    def get_compact_graph(self):
        """
        return the graph with customer nodes and shortest path
        access : truck_shortest_time.get(u).get(v) = time of shortest path between u and v, customers
        truck_shortest_time.get(u).get(v) must be equal to truck_shortest_time.get(v).get(u)
        """
        return self.truck_shortest_time

    def get_drone_graph(self):
        """
        return the dictionary of a complete graph for drone
        """
        return self.drone_time

    def get_node_location(self, id):
        if id < 0 or id > self.road_graph.number_of_nodes():
            return None
        lat = self.df_vertices.loc[id].lat
        lon = self.df_vertices.loc[id].lon
        return lat, lon

    def get_edge_cost(self, id):
        if id < 0 or id > self.df_edges.index.size:
            return None
        return self.df_edges.loc[id].costs

    def get_demand_nodes(self):
        """
        return list of nodes id with positive demand and depot at first
        """
        return self.df_customers.node_id.to_list()

    def get_number_demand_nodes(self):
        """
        return the number of nodes with positive demand
        """
        return self.df_customers.index.size - 1

    def get_total_demand(self):
        """
        return the sum of all demand
        """
        return self.df_vertices['amount'].sum()

    def shortest_path(self, start, end, get_path = False):
        """
        return the shortest path from start to end and its value
        if star or end aren't a node then return None
        """
        if start < 0 or start > self.road_graph.number_of_nodes() or end < 0 or end > self.road_graph.number_of_nodes() :
            return None
        if get_path == True:
            path = self.all_paths[start][end]
            value = self.all_paths_length[start][end]
            return path, value
        else: 
            value = self.all_paths_length[start][end]
            return value


    def display(self, name="graph"):
        center = (self.df_vertices.lat.mean(), self.df_vertices.lon.mean())
        map = folium.Map(location=center, zoom_start=13, control_scale=True)
        for index, vertex in self.df_vertices.iterrows():
            location = (vertex["lat"], vertex["lon"])
            folium.Circle(radius=10, location=location, popup=index, color="#0000FF", fill=False).add_to(map)
        for index, edge in self.df_edges.iterrows():
            location_start = (self.df_vertices.loc[edge.start_id].lat, self.df_vertices.loc[edge.start_id].lon)
            location_end = (self.df_vertices.loc[edge.end_id].lat, self.df_vertices.loc[edge.end_id].lon)
            folium.PolyLine([location_start, location_end]).add_to(map)
        map.save(name + ".html")
