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
        t_edges[i].setGraphID(i);

        if (adjacencyMatrix[t_edges[i].getStartID()][t_edges[i].getEndID()] == -1){
            adjacencyMatrix[t_edges[i].getStartID()][t_edges[i].getEndID()] = t_edges[i].getLength();

            adjacencyList[t_edges[i].getStartID()].push_back(t_edges[i]);
            adjacencyList[t_edges[i].getEndID()].push_back(t_edges[i]);

            m_edges.push_back(t_edges[i]);
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

Graph Graph::getUnitDemandGraph() const
{
    std::vector<Vertex> vertices;
    const unsigned int n = m_vertices.size();
    std::vector<std::vector<unsigned int>> id_vertices = std::vector<std::vector<unsigned int>>(n, std::vector<unsigned int>());
    Vertex current_vertex;
    unsigned int current_id;
    Position current_position;
    std::vector<Demand> demands;
    Demand current_demand;
    std::vector<Edge> edges;
    double current_lenght;
    std::string current_roadType;
    int demand_id = 0, node_id = 0, edge_id = 0;

    for(unsigned int v = 0; v < n; ++v)
    {
        current_vertex = m_vertices[v];
        current_position = current_vertex.getPos();
        current_id = current_vertex.getGraphID();
        if(current_vertex.getTDA() == 0)
        {
            vertices.emplace_back(current_position, current_vertex.getDemands(), node_id);
            id_vertices[current_id].emplace_back(node_id);
            ++node_id;
        }
        for(unsigned int d = 0; d < current_vertex.getTDA(); ++d)
        {
            //demand
            current_demand = Demand(current_position, 1, demand_id);
            current_demand.setGraphID(demand_id);
            ++demand_id;
            current_demand.setNodeGraphID(node_id);
            demands.emplace_back(current_demand);
            //vertex
            vertices.emplace_back(current_position, demands, node_id);
            demands.pop_back();
            id_vertices[current_id].emplace_back(node_id);
            ++node_id;
        }
    }
    for(const Edge& edge : m_edges)
    {
        current_lenght = edge.getLength();
        current_roadType = edge.getRoadType();
        for(unsigned int start : id_vertices[edge.getStartID()])
        {
            for(unsigned int end : id_vertices[edge.getEndID()])
            {
                edges.emplace_back(start, end, current_lenght, current_roadType, edge_id);
                ++edge_id;
            }
        }
    }
    for(const auto& id : id_vertices)
    {
        unsigned int size = id.size();
        if(size <= 1)
            continue;
        for(int i = 0; i < size; ++i)
        {
            for(int j = i + 1; j < size; ++j)
            {
                if(i == j)
                    continue;
                edges.emplace_back(i, j, 0, " ", edge_id);
                ++edge_id;
            }
        }
    }

    return {vertices, edges};
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