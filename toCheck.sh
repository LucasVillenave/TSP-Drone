#!/bin/bash

mkdir -p build
cd build

g++ -o toCheck.exe ../Mains/toCheck.cpp ../src/Instance.cpp ../src/Event.cpp ../src/parser.cpp ../src/utils.cpp ../src/Graph/Demand.cpp ../src/Graph/Edge.cpp ../src/Graph/Graph.cpp ../src/Graph/Position.cpp ../src/Graph/Vertex.cpp ../src/Solution/Solution.cpp ../src/Solution/SolutionParser.cpp -I ../include
chmod +x toCheck.exe

cd ..

EXECUTABLE=./build/toCheck.exe
DATA=$1
INPUT=$2

$EXECUTABLE $DATA $INPUT
