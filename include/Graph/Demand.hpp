#ifndef DEMAND
#define DEMAND

#include "Graph/Position.hpp"

class Demand{
    private :
        Position pos;
        int amount = -1;
        int ID;
        int GraphID = -1;

    public :
        Demand();
        Demand(Position pos, int amount,int ID=-1);

        int getAmount();
        void setAmount(int amount);

        int getLatitude();
        int getLongitude();
        void setLongitude(int longitude);
        void setLatitude(int latitude);

        Position getPos();
        void setPos(Position pos);

        int getID();
        int getGraphID();
        void setID(int ID);
        void setGraphID(int GraphID);
};

inline std::ostream &operator<<(std::ostream &os, Demand d)
{
    os << "demand " << d.getGraphID() << "(" << d.getID() << ") of pos : " << d.getPos() << " and amount " << d.getAmount();
    return os;
}

inline bool operator==(Demand d1, Demand d2)
{
    return (
        d1.getPos() == d2.getPos() &&
        d1.getAmount() == d2.getAmount() && 
        d1.getID() == d2.getID()
    );
}

#endif