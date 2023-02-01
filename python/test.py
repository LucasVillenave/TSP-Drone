import pandas as pd
import numpy as np
import igraph as ig
import matplotlib.pyplot as plt
import networkx as nx

df = pd.read_json("../Data/init/Graph.txt")
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

df_demands = pd.read_json("../Data/init/Demand.txt")
df_demands['points'] = list(zip(df_demands.lat, df_demands.lon))
vertices = vertices.merge(df_demands, left_on='points', right_on='points', how = 'left').reindex(columns=['index', 'points', 'amount'])

g = ig.Graph.DataFrame(df, directed=False, vertices=vertices)

fig, ax = plt.subplots()
ig.plot(g, target=ax)
plt.show()