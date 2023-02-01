import matplotlib.pyplot as plt
from TSPDData import *

def draw_on_plan(instance, order):
    for i in range(len(order)):
        print("truck does (" + str(order[i-1]) + "," + str(order[i]) + ") with costs : "+ str(instance.distance_customers[order[i-1]][order[i]]))
        x1 = instance.positions[instance.indices_customers[order[i-1]]][0]
        y1 = instance.positions[instance.indices_customers[order[i-1]]][1]
        x2 = instance.positions[instance.indices_customers[order[i]]][0]
        y2 = instance.positions[instance.indices_customers[order[i]]][1]
        #print(x1,y1,x2,y2)
        plt.plot([x1,x2],[y1,y2])

    plt.show()
        
def write_in_txt(instance,order):
    with open("results_tsp.txt", 'w') as file:
        file.write("#indice client dans liste des position, indice du client dans la solution optimal, longitude, latitude\n")
        for i in range(len(order)):
            x = instance.positions[instance.indices_customers[order[i]]][0]
            y = instance.positions[instance.indices_customers[order[i]]][1]
            file.write(str(instance.indices_customers[order[i]]) + " " + str(str(order[i]) + " " + str(x) + " " + str(y) + "\n"))
