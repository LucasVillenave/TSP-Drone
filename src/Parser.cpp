#include "Parser.hpp"
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
//#include <algorithm>

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

Instance load(string instancePath, string instanceName){
    string line;
    fstream f;
    vector<string> arcs;
    vector<Edge> edges;

    vector<Position> positions;
    vector<Vertex> vertices;

    cout << "trying to load : " << instancePath << instanceName << endl;
    f.open(instancePath + instanceName);
    if (!f){
        cout<<"\ncouldn't load\n"<<endl;
    }else{
        cout<<"\nsuccessful load\n"<<endl;
    }

    getline(f,line);
    arcs = split(line,'{');

    for (int k=1; k<arcs.size(); ++k){
        string arc = arcs[k];
        vector<string> parameters = split(arc,':');
        vector<string> parameterNames;
        vector<string> value;

        int minIndex=-1;
        int maxIndex=-1;
        int ID = -1;
        int roadSpeed = -1;

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
                        int actualSize = positions.size();
                        maxIndex = addPosition(pos,positions);
                        if (positions.size()!=actualSize){
                            vertices.push_back(Vertex(positions[maxIndex]));
                        }
                    }
                }
            }

            if (parameterName == "lat_min"){
                for (int j=0; j<parameterNames.size(); ++j){
                    string ndParameterName = parameterNames[j];
                    if (ndParameterName == "lon_min"){
                        Position pos(stod(value[i]),stod(value[j]));
                        int actualSize = positions.size();
                        minIndex = addPosition(pos,positions);
                        if (positions.size()!=actualSize){
                            vertices.push_back(Vertex(positions[minIndex]));
                        }
                    }
                }
            }

            if (parameterName == "type"){
                if (value[i]=="primary"){
                    roadSpeed = 60;
                }else{
                    if (value[i]=="secondary"){
                        roadSpeed = 45;
                    }else{
                        roadSpeed = 30;
                    }
                }
            }
        }

        edges.push_back(Edge(vertices[minIndex],vertices[minIndex],roadSpeed,ID));
    }

    return Instance(Graph(vertices,edges),instanceName);
}


void write(Solution sol){
    //ToDo
}