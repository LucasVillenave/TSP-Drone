#ifndef SOLUTION
#define SOLUTION

#include <stdexcept>
#include <iostream>
#include <utility>
#include "Instance.hpp"
#include "Event.hpp"

class Solution{
    private :
        Instance instance;
        std::vector<Event> eventList;
        std::vector<int> isValid = std::vector<int>(4,0);

    public :
        Solution(Instance t_instance, const std::vector<Event>& t_eventList);

        const std::vector<Event>& getEvents() const;
        const std::vector<Vertex>& getVertices() const;
        const std::vector<Edge>& getEdges() const;
        const std::vector<Demand>& getDemands() const;

        int getIsValid(unsigned int scenar) const;
        std::vector<int> getIsValid() const;

        void check();
        int checkTruck();
        int checkDrones();
        int checkDemandSatisfaction();
        void checkScenarsSpecifics();

};

inline std::ostream &operator<<(std::ostream &os, Solution s)
{
    os << "TEMPS ; EVENEMENT ; LOCALISATION" << std::endl;
    for (Event e : s.getEvents()){
        os << e << std::endl;
    }
    return os;
}

#endif