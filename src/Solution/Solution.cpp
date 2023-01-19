#include "Solution/Solution.hpp"

Solution::Solution(Instance t_instance, const std::vector<Event>& t_eventList)
    : instance(std::move(t_instance)), eventList(std::move(t_eventList))
{
    int time = 0;
    for (Event e : t_eventList){
        if (time>e.getTime()){
            throw std::invalid_argument("event list given in solution creation isn't timely ordered");
        }
        time = e.getTime();
    }
}

void Solution::check(){
    isValid = std::vector<int>(4,1);
    checkTruck();
    checkDronesTimes();
    checkDemandSatisfaction();
    checkScenarsSpecifics();
}

// the 4 first checks checks for flow conservation. the last two check if the truck is time coherent and on roads.
int Solution::checkTruck(){

    Event previousEvent;
    int previousEventType = -1;
    for (int i=0; i<eventList.size();++i){
        Event actualEvent = eventList[i];
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
            if ((actualEventType==0) && ((actualEvent.getPos1()==previousEvent.getPos2())!=1)){
                isValid = std::vector<int>(4,-4);
                return 0;
            }

            //trucker lied (arrive to a different place from planned one)
            if ((actualEventType==1) && ((actualEvent.getPos1()==previousEvent.getPos2())!=1)){
                isValid = std::vector<int>(4,-5);
                return 0;
            }

            if (actualEventType==0){
                int isEdgeExisting = 0;
                Position EventP1 = actualEvent.getPos1();
                Position EventP2 = actualEvent.getPos2();
                for (Edge edge : this->instance.getGraph().getEdges()){
                    Position EdgeP1 = this->instance.getGraph().getVertice(edge.getStartID()).getPos();
                    Position EdgeP2 = this->instance.getGraph().getVertice(edge.getEndID()).getPos();
                    if ( ((EventP1 == EdgeP1) && (EventP2 == EdgeP2)) || ((EventP1 == EdgeP2) && (EventP2 == EdgeP1))){
                        isEdgeExisting = 1;

                        //trucker warped space-time again (arriving before depart time + travel time)
                        if (actualEvent.getTime() < (previousEvent.getTime() + this->instance.getTravelTime(edge))){
                            isValid = std::vector<int>(4,-6);
                            return 0;
                        }
                        break;
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
    return 1;
}

int Solution::checkDronesTimes(){
    //ToDo
    return 1;
}

int Solution::checkDemandSatisfaction(){
    //ToDo
    return 1;
}

void Solution::checkScenarsSpecifics(){
    //ToDo
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