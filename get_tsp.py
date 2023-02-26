from python_propre.TSPDData import TSPDData
from python_propre.TSPModel import TSPModel
from python_propre.TSPDSolution import TSPDSolution
from python_propre.TSPDModelSPCas1 import TSPDModelSPCas1
from python_propre.TSPDModelSPCas1V2 import TSPDModelSPCas1V2
from python_propre.TSPDModelSPCas1V3 import TSPDModelSPCas1V3
import time
import collections



filename = '100'
data = TSPDData("Data/" + filename)
model = TSPModel(data)
obj, tour = model.solve()
