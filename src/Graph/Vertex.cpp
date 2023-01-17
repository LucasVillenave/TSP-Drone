#include "Graph/Vertex.hpp"

Vertex::Vertex(Position t_pos, const std::vector<Demand>& t_demands, int t_ID)
    : pos(t_pos), demands(t_demands), ID(t_ID)
{
    for (Demand d : t_demands){
        TDA += d.getAmount();
    }
}

double Vertex::getLatitude() const{
    return pos.getLatitude();
}

double Vertex::getLongitude() const{
    return pos.getLongitude();
}

void Vertex::setLongitude(double longitude){
    pos.setX(longitude);
}

void Vertex::setLatitude(double latitude){
    pos.setY(latitude);
}

Position Vertex::getPos() const{
    return pos;
}

void Vertex::setPos(Position t_pos){
    pos.setX(t_pos.getX());
    pos.setY(t_pos.getY());
}

const std::vector<Demand>& Vertex::getDemands() const{
    return demands;
}

Demand Vertex::getDemand(int t_ID) const{
    for (Demand d : demands){
        if (d.getID() == t_ID){
            return d;
        }
    }
    return {};
}

int Vertex::getTDA() const{
    return TDA;
}

void Vertex::addDemand(Demand d){
    TDA += d.getAmount();
    demands.push_back(d);
}

int Vertex::getID() const{
    return ID;
}

int Vertex::getGraphID() const{
    return GraphID;
}

void Vertex::setID(int t_ID){
    ID = t_ID;
}

void Vertex::setGraphID(int t_GraphID){
    GraphID = t_GraphID;
}