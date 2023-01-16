#include "Graph/Edge.hpp"

Edge::Edge(){}

Edge::Edge(int startID, int endID, int roadSpeed, int ID){
    this->startID = startID;
    this->endID = endID;
    this->roadSpeed = roadSpeed;
    this->ID = ID;
}

int Edge::getStartID() const{
    return startID;
}

int Edge::getEndID() const{
    return endID;
}

int Edge::getID(){
    return ID;
}

int Edge::getGraphID(){
    return GraphID;
}

void Edge::setID(int ID){
    this->ID = ID;
}

void Edge::setGraphID(int GraphID){
    this->GraphID = GraphID;
}

int Edge::getRoadSpeed(){
    return roadSpeed;
}

void Edge::setRoadSpeed(int roadSpeed){
    this->roadSpeed = roadSpeed;
}