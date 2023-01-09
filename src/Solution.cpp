#include "Solution.hpp"
#include <stdexcept>
#include <iostream>

Solution::Solution(Instance instance, std::vector<Event> eventList){
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