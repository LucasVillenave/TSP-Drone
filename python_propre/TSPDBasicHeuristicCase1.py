import gurobipy as gp
from gurobipy import GRB
from TSPDData import *

class TSPDBasicHeuristicCase1:
    def __init__(self, data):
        self.data = data
        #truck visit a subset of vertices 
