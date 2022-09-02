#!/bin/bash
# Run with ./run-project.sh [project]

GCC_ARGS="-std=c++14 -O0 -g"

mkdir -p bin
mkdir -p bin/objects

echo "Available:"

echo "  intro"
function intro {
	g++ src/intro.cpp -o bin/intro $GCC_ARGS $1 $2 $3 $4 $5

	bin/intro
}

echo "  calc [-D DEBUG]"
function calc {
	g++ -c src/calc/main.cpp -o bin/objects/main.o $GCC_ARGS $1 $2 $3 $4 $5
	g++ -c src/calc/stack.cpp -o bin/objects/stack.o $GCC_ARGS $1 $2 $3 $4 $5
	g++ -c src/calc/calc.cpp -o bin/objects/calc.o $GCC_ARGS $1 $2 $3 $4 $5

	g++ bin/objects/main.o bin/objects/stack.o bin/objects/calc.o -o bin/calc

	bin/calc
}

echo ""
echo "Trying to build and run $1"
echo ""
$1 $2 $3 $4 $5 $6 || { echo ""; echo "Failed"; exit 1; }