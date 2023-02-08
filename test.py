from TSPDModelSPCas1 import *
import collections

instance = TSPDData("Data/init")

cas1 = TSPDModelSPCas1(instance)

cas1.solve()

