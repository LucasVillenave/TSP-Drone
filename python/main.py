from TSPDData import *
from heur_short_path import *

instance = TSPDData("../Data/init/Graph.txt", "../Data/init/Demand.txt")

solver = heur_short_path(instance)
obj,val_x,val_w = solver.solve()