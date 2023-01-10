#ifndef EDGE
#define EDGE

#include "Graph/Vertex.hpp"

class Edge{
    private :
        int startID;
        int endID;
        int roadSpeed = -1;
        int ID;
        int GraphID = -1;

    public :
        Edge();
        Edge(int startID, int endID, int roadSpeed, int ID=-1);

        int getStartID();
        int getEndID();

        int getID();
        int getGraphID();
        void setID(int ID);
        void setGraphID(int GraphID);

        int getRoadSpeed();
        void setRoadSpeed(int roadSpeed);
};

inline std::ostream &operator<<(std::ostream &os, Edge e)
{
    os << "edge " << e.getGraphID() << "(" << e.getID() << ") between " << e.getStartID()  << " and " << e.getEndID();
    return os;
}

inline bool operator==(Edge e1, Edge e2)
{
    return (
        e1.getStartID() == e2.getStartID() &&
        e1.getEndID() == e2.getEndID() &&
        e1.getRoadSpeed() == e2.getRoadSpeed() &&
        e1.getID() == e2.getID()
        );
}

#endif