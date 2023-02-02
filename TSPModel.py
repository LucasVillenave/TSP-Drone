import gurobipy as gp
from gurobipy import GRB
from TSPDData import *

class TSPModel:
    def __init__(self, data):
        self.data = data
