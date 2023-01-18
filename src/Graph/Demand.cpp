#include "Graph/Demand.hpp"

Demand::Demand(Position pos, int amount, int ID)
    : pos(pos), amount(amount), ID(ID) {}

int Demand::getAmount() const{
    return amount;
}

void Demand::setAmount(int t_amount){
    amount = t_amount;
}

double Demand::getLongitude() const{
    return pos.getLongitude();
}

double Demand::getLatitude() const{
    return pos.getLatitude();
}

double Demand::getX() const{
    return pos.getX();
}

double Demand::getY() const{
    return pos.getY();
}

void Demand::setX(double t_x){
    pos.setX(t_x);
}

void Demand::setY(double t_y){
    pos.setY(t_y);
}

const Position& Demand::getPos() const{
    return pos;
}

void Demand::setPos(Position t_pos){
    pos.setX(t_pos.getX());
    pos.setY(t_pos.getY());
}

int Demand::getID() const{
    return ID;
}

int Demand::getGraphID() const{
    return GraphID;
}

void Demand::setID(int t_ID){
    ID = t_ID;
}

void Demand::setGraphID(int t_GraphID){
    GraphID = t_GraphID;
}