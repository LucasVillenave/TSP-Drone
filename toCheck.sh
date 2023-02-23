#!/bin/bash

mkdir -p build
cd build

g++ -o toCheck.exe ../Mains/toCheck.cpp ../src/Instance.cpp ../src/Event.cpp ../src/parser.cpp ../src/utils.cpp ../src/Graph/Demand.cpp ../src/Graph/Edge.cpp ../src/Graph/Graph.cpp ../src/Graph/Position.cpp ../src/Graph/Vertex.cpp ../src/Solution/Solution.cpp ../src/Solution/SolutionParser.cpp -I ../include
chmod +x toCheck.exe

EXECUTABLE=./toCheck.exe

#the data path TO CHANGE
DATA=../Data/30/
#the solution file path TO CHANGE
INPUT=../results/solTSP30.txt

$EXECUTABLE $DATA $INPUT