#include "Graph/Edge.hpp"

#include <utility>

const std::vector<std::string> Edge::ROAD_TYPE = {"primary", "secondary", "tertiary"};
const std::vector<double> Edge::ROAD_SPEED = {60/3.6, 45/3.6, 30/3.6};

Edge::Edge(int t_startID, int t_endID, double t_lenght, std::string t_roadType, int t_ID)
    : startID(t_startID), endID(t_endID), length(t_lenght), roadType(std::move(t_roadType)), ID(t_ID)
{
    if(roadType == ROAD_TYPE[0])
    {
        cost = length/ROAD_SPEED[0];
    }
    else if(roadType == ROAD_TYPE[1])
    {
        cost = length/ROAD_SPEED[1];
    }
    else
    {
        cost = length/ROAD_SPEED[2];
    }
}

Edge::Edge(int t_startID, int t_endID, double t_cost, int t_ID)
        : startID(t_startID), endID(t_endID), cost(t_cost), ID(t_ID) {}

int Edge::getStartID() const{
    return startID;
}

int Edge::getEndID() const{
    return endID;
}

double Edge::getLength() const{
    return length;
}

double Edge::getCost() const
{
    return cost;
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
