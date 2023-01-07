#ifndef DEMAND
#define DEMAND

#include "Graph/Position.hpp"

class Vertex;

class Demand{
    private :
        Position pos;
        Vertex v;
        int amount = -1;
        int ID;
        int GraphID = -1;

    public :
        Demand(){}
        Demand(Position pos, int amount, Vertex v=Vertex(), int ID=-1){
            this->pos = pos;
            this->amount = amount;
            this->v = v;
            this->ID = ID;
        }

        Vertex getVertex(){
            return v;
        }

        void setVertex(Vertex v){
            this->v = v;
        }

        int getAmount(){
            return amount;
        }

        void setAmount(int amount){
            this->amount = amount;
        }

        int getLatitude(){
            return pos.getLatitude();
        }

        int getLongitude(){
            return pos.getLongitude();
        }

        void setLongitude(int longitude){
            pos.setLongitude(longitude);
        }

        void setLatitude(int latitude){
            pos.setLatitude(latitude);
        }

        Position getPos(){
            return pos;
        }

        void setPos(Position pos){
            this->pos = pos;
        }

        int getID(){
            return ID;
        }

        int getGraphID(){
            return GraphID;
        }

        void setID(int ID){
            this->ID = ID;
        }

        void setGraphID(int GraphID){
            this->GraphID = GraphID;
        }
};

inline std::ostream &operator<<(std::ostream &os, Demand d)
{
    os << "demand " << d.getGraphID() << "(" << d.getID() << ") of pos :" << d.getPos() << " and amount " << d.getAmount();
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