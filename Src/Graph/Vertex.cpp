#include "Vertex.hpp"

Vertex::Vertex(){}

Vertex::Vertex(Position pos, std::vector<Demand> demands=std::vector<Demand>(), int ID=-1){
    this->pos = pos;
    this->demands = demands;
    for (Demand d : demands){
        TDA += d.getAmount();
    }
    this->ID = ID;
}

int Vertex::getLatitude(){
    return pos.getLatitude();
}

int Vertex::getLongitude(){
    return pos.getLongitude();
}

void Vertex::setLongitude(int longitude){
    pos.setLongitude(longitude);
}

void Vertex::setLatitude(int latitude){
    pos.setLatitude(latitude);
}

Position Vertex::getPos(){
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
}

Demand Vertex::getDemand(int GraphID){
    for (Demand d : demands){
        if (d.getGraphID()==GraphID){
            return d;
        }
    }
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

int Vertex::getGraphID(){
    return GraphID;
}

void Vertex::setID(int ID){
    this->ID = ID;
}

void Vertex::setGraphID(int GraphID){
    this->GraphID = GraphID;
}