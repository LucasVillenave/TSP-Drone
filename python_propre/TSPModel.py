import gurobipy as gp
from gurobipy import GRB
import python_propre.TSPDData
from itertools import combinations

class TSPModel:
    def __init__(self, data):
        self.data = data

    def solve(self):
        # Callback - use lazy constraints to eliminate sub-tours
        def subtourelim(model, where):
            if where == GRB.Callback.MIPSOL:
                vals = model.cbGetSolution(model._vars)
                # find the shortest cycle in the selected edge list
                tour = subtour(vals)
                if len(tour) < n:
                    # add subtour elimination constr. for every pair of cities in tour
                    model.cbLazy(gp.quicksum(model._vars[i, j]
                                            for i, j in combinations(tour, 2))
                                <= len(tour)-1)

        # Given a tuplelist of edges, find the shortest subtour

        def subtour(vals):
            # make a list of edges selected in the solution
            edges = gp.tuplelist((i, j) for i, j in vals.keys()
                                if vals[i, j] > 0.5)
            unvisited = list(range(n))
            cycle = range(n+1)  # initial length has 1 more city
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

        instance = self.data.get_compact_graph()
        n = len(instance)
        dist = {(i, j):
                instance[i][j]
                #math.sqrt(sum((instance.positions[instance.indices_customers[i]][k]-instance.positions[instance.indices_customers[j]][k])**2 for k in range(2)))  #remplacer ligne précédente par celle-là pour farie avec dist à vol d'oiseau
                for i in range(n) for j in range(i)}



        m = gp.Model()

        # Create variables
        vars = m.addVars(dist.keys(), obj=dist, vtype=GRB.BINARY, name='e')
        for i, j in vars.keys():
            vars[j, i] = vars[i, j]  # edge in opposite direction

        # You could use Python looping constructs and m.addVar() to create
        # these decision variables instead.  The following would be equivalent
        # to the preceding m.addVars() call...
        #
        # vars = tupledict()
        # for i,j in dist.keys():
        #   vars[i,j] = m.addVar(obj=dist[i,j], vtype=GRB.BINARY,
        #                        name='e[%d,%d]'%(i,j))


        # Add degree-2 constraint

        m.addConstrs(vars.sum(i, '*') == 2 for i in range(n))

        # Using Python looping constructs, the preceding would be...
        #
        # for i in range(n):
        #   m.addConstr(sum(vars[i,j] for j in range(n)) == 2)


        # Optimize model

        m._vars = vars
        m.Params.LazyConstraints = 1
        m.optimize(subtourelim)

        vals = m.getAttr('X', vars)
        tour = subtour(vals)
        assert len(tour) == n

        tour.append(0)
        obj = m.ObjVal + self.data.get_number_demand_nodes()*self.data.truck_delivery_time
        print('')
        print('Optimal tour: %s' % str(tour))
        print('Optimal cost: %g' % obj)
        print('')
        return obj, tour
