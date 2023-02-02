from TSPDData import *
from parser import *
from TSPModel import *
from TSPDSolution import *

if __name__ == "__main__":
    data = TSPDData("Data/init")
    model = TSPModel(data)
    solution = TSPDSolution(data)
    obj, tour = model.solve()
    solution.import_TSP(obj, tour)
    solution.display()
