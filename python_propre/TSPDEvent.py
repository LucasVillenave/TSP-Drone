class TSPDEvent:
    def __init__(self, time, location=(-1,-1), destination=(-1,-1), droneID=-1, demandID=-1):
        self.time = time
        self.location = location
        self.destination = destination
        self.droneID = droneID
        self.demandID = demandID

    def display(self):
        location = "(LAT : " + str(self.location[0]) + " ; LON : " + str(self.location[1]) + ")"
        destination = "(LAT : " + str(self.destination[0]) + " ; LON : " + str(self.destination[1]) + ")"
        result = str(self.time) + " ; "
        if self.location != (-1,-1) and self.destination != (-1,-1):
            return result + "DEPLACEMENT VEHICULE DESTINATION : " + destination + " ; " + location + "\n"
        if self.location != (-1,-1) and self.droneID == -1 and self.demandID == -1:
            return result + "ARRIVEE VEHICULE ; " + location + "\n"
        if self.location != (-1,-1) and self.droneID != -1 and self.demandID != -1:
            return result + "LARGAGE DRONE " + str(self.droneID) + " POUR LIVRAISON COLIS ID : " + str(self.demandID) + " ; " + location + "\n"
        if self.location != (-1,-1) and self.droneID != -1 and self.demandID == -1:
            return result + "RECUPERATION DRONE " + str(self.droneID) + " ; " + location + "\n"
        if self.location != (-1,-1) and self.droneID == -1 and self.demandID != -1:
            return result + "LIVRAISON COLIS ID : " + str(self.demandID) + " ; " + location + "\n"
        if self.location == (-1,-1) and self.droneID != -1 and self.demandID != -1:
            return result + "LIVRAISON DRONE " + str(self.droneID) + " COLIS ID : " + str(self.demandID) + "\n"
        raise Exception("Wrong Event")
