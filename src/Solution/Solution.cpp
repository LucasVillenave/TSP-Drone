#include "Solution/Solution.hpp"
#include "utils.hpp"
#include <math.h>

Solution::Solution(Instance t_instance, const std::vector<Event>& t_eventList)
    : instance(std::move(t_instance)), eventList(t_eventList)
{
    int time = 0;
    for (Event e : t_eventList){
        if (time>e.getTime()){
            throw std::invalid_argument("event list given in solution creation isn't timely ordered");
        }
        time = e.getTime();
    }
    std::cout << "checking" << std::endl;
    check();
    std::cout << "finished checking" << std::endl;
}

void Solution::check(){
    isValid = std::vector<int>(4,1);
    checkTruck();
    checkDrones();
    checkDemandSatisfaction();
    checkScenarsSpecifics();
}

// the 4 first checks checks for flow conservation. the last two check if the truck is time coherent and on roads.
int Solution::checkTruck(){

    Event previousEvent(instance.getGraph().getVertice(instance.getDepotId()).getPos(),0,1);
    int previousEventType = 1;
    for (int i=0; i<eventList.size();++i){
        const Event& actualEvent = eventList[i];
        int actualEventType = actualEvent.getEventType();
        if (actualEventType<=1){

            //truck arrive without moving
            if ((actualEventType == 1) && (previousEventType !=0)){
                isValid = std::vector<int>(4,-2);
                return 0;
            }

            //truck moving twice
            if ((actualEventType == 0) && (previousEventType == 0)){
                isValid = std::vector<int>(4,-3);
                return 0;
            }

            //truck teleporting (moving from place different from arrival)
            if ((actualEventType==0) && (previousEventType != -1) && ((actualEvent.getPos1()==previousEvent.getPos1())!=1)){
                std::cout << "teleporting from " << previousEvent.getPos1() << " to " << actualEvent.getPos1() << std::endl;
                std::cout << previousEvent << std::endl;
                std::cout << actualEvent << std::endl;
                isValid = std::vector<int>(4,-4);
                return 0;
            }

            //trucker lied (arrive to a different place from planned one)
            if ((actualEventType==1) && ((actualEvent.getPos1()==previousEvent.getPos2())!=1)){
                isValid = std::vector<int>(4,-5);
                return 0;
            }

            if (actualEventType == 1){
                int isEdgeExisting = 0;
                const Position& EventP1 = previousEvent.getPos1();
                const Position& EventP2 = previousEvent.getPos2();
                for (const Edge& edge : this->instance.getGraph().getEdges()){
                    const Position& EdgeP1 = this->instance.getGraph().getVertice(edge.getStartID()).getPos();
                    const Position& EdgeP2 = this->instance.getGraph().getVertice(edge.getEndID()).getPos();
                    if ( ((EventP1 == EdgeP1) && (EventP2 == EdgeP2)) || ((EventP1 == EdgeP2) && (EventP2 == EdgeP1))){
                        isEdgeExisting = 1;

                        //trucker warped space-time again (arriving before depart time + travel time)
                        if ( (actualEvent.getTime() - floor(previousEvent.getTime()*100)/100) < floor(this->instance.getTravelTime(edge)*100)/100)
                        {
                            std::cout << "time necessary " << floor(this->instance.getTravelTime(edge)*100)/100 << " when "
                            << (actualEvent.getTime() - floor(previousEvent.getTime()*100)/100) << " was taken"<< std::endl;
                            std::cout << actualEvent << std::endl;
                            std::cout << previousEvent << std::endl;
                            std::cout << edge << std::endl;
                            std::cout << this->instance.getGraph().getVertice(edge.getStartID()) << std::endl;
                            std::cout << this->instance.getGraph().getVertice(edge.getEndID()) << std::endl;
                            std::cout << instance.getGraph().getTSPKernelTime(edge.getEndID(),edge.getStartID()) << std::endl;
                            for (int karl=0; karl<instance.getGraph().getTSPKernelPath(edge.getEndID(),edge.getStartID()).size(); ++karl){
                                std::cout << instance.getGraph().getTSPKernelPath(edge.getEndID(),edge.getStartID())[karl] << " ";
                            }
                            std::cout << std::endl;
                            isValid = std::vector<int>(4,-6);
                            return 0;
                        }
                        // break;
                    }
                }

                //Truck should be driving on road
                if (isEdgeExisting==0){
                    isValid = std::vector<int>(4,-1);
                    return 0;
                }
            }
            

            previousEventType = actualEventType;
            previousEvent = actualEvent;
        }
    }

    //At the end you should arrive to depot
    if ((previousEventType!=1) || (1!=(previousEvent.getPos1()==instance.getDepotLocation()))){
        isValid = std::vector<int>(4,-69);
        return 0;
    }

    return 1;
}

int Solution::checkDrones(){
    std::vector<Event> previousEvent(2,Event());
    std::vector<int> previousEventType(2,3);
    for (int i=0; i<eventList.size();++i){
        Event actualEvent = eventList[i];
        int actualEventType = actualEvent.getEventType();
        if ( (actualEventType == 2) || (actualEventType == 3) || (actualEventType == 5) ){
            int droneID = actualEvent.getDroneID();

            //Drone lift off without being picked up
            if ((actualEventType == 2) && (previousEventType[droneID] != 3)){
                isValid = std::vector<int>(4,-7);
                return 0;
            }

            //Drone arrive without delivering
            if ((actualEventType == 3) && (previousEventType[droneID] != 5)){
                isValid = std::vector<int>(4,-8);
                return 0;
            }

            //Drone deliver without lifting off
            if ((actualEventType == 5) && (previousEventType[droneID] != 2)){
                isValid = std::vector<int>(4,-9);
                return 0;
            }

            //Administration error (the planned demand wasn't at the right position)
            if ((actualEventType == 5) && ((actualEvent.getPos1() == this->instance.getGraph().getDemand(actualEvent.getDemandID()).getNodePos())!=1)){
                isValid = std::vector<int>(4,-12);
                return 0;
            }

            //Drone stolled (delivered from place different from planned)
            Position pos2 = instance.getGraph().getDemand(previousEvent[droneID].getDemandID()).getNodePos();
            if ((actualEventType==5) && ((actualEvent.getPos1()==pos2)!=1)){
                std::cout << "incoherence, drone on demand " << previousEvent[droneID].getDemandID() << " should be going to " << pos2 << "---" << previousEvent[droneID].getPos2() << " gone to " << actualEvent.getPos1() << std::endl;
                std::cout << previousEvent[droneID] << std::endl;
                std::cout << actualEvent << std::endl;
                isValid = std::vector<int>(4,-10);
                return 0;
            }

            if (actualEventType!=2){
                //Drone warped space-time again (arriving before previous time + travel time)
                if (actualEvent.getTime() - previousEvent[droneID].getTime() < distance(previousEvent[droneID].getPos1(),actualEvent.getPos1())/this->instance.getDroneSpeed()){
                    std::cout << previousEvent[droneID].getTime() << std::endl;
                    std::cout << previousEvent[droneID] << std::endl;
                    std::cout << actualEvent << std::endl;
                    std::cout << actualEvent.getTime() << " " <<  
                    (previousEvent[droneID].getTime() + (distance(previousEvent[droneID].getPos1(),actualEvent.getPos1())/this->instance.getDroneSpeed()))
                    << std::endl;
                    isValid = std::vector<int>(4,-11);
                    return 0;
                }
            }

            previousEventType[droneID] = actualEventType;
            previousEvent[droneID] = actualEvent;
        }
    }
    return 1;
}

int Solution::checkDemandSatisfaction(){
    std::vector<int> deliveredDemandAmounts;
    const std::vector<Demand>& demands = instance.getGraph().getDemands();
    for (Demand d : demands){
        deliveredDemandAmounts.push_back(d.getAmount());
    }

    for (int i=0; i<eventList.size(); ++i){
        Event event = eventList[i];
        if (event.getEventType()>=4){
            if (event.getEventType()==4){
                Event arrivalTruckEvent(instance.getDepotLocation(),0,1);
                Event departureTruckEvent;
                for (int j=0; j<i; ++j){
                    Event newSuperPlusEvent = eventList[j];
                    if (newSuperPlusEvent.getEventType()==1){
                        arrivalTruckEvent = newSuperPlusEvent;
                    }
                }
                for (int j=i+1; (j<eventList.size()) && (departureTruckEvent.getEventType()==-1); ++j){
                    Event newSuperPlusEvent = eventList[j];
                    if (newSuperPlusEvent.getEventType()==0){
                        departureTruckEvent = newSuperPlusEvent;
                        break;
                    }
                }

                //truck don't go before demand is delivered, lazy fucker !
                if (departureTruckEvent.getTime() - floor(event.getTime()*1000)/1000 < instance.getTruckDeliveryTime() && departureTruckEvent.getEventType() != -1){
                    isValid = std::vector<int>(4,-27);
                    return 0;
                }

                //truck must be where he deliver
                if ((event.getPos1()==arrivalTruckEvent.getPos1())!=1){
                    isValid = std::vector<int>(4,-26);
                    return 0;
                }
            }

            // invalid event DemandID
            if (event.getDemandID()<0 || event.getDemandID() > instance.getGraph().getDemands().size()){
                isValid = std::vector<int>(4,-25);
                return 0;
            }

            deliveredDemandAmounts[event.getDemandID()] -= 1;
        }
    }

    for (int i = 0; i<deliveredDemandAmounts.size(); ++i){
        int amountDelivered = deliveredDemandAmounts[i];        
        //Delivered too much or not enought
        if (amountDelivered != 0){
            std::cout << "on demand " << i << " delivered " << (demands[i].getAmount()-amountDelivered) << " instead of " << demands[i].getAmount() << std::endl;
            isValid = std::vector<int>(4,-13);
            return 0;
        }
    }

    return 1;
}

void Solution::checkScenarsSpecifics(){
    // Drones need to not be used in case 0
    for (Event event : eventList){
        if ( (event.getEventType()==2) || (event.getEventType()==3) || (event.getEventType()==5)){
            isValid[0] = -14;
            break;
        }
    }

    for (int i=0; i<eventList.size(); ++i){
        Event event = eventList[i];
        int droneID = event.getDroneID();
        if (event.getEventType() == 2){
            Event arrivalDroneEvent;
            int arrivalDroneTime;
            for (int j=i+1; j<eventList.size(); ++j){
                if ((eventList[j].getEventType()==3) && (eventList[j].getDroneID()==droneID)){
                    arrivalDroneEvent = eventList[j];
                    arrivalDroneTime = j;
                    break;
                }
            }

            std::vector<int> isLastArrival(2,-1);
            std::vector<int> arrivalTime (2,-1);

            std::vector<Event> arrivalTruckEvent(2,Event(instance.getGraph().getVertice(0).getPos(),0,1));
            std::vector<Event> departureTruckEvent(2,Event(instance.getGraph().getVertice(0).getPos(),0,1,instance.getGraph().getVertice(0).getPos()));

            for (int j=i-1; j>0; --j){
                if (eventList[j].getEventType()==1){
                    // std::cout << "arrival 0 " << j << std::endl;
                    arrivalTruckEvent[0] = eventList[j];
                    arrivalTime[0] = j;
                    break;
                }
            }

            for (int j=i-1; j>=0; --j){
                if (eventList[j].getEventType()==0){
                    // std::cout << "departure 0 " << j << std::endl;
                    departureTruckEvent[0] = eventList[j];
                    isLastArrival[0] = (j<arrivalTime[0]);
                    break;
                }
            }

            for (int j=arrivalDroneTime-1; j>0; --j){
                if (eventList[j].getEventType()==1){
                    // std::cout << "arrival 1 " << j << std::endl;
                    arrivalTruckEvent[1] = eventList[j];
                    arrivalTime[1] = j;
                    break;
                }
            }

            for (int j=arrivalDroneTime-1; j>=0; --j){
                if (eventList[j].getEventType()==0){
                    // std::cout << "departure 1 " << j << std::endl;
                    departureTruckEvent[1] = eventList[j];
                    isLastArrival[1] = (j<arrivalTime[1]);
                    break;
                }
            }

            //Case 1 -> pick up and drop off must be at same point
            if ((event.getPos1() == arrivalDroneEvent.getPos1())!=1){
                isValid[1] = -15;
            }

            //Truck need to be here to lift off drones
            if (event.getTime()!=0){
                if ((event.getPos1() == arrivalTruckEvent[0].getPos1())!=1){
                    std::cout << arrivalTruckEvent[0] << std::endl;
                    std::cout << event << std::endl;
                    std::cout << departureTruckEvent[0] << std::endl;
                    isValid[1] = -16;
                    isValid[2] = -16;
                    if (isLastArrival[0]==1){
                        isValid[3] = -16;
                        std::cout << "lift off" << std::endl;
                    }
                }
            }

            //Truck need to be here to pick up drones
            if (arrivalDroneEvent.getTime()!=0){
                if ((arrivalDroneEvent.getPos1() == arrivalTruckEvent[1].getPos1())!=1){
                    std::cout << arrivalTruckEvent[0] << std::endl;
                    std::cout << arrivalDroneEvent << std::endl;
                    std::cout << departureTruckEvent[0] << std::endl;
                    isValid[1] = -17;
                    isValid[2] = -17;
                    if (isLastArrival[1]==1){
                        isValid[3] = -17;
                        std::cout << "pick up" << std::endl;
                    }
                }
            }

            //Truck need to be here to drop off drones (3rd scenario)
            if (isLastArrival[0]==0){
                // truck moving when drones are to be dropped
                isValid[1] = -18;
                isValid[2] = -18;

                double edgeTravelTime;
                Position EventP1 = departureTruckEvent[0].getPos1();
                Position EventP2 = departureTruckEvent[0].getPos2();
                for (Edge edge : this->instance.getGraph().getEdges()){
                    Position EdgeP1 = this->instance.getGraph().getVertice(edge.getStartID()).getPos();
                    Position EdgeP2 = this->instance.getGraph().getVertice(edge.getEndID()).getPos();
                    if ( ((EventP1 == EdgeP1) && (EventP2 == EdgeP2)) || ((EventP1 == EdgeP2) && (EventP2 == EdgeP1))){
                        edgeTravelTime = this->instance.getTravelTime(edge);
                        break;
                    }
                }

                double coef = (EventP1.getY()-EventP2.getY())/(EventP1.getX()-EventP2.getX());
                double distanceToEdge = event.getPos1().getX()*coef - event.getPos1().getY();

                // drop off not on the edge the truck is on
                if (distanceToEdge>=0.1 || distanceToEdge<=-0.1){
                    isValid[3] = -19;
                }

                double ratio = (event.getPos1().getX()-EventP2.getX())/(EventP1.getX()-EventP2.getX());

                double arrivalTruckTime = departureTruckEvent[0].getTime() + ratio*edgeTravelTime;

                // drop off timing not coinciding with truck speed
                if ( ((arrivalTruckTime-event.getTime()) < -0.1) || ((arrivalTruckTime-event.getTime()) > 0.1)){
                    isValid[3] = -20;
                }
            }

            //Truck need to be here to pick up drones (3rd scenario)
            if (isLastArrival[1]==0){
                // truck moving when drones are to be picked up
                isValid[1] = -18;
                isValid[2] = -18;

                double edgeTravelTime;
                Position EventP1 = departureTruckEvent[1].getPos1();
                Position EventP2 = departureTruckEvent[1].getPos2();
                for (Edge edge : this->instance.getGraph().getEdges()){
                    Position EdgeP1 = this->instance.getGraph().getVertice(edge.getStartID()).getPos();
                    Position EdgeP2 = this->instance.getGraph().getVertice(edge.getEndID()).getPos();
                    if ( ((EventP1 == EdgeP1) && (EventP2 == EdgeP2)) || ((EventP1 == EdgeP2) && (EventP2 == EdgeP1))){
                        edgeTravelTime = this->instance.getTravelTime(edge);
                        break;
                    }
                }

                double coef = (EventP1.getY()-EventP2.getY())/(EventP1.getX()-EventP2.getX());
                double distanceToEdge = arrivalDroneEvent.getPos1().getX()*coef - arrivalDroneEvent.getPos1().getY();

                // drop off not on the edge the truck is on
                if (distanceToEdge>=0.1 || distanceToEdge<=-0.1){
                    isValid[3] = -19;
                }

                double ratio = (arrivalDroneEvent.getPos1().getX()-EventP2.getX())/(EventP1.getX()-EventP2.getX());

                double arrivalTruckTime = departureTruckEvent[0].getTime() + ratio*edgeTravelTime;

                // drop off timing not coinciding with truck speed
                if ( ((arrivalTruckTime-arrivalDroneEvent.getTime()) < -0.1) || ((arrivalTruckTime-arrivalDroneEvent.getTime()) > 0.1)){
                    isValid[3] = -20;
                }
            }
        }
    }
}

const std::vector<Event>& Solution::getEvents() const
{
    return eventList;
}

const std::vector<Vertex>& Solution::getVertices() const
{
    return instance.getGraph().getVertices();
}

const std::vector<Edge>& Solution::getEdges() const
{
    return instance.getGraph().getEdges();
}

int Solution::getIsValid(unsigned int scenar) const {
    return isValid[scenar];
}

std::vector<int> Solution::getIsValid() const{
    return isValid;
}

const std::vector<Demand>& Solution::getDemands() const
{
    return instance.getGraph().getDemands();
}