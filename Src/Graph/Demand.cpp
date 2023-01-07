#include "Graph/Demand.hpp"
#include "Vertex.hpp"

Demand::Demand(){}

Demand::Demand(Position pos, int amount, Vertex v=Vertex(), int ID=-1){
    this->pos = pos;
    this->amount = amount;
    this->v = v;
    this->ID = ID;
}

Vertex Demand::getVertex(){
    return v;
}

void Demand::setVertex(Vertex v){
    this->v = v;
}

int Demand::getAmount(){
    return amount;
}

void Demand::setAmount(int amount){
    this->amount = amount;
}

int Demand::getLatitude(){
    return pos.getLatitude();
}

int Demand::getLongitude(){
    return pos.getLongitude();
}

void Demand::setLongitude(int longitude){
    pos.setLongitude(longitude);
}

void Demand::setLatitude(int latitude){
    pos.setLatitude(latitude);
}

Position Demand::getPos(){
    return pos;
}

void Demand::setPos(Position pos){
    this->pos = pos;
}

int Demand::getID(){
    return ID;
}

int Demand::getGraphID(){
    return GraphID;
}

void Demand::setID(int ID){
    this->ID = ID;
}

void Demand::setGraphID(int GraphID){
    this->GraphID = GraphID;
}