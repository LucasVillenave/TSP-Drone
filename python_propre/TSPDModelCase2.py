import gurobipy as gp
from gurobipy import GRB
from TSPDData import *

class TSPDModelCase2:
    def __init__(self, data):
        self.data = data
