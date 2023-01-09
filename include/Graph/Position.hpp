#ifndef POSITION
#define POSITION

#include "ostream"

class Position{
    private :
        double longitude = -1;
        double latitude = -1;

    public :
        Position();
        Position(double latitude, double longitude);

        double getLatitude();
        double getLongitude();

        void setLongitude(double longitude);
        void setLatitude(double latitude);
};

inline std::ostream &operator<<(std::ostream &os, Position p)
{
    os << "(LAT : " << p.getLatitude() << " ;LON : " << p.getLongitude() << ")";
    return os;
}

inline bool operator==(Position p1, Position p2)
{
    return (
        p1.getLatitude() == p2.getLatitude() &&
        p1.getLongitude() == p2.getLongitude());
}

#endif