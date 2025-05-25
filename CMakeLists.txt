cmake_minimum_required(VERSION 3.30)
project(aizo_proj_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(aizo_proj_2 main.cpp
        model/graph.cpp
        model/graph.h
        utility/graph_creator.cpp
        utility/graph_creator.h
        algorithm/kruskal.cpp
        algorithm/kruskal.h
        model/dsu.cpp
        model/dsu.h
        model/min_heap.cpp
        model/min_heap.h
        algorithm/prim.cpp
        algorithm/prim.h
        algorithm/dijkstra.cpp
        algorithm/dijkstra.h
        algorithm/bellman_ford.cpp
        algorithm/bellman_ford.h
        utility/result_parser.cpp
        utility/result_parser.h
        utility/config_parser.cpp
        utility/config_parser.h
)
