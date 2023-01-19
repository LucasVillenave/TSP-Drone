#ifndef DEMAND
#define DEMAND

#include "Graph/Position.hpp"

class Demand{
    private :
        Position initPos;
        Position nodePos;
        int nodeGraphID = -1;

        int amount = -1;
        int ID;
        int GraphID = -1;

    public :
        Demand()=default;
        Demand(Position t_initPos, int amount, int ID=-1);

        void setNodePos(Position nodePos);
        void setNodeGraphID(int nodeGraphID);
        Position getInitPos() const;
        Position getNodePos() const;
        int getNodeGraphID() const;

        int getAmount() const;
        void setAmount(int t_amount);

        double getLongitude() const;
        double getLatitude() const;
        double getX() const;
        double getY() const;
        void setX(double t_x);
        void setY(double t_y);

        const Position& getPos() const;
        void setPos(Position t_pos);

        int getID() const;
        int getGraphID() const;
        void setID(int t_ID);
        void setGraphID(int t_GraphID);
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