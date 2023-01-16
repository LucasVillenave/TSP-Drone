#ifndef VERTEX
#define VERTEX

#include <vector>
#include "Graph/Demand.hpp"

class Vertex{
    private :
        Position pos;
        std::vector<Demand> demands;
        int ID = -1;
        int GraphID = -1;
        int TDA = 0; //total demand amount

    public :
        Vertex();
        Vertex(Position pos, std::vector<Demand> demands=std::vector<Demand>(), int ID=-1);

        double getLatitude() const;
        double getLongitude() const;
        void setLongitude(int longitude);
        void setLatitude(int latitude);

        Position getPos() const;
        void setPos(Position pos);

        std::vector<Demand> getDemands();
        Demand getDemand(int ID);
        void addDemand(Demand d);

        int getTDA();

        int getID();
        int getGraphID() const;
        void setID(int ID);
        void setGraphID(int GraphID);

};

inline std::ostream &operator<<(std::ostream &os, Vertex v)
{
    os << "vertex " << v.getGraphID() << "(" << v.getID() << ") of pos : " << v.getPos() << " and TDA : " << v.getTDA();
    return os;
}

inline bool operator==(Vertex v1, Vertex v2)
{
    int sameDemands = 0;
    if (v1.getDemands().size()==v2.getDemands().size()){
        sameDemands = 1;
        for (Demand d : v1.getDemands()){
            sameDemands = 0;
            for (Demand d2 : v2.getDemands()){
                if (d==d2){
                    sameDemands = 1;
                }
            }
            if (sameDemands==0){
                break;
            }
        }
    }

    return (
        v1.getPos() == v2.getPos() &&
        v1.getID() == v2.getID() && 
        sameDemands);
}

#endif