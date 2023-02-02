import pandas as pd
import numpy as np
import folium
import networkx as nx
import geopy.distance

class TSPDData:
    def __init__(self, path):
        #speed of drone (0) and truck on primary (1), secondary (2) and other (3) type road
        self.speed = [50/3.6, 65/3.6, 45/3.6, 30/3.6]
        self.truck_delivery_time = 60
        self.drone_delivery_time = 0
        self.recharge_time = 30
        self.depot_location = (44.8500102, 0.5370699)
        self.depot_id = -1
        #DataFrame with attributes
        self.df_edges, self.df_vertices = self.parse_file(path)
        #DataFrame with customers and depot
        self.df_customers = self.get_all_customers()
        self.create_all_graphs()

    def parse_file(self, filepath):
        """
        read graph and demand file to get information in edges and vertices DataFrame
        """
        df_edges = pd.read_json(filepath + "/Graph.txt")
        df_demands = pd.read_json(filepath + "/Demand.txt")

        col_arrivees = pd.DataFrame({'lat' : df_edges['lat_max'], 'lon' : df_edges['lon_max']})
        col_departs = pd.DataFrame({'lat' : df_edges['lat_min'], 'lon' : df_edges['lon_min']})
        df_vertices = col_arrivees.merge(col_departs)
        df_vertices.drop_duplicates(inplace=True)
        df_vertices.reset_index(inplace=True)
        df_vertices.drop(columns = ['index'], inplace=True)
        df_vertices.reset_index(inplace=True)

        df_edges = df_edges.merge(df_vertices, left_on=['lat_max', 'lon_max'], right_on=['lat', 'lon']).reindex(columns=['index', 'lat_min', 'lon_min', 'lat_max', 'lon_max', 'length', 'type'])
        df_edges.rename(columns={'index': 'start_id'}, inplace = True)
        df_edges = df_edges.merge(df_vertices, left_on=['lat_min', 'lon_min'], right_on=['lat', 'lon']).reindex(columns=['start_id','index', 'length', 'type'])
        df_edges.rename(columns={'index': 'end_id'}, inplace = True)
        df_edges.replace({'type' : { 'primary' : self.speed[1], 'secondary' : self.speed[2]}}, inplace=True)
        df_edges['type'].loc[(df_edges['type'] != 'primary') & (df_edges['type'] != 'secondary')] = self.speed[3]
        df_edges['costs'] = df_edges['length'] / df_edges['type']
        df_edges.drop(columns=['length','type'], inplace=True)

        df_vertices = df_vertices.merge(df_demands, left_on=['lat', 'lon'], right_on=['lat', 'lon'], how = 'left').reindex(columns=['index', 'lat', 'lon', 'amount'])

        return df_edges, df_vertices

    def get_all_customers(self):
        """
        return a DataFrame with index of demand (index), index of node (node_id) and amount (amount)
        """
        depot = pd.DataFrame({'index':self.depot_id, 'lat':self.depot_location[0], 'lon':self.depot_location[1], 'amount':0}, index=[0])
        customers = self.df_vertices.dropna()
        customers = pd.concat([depot,customers.loc[:]]).reset_index(drop=True)
        customers.reset_index(inplace=True)
        customers.drop(columns = ['index'], inplace=True)
        customers.reset_index(inplace=True)
        customers.rename(columns={'level_0':'node_id'}, inplace=True)
        return customers

    def create_all_graphs(self):
        g = nx.from_pandas_edgelist(self.df_edges, 'start_id','end_id','costs')

        all_paths = dict(nx.all_pairs_dijkstra_path(g, weight='costs'))
        all_paths_length = dict(nx.all_pairs_dijkstra_path_length(g, weight='costs'))
        nb_vertices = len(self.df_vertices.index)

        self.drone_time = {u:{ v:{
                                'drone_time':geopy.distance.geodesic((self.df_vertices.at[u,'lat'],self.df_vertices.at[u,'lon']), (self.df_vertices.at[v,'lat'],self.df_vertices.at[v,'lon']))}
                                for v in range(nb_vertices)}
                                for u in range(nb_vertices)}
        self.truck_shortest_time = {u:{ v:all_paths_length[u][v]
                                for v in self.df_customers.index.tolist()}
                                for u in self.df_customers.index.tolist()}
        self.truck_shortest_path = {u:{ v: all_paths[u][v]
                                for v in self.df_customers.index.tolist()}
                                for u in self.df_customers.index.tolist()}
        self.road_time = {u:{ v:g[u][v]['costs']
                                for v in range(nb_vertices) if g.has_edge(u,v)}
                                for u in range(nb_vertices)}


    def get_road_graph(self):
        """
        return the road graph, the original one from file
        """
        return None

    def get_unit_graph(self):
        """
        return the graph with unit demand
        """
        return None

    def get_compact_graph(self):
        """
        return the graph with customer nodes and shortest path
        """
        return self.truck_shortest_time

    def get_number_demand_nodes(self):
        return self.df_customers.index.size - 1

    def get_number_unit_demand_nodes(self):
        return 0

    def shortest_path(self, point1, point2):
        value = self.truck_shortest_time.get(point1).get(point2)
        path = self.truck_shortest_path.get(point1).get(point2)
        return value, path

    def display(self, name="graph"):
        center = (self.df_vertices.lat.mean(), self.df_vertices.lon.mean())
        map = folium.Map(location=center, zoom_start=13, control_scale=True)
        for index, vertex in self.df_vertices.iterrows():
            location = (vertex["lat"], vertex["lon"])
            folium.Circle(radius=10, location=location, popup=index, color="#0000FF", fill=False).add_to(map)
        for index, edge in self.df_edge.iterrows():
            location_start = (self.df_vertices.loc[edge.start_id].lat, self.df_vertices.loc[edge.start_id].lon)
            location_end = (self.df_vertices.loc[edge.end_id].lat, self.df_vertices.loc[edge.end_id].lon)
            folium.PolyLine([location_start, location_end]).add_to(map)
        map.save(name + ".html")
