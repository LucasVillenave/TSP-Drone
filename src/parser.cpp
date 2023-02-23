#include "parser.hpp"
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

vector<string> split(string s, char delim) {
    vector<string> words;
    stringstream ss(s);
    string word;
    while (getline(ss, word, delim)) {
        if (word!=""){
            words.push_back(word);
        }
    }
    return words;
}

double getLatMin(std::vector<Position> & positions){
    double lat_min = 90;

    for (Position p : positions){
        if (p.getLatitude()<lat_min){
            lat_min = p.getLatitude();
        }
    }

    return lat_min;
}

double getLonMin(std::vector<Position> & positions){
    double lon_min = 180;

    for (Position p : positions){
        if (p.getLongitude()<lon_min){
            lon_min = p.getLongitude();
        }
    }

    return lon_min;
}

void updatePos(std::vector<Position> & positions, double lat_min, double lon_min){
    for (int i=0; i<positions.size(); ++i){
        Position& p = positions[i];
        p.setY((p.getLatitude() - lat_min) * 10000);
        p.setX((p.getLongitude() - lon_min) * 10000);
    }
}

//add without copying elements, return the index of added element in the list
int addPosition(Position pos, vector<Position>& positions){
    int trigger = -1;
    for (int i=0; i<positions.size(); ++i){
        Position p = positions[i];
        if (p==pos){
            trigger=i;
        }
    }
    if (trigger==-1){
        trigger = positions.size();
        positions.push_back(pos);
    }
    return trigger;
}

Graph loadGraph(const string& instancePath, const string& instanceName){
    string line;
    fstream f;
    vector<string> arcs;
    vector<Edge> edges;

    vector<Position> positions;
    vector<Vertex> vertices;

    int isFirst = 1;

    cout << "trying to load : " << instancePath << instanceName << "/Graph.txt" << endl;
    f.open(instancePath + instanceName + "/Graph.txt");
    if (!f){
        throw std::invalid_argument("Couldn't load Graph.txt");
    }else{
        cout<<"\nsuccessful load\n"<<endl;
    }

    while(getline(f,line)){
        arcs = split(line,'{');
        for (int k=isFirst; k<arcs.size(); ++k){
            isFirst=0;
            string arc = arcs[k];
            vector<string> parameters = split(arc,':');
            vector<string> parameterNames;
            vector<string> value;

            int minIndex=-1;
            int maxIndex=-1;
            double length=-1;
            int ID = -1;
            std::string roadType;

            parameterNames.push_back(split(parameters[0],'"')[0]);
            for (int i=1; i<(parameters.size()-1); ++i){
                parameterNames.push_back(split(parameters[i],'"')[1]);
            }

            for (int i=1; i<(parameters.size()-1); ++i){
                value.push_back(split(parameters[i],',')[0]);
            }
            value.push_back(split(parameters[parameters.size()-1],'}')[0]);

            for (int i=0; i<parameterNames.size(); ++i){
                string parameterName = parameterNames[i];

                if (parameterName == "lat_max"){
                    for (int j=0; j<parameterNames.size(); ++j){
                        string ndParameterName = parameterNames[j];
                        if (ndParameterName == "lon_max"){
                            Position pos(stod(value[i]),stod(value[j]));
                            maxIndex = addPosition(pos,positions);
                        }
                    }
                }

                if (parameterName == "lat_min"){
                    for (int j=0; j<parameterNames.size(); ++j){
                        string ndParameterName = parameterNames[j];
                        if (ndParameterName == "lon_min"){
                            Position pos(stod(value[i]),stod(value[j]));
                            minIndex = addPosition(pos,positions);
                        }
                    }
                }

                if (parameterName == "type"){
                    roadType = value[i];
                    roadType = roadType.substr(1, roadType.size()-2);
                }

                if (parameterName == "length"){
                    length = stod(value[i]);
                }
            }

            edges.emplace_back(minIndex,maxIndex,length,roadType,ID);
        }
    }
    for (int i=0; i<edges.size();++i){
        edges[i].setID(i);
    }

    for (int i=0; i<positions.size();++i){
        vertices.emplace_back(positions[i],std::vector<Demand>(),i);
    }
    return {vertices,edges};
}

vector<Demand> loadDemands(const string& instancePath, const string& instanceName){
    string line;
    fstream f;
    vector<Demand> demands;

    cout << "trying to load : " << instancePath << instanceName << "/Demand.txt" << endl;
    f.open(instancePath + instanceName + "/Demand.txt");
    if (!f){
        throw std::invalid_argument("Couldn't load Demand.txt");
    }else{
        cout<<"\nsuccessful load\n"<<endl;
    }

    getline(f,line);
    while(line!=""){
        vector<string> parameters = split(line,':');
        vector<string> parametersNames;
        vector<string> value;

        double longitude = -1;
        double latitude = -1;
        int amount = -1;

        parametersNames.push_back(split(parameters[0],'"')[1]);
        for (int i=1; i<(parameters.size()-1); ++i){
            parametersNames.push_back(split(parameters[i],'"')[1]);
            value.push_back(split(parameters[i],',')[0]);
        }
        value.push_back(split(parameters[parameters.size()-1],',')[0]);

        for (int i=0; i<parametersNames.size(); ++i){
            string parameterName = parametersNames[i];
            if (parameterName == "amount"){
                amount = stoi(value[i]);
            }

            if (parameterName == "lat"){
                latitude = stod(value[i]);
            }

            if (parameterName == "lon"){
                longitude = stod(value[i]);
            }
        }
        bool alreadyExisting = false;
        for(Demand& demand : demands)
        {
            if(demand.getLatitude() == latitude && demand.getLongitude() == longitude)
            {
                alreadyExisting = true;
                demand.setAmount(demand.getAmount() + amount);
            }
        }
        if(!alreadyExisting)
            demands.emplace_back(Position(latitude,longitude),amount);

        getline(f,line);
    }

    for (int i=0; i<demands.size();++i){
        demands[i].setID(i);
    }

    return demands;
}

Instance load(const string& instancePath, const string& instanceName){
    
    Graph g = loadGraph(instancePath, instanceName);
    vector<Demand> demands = loadDemands(instancePath, instanceName);

    std::vector<Position> verticesPos;
    std::vector<Position> demandPos;

    for (Vertex v : g.getVertices()){
        verticesPos.push_back(v.getPos());
    }

    for (Demand d : demands){
        demandPos.push_back(d.getPos());
    }

    updatePos(demandPos,getLatMin(verticesPos),getLonMin(verticesPos));

    updatePos(verticesPos,getLatMin(verticesPos),getLonMin(verticesPos));

    for (int i=0; i<demands.size();++i){
        Demand& d = demands[i];
        d.setY(demandPos[i].getY());
        d.setX(demandPos[i].getX());
    }

    std::vector<Vertex> vertices;
    for (int i=0; i<g.getVertices().size();++i){
        vertices.emplace_back(
                verticesPos[i],
                std::vector<Demand>(),
                i
            );
    }
    g = Graph(vertices,g.getEdges());
    g.addDemands(demands);

    return {g,instanceName};
}