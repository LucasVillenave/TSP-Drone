#include <iostream>
#include <math.h>
#include "parser.hpp"
#include "Solution/Solution.hpp"
#include "Solution/SolutionParser.hpp"
#include "Graph/Graph.hpp"
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
    time += distance(p18, p15) / instance.getDroneSpeed() + 100;
    events.emplace_back(p15, time, 5, Position(), 4, 0);
    time += distance(p18, p15) / instance.getDroneSpeed() + 100;
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
    time += distance(p5, p3) / instance.getDroneSpeed();
    events.emplace_back(p3, time, 5, Position(), 3, 0);
    time += distance(p3, p5) / instance.getDroneSpeed();
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
    double time_drone_1 = time, time_drone_0 = time;
    events.emplace_back(p18, time_drone_0, 2, Position(), 0, 0);
    events.emplace_back(p18, time_drone_1, 2, Position(), 3, 1);
    //truck
    events.emplace_back(p18, time, 0, p1);
    time += instance.getTravelTime(graph.getEdge(16))+1;
    events.emplace_back(p1, time, 1);
    //livraison drone 1
    time_drone_1 += distance(p18, p3) / instance.getDroneSpeed();
    events.emplace_back(p3, time_drone_1, 5, Position(), 3, 1);
    //livraison drone 2
    time_drone_0 += distance(p18, p12) / instance.getDroneSpeed();
    events.emplace_back(p12, time_drone_0, 5, Position(), 0, 0);
    //retour drone
    time_drone_1 += distance(p3, p1) / instance.getDroneSpeed();
    events.emplace_back(p1, time_drone_1, 3, Position(), -1, 1);
    time_drone_0 += distance(p12, p1) / instance.getDroneSpeed();
    events.emplace_back(p1, time_drone_0, 3, Position(), -1, 0);
    //truck
    time += std::max(time_drone_0, time_drone_1);
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
    events.emplace_back(p0, time, 0, p5);
    time += instance.getTravelTime(graph.getEdge(15))+1;
    events.emplace_back(p5, time, 1);
    //drone 0
    events.emplace_back(p5, time, 2, Position(), 3, 0);
    time += distance(p5, p3) / instance.getDroneSpeed();
    events.emplace_back(p3, time, 5, Position(), 3, 0);
    time += distance(p3, p5) / instance.getDroneSpeed();
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
    double time_drone_1 = time, time_drone_0 = time;
    events.emplace_back(p18, time_drone_0, 2, Position(), 0, 0);
    events.emplace_back(p18, time_drone_1, 2, Position(), 3, 1);
    //truck
    events.emplace_back(p18, time, 0, p1);
    time += instance.getTravelTime(graph.getEdge(16))+1;
    events.emplace_back(p1, time, 1);
    //livraison drone 1
    time_drone_1 += distance(p18, p3) / instance.getDroneSpeed();
    events.emplace_back(p3, time_drone_1, 5, Position(), 3, 1);
    //livraison drone 2
    time_drone_0 += distance(p18, p12) / instance.getDroneSpeed();
    events.emplace_back(p12, time_drone_0, 5, Position(), 0, 0);
    //retour drone
    time_drone_1 += distance(p3, p1) / instance.getDroneSpeed();
    events.emplace_back(p1, time_drone_1, 3, Position(), -1, 1);
    time_drone_0 += distance(p12, p1) / instance.getDroneSpeed();
    events.emplace_back(p1, time_drone_0, 3, Position(), -1, 0);
    //truck
    time += std::max(time_drone_0, time_drone_1);
    events.emplace_back(p1, time, 0, p0);
    time += instance.getTravelTime(graph.getEdge(0))+1;
    events.emplace_back(p0, time, 1);
    return events;
}

int main(){
    //Solution solution = solution_parse("../Data/", "init", "../sol.txt");
    //std::cout << solution << std::endl;

    Instance instance = load("../Data/", "30");
    const Graph& graph = instance.getGraph();

    //COMPARAISON DES DISTANCES
    //std::filebuf fb;
    //fb.open ("../distanceTruck.txt",std::ios::out);
    //std::ostream os(&fb);
    //os << "Lat_start;Lon_start;Lat_end;Lon_end;Longueur;type;time" << std::endl;
    //for(const Edge& edge : graph.getEdges())
    //{
    //    const Position& start = graph.getVertice(edge.getStartID()).getPos(),
    //        end = graph.getVertice(edge.getEndID()).getPos();
    //    const double time = instance.getTravelTime(edge);
    //    os << std::setprecision(10) << start.getLatitude() <<  ";" << start.getLongitude() <<  ";" << end.getLatitude() <<  ";" << end.getLongitude() <<  ";" << edge.getCost() <<  ";" << edge.getRoadType() << ";" << time << std::endl;
    //}
    //fb.close();


    //CHECKER
    //Solution solution = solution_parse("../Data/", "30", "../results/sol.txt");
    //for(unsigned int i = 0, n = solution.getIsValid().size(); i < n; ++i)
    //    std::cout << solution.getIsValid(i) << std::endl;

    //ÉCRITURE DE LA SOLUTION DANS UN FICHIER
    //std::filebuf fb;
    //fb.open ("../results/solution.txt",std::ios::out);
    //std::ostream os(&fb);
    //os << solution;
    //fb.close();

}