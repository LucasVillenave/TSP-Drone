#ifndef EDGE
#define EDGE

#include "Graph/Vertex.hpp"

class Edge{
    private :
        Vertex start;
        Vertex end;
        int roadSpeed = -1;
        int ID;
        int GraphID = -1;

    public :
        Edge();
        Edge(Vertex start, Vertex end, int roadSpeed, int ID=-1);

        Vertex getStart();
        Vertex getEnd();
        void setEnd(Vertex end);
        void setStart(Vertex start);

        int getID();
        int getGraphID();
        void setID(int ID);
        void setGraphID(int GraphID);

        int getRoadSpeed();
        void setRoadSpeed(int roadSpeed);
};

inline std::ostream &operator<<(std::ostream &os, Edge e)
{
    os << "edge " << e.getGraphID() << "(" << e.getID() << ") between :" << std::endl << "--- " << e.getStart() << std::endl << "--- " << e.getEnd();
    return os;
}

inline bool operator==(Edge e1, Edge e2)
{
    return (
        e1.getStart() == e2.getStart() &&
        e1.getEnd() == e2.getEnd() &&
        e1.getRoadSpeed() == e2.getRoadSpeed() &&
        e1.getID() == e2.getID()
        );
}

#endif