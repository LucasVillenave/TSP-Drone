#include "Graph/Edge.hpp"

Edge::Edge(){}

Edge::Edge(int startID, int endID, int length, std::string roadType, int ID){
    this->startID = startID;
    this->endID = endID;
    this->roadType = roadType;
    this->ID = ID;
    this->length = length;
}

int Edge::getStartID(){
    return startID;
}

int Edge::getEndID(){
    return endID;
}

double Edge::getLength(){
    return length;
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

std::string Edge::getRoadType(){
    return roadType;
}

void Edge::setRoadType(std::string roadType){
    this->roadType = roadType;
}