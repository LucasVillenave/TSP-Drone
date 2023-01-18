#include "Graph/Position.hpp"

Position::Position(double t_latitude, double t_longitude)
    : m_latitude(t_latitude), m_longitude(t_longitude) {}

double Position::getLatitude() const{
    return m_latitude;
}

double Position::getLongitude() const{
    return m_longitude;
}

double Position::getX() const
{
    return m_x;
}

double Position::getY() const
{
    return m_y;
}

void Position::setX(double t_x){
    m_x = t_x;
}

void Position::setY(double t_y){
    m_y = t_y;
}