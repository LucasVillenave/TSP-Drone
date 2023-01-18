#include "Solution/Solution.hpp"

Solution::Solution(Instance t_instance, const std::vector<Event>& t_eventList)
    : instance(std::move(t_instance)), eventList(t_eventList)
{
    int time = 0;
    for (Event e : t_eventList){
        if (time>e.getTime()){
            throw std::invalid_argument("event list given in solution creation isn't timely ordered");
        }
        time = e.getTime();
    }
}

int Solution::check(int scenario){
    if (scenario>3 || scenario<0){
        std::cout << "scenario should be between 0 and 3, " << scenario << " was given" << std::endl;
        throw std::invalid_argument("");
    }
    if (isValid[scenario]==-1){
        //Todo
    }
    return isValid[scenario];
}

const std::vector<Event>& Solution::getEvents() const
{
    return eventList;
}


const std::vector<Vertex>& Solution::getVertices() const
{
    return instance.getGraph().getVertices();
}

const std::vector<Edge>& Solution::getEdges() const
{
    return instance.getGraph().getEdges();
}