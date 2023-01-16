#ifndef SOLUTION
#define SOLUTION

#include "Instance.hpp"
#include "Event.hpp"

class Solution{
    private :
        Instance instance;
        std::vector<Event> eventList;
        std::vector<int> isValid = std::vector<int>(4,-1);

    public :
        Solution(Instance instance, std::vector<Event> eventList);

        const std::vector<Event>& getEvents();
        const std::vector<Vertex>& getVertices() const;
        const std::vector<Edge>& getEdges() const;

        int check(int scenario);

};

inline std::ostream &operator<<(std::ostream &os, Solution s)
{
    for (Event e : s.getEvents()){
        os << e << std::endl;
    }
    return os;
}

#endif