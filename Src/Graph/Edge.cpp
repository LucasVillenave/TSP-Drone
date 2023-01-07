#include "Graph/Edge.hpp"

Edge::Edge(){}

Edge::Edge(Vertex start, Vertex end, int roadSpeed, int ID=-1){
    this->start = start;
    this->end = end;
    this->roadSpeed = roadSpeed;
    this->ID = ID;
}

Vertex Edge::getStart(){
    return start;
}

Vertex Edge::getEnd(){
    return end;
}

void Edge::setEnd(Vertex end){
    this->end = end;
}

void Edge::setStart(Vertex start){
    this->start = start;
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