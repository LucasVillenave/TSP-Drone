#ifndef HEUR_CAS1_LEP
#define HEUR_CAS1_LEP

#include <vector>
#include <string>

#include "Solution/Solution.hpp"
#include "utils.hpp"

class Heur_Cas1_leP{
    public : 
        std::vector<int> m_ordre_clients;

        Heur_Cas1_leP()=default;
        Heur_Cas1_leP(std::vector<int> t_ordre_clients){this->m_ordre_clients = t_ordre_clients;}

        Solution solve(Instance t_instance);
};

#endif