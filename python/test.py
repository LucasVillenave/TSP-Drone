import pandas as pd
import numpy as np
import igraph as ig
import matplotlib.pyplot as plt
import networkx as nx
import geopy.distance


def parser(path):
    df = pd.read_json(path + "Graph.txt")
    df[['lat_min','lon_min','lat_max','lon_max','length']].astype(int)


    df['departs'] = list(zip(df.lat_min, df.lon_min))
    df['arrivees'] = list(zip(df.lat_max, df.lon_max))

    col_arrivees = {'points' : df['departs']}
    col_departs = {'points' : df['arrivees']}
    col_arrivees = pd.DataFrame(col_arrivees)
    col_departs = pd.DataFrame(col_departs)
    vertices = col_arrivees.merge(col_departs)
    vertices = vertices.drop_duplicates()
    vertices = vertices.reset_index()
    vertices = vertices.drop(columns = ['index'])
    vertices = vertices.reset_index()


    df = df.merge(vertices, left_on='departs', right_on='points').reindex(columns=['index', 'lat_min', 'lon_min', 'lat_max', 'lon_max', 'length', 'type', 'arrivees'])
    df.rename(columns={'index': 'start_id'}, inplace = True)
    df = df.merge(vertices, left_on='arrivees', right_on='points').reindex(columns=['start_id','index', 'lat_min', 'lon_min', 'lat_max', 'lon_max', 'length', 'type'])
    df.rename(columns={'index': 'end_id'}, inplace = True)

    df['type'].loc[(df['type'] != 'primary') & (df['type'] != 'secondary')] = 30/3.6
    df.replace({'type' : { 'primary' : 65/3.6, 'secondary' : 45/3.6}}, inplace=True)

    df['costs'] = df['length'] / df['type']
    df.drop(columns=['length','type','lat_min','lon_min','lat_max','lon_max'], inplace=True)

    df_demands = pd.read_json(path + "Demand.txt")
    df_demands['points'] = list(zip(df_demands.lat, df_demands.lon))
    vertices = vertices.merge(df_demands, left_on='points', right_on='points', how = 'left').reindex(columns=['index', 'points', 'amount'])


    g= nx.from_pandas_edgelist(df, 'start_id','end_id','costs')

    all_paths = dict(nx.all_pairs_dijkstra_path(g, weight='costs'))
    all_paths_length = dict(nx.all_pairs_dijkstra_path_length(g, weight='costs'))

    drone_time = {u:{ v:{
                            'drone_time':geopy.distance.geodesic(vertices.at[u,'points'], vertices.at[v,'points']).m/(50/3.6)}
                            for v in range(len(vertices.index))} 
                            for u in range(len(vertices.index))}
    truck_shortest_time = {u:{ v:{
                            'truck_shortest_time':all_paths_length[u][v]}
                            for v in range(len(vertices.index))} 
                            for u in range(len(vertices.index))}
    truck_shortest_path = {u:{ v:{
                            'truck_shortest_path':all_paths[u][v]}
                            for v in range(len(vertices.index))} 
                            for u in range(len(vertices.index))}
    road_graph = {u:{ v:{
                            'road_time':g[u][v]['costs']}
                            for v in range(len(vertices.index)) if g.has_edge(u,v)} 
                            for u in range(len(vertices.index))}

    return drone_time, truck_shortest_time, truck_shortest_path, road_graph