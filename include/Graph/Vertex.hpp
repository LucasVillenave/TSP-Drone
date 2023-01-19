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
        Vertex()=default;
        Vertex(Position t_pos, const std::vector<Demand>& t_demands=std::vector<Demand>(), int t_ID=-1);

        double getLatitude() const;
        double getLongitude() const;
        void setLongitude(double longitude);
        void setLatitude(double latitude);

        Position getPos() const;
        void setPos(Position t_pos);

        const std::vector<Demand>& getDemands() const;
        Demand getDemand(int t_ID) const;
        void addDemand(Demand d);

        int getTDA() const;

        int getID() const;
        int getGraphID() const;
        void setID(int t_ID);
        void setGraphID(int t_GraphID);

};

inline std::ostream &operator<<(std::ostream &os, const Vertex& v)
{
    os << "vertex " << v.getGraphID() << "(" << v.getID() << ") of pos : " << v.getPos() << " and TDA : " << v.getTDA();
    return os;
}

inline bool operator==(const Vertex& v1, const Vertex& v2)
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