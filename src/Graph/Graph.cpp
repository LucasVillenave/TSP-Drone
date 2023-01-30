#include "Graph/Graph.hpp"
#include "utils.hpp"
#include <stdexcept>
#include <iostream>

Graph::Graph(std::vector<Vertex> t_vertices, std::vector<Edge> t_edges) : vertices(t_vertices)
{
    adjacencyList = std::vector<std::vector<Edge>>(t_vertices.size(),std::vector<Edge>());
    adjacencyMatrix = std::vector<std::vector<int>>(t_vertices.size(),std::vector<int>(t_vertices.size(),-1));
    TSPKernelDist = std::vector<std::vector<double>>(t_vertices.size(),std::vector<double>(t_vertices.size(),-1));
    nbr_noeuds_demandes = 0;
    nbr_demandes_unitaires = 0;

    for (int i=0; i < t_vertices.size(); ++i){
        vertices[i].setGraphID(i);
    }

    for (int i=0; i < t_edges.size(); ++i){
        t_edges[i].setGraphID(i);

        if (adjacencyMatrix[t_edges[i].getStartID()][t_edges[i].getEndID()] == -1){
            adjacencyMatrix[t_edges[i].getStartID()][t_edges[i].getEndID()] = t_edges[i].getLength();

            adjacencyList[t_edges[i].getStartID()].push_back(t_edges[i]);
            adjacencyList[t_edges[i].getEndID()].push_back(t_edges[i]);

            edges.push_back(t_edges[i]);
        }

        if ((t_edges[i].getStartID() < 0 || t_edges[i].getStartID() >= t_vertices.size() ||
             t_edges[i].getEndID() < 0 || t_edges[i].getEndID() >= t_vertices.size())){
            throw std::invalid_argument("Bad t_vertices index on t_edges");
        }
    }
}

void Graph::addDemands(const std::vector<Demand>& t_demands){
    for (const Demand& d : t_demands){
        addDemand(d);
    }
}

void Graph::addDemand(Demand d){
    int closestID = closest(vertices,d.getPos());
    if (vertices[closestID].getTDA() <= 0){
        nbr_noeuds_demandes = nbr_noeuds_demandes + 1;
    }
    vertices[closestID].addDemand(d);
    d.setNodeGraphID(closestID);
    d.setNodePos(vertices[closestID].getPos());
    d.setGraphID(demands.size());
    demands.push_back(d);
    nbr_demandes_unitaires = nbr_demandes_unitaires + d.getAmount();
    Demand d_unit = Demand(d.getInitPos(), 1, -1);
    d_unit.setNodeGraphID(closestID);
    d_unit.setNodePos(vertices[closestID].getPos());
    d_unit.setGraphID(demandsUnit.size());
    for (int eps=0; eps<d.getAmount(); ++eps){
        demandsUnit.push_back(d_unit);
    }
}

int Graph::getNbr_noeuds_demandes() const{
    return nbr_noeuds_demandes;
}

int Graph::getNbr_demandes_unitaires() const{
    return nbr_demandes_unitaires;
}

const std::vector<Vertex>& Graph::getVertices() const{
    return vertices;
}

const Vertex& Graph::getVertice(int GraphID) const{
    return vertices[GraphID];
}

const std::vector<Demand>& Graph::getDemands() const{
    return demands;
}

const Demand& Graph::getDemand(int GraphID) const{
    return demands[GraphID];
}

const std::vector<Demand>& Graph::getDemandsUnit() const{
    return demands;
}

const Demand& Graph::getDemandUnit(int GraphID) const{
    return demands[GraphID];
}

const std::vector<Edge>& Graph::getEdges() const{
    return edges;
}

const Edge& Graph::getEdge(int GraphID) const{
    return edges[GraphID];
}

void Graph::kernelize(Instance instance){
    std::vector<int> TMPvertices;
    for (int i=0; i<vertices.size();++i){
        TMPvertices.push_back(i);
    }
    TSPKernelPath = updateDistMatrix(TSPKernelDist,adjacencyList,TMPvertices, instance);
}

const std::vector<std::vector<double>>& Graph::getTSPKernelDist() const{
    return TSPKernelDist;
}

const std::vector<std::vector<std::vector<int>>>& Graph::getTSPKernelPath() const{
    return TSPKernelPath;
}

double Graph::getTSPKernelDist(int i, int j) const{
    return TSPKernelDist[i][j];
}

const std::vector<int>& Graph::getTSPKernelPath(int i, int j) const{
    return TSPKernelPath[i][j];
}

const std::vector<std::vector<Edge>>& Graph::getAdjacencyList() const{
    return adjacencyList;
}

const std::vector<Edge>& Graph::getAdjacencyList(int vertexID) const{
    return adjacencyList[vertexID];
}

const std::vector<std::vector<int>>& Graph::getAdjacencyMatrix() const{
    return adjacencyMatrix;
}

const std::vector<int>& Graph::getAdjacencyMatrix(int vertexID) const{
    return adjacencyMatrix[vertexID];
}