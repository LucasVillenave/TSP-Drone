#include "Event.hpp"

Event::Event(){}
Event::Event(Position pos1, int time, int eventType, Position pos2, int demandID, int droneID){
    if (time<0 || eventType<0 || eventType>5){
        std::invalid_argument("wrong argument in event creation");
    }
    if ( ((eventType == 2) || (eventType == 3) || (eventType == 5)) && (droneID<0 || droneID>1) ){
        std::invalid_argument("event related to drones should have valid droneID");
    }
    if ( (eventType>3) && (demandID<0) ){
        std::invalid_argument("event related to demands should have valid demandID");
    }
    
    this->pos1 = pos1;
    this->pos2 = pos2;
    this->time = time;
    this->eventType = eventType;
    this->demandID = demandID;
    this->droneID = droneID;
}

int Event::getTime(){
    return time;
}

int Event::getDemandID(){
    return this->demandID;
}

int Event::getEventType(){
    return eventType;
}

int Event::getDroneID(){
    return droneID;
}

Position Event::getPos1(){
    return pos1;
}

Position Event::getPos2(){
    return pos2;
}