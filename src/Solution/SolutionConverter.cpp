#include "Solution/SolutionConverter.hpp"

class SolutionConverter {

    std::vector<Vertex> m_vertices;
    std::vector<bool> m_have_demand;
    std::vector<Edge> m_edges;
    Instance m_instance;
    std::vector<Event> m_events;

    static std::vector<Position> read_lines(std::ifstream& t_file);
    void find_vertices(std::vector<Position> t_positions);
    Vertex find_matching_vertex(Position t_position);
    void find_matching_edge();
    int find_mathching_demandID(int t_vertex_id);
    void create_events();

public:
    SolutionConverter(const std::string& t_instance_path, const std::string& t_instance_name, const std::string& t_solution_path);

    Solution operator()();
};

std::vector<Position> SolutionConverter::read_lines(std::ifstream &t_file)
{
    std::vector<Position> positions;
    std::string line;
    std::vector<std::string> tab;
    getline(t_file, line);
    while(getline(t_file, line))
    {
        tab = split(line, ' ');
        positions.emplace_back(stod(tab[1]), stod(tab[2]));
    }
    return positions;
}

void SolutionConverter::find_vertices(std::vector<Position> t_positions)
{
    t_positions.emplace_back(t_positions[0]);
    Vertex previous_vertex, vertex = find_matching_vertex(t_positions[0]);
    m_vertices.emplace_back(vertex);
    m_have_demand.emplace_back(false);
    for(unsigned int p = 1, n = t_positions.size(); p < n; ++p)
    {
        previous_vertex = vertex;
        vertex = find_matching_vertex(t_positions[p]);
        for(auto& v : m_instance.getGraph().getTSPKernelPath(previous_vertex.getGraphID(), vertex.getGraphID()))
        {
            if(v == previous_vertex.getGraphID() || v == vertex.getGraphID())
                continue;
            m_vertices.emplace_back(m_instance.getGraph().getVertice(v));
            m_have_demand.emplace_back(false);
        }
        m_vertices.emplace_back(vertex);
        m_have_demand.emplace_back(true);
    }
}

Vertex SolutionConverter::find_matching_vertex(Position t_position)
{
    for(auto& vertex : m_instance.getGraph().getVertices())
    {
        if(t_position == vertex.getPos())
        {
            return vertex;
        }
    }
    throw std::invalid_argument("no matching vertex in solution converter");
}

void SolutionConverter::find_matching_edge()
{
    for(unsigned int i = 0, n = m_vertices.size() - 1; i < n; ++i)
    {
        bool find = false;
        for(auto& edge : m_instance.getGraph().getEdges())
        {
            if((edge.getStartID() == m_vertices[i].getGraphID() && edge.getEndID() == m_vertices[i + 1].getGraphID()) || (edge.getStartID() == m_vertices[i + 1].getGraphID() && edge.getEndID() == m_vertices[i].getGraphID()))
            {
                m_edges.emplace_back(edge);
                find = true;
                break;
            }
        }
        if(not find)
        {
            throw std::invalid_argument("no matching edge in solution converter");
        }
    }
}

int SolutionConverter::find_mathching_demandID(int t_vertex_id)
{
    for(auto& demand : m_instance.getGraph().getDemands())
        if(demand.getNodeGraphID() == t_vertex_id)
            return demand.getGraphID();
    throw std::invalid_argument("no matching demand in solution converter");
}

void SolutionConverter::create_events()
{
    int time = 0, demandID = -1;
    Position pos1, pos2;
    for(unsigned int i = 0, n = m_vertices.size() - 1; i < n; ++i)
    {
        pos1 = m_vertices[i].getPos();
        pos2 = m_vertices[i + 1].getPos();
        //livraison
        if(m_have_demand[i])
        {
            demandID = find_mathching_demandID(m_vertices[i].getGraphID());
            for(unsigned j = 0; j < m_instance.getGraph().getDemand(demandID).getAmount(); ++j)
            {
                m_events.emplace_back(pos1, time, 4, Position(), demandID);
                time += m_instance.getTruckDeliveryTime();
            }
        }
        //deplacement
        m_events.emplace_back(pos1, time, 0, pos2);
        time += m_instance.getTravelTime(m_edges[i])+1; //+1!!!
        //arrivee
        m_events.emplace_back(pos2, time, 1);
    }
}

SolutionConverter::SolutionConverter(const std::string& t_instance_path, const std::string& t_instance_name, const std::string& t_solution_path)
{
    m_instance = load(t_instance_path, t_instance_name);
    std::ifstream file;
    std::cout << "Trying to load : " << t_solution_path << std::endl;
    file.open(t_solution_path);
    if (!file.is_open())
    {
        throw std::runtime_error("Couldn't open file " + t_solution_path);
    }
    find_vertices(read_lines(file));
    find_matching_edge();
    file.close();
}

Solution SolutionConverter::operator()()
{
    create_events();
    return {m_instance, m_events};
}

Solution solution_convert(const std::string& t_instance_path, const std::string& t_instance_name, const std::string& t_solution_path)
{
    return SolutionConverter(t_instance_path, t_instance_name, t_solution_path)();
}