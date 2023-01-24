#include <iostream>
#include "parser.hpp"
#include "Solution/Solution.hpp"
#include "Solution/SolutionPreview.hpp"
#include "Graph/Graph.hpp"
#include "Graph/GraphPreview.hpp"
#include "utils.hpp"
#include "gurobi_c++.h"

std::vector<Event> getEventCase0(const Instance& instance)
{
    const Graph& graph = instance.getGraph();
    std::vector<Event> events = {};
    Position p0 = graph.getVertice(0).getPos();
    Position p1 = graph.getVertice(1).getPos();
    Position p2 = graph.getVertice(2).getPos();
    Position p3 = graph.getVertice(3).getPos();
    Position p4 = graph.getVertice(4).getPos();
    Position p5 = graph.getVertice(5).getPos();
    Position p6 = graph.getVertice(6).getPos();
    Position p7 = graph.getVertice(7).getPos();
    Position p8 = graph.getVertice(8).getPos();
    Position p10 = graph.getVertice(10).getPos();
    Position p11 = graph.getVertice(11).getPos();
    Position p12 = graph.getVertice(12).getPos();
    Position p13 = graph.getVertice(13).getPos();
    Position p15 = graph.getVertice(15).getPos();
    Position p17 = graph.getVertice(17).getPos();
    Position p18 = graph.getVertice(18).getPos();
    double time = 0;
    //move
    events.emplace_back(p0, time, 0, p5);
    time += instance.getTravelTime(graph.getEdge(15))+1;
    events.emplace_back(p5, time, 1);
    events.emplace_back(p5, time, 0, p4);
    time += instance.getTravelTime(graph.getEdge(2))+1;
    events.emplace_back(p4, time, 1);
    //delivery
    events.emplace_back(p4, time, 4, Position(), 2);
    time += instance.getTruckDeliveryTime()+100;
    //move
    events.emplace_back(p4, time, 0, p8);
    time += instance.getTravelTime(graph.getEdge(17))+1;
    events.emplace_back(p8, time, 1);
    events.emplace_back(p8, time, 0, p7);
    time += instance.getTravelTime(graph.getEdge(4))+1;
    events.emplace_back(p7, time, 1);
    //delivery
    events.emplace_back(p7, time, 4, Position(), 1);
    time += instance.getTruckDeliveryTime()+100;
    //move
    events.emplace_back(p7, time, 0, p6);
    time += instance.getTravelTime(graph.getEdge(3))+1;
    events.emplace_back(p6, time, 1);
    events.emplace_back(p6, time, 0, p2);
    time += instance.getTravelTime(graph.getEdge(18))+1;
    events.emplace_back(p2, time, 1);
    events.emplace_back(p2, time, 0, p3);
    time += instance.getTravelTime(graph.getEdge(1))+1;
    events.emplace_back(p3, time, 1);
    //delivery
    events.emplace_back(p3, time, 4, Position(), 3);
    time += instance.getTruckDeliveryTime()+100;
    events.emplace_back(p3, time, 4, Position(), 3);
    time += instance.getTruckDeliveryTime()+100;
    //move
    events.emplace_back(p3, time, 0, p1);
    time += instance.getTravelTime(graph.getEdge(21))+1;
    events.emplace_back(p1, time, 1);
    events.emplace_back(p1, time, 0, p18);
    time += instance.getTravelTime(graph.getEdge(16))+1;
    events.emplace_back(p18, time, 1);
    events.emplace_back(p18, time, 0, p17);
    time += instance.getTravelTime(graph.getEdge(12))+1;
    events.emplace_back(p17, time, 1);
    events.emplace_back(p17, time, 0, p15);
    time += instance.getTravelTime(graph.getEdge(11))+1;
    events.emplace_back(p15, time, 1);
    //delivery
    events.emplace_back(p15, time, 4, Position(), 4);
    time += instance.getTruckDeliveryTime()+100;
    //move
    events.emplace_back(p15, time, 0, p13);
    time += instance.getTravelTime(graph.getEdge(24))+1;
    events.emplace_back(p13, time, 1);
    events.emplace_back(p13, time, 0, p12);
    time += instance.getTravelTime(graph.getEdge(8))+1;
    events.emplace_back(p12, time, 1);
    //delivery
    events.emplace_back(p12, time, 4, Position(), 0);
    time += instance.getTruckDeliveryTime()+100;
    //move
    events.emplace_back(p12, time, 0, p10);
    time += instance.getTravelTime(graph.getEdge(7))+1;
    events.emplace_back(p10, time, 1);
    events.emplace_back(p10, time, 0, p11);
    time += instance.getTravelTime(graph.getEdge(6))+1;
    events.emplace_back(p11, time, 1);
    events.emplace_back(p11, time, 0, p0);
    time += instance.getTravelTime(graph.getEdge(13))+1;
    events.emplace_back(p0, time, 1);
    return events;
}

std::vector<Event> getEventCase1(const Instance& instance)
{
    const Graph& graph = instance.getGraph();
    std::vector<Event> events = {};
    Position p0 = graph.getVertice(0).getPos();
    Position p1 = graph.getVertice(1).getPos();
    Position p2 = graph.getVertice(2).getPos();
    Position p3 = graph.getVertice(3).getPos();
    Position p4 = graph.getVertice(4).getPos();
    Position p5 = graph.getVertice(5).getPos();
    Position p6 = graph.getVertice(6).getPos();
    Position p7 = graph.getVertice(7).getPos();
    Position p8 = graph.getVertice(8).getPos();
    Position p10 = graph.getVertice(10).getPos();
    Position p11 = graph.getVertice(11).getPos();
    Position p12 = graph.getVertice(12).getPos();
    Position p13 = graph.getVertice(13).getPos();
    Position p15 = graph.getVertice(15).getPos();
    Position p18 = graph.getVertice(18).getPos();
    double time = 0;
    //move
    events.emplace_back(p0, time, 0, p5);
    time += instance.getTravelTime(graph.getEdge(15))+1;
    events.emplace_back(p5, time, 1);
    events.emplace_back(p5, time, 0, p4);
    time += instance.getTravelTime(graph.getEdge(2))+1;
    events.emplace_back(p4, time, 1);
    //delivery
    events.emplace_back(p4, time, 4, Position(), 2);
    time += instance.getTruckDeliveryTime()+100;
    //move
    events.emplace_back(p4, time, 0, p8);
    time += instance.getTravelTime(graph.getEdge(17))+1;
    events.emplace_back(p8, time, 1);
    events.emplace_back(p8, time, 0, p7);
    time += instance.getTravelTime(graph.getEdge(4))+1;
    events.emplace_back(p7, time, 1);
    //delivery
    events.emplace_back(p7, time, 4, Position(), 1);
    time += instance.getTruckDeliveryTime()+100;
    //move
    events.emplace_back(p7, time, 0, p6);
    time += instance.getTravelTime(graph.getEdge(3))+1;
    events.emplace_back(p6, time, 1);
    events.emplace_back(p6, time, 0, p2);
    time += instance.getTravelTime(graph.getEdge(18))+1;
    events.emplace_back(p2, time, 1);
    events.emplace_back(p2, time, 0, p3);
    time += instance.getTravelTime(graph.getEdge(1))+1;
    events.emplace_back(p3, time, 1);
    //delivery
    events.emplace_back(p3, time, 4, Position(), 3);
    time += instance.getTruckDeliveryTime()+100;
    events.emplace_back(p3, time, 4, Position(), 3);
    time += instance.getTruckDeliveryTime()+100;
    //move
    events.emplace_back(p3, time, 0, p1);
    time += instance.getTravelTime(graph.getEdge(21))+1;
    events.emplace_back(p1, time, 1);
    events.emplace_back(p1, time, 0, p18);
    time += instance.getTravelTime(graph.getEdge(16))+1;
    events.emplace_back(p18, time, 1);
    //drone 0
    events.emplace_back(p18, time, 2, Position(), 4, 0);
    time += euclidianDistance(p18.getLongitude(), p18.getLongitude(), p15.getLongitude(), p15.getLatitude()) / instance.getDroneSpeed() + 100;
    events.emplace_back(p15, time, 5, Position(), 4, 0);
    time += euclidianDistance(p18.getLongitude(), p18.getLongitude(), p15.getLongitude(), p15.getLatitude()) / instance.getDroneSpeed() + 100;
    events.emplace_back(p18, time, 3, Position(), -1, 0);
    //move
    events.emplace_back(p18, time, 0, p13);
    time += instance.getTravelTime(graph.getEdge(23))+1;
    events.emplace_back(p13, time, 1);
    events.emplace_back(p13, time, 0, p12);
    time += instance.getTravelTime(graph.getEdge(8))+1;
    events.emplace_back(p12, time, 1);
    //delivery
    events.emplace_back(p12, time, 4, Position(), 0);
    time += instance.getTruckDeliveryTime()+100;
    //move
    events.emplace_back(p12, time, 0, p10);
    time += instance.getTravelTime(graph.getEdge(7))+1;
    events.emplace_back(p10, time, 1);
    events.emplace_back(p10, time, 0, p11);
    time += instance.getTravelTime(graph.getEdge(6))+1;
    events.emplace_back(p11, time, 1);
    events.emplace_back(p11, time, 0, p0);
    time += instance.getTravelTime(graph.getEdge(13))+1;
    events.emplace_back(p0, time, 1);
    return events;
}

std::vector<Event> getEventCase2(const Instance& instance)
{
    const Graph& graph = instance.getGraph();
    std::vector<Event> events = {};
    Position p0 = graph.getVertice(0).getPos();
    Position p1 = graph.getVertice(1).getPos();
    Position p3 = graph.getVertice(3).getPos();
    Position p4 = graph.getVertice(4).getPos();
    Position p5 = graph.getVertice(5).getPos();
    Position p7 = graph.getVertice(7).getPos();
    Position p8 = graph.getVertice(8).getPos();
    Position p9 = graph.getVertice(9).getPos();
    Position p12 = graph.getVertice(12).getPos();
    Position p14 = graph.getVertice(14).getPos();
    Position p15 = graph.getVertice(15).getPos();
    Position p16 = graph.getVertice(16).getPos();
    Position p17 = graph.getVertice(17).getPos();
    Position p18 = graph.getVertice(18).getPos();

    double time = 0;
    events.emplace_back(p0, time, 0, p5);
    time += instance.getTravelTime(graph.getEdge(15))+1;
    events.emplace_back(p5, time, 1);
    //drone 0
    events.emplace_back(p5, time, 2, Position(), 3, 0);
    time += euclidianDistance(p5.getLongitude(), p5.getLongitude(), p3.getLongitude(), p3.getLatitude()) / instance.getDroneSpeed();
    events.emplace_back(p3, time, 5, Position(), 3, 0);
    time += euclidianDistance(p3.getLongitude(), p3.getLongitude(), p5.getLongitude(), p5.getLatitude()) / instance.getDroneSpeed();
    events.emplace_back(p5, time, 3, Position(), -1, 0);
    //truck
    events.emplace_back(p5, time, 0, p4);
    time += instance.getTravelTime(graph.getEdge(2))+1;
    events.emplace_back(p4, time, 1);
    //delivery
    events.emplace_back(p4, time, 4, Position(), 2, -1);
    time += instance.getTruckDeliveryTime()+100;
    events.emplace_back(p4, time, 0, p8);
    time += instance.getTravelTime(graph.getEdge(17))+1;
    events.emplace_back(p8, time, 1);
    events.emplace_back(p8, time, 0, p7);
    time += instance.getTravelTime(graph.getEdge(4))+1;
    events.emplace_back(p7, time, 1);
    //delivery
    events.emplace_back(p7, time, 4, Position(), 1, -1);
    time += instance.getTruckDeliveryTime()+100;
    events.emplace_back(p7, time, 0, p9);
    time += instance.getTravelTime(graph.getEdge(5))+1;
    events.emplace_back(p9, time, 1);
    events.emplace_back(p9, time, 0, p16);
    time += instance.getTravelTime(graph.getEdge(19))+1;
    events.emplace_back(p16, time, 1);
    events.emplace_back(p16, time, 0, p14);
    time += instance.getTravelTime(graph.getEdge(20))+1;
    events.emplace_back(p14, time, 1);
    events.emplace_back(p14, time, 0, p15);
    time += instance.getTravelTime(graph.getEdge(9))+1;
    events.emplace_back(p15, time, 1);
    //delivery
    events.emplace_back(p15, time, 4, Position(), 4, -1);
    time += instance.getTruckDeliveryTime()+100;
    events.emplace_back(p15, time, 0, p17);
    time += instance.getTravelTime(graph.getEdge(11))+1;
    events.emplace_back(p17, time, 1);
    events.emplace_back(p17, time, 0, p18);
    time += instance.getTravelTime(graph.getEdge(12))+1;
    events.emplace_back(p18, time, 1);
    //drones
    events.emplace_back(p18, time, 2, Position(), 0, 0);
    double depart_time = time;
    events.emplace_back(p18, time, 2, Position(), 3, 1);
    time += euclidianDistance(p18.getLongitude(), p18.getLongitude(), p12.getLongitude(), p12.getLatitude()) / instance.getDroneSpeed();
    events.emplace_back(p12, time, 5, Position(), 0, 0);
    time += euclidianDistance(p12.getLongitude(), p12.getLongitude(), p18.getLongitude(), p18.getLatitude()) / instance.getDroneSpeed();
    events.emplace_back(p18, time, 3, Position(), -1, 0);
    //truck
    events.emplace_back(p18, time, 0, p1);
    time += instance.getTravelTime(graph.getEdge(16))+1;
    events.emplace_back(p1, time, 1);
    //drone
    depart_time += euclidianDistance(p18.getLongitude(), p18.getLongitude(), p3.getLongitude(), p3.getLatitude()) / instance.getDroneSpeed();
    events.emplace_back(p3, depart_time, 5, Position(), 3, 1);
    time = depart_time + euclidianDistance(p3.getLongitude(), p3.getLongitude(), p18.getLongitude(), p18.getLatitude()) / instance.getDroneSpeed();
    events.emplace_back(p1, time, 3, Position(), -1, 1);
    //truck
    events.emplace_back(p1, time, 0, p0);
    time += instance.getTravelTime(graph.getEdge(0))+1;
    events.emplace_back(p0, time, 1);
    return events;
}

std::vector<Event> getEventCase3(const Instance& instance)
{
    const Graph& graph = instance.getGraph();
    std::vector<Event> events = {};
    Position p0 = graph.getVertice(0).getPos();
    Position p1 = graph.getVertice(1).getPos();
    Position p2 = graph.getVertice(2).getPos();
    Position p3 = graph.getVertice(3).getPos();
    Position p4 = graph.getVertice(4).getPos();
    Position p5 = graph.getVertice(5).getPos();
    Position p6 = graph.getVertice(6).getPos();
    Position p7 = graph.getVertice(7).getPos();
    Position p8 = graph.getVertice(8).getPos();
    Position p9 = graph.getVertice(9).getPos();
    Position p10 = graph.getVertice(10).getPos();
    Position p11 = graph.getVertice(11).getPos();
    Position p12 = graph.getVertice(12).getPos();
    Position p13 = graph.getVertice(13).getPos();
    Position p14 = graph.getVertice(14).getPos();
    Position p15 = graph.getVertice(15).getPos();
    Position p16 = graph.getVertice(16).getPos();
    Position p17 = graph.getVertice(17).getPos();
    Position p18 = graph.getVertice(18).getPos();
    double time = 0;

    return events;
}

int main(){
    Instance instance = load("../Data/","test");
    /*
    for(const auto& edge : instance.getGraph().getEdges())
    {
        std::cout << std::endl << edge.getID() << std::endl;
        std::cout << "length=" << edge.getLength() << std::endl;
        std::cout << "distance=" << euclidianDistance(instance.getGraph().getVertice(edge.getStartID()).getLongitude(),
                                                      instance.getGraph().getVertice(edge.getStartID()).getLatitude(),
                                                      instance.getGraph().getVertice(edge.getEndID()).getLongitude(),
                                                      instance.getGraph().getVertice(edge.getEndID()).getLatitude()) << std::endl;
        std::cout << "time=" << instance.getTravelTime(edge) << std::endl;
    }
    */

    //const Graph& graph = instance.getGraph();
    //std::cout << graph << std::endl;
    //graphPreview(graph);
    //system("pdflatex graph.tex");
    //system("xdg-open graph.pdf");



    Solution solution = Solution(instance, getEventCase1(instance));
    //std::cout << solution << std::endl;
    for(unsigned int i = 0, n = solution.getIsValid().size(); i < n; ++i)
        std::cout << solution.getIsValid(i) << std::endl;
    //solutionPreview(solution);
    //system("pdflatex solution.tex");
    //system("xdg-open solution.pdf");

}