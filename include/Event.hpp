#ifndef EVENT
#define EVENT

#include "Graph/Position.hpp"

class Event{
    private :
        Position pos1;
        Position pos2;
        int time = -1;
        int demandID = -1;
        int droneID = -1;

        //0 -> truck movement
        //1 -> truck arrival
        //2 -> drone lift off
        //3 -> drone pick up
        //4 -> truck delivery
        //5 -> drone delivery
        int eventType = -1;

    public : 
        Event()=default;
        Event(Position t_pos1, int t_time, int t_eventType, Position t_pos2 = Position(), int t_demandID = -1, int t_droneID = -1);

        int getTime() const;
        int getEventType() const;
        int getDemandID() const;
        int getDroneID() const;
        Position getPos1() const;
        Position getPos2() const;
};

inline std::ostream &operator<<(std::ostream &os, Event e)
{
    switch (e.getEventType()){
        case 0 :
            os << e.getTime() << " ; DEPLACEMENT VEHICULE DESTINATION ; (LAT : " << e.getPos1() << " : " << e.getPos2();
            break;

        case 1 :
            os << e.getTime() << " ; ARRIVEE VEHICULE ; " << e.getPos1();
            break;

        case 2 :
            os << e.getTime() << " ; LARGAGE DRONE " << e.getDroneID() << " POUR LIVRAISON COLIS ID : " << e.getDemandID() << " ; " << e.getPos1();
            break;

        case 3 :
            os << e.getTime() << " ; RECUPERATION DRONE " << e.getDroneID() << " ;" << e.getPos1();
            break;

        case 4 :
            os << e.getTime() << " ; LIVRAISON COLIS ID : " << e.getDemandID() << " ; " << e.getPos1();
            break;

        case 5 :
            os << e.getTime() << " ; LIVRAISON DRONE " << e.getDroneID() << " COLIS ID : " << e.getDemandID() << " ; " << e.getPos1();
            break;
        
        default :
            break;
    }
    return os;
}

#endif