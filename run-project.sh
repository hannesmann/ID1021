#!/bin/bash
# Run with ./run-project.sh [project]

set -e
set -o pipefail

GCC_ARGS="-std=c++17 -O0 -g -Wall"

rm -R bin
mkdir -p bin
mkdir -p bin/objects

echo "Available:"

echo "  intro"
function intro {
	g++ src/intro/main.cpp -o bin/intro $GCC_ARGS $1 $2 $3 $4 $5

	time nice -n -5 bin/intro
}

echo "  calc [-D DEBUG]"
function calc {
	g++ -c src/calc/main.cpp -o bin/objects/main.o $GCC_ARGS $1 $2 $3 $4 $5
	g++ -c src/calc/stack.cpp -o bin/objects/stack.o $GCC_ARGS $1 $2 $3 $4 $5
	g++ -c src/calc/calc.cpp -o bin/objects/calc.o $GCC_ARGS $1 $2 $3 $4 $5

	g++ bin/objects/main.o bin/objects/stack.o bin/objects/calc.o -o bin/calc

	time nice -n -5 bin/calc
}

echo "  binary_search"
function binary_search {
	g++ -c src/binary_search/main.cpp -o bin/objects/main.o $GCC_ARGS $1 $2 $3 $4 $5
	g++ -c src/binary_search/search.cpp -o bin/objects/search.o $GCC_ARGS $1 $2 $3 $4 $5

	g++ bin/objects/main.o bin/objects/search.o -o bin/binary_search

	time nice -n -5 bin/binary_search
}

echo "  sort"
function sort {
	g++ -c src/sort/main.cpp -o bin/objects/main.o $GCC_ARGS $1 $2 $3 $4 $5
	g++ -c src/sort/algorithms.cpp -o bin/objects/algorithms.o $GCC_ARGS $1 $2 $3 $4 $5

	g++ bin/objects/main.o bin/objects/algorithms.o -o bin/sort

	time nice -n -5 bin/sort
}

echo "  linked_list"
function linked_list {
	g++ -c src/linked_list/main.cpp -o bin/objects/main.o $GCC_ARGS $1 $2 $3 $4 $5
	g++ -c src/linked_list/stack.cpp -o bin/objects/stack.o $GCC_ARGS $1 $2 $3 $4 $5

	g++ bin/objects/main.o bin/objects/stack.o -o bin/linked_list

	time nice -n -5 bin/linked_list
}

echo "  doubly_linked_list"
function doubly_linked_list {
	g++ -c src/doubly_linked_list/main.cpp -o bin/objects/main.o $GCC_ARGS $1 $2 $3 $4 $5

	g++ bin/objects/main.o -o bin/doubly_linked_list

	time nice -n -5 bin/doubly_linked_list
}

echo "  trees"
function trees {
	g++ -c src/trees/main.cpp -o bin/objects/main.o $GCC_ARGS $1 $2 $3 $4 $5
	g++ -c src/trees/tree.cpp -o bin/objects/tree.o $GCC_ARGS $1 $2 $3 $4 $5
	g++ -c src/trees/iterator.cpp -o bin/objects/iterator.o $GCC_ARGS $1 $2 $3 $4 $5

	g++ bin/objects/main.o bin/objects/tree.o bin/objects/iterator.o -o bin/trees

	time nice -n -5 bin/trees
}

echo "  queue"
function queue {
	g++ -c src/queue/main.cpp -o bin/objects/main.o $GCC_ARGS $1 $2 $3 $4 $5
	g++ -c src/queue/tree.cpp -o bin/objects/tree.o $GCC_ARGS $1 $2 $3 $4 $5
	g++ -c src/queue/iterator.cpp -o bin/objects/iterator.o $GCC_ARGS $1 $2 $3 $4 $5

	g++ bin/objects/main.o bin/objects/tree.o bin/objects/iterator.o -o bin/queue

	time nice -n -5 bin/queue
}

echo "  priority_queue"
function priority_queue {
	g++ -c src/priority_queue/main.cpp -o bin/objects/main.o $GCC_ARGS $1 $2 $3 $4 $5
	g++ -c src/priority_queue/list_queue.cpp -o bin/objects/list_queue.o $GCC_ARGS $1 $2 $3 $4 $5
	g++ -c src/priority_queue/heap_queue.cpp -o bin/objects/heap_queue.o $GCC_ARGS $1 $2 $3 $4 $5

	g++ bin/objects/main.o bin/objects/list_queue.o bin/objects/heap_queue.o -o bin/priority_queue

	time nice -n -5 bin/priority_queue
}

echo "  quicksort"
function quicksort {
	g++ -c src/quicksort/main.cpp -o bin/objects/main.o $GCC_ARGS $1 $2 $3 $4 $5

	g++ bin/objects/main.o -o bin/quicksort

	time nice -n -5 bin/quicksort
}

echo ""
echo "Trying to build and run $1"
echo ""
$1 $2 $3 $4 $5 $6