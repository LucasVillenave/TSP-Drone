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
        Edge()=default;
        Edge(int t_startID, int t_endID, int t_lenght, std::string t_roadType, int t_ID=-1);

        int getStartID() const;
        int getEndID() const;

        double getLength() const;

        int getID() const;
        int getGraphID() const;
        void setID(int t_ID);
        void setGraphID(int t_GraphID);

        std::string getRoadType() const;
        void setRoadType(std::string t_roadType);
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