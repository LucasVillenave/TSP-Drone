#include "Solution.hpp"
#include <stdexcept>
#include <iostream>

Solution::Solution(Instance instance, std::vector<Event> eventList){
    int time = 0;
    for (Event e : eventList){
        if (time>e.getTime()){
            throw std::invalid_argument("event list given in solution creation isn't timely ordered");
        }
        time = e.getTime();
    }
    
    this->instance = instance;
    this->eventList = eventList;
}

int Solution::check(int scenario){
    if (scenario>3 || scenario<0){
        std::cout << "scenario should be between 0 and 3, " << scenario << " was given" << std::endl;;
        throw std::invalid_argument("");
    }
    //Todo
    return isValid[scenario];
}

std::vector<Event> Solution::getEvents(){
    return eventList;
}