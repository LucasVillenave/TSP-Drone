#include "Graph/Edge.hpp"

#include <utility>

Edge::Edge(int t_startID, int t_endID, int t_lenght, std::string t_roadType, int t_ID)
    : startID(t_startID), endID(t_endID), length(t_lenght), roadType(std::move(t_roadType)), ID(t_ID) {}

int Edge::getStartID() const{
    return startID;
}

int Edge::getEndID() const{
    return endID;
}

double Edge::getLength() const{
    return length;
}

int Edge::getID() const{
    return ID;
}

int Edge::getGraphID() const{
    return GraphID;
}

void Edge::setID(int t_ID){
    ID = t_ID;
}

void Edge::setGraphID(int t_GraphID){
    GraphID = t_GraphID;
}

std::string Edge::getRoadType() const{
    return roadType;
}

void Edge::setRoadType(std::string t_roadType){
    roadType = std::move(t_roadType);
}