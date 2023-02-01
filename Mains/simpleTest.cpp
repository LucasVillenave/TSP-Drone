#include <iostream>
#include "parser.hpp"
#include "Solution/Solution.hpp"
#include "Solution/SolutionPreview.hpp"
#include "Graph/Graph.hpp"
#include "Graph/GraphPreview.hpp"
#include "utils.hpp"

int main(){
    
    //Positions
    Position p0, p1, p2, p3, p4, p5;
    p0.setX(0); p0.setY(0);
    p1.setX(0); p1.setY(1);
    p2.setX(1); p2.setY(0);
    p3.setX(1); p3.setY(1);
    p4.setX(2); p4.setY(1);
    p5.setX(1); p5.setY(2);

    std::vector<Position> positions;
    std::vector<Vertex> vertices;
    positions.push_back(p0); positions.push_back(p1); positions.push_back(p2); 
    positions.push_back(p3); positions.push_back(p4); positions.push_back(p5);

    //Vertices to positions
    for (int i=0; i<positions.size(); ++i){
        vertices.push_back(Vertex(positions[i]));
    }

    //Edges
    // Edge e0(0,1,euclidianDistance(p0,p1),"route étoilé");
    // Edge e1(0,2,euclidianDistance(p0,p2),"route champi");
    // Edge e2(1,3,euclidianDistance(p1,p3),"route torillas");
    // Edge e3(3,4,euclidianDistance(p3,p4),"route de perlinpimpin");
    // Edge e4(2,5,euclidianDistance(p2,p5),"route de la soie");
    // Edge e5(3,5,euclidianDistance(p3,p5),"destination finale");

    Edge e0(0,1,8,"route étoilé");
    Edge e1(0,2,6,"route champi");
    Edge e2(1,3,25,"route torillas");
    Edge e3(3,4,1,"route de perlinpimpin");
    Edge e4(2,5,3,"route de la soie");
    Edge e5(3,5,12,"destination finale");

    std::vector<Edge> edges;
    edges.push_back(e0); edges.push_back(e1); edges.push_back(e2);
    edges.push_back(e3); edges.push_back(e4); edges.push_back(e5);

    //Demands
    Demand d0(p1,1,-5);
    Demand d1(p5,3,-4);

    //Basic stuffs
    Graph graph(vertices,edges);
    graph.addDemand(d0);
    graph.addDemand(d1);

    Instance instance(graph,"dindon");

    //Events
    std::vector<Event> events;
    events.push_back(Event(p0,0,0,p1));
    events.push_back(Event(p1,200,1));
    events.push_back(Event(p1,200,4,Position(),0));
    events.push_back(Event(p1,260,0,p3));
    events.push_back(Event(p3,460,1));
    events.push_back(Event(p3,460,0,p5));
    events.push_back(Event(p5,660,1));
    events.push_back(Event(p5,660,4,Position(),1));
    events.push_back(Event(p5,720,4,Position(),1));
    events.push_back(Event(p5,780,4,Position(),1));
    events.push_back(Event(p5,840,0,p2));
    events.push_back(Event(p2,1040,1));
    events.push_back(Event(p2,1040,0,p0));
    events.push_back(Event(p0,1240,1));

    Solution sol(instance,events);

    std::cout << "after check case 0 is valid : " << sol.getIsValid(0) << std::endl;

    std::vector<int> tourney;
    
    tourney.push_back(0);
    tourney.push_back(5);
    tourney.push_back(2);
    tourney.push_back(3);
    tourney.push_back(4);
    tourney.push_back(1);

    tourney = TSP2OPT(instance.getGraph().getTSPKernelTime(), tourney);
    std::vector<std::vector<std::vector<int>>> x(6,std::vector<std::vector<int>>(6,std::vector<int>(6,0)));
    std::vector<std::vector<std::vector<int>>> z(6,std::vector<std::vector<int>>(instance.getGraph().getDemands().size(),std::vector<int>(2,0)));

    z[0][0][0] = 1;

    for (int i=0; i<6; ++i){
        for (int j=0; j<6; ++j){
            std::cout << instance.getGraph().getTSPKernelTime(i,j) << ",   ";
        }
        std::cout << std::endl;
    }

    std::cout << "tourney : ";
    for (int i=0; i<5; ++i){
        std::cout << tourney[i] << " ";
        x[i][tourney[i]][tourney[i+1]] = 1;
    }
    std::cout << tourney[5] << std::endl;
    x[5][tourney[5]][tourney[0]] = 1;

    std::cout << vertices[0] << std::endl;

    Solution sol2 = convertCase01(instance,x,z);
    
    std::cout << "after check, sol 2 case 0 is valid : " << sol2.getIsValid(0) << std::endl;
    std::cout << "after check, sol 2 case 1 is valid : " << sol2.getIsValid(1) << std::endl;

}