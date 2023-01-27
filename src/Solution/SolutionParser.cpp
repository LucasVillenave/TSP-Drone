#include "Solution/SolutionParser.hpp"

class SolutionParser {

    std::vector<Event> m_events;
    Instance m_instance;

    void read_lines(std::ifstream& t_file);
    static Position get_localisation(const std::string& t_lat, const std::string& t_lon);

public:
    SolutionParser(const std::string& t_instance_path, const std::string& t_instance_name, const std::string& t_solution_path);

    Solution operator()();
};

void SolutionParser::read_lines(std::ifstream &t_file)
{
    std::string line, eventName;
    std::vector<std::string> tab;
    int time, eventType = -1, demandID = -1, droneID = -1;
    Position pos1, pos2;

    getline(t_file, line);
    while(getline(t_file, line))
    {
        tab = split(line, ';');
        time = stoi(tab[0]);
        eventName = tab[1].substr(1, 11);
        if(eventName == "DEPLACEMENT")
        {
            eventType = 0;
            pos2 = get_localisation(tab[1].substr(35), tab[2]);
            pos1 = get_localisation(tab[3], tab[4]);
        }
        else
        {
            pos1 = get_localisation(tab[2], tab[3]);
        }
        if(eventName == "ARRIVEE VEH")
        {
            eventType = 1;
        }
        else if(eventName == "LARGAGE DRO")
        {
            eventType = 2;
            std::string id = tab[1].substr(15, 1);
            droneID = stoi(id);
            id = split(tab[1], ':')[1];
            id = id.substr(1, id.size()-2);
            demandID = stoi(id);
        }
        else if(eventName == "RECUPERATIO")
        {
            eventType = 3;
            std::string id = tab[1].substr(20, 1);
            droneID = stoi(id);
        }
        else if(eventName == "LIVRAISON C")
        {
            eventType = 4;
            std::string id = split(tab[1], ':')[1];
            id = id.substr(1, id.size()-2);
            demandID = stoi(id);
        }
        else if(eventName == "LIVRAISON D")
        {
            eventType = 5;
            std::string id = tab[1].substr(17, 1);
            droneID = stoi(id);
            id = split(tab[1], ':')[1];
            id = id.substr(1, id.size()-2);
            demandID = stoi(id);
        }
        m_events.emplace_back(pos1, time, eventType, pos2, demandID, droneID);
    }
}

Position SolutionParser::get_localisation(const std::string& t_lat, const std::string& t_lon)
{
    std::string lat = split(t_lat, ':')[1];
    lat = split(lat, '(')[0];
    lat = lat.substr(1);
    std::string lon = split(t_lon, ':')[1];
    lon = split(lon, '(')[0];
    lon = lon.substr(1);
    return {std::stod(lat), std::stod(lon)};
}

SolutionParser::SolutionParser(const std::string& t_instance_path, const std::string& t_instance_name, const std::string& t_solution_path)
{
    m_instance = load(t_instance_path, t_instance_name);
    std::ifstream file;
    std::cout << "Trying to load : " << t_solution_path << std::endl;
    file.open(t_solution_path);
    if (!file.is_open())
    {
        throw std::runtime_error("Couldn't open file " + t_solution_path);
    }
    read_lines(file);
    file.close();
}

Solution SolutionParser::operator()()
{
    return {m_instance, m_events};
}

Solution solution_parse(const std::string& t_instance_path, const std::string& t_instance_name, const std::string& t_solution_path)
{
    return SolutionParser(t_instance_path, t_instance_name, t_solution_path)();
}