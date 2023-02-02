import pandas as pd
import numpy as np
import folium
import networkx as nx
import geopy.distance

class TSPDData:
    def __init__(self, path):
        self.parse_file(path)

    def parse_file(self, filepath):
        df = pd.read_json(filepath + "/Graph.txt")

        col_arrivees = {'lat' : df['lat_max'], 'lon' : df['lon_max']}
        col_departs = {'lat' : df['lat_min'], 'lon' : df['lon_min']}
        col_arrivees = pd.DataFrame(col_arrivees)
        col_departs = pd.DataFrame(col_departs)
        vertices = col_arrivees.merge(col_departs)
        vertices = vertices.drop_duplicates()
        vertices = vertices.reset_index()
        vertices = vertices.drop(columns = ['index'])
        vertices = vertices.reset_index()

        df = df.merge(vertices, left_on=['lat_max', 'lon_max'], right_on=['lat', 'lon']).reindex(columns=['index', 'lat_min', 'lon_min', 'lat_max', 'lon_max', 'length', 'type'])
        df.rename(columns={'index': 'start_id'}, inplace = True)
        df = df.merge(vertices, left_on=['lat_min', 'lon_min'], right_on=['lat', 'lon']).reindex(columns=['start_id','index', 'length', 'type'])
        df.rename(columns={'index': 'end_id'}, inplace = True)
        df['type'].loc[(df['type'] != 'primary') & (df['type'] != 'secondary')] = 30/3.6
        df.replace({'type' : { 'primary' : 65/3.6, 'secondary' : 45/3.6}}, inplace=True)
        df['costs'] = df['length'] / df['type']
        df.drop(columns=['length','type'], inplace=True)

        df_demands = pd.read_json(filepath + "/Demand.txt")
        vertices = vertices.merge(df_demands, left_on=['lat', 'lon'], right_on=['lat', 'lon'], how = 'left').reindex(columns=['index', 'lat', 'lon', 'amount'])

        self.df_edges = df
        self.df_vertices = vertices

    def create_all_graphs(self):
        g = nx.from_pandas_edgelist(self.df_edges, 'start_id','end_id','costs')

        all_paths = dict(nx.all_pairs_dijkstra_path(g, weight='costs'))
        all_paths_length = dict(nx.all_pairs_dijkstra_path_length(g, weight='costs'))
        nb_vertices = len(self.df_vertices.index)

        self.drone_time = {u:{ v:{
                                'drone_time':geopy.distance.geodesic((self.df_vertices.at[u,'lat'],self.df_vertices.at[u,'lon']), (self.df_vertices.at[v,'lat'],self.df_vertices.at[v,'lon']))}
                                for v in range(nb_vertices)} 
                                for u in range(nb_vertices)}
        self.truck_shortest_time = {u:{ v:{
                                'truck_shortest_time':all_paths_length[u][v]}
                                for v in range(nb_vertices)} 
                                for u in range(nb_vertices)}
        self.truck_shortest_path = {u:{ v:{
                                'truck_shortest_path':all_paths[u][v]}
                                for v in range(nb_vertices)} 
                                for u in range(nb_vertices)}
        self.road_graph = {u:{ v:{
                                'road_time':g[u][v]['costs']}
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
        return None

    def get_number_demand_nodes(self):
        return 0

    def 

    def shortest_path(self, point1, point2):
        value = 0
        path = {}
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
        map.save("graph.html")
