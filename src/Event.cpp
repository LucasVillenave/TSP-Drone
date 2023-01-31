#include "Event.hpp"
#include <iostream>

Event::Event(Position t_pos1, int t_time, int t_eventType, Position t_pos2, int t_demandID, int t_droneID)
    : pos1(t_pos1), pos2(t_pos2), time(t_time), eventType(t_eventType), demandID(t_demandID), droneID(t_droneID)
{
    if (t_time < 0 || t_eventType < 0 || t_eventType > 5){
        throw std::invalid_argument("wrong argument in event creation");
    }
    if ( (((t_eventType == 2) || (t_eventType == 3) || (t_eventType == 5)) && (t_droneID < 0 || t_droneID > 1)) ){
        throw std::invalid_argument("event related to drones should have valid t_droneID");
    }
    if ((t_eventType > 3) && (t_demandID < 0) ){
        throw std::invalid_argument("event related to m_demands should have valid t_demandID");
    }

}

int Event::getTime() const{
    return time;
}

int Event::getDemandID() const{
    return demandID;
}

int Event::getEventType() const{
    return eventType;
}

int Event::getDroneID() const{
    return droneID;
}

Position Event::getPos1() const{
    return pos1;
}

Position Event::getPos2() const{
    return pos2;
}