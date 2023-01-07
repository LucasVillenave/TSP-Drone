#ifndef POSITION
#define POSITION

#include "ostream"

class Position{
    private :
        int longitude = -1;
        int latitude = -1;

    public :
        Position();
        Position(int latitude, int longitude);

        int getLatitude();
        int getLongitude();

        void setLongitude(int longitude);
        void setLatitude(int latitude);
};

inline std::ostream &operator<<(std::ostream &os, Position p)
{
    os << "(" << p.getLatitude() << ";" << p.getLongitude() << ")";
    return os;
}

inline bool operator==(Position p1, Position p2)
{
    return (
        p1.getLatitude() == p2.getLatitude() &&
        p1.getLongitude() == p2.getLongitude());
}

#endif