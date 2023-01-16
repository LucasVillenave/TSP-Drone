#include "Graph/Vertex.hpp"

Vertex::Vertex(){}

Vertex::Vertex(Position pos, std::vector<Demand> demands, int ID){
    this->pos = pos;
    this->demands = demands;
    for (Demand d : demands){
        TDA += d.getAmount();
    }
    this->ID = ID;
}

double Vertex::getLatitude() const{
    return pos.getLatitude();
}

double Vertex::getLongitude() const{
    return pos.getLongitude();
}

void Vertex::setLongitude(int longitude){
    pos.setLongitude(longitude);
}

void Vertex::setLatitude(int latitude){
    pos.setLatitude(latitude);
}

Position Vertex::getPos() const{
    return pos;
}

void Vertex::setPos(Position pos){
    this->pos = pos;
}

std::vector<Demand> Vertex::getDemands(){
    return demands;
}

Demand Vertex::getDemand(int ID){
    for (Demand d : demands){
        if (d.getID()==ID){
            return d;
        }
    }
    return Demand();
}

int Vertex::getTDA(){
    return TDA;
}

void Vertex::addDemand(Demand d){
    this->TDA += d.getAmount();
    demands.push_back(d);
}

int Vertex::getID(){
    return ID;
}

int Vertex::getGraphID() const{
    return GraphID;
}

void Vertex::setID(int ID){
    this->ID = ID;
}

void Vertex::setGraphID(int GraphID){
    this->GraphID = GraphID;
}