#include "Graph/Position.hpp"

Position::Position(){}
Position::Position(double latitude, double longitude){
    this->longitude = longitude;
    this->latitude = latitude;
}

double Position::getLatitude() const{
    return latitude;
}

double Position::getLongitude() const{
    return longitude;
}

void Position::setLongitude(double longitude){
    this->longitude = longitude;
}

void Position::setLatitude(double latitude){
    this->latitude = latitude;
}