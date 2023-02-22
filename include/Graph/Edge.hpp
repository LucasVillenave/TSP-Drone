#ifndef EDGE
#define EDGE

#include "Graph/Vertex.hpp"

class Edge{
    private :
        static const std::vector<std::string> ROAD_TYPE;
        static const std::vector<double> ROAD_SPEED; //m/s

        int startID = -1;
        int endID = -1;
        double length = -1;
        std::string roadType = "unclassified";
        double cost = -1; //s
        int ID = -1;
        int GraphID = -1;

    public :
        Edge()=default;
        Edge(int t_startID, int t_endID, double t_lenght, std::string t_roadType, int t_ID=-1);
        Edge(int t_startID, int t_endID, double t_cost, int t_ID=-1);

        int getStartID() const;
        int getEndID() const;

        double getLength() const;
        double getCost() const;

        int getID() const;
        int getGraphID() const;
        void setID(int t_ID);
        void setGraphID(int t_GraphID);

        std::string getRoadType() const;
};

inline std::ostream &operator<<(std::ostream &os, const Edge& e)
{
    os << "edge " << e.getGraphID() << "(" << e.getID() << ") between " << e.getStartID()  << " and " << e.getEndID() << ", time = " << e.getCost();
    return os;
}

inline bool operator==(const Edge& e1, const Edge& e2)
{
    return (
        e1.getStartID() == e2.getStartID() &&
        e1.getEndID() == e2.getEndID() &&
        e1.getRoadType() == e2.getRoadType() &&
        e1.getID() == e2.getID()
        );
}

#endif