#include "Graph/Position.hpp"

Position::Position(){}
Position::Position(int longitude, int latitude){
    this->longitude = longitude;
    this->latitude = latitude;
}

int Position::getLatitude(){
    return latitude;
}

int Position::getLongitude(){
    return longitude;
}

void Position::setLongitude(int longitude){
    this->longitude = longitude;
}

void Position::setLatitude(int latitude){
    this->latitude = latitude;
}