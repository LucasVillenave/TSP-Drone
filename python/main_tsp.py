from itertools import combinations
import gurobipy as gp
from gurobipy import GRB
from TSPDData import *
from usefull_fonctions import *

# Callback - use lazy constraints to eliminate sub-tours
def subtourelim(model, where):
    if where == GRB.Callback.MIPSOL:
        vals = model.cbGetSolution(model._vars)
        # find the shortest cycle in the selected edge list
        tour = subtour(vals)
        if len(tour) < nb_clients:
            # add subtour elimination constr. for every pair of cities in tour
            model.cbLazy(gp.quicksum(model._vars[i, j] for i, j in combinations(tour, 2)) <= len(tour)-1)

# Given a tuplelist of edges, find the shortest subtour

def subtour(vals):
    # make a list of edges selected in the solution
    edges = gp.tuplelist((i, j) for i, j in vals.keys()
                        if vals[i, j] > 0.5)
    unvisited = list(range(nb_clients))
    cycle = range(nb_clients+1)  # initial length has 1 more city
    while unvisited:  # true if list is non-empty
        thiscycle = []
        neighbors = unvisited
        while neighbors:
            current = neighbors[0]
            thiscycle.append(current)
            unvisited.remove(current)
            neighbors = [j for i, j in edges.select(current, '*')
                        if j in unvisited]
        if len(cycle) > len(thiscycle):
            cycle = thiscycle
    return cycle

instance = TSPDData("../Data/init/Graph.txt", "../Data/init/Demand.txt")
nb_clients = len(instance.distance_customers)
dist = {(i,j) : instance.distance_customers[i][j] for i in range(nb_clients) for j in range(nb_clients)}
    
m = gp.Model()
vars = m.addVars(dist.keys(), obj=dist, vtype=GRB.BINARY, name='e')
for i, j in vars.keys():
    vars[j, i] = vars[i, j]  # edge in opposite direction

# Add degree-2 constraint
m.addConstrs(vars.sum(i,'*') == 1 for i in range(nb_clients))
m.addConstrs(vars.sum('*',i) == 1 for i in range(nb_clients))

# Optimize model

m._vars = vars
m.Params.LazyConstraints = 1
m.optimize(subtourelim)

vals = m.getAttr('X', vars)
tour = subtour(vals)