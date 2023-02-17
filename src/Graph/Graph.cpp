#include "Graph/Graph.hpp"
#include "utils.hpp"
#include <stdexcept>
#include <iostream>
#include <utility>

Graph::Graph(const std::vector<Vertex>& t_vertices, std::vector<Edge> t_edges) : m_vertices(t_vertices)
{
    const unsigned int size_v = m_vertices.size();
    adjacencyList = std::vector<std::vector<Edge>>(size_v, std::vector<Edge>());
    adjacencyMatrix = std::vector<std::vector<int>>(size_v, std::vector<int>(size_v, -1));
    TSPKernelTime = std::vector<std::vector<double>>(size_v, std::vector<double>(size_v, -1));
    m_number_of_demand_nodes = 0;

    for (int i=0; i < size_v; ++i){
        m_vertices[i].setGraphID(i);
        if(not m_vertices[i].getDemands().empty())
        {
            ++m_number_of_demand_nodes;
            for (const Demand &demand: m_vertices[i].getDemands())
                m_demands.emplace_back(demand);
        }
    }

    for (int i=0; i < t_edges.size(); ++i){
        int isEdgeAlreadyExisting = 0;
        for (int j=0; j<m_edges.size(); ++j){
            if (((m_edges[j].getEndID() == t_edges[i].getEndID()) && (m_edges[j].getStartID() == t_edges[i].getStartID()))
            ||
            ((m_edges[j].getStartID() == t_edges[i].getEndID()) && (m_edges[j].getEndID() == t_edges[i].getStartID()))){
                isEdgeAlreadyExisting=1;
            }
        }

        if (isEdgeAlreadyExisting==0){
            t_edges[i].setGraphID(m_edges.size());

            if ((t_edges[i].getStartID() < 0 || t_edges[i].getStartID() >= t_vertices.size() ||
                t_edges[i].getEndID() < 0 || t_edges[i].getEndID() >= t_vertices.size())){
                throw std::invalid_argument("Bad t_vertices index on t_edges");
            }

            if (adjacencyMatrix[t_edges[i].getStartID()][t_edges[i].getEndID()] == -1){
                adjacencyMatrix[t_edges[i].getStartID()][t_edges[i].getEndID()] = t_edges[i].getLength();

                adjacencyList[t_edges[i].getStartID()].push_back(t_edges[i]);
                adjacencyList[t_edges[i].getEndID()].push_back(t_edges[i]);

                m_edges.push_back(t_edges[i]);
            }
        }
    }
}

void Graph::addDemands(const std::vector<Demand>& t_demands){
    for (const Demand& d : t_demands){
        addDemand(d);
    }
}

void Graph::addDemand(Demand d){
    int closestID = closest(m_vertices, d.getPos());
    if (m_vertices[closestID].getTDA() <= 0){
        m_number_of_demand_nodes = m_number_of_demand_nodes + 1;
    }
    m_vertices[closestID].addDemand(d);
    d.setNodeGraphID(closestID);
    d.setNodePos(m_vertices[closestID].getPos());
    d.setGraphID(m_demands.size());
    m_demands.push_back(d);
}

int Graph::getNumberDemandNodes() const{
    return m_number_of_demand_nodes;
}

Graph Graph::getUnitDemandGraph()
{
    std::vector<Vertex> unitVertices;
    for (Vertex v : m_vertices){
        Vertex uv(v.getPos());
        uv.setID(v.getID());
        unitVertices.push_back(uv);
    }
    Graph unitDemandGraph(unitVertices,m_edges);
    std::vector<Demand> unitDemands;
    for (Demand d : m_demands){
        for (int i=0; i<d.getAmount(); ++i){
            UnitaryToOriginalDemandID.push_back(d.getGraphID());
            Demand ud(d.getInitPos(),1,d.getGraphID());
            ud.setGraphID(unitDemands.size());
            ud.setNodeGraphID(d.getNodeGraphID());
            ud.setNodePos(d.getNodePos());
            ud.setX(d.getX());
            ud.setY(d.getY());
            unitDemands.push_back(ud);
        }
    }
    unitDemandGraph.addDemands(unitDemands);
    return unitDemandGraph;
}

const int Graph::getOriginalDemandID(int unitID){
    getUnitDemandGraph();
    return UnitaryToOriginalDemandID[unitID];
}

const std::vector<Vertex>& Graph::getVertices() const{
    return m_vertices;
}

const Vertex& Graph::getVertice(int GraphID) const{
    return m_vertices[GraphID];
}

const std::vector<Demand>& Graph::getDemands() const{
    return m_demands;
}

const Demand& Graph::getDemand(int GraphID) const{
    return m_demands[GraphID];
}

const std::vector<Demand>& Graph::getDemandsUnit() const{
    return m_demands;
}

const Demand& Graph::getDemandUnit(int GraphID) const{
    return m_demands[GraphID];
}

const std::vector<Edge>& Graph::getEdges() const{
    return m_edges;
}

const Edge& Graph::getEdge(int GraphID) const{
    return m_edges[GraphID];
}

void Graph::kernelize(Instance instance){
    std::vector<int> TMPvertices;
    for (int i=0; i < m_vertices.size(); ++i){
        TMPvertices.push_back(i);
    }
    TSPKernelPath = updateDistMatrix(TSPKernelTime, adjacencyList,TMPvertices, std::move(instance));
}

const std::vector<std::vector<double>>& Graph::getTSPKernelTime() const{
    return TSPKernelTime;
}

const std::vector<std::vector<std::vector<int>>>& Graph::getTSPKernelPath() const{
    return TSPKernelPath;
}

double Graph::getTSPKernelTime(int i, int j) const{
    return TSPKernelTime[i][j];
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