#ifndef POSITION
#define POSITION

#include "ostream"
#include <iomanip>

class Position{
    private :
        const double m_longitude = -1;
        const double m_latitude = -1;
        double m_x = -1;
        double m_y = -1;

    public :
        Position()=default;
        Position(double t_latitude, double t_longitude);

        double getLongitude() const;
        double getLatitude() const;
        double getX() const;
        double getY() const;

        void setX(double t_x);
        void setY(double t_y);
};

inline std::ostream &operator<<(std::ostream &os, Position p)
{
    os << std::setprecision(10) << "(LAT : " << p.getLatitude() << "(" << p.getX() << ") ; LON : " 
    << p.getLongitude() << "(" << p.getY() << ")" << ")";
    return os;
}

inline bool operator==(Position p1, Position p2)
{
    return (
        p1.getLatitude() == p2.getLatitude() &&
        p1.getLongitude() == p2.getLongitude());
}

#endif