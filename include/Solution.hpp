#ifndef SOLUTION
#define SOLUTION

#include "Instance.hpp"
#include "Event.hpp"

class Solution{
    private :
        Instance instance;
        std::vector<Event> eventList;
        std::vector<int> isValid = std::vector<int>(4,0);

    public :
        Solution(Instance instance, std::vector<Event> eventList);

        std::vector<Event> getEvents();

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