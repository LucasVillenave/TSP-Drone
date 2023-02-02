import gurobipy as gp
from gurobipy import GRB
from TSPDData import *

class TSPDBasicHeuristicCase2:
    def __init__(self, data):
        #subset of vertices
