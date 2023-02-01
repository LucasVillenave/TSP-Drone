from itertools import combinations
import gurobipy as gp
from gurobipy import GRB
from TSPDData import *
from usefull_fonctions import *
import math
import matplotlib.pyplot as plt

# Callback - use lazy constraints to eliminate sub-tours
def subtourelim(model, where):
    if where == GRB.Callback.MIPSOL:
        vals = model.cbGetSolution(model._vars)
        # find the shortest cycle in the selected edge list
        #print("WAZAIBIZ")
        sets = subtour(vals)
        #print(instance.indices_customers)
        #print(len(sets),sum([len(set) for set in sets]), sets)
        for set in sets:
            sum1 = 0
            sum2 = 0
            for i in set:
                for j in range(len(instance.adjacency_matrix[i])):
                    if not instance.adjacency_matrix[i][j][0] in set:
                        sum1 += model._vars[i, instance.adjacency_matrix[i][j][0]]
                        sum2 += model._vars[instance.adjacency_matrix[i][j][0], i]
            #print(sum1)
            model.cbLazy(sum1 >= 1)
            model.cbLazy(sum2 >= 1)

# Given a tuplelist of edges, find the shortest subtour

def subtour(vals):
    # make a list of edges selected in the solution
    edges = gp.tuplelist((i, j) for i, j in vals.keys()
                         if vals[i, j] > 0.5)
    
    #for ele in vals:
    #    if vals[ele] == 1:# or vals[ele] == 0: 
    #        x1 = instance.positions[ele[0]][0]
    #        y1 = instance.positions[ele[0]][1]
    #        x2 = instance.positions[ele[1]][0]
    #        y2 = instance.positions[ele[1]][1]
    #        plt.plot([x1,x2],[y1,y2])
    #for pos in instance.positions:
    #    plt.plot(pos[0],pos[1], markersize = 1)
    #print("WAZAI")
    #plt.show()
    unvisited = [(i,j) for i,j in edges]
    
    sets = [] 
    nb_rounds = 0
    while unvisited:  # true if list is non-empty
        this_set = []
        neighbors = unvisited
        to_check = [neighbors[0]]
        nb_cust_in_comp = 0
        while to_check:
            current = to_check[0]
            if current[0] not in this_set: 
                if current[0] in instance.indices_customers or current[0] == 0:
                    nb_cust_in_comp += 1
                this_set.append(current[0])
            if current[1] not in this_set: 
                if current[1] in instance.indices_customers or current[1] == 0:
                    nb_cust_in_comp += 1
                this_set.append(current[1])
            to_check.remove(current)
            for i,j in edges.select(current[0], '*'):
                if (i,j) in unvisited and not (i,j) in to_check:
                    unvisited.remove((i,j))
                    to_check.append((i,j))
            for i,j in edges.select(current[1], '*'):
                if (i,j) in unvisited and not (i,j) in to_check:
                    unvisited.remove((i,j))
                    to_check.append((i,j))
        if nb_cust_in_comp >= 1 and nb_cust_in_comp <= len(instance.indices_customers):
            sets.append(this_set)
    return sets

instance = TSPDData("../Data/init/Graph.txt", "../Data/init/Demand.txt")

n = len(instance.positions)
dist = {(i, instance.adjacency_matrix[i][j][0]):
        instance.adjacency_matrix[i][j][1]
        #math.sqrt(sum((instance.positions[instance.indices_customers[i]][k]-instance.positions[instance.indices_customers[j]][k])**2 for k in range(2)))  #remplacer ligne précédente par celle-là pour farie avec dist à vol d'oiseau
        for i in range(n) for j in range(len(instance.adjacency_matrix[i]))}
    
for i in range(n):
    for j in range(len(instance.adjacency_matrix[i])):
        print((i,instance.adjacency_matrix[i][j][0]))
        print(dist[(i,instance.adjacency_matrix[i][j][0])])
        print(dist[(instance.adjacency_matrix[i][j][0],i)])

envi = gp.Env()
#envi.setParam('TimeLimit', 5*60)
m = gp.Model(env = envi)

# Create variables

vars = m.addVars(dist.keys(), obj=dist, vtype=GRB.BINARY, name='e')
# You could use Python looping constructs and m.addVar() to create
# these decision variables instead.  The following would be equivalent
# to the preceding m.addVars() call...
#
# vars = tupledict()
# for i,j in dist.keys():
#   vars[i,j] = m.addVar(obj=dist[i,j], vtype=GRB.BINARY,
#                        name='e[%d,%d]'%(i,j))


# Add degree-2 constraint

for i in range(n):
    if instance.positions[i][2] != 0 or i == 0:
        m.addConstr(sum(vars[(i,instance.adjacency_matrix[i][j][0])] for j in range(len(instance.adjacency_matrix[i]))) >= 1)
        m.addConstr(sum(vars[(instance.adjacency_matrix[i][j][0],i)] for j in range(len(instance.adjacency_matrix[i]))) >= 1)
    m.addConstr(sum(vars[i,instance.adjacency_matrix[i][j][0]] for j in range(len(instance.adjacency_matrix[i]))) == sum(vars[instance.adjacency_matrix[i][j][0],i] for j in range(len(instance.adjacency_matrix[i]))))
    


#for i, j in vars.keys():
    #m.addConstr(vars[i,j] + vars[j,i] <= 1)
    

# Optimize model
count_CB = 0
m._vars = vars
m.Params.LazyConstraints = 1
m.optimize(subtourelim)

vals = m.getAttr('X', vars)

for ele in vals:
    if vals[ele] == 1:# or vals[ele] == 0: 
        print(ele, vals[ele], dist[ele])
        x1 = instance.positions[ele[0]][0]
        y1 = instance.positions[ele[0]][1]
        x2 = instance.positions[ele[1]][0]
        y2 = instance.positions[ele[1]][1]
        plt.plot([x1,x2],[y1,y2])
for pos in instance.positions:
    plt.plot(pos[0],pos[1])
plt.show()
set= subtour(vals)
#assert not is_good

print('')
print('Optimal set:', set)
print('Optimal cost: %g' % m.ObjVal)
print('')

#draw_on_plan(instance, tour)
#write_in_txt(instance, tour)