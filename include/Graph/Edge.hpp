#ifndef EDGE
#define EDGE

#include "Graph/Vertex.hpp"

class Edge{
    private :
        int startID;
        int endID;
        double length;
        std::string roadType;
        int TruckTime;
        int ID;
        int GraphID = -1;

    public :
        Edge();
        Edge(int startID, int endID, int lenght, std::string roadType, int ID=-1);

        int getStartID();
        int getEndID();

        double getLength();

        int getID();
        int getGraphID();
        void setID(int ID);
        void setGraphID(int GraphID);

        std::string getRoadType();
        void setRoadType(std::string roadType);
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
        e1.getRoadType() == e2.getRoadType() &&
        e1.getID() == e2.getID()
        );
}

#endif