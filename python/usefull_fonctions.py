import matplotlib.pyplot as plt
from TSPDData import *

def draw_on_plan(instance, order):
    for i in range(len(order)):
        x1 = instance.positions[instance.indices_customers[order[i-1]]][0]
        y1 = instance.positions[instance.indices_customers[order[i-1]]][1]
        x2 = instance.positions[instance.indices_customers[order[i]]][0]
        y2 = instance.positions[instance.indices_customers[order[i]]][1]
        #print(x1,y1,x2,y2)
        plt.plot([x1,x2],[y1,y2])

    plt.show()
        
def write_in_txt(instance,order):
    with open("results_tsp.txt", 'w') as file:
        file.write("#indice client, latitude, longitude\n")
        for i in range(len(order)):
            x = instance.positions[instance.indices_customers[order[i]]][0]
            y = instance.positions[instance.indices_customers[order[i]]][1]
            file.write(str(order[i]) + " " + str(x) + " " + str(y) + "\n")
