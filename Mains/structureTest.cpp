#include <iostream>
#include "Solution/Solution.hpp"
#include "gurobi_c++.h"

int main(){
    std::cout << "Hello World" << std::endl;

    GRBEnv env(true);

    Position p1(1,2);
    Position p2(10,20);
    Position p3(50,100);
    Position p4(5,5);

    Position p5(11,25);
    Position p6(15,35);

    Demand d1(p4,5);
    Demand d2(p5,1);
    Demand d3(p6,1);

    Vertex v1(p1 ,std::vector<Demand>(), 12);
    Vertex v2(p2 ,std::vector<Demand>(), 18);
    Vertex v3(p3 ,std::vector<Demand>(), 21);
    Vertex v4(p4 ,std::vector<Demand>(), 2);

    Edge e1(0,1,50,6);
    Edge e2(1,3,20,3);

    std::vector<Vertex> vertices;
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    std::vector<Edge> edges;
    edges.push_back(e1);
    edges.push_back(e2);

    std::vector<Demand> demands;
    demands.push_back(d1);
    demands.push_back(d2);
    demands.push_back(d3);

    Graph g(vertices,edges);
    g.addDemands(demands);

    std::cout << g << std::endl;
    for (Vertex v : g.getVertices()){
        std::cout << v << std::endl;
    }

    Instance i(g,"test");

    std::vector<Event> events;
    events.push_back(Event(p6,3,0));
    events.push_back(Event(p5,5,1));
    events.push_back(Event(p4,10,2,Position(),-1,1));
    events.push_back(Event(p4,10,3,Position(),-1,0));
    events.push_back(Event(p1,15,4,Position(),15,-1));
    events.push_back(Event(p2,155,5,Position(),160,1));

    Solution s(i,events);
    std::cout << s << std::endl;


}