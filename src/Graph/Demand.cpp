#include "Graph/Demand.hpp"

Demand::Demand(Position t_intiPos, int amount, int ID)
    : initPos(t_intiPos), amount(amount), ID(ID) {}

int Demand::getAmount() const{
    return amount;
}

void Demand::setAmount(int t_amount){
    amount = t_amount;
}

double Demand::getLongitude() const{
    return initPos.getLongitude();
}

double Demand::getLatitude() const{
    return initPos.getLatitude();
}

double Demand::getX() const{
    return initPos.getX();
}

double Demand::getY() const{
    return initPos.getY();
}

void Demand::setX(double t_x){
    initPos.setX(t_x);
}

void Demand::setY(double t_y){
    initPos.setY(t_y);
}

const Position& Demand::getPos() const{
    return initPos;
}

void Demand::setPos(Position t_pos){
    initPos.setX(t_pos.getX());
    initPos.setY(t_pos.getY());
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

void Demand::setNodePos(Position nodePos){
    this->nodePos = nodePos;
}

void Demand::setNodeGraphID(int nodeGraphID){
    this->nodeGraphID = nodeGraphID;
}

Position Demand::getInitPos() const{
    return initPos;
}

Position Demand::getNodePos() const{
    return nodePos;
}

int Demand::getNodeGraphID() const{
    return nodeGraphID;
}