#include "Graph/Demand.hpp"

Demand::Demand(){}

Demand::Demand(Position pos, int amount,int ID){
    this->pos = pos;
    this->amount = amount;
    this->ID = ID;
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