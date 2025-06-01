//
// Created by Maciej on 25.05.2025.
//


#include "config_parser.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

Config ConfigParser::readConfig(const std::string &configPath) {
    Config config;

    // Ustawienia domyślne
    config.isModeSimulation = false;
    config.representation = Representation::LIST;
    config.algorithms.push_back(Algorithm::BELLMAN_FORD);
    config.vertices = {80, 120, 160, 180, 220, 240, 280};
    config.densities = {0.25, 0.50, 0.99};
    config.isReadFromFile = false;
    config.testDataPath = "";
    config.isGraphPrinting = false;
    config.startVertex = 0;

    std::ifstream file(configPath);

    // Sprawdzanie, czy plik został poprawnie otwarty
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open config file at " << configPath << std::endl;
        return config; // Zwrócenie domyślnej konfiguracji
    }

    std::string line;

    std::cout << "[INFO] Loading configuration file..." << std::endl;

    while (std::getline(file, line)) {
        // Ignorowanie komentarzy w pliku .cfg
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value, '=')) {
            key.erase(std::ranges::remove_if(key, ::isspace).begin(), key.end());
            value.erase(std::ranges::remove_if(value, ::isspace).begin(), value.end());

            if (key == "mode") {
                if (value == "simulation") config.isModeSimulation = true;
                else if (value == "test") config.isModeSimulation = false;
                else {
                    std::cerr << "[ERROR] Invalid value for mode: " << value << std::endl;
                    std::cout << "[INFO] Restoring default values..." << std::endl;
                }
            } else if (key == "simulation_representation") {
                if (value == "matrix") config.representation = Representation::MATRIX;
                else if (value == "list") config.representation = Representation::LIST;
                else {
                    std::cerr << "[ERROR] Invalid value for representation: " << value << std::endl;
                    std::cout << "[INFO] Restoring default values..." << std::endl;
                }
            } else if (key == "algorithms") {
                std::istringstream sizeStream(value);
                std::string algorithm;
                config.algorithms.clear();
                while (std::getline(sizeStream, algorithm, ',')) {
                    if (algorithm == "prim") config.algorithms.push_back(Algorithm::PRIM);
                    else if (algorithm == "kruskal") config.algorithms.push_back(Algorithm::KRUSKAL);
                    else if (algorithm == "dijkstra") config.algorithms.push_back(Algorithm::DIJKSTRA);
                    else if (algorithm == "bellman-ford") config.algorithms.push_back(Algorithm::BELLMAN_FORD);
                    else {
                        std::cerr << "[ERROR] Invalid value for algorithms: " << algorithm << std::endl;
                        std::cout << "[INFO] Restoring default values..." << std::endl;
                    }
                }
            } else if (key == "vertices") {
                std::istringstream sizeStream(value);
                std::string size;
                config.vertices.clear();
                bool validSizes = true;
                while (std::getline(sizeStream, size, ',')) {
                    try {
                        int intSize = std::stoi(size);
                        config.vertices.push_back(intSize);
                    } catch (std::exception&) {
                        std::cerr << "[ERROR] Invalid value for vertices: " << size << std::endl;
                        validSizes = false;
                    }
                }
                if (!validSizes) {
                    std::cout << "[INFO] Restoring default values for vertices..." << std::endl;
                    config.vertices = {80, 120, 160, 180, 220, 240, 280}; // Przywrócenie wartości domyślnych
                }
            } else if (key == "densities") {
                std::istringstream densityStream(value);
                std::string density;
                config.densities.clear();
                bool validSizes = true;
                while (std::getline(densityStream, density, ',')) {
                    try {
                        double doubleDensity = std::stod(density);
                        config.densities.push_back(doubleDensity);
                    } catch (std::exception&) {
                        std::cerr << "[ERROR] Invalid value for densities: " << density << std::endl;
                        validSizes = false;
                    }
                }
                if (!validSizes) {
                    std::cout << "[INFO] Restoring default values for densities..." << std::endl;
                    config.densities = {0.25, 0.50, 0.99}; // Przywrócenie wartości domyślnych
                }
            } else if (key == "test_mode") {
                if (value == "read") config.isReadFromFile = true;
                else if (value == "generate") config.isReadFromFile = false;
                else {
                    std::cerr << "[ERROR] Invalid value for test_mode: " << value << std::endl;
                    std::cout << "[INFO] Restoring default values..." << std::endl;
                }
            } else if (key == "test_data_path") {
                config.testDataPath = value;
            } else if (key == "test_print_graph") {
                if (value == "true") config.isGraphPrinting = true;
                else if (value == "false") config.isGraphPrinting = false;
                else {
                    std::cerr << "[ERROR] Invalid value for test_print_graph: " << value << std::endl;
                    std::cout << "[INFO] Restoring default values..." << std::endl;
                }
            } else if (key == "test_sp_start_vertex") {
                try {
                    int startVertex = std::stoi(value);
                    int minVertex = *std::ranges::min_element(config.vertices);
                    if (startVertex >= 0 && startVertex < minVertex) {
                        config.startVertex = startVertex;
                    } else {
                        std::cerr << "[ERROR] Value for test_sp_start_vertex out of valid range [0, " << minVertex << ")" << std::endl;
                        std::cout << "[INFO] Restoring default value for startVertex..." << std::endl;
                    }
                } catch (std::exception&) {
                    std::cerr << "[ERROR] Invalid value for test_sp_start_vertex: " << value << std::endl;
                    std::cout << "[INFO] Restoring default value for startVertex..." << std::endl;
                }
            }
        }
    }
    return config;
}

void ConfigParser::printConfig(const Config &config) {
    std::cout << "\n---------APPLICATION PROPERTIES---------\n";

    std::cout << "mode = " << (config.isModeSimulation ? "simulation" : "test") << std::endl;
    if (config.isModeSimulation)
        std::cout << "simulation_representation = " << toString(config.representation) << std::endl;

    std::cout << "algorithms = ";
    for (const auto& alg : config.algorithms) {
        std::cout << toString(alg) << " ";
    }
    std::cout << std::endl;

    if (!config.isReadFromFile) {
        std::cout << "vertices = ";
        for (const int v : config.vertices) {
            std::cout << v << " ";
        }
        std::cout << std::endl;

        std::cout << "densities = ";
        for (const double d : config.densities) {
            std::cout << d << " ";
        }
        std::cout << std::endl;
    }

    if (!config.isModeSimulation) {
        std::cout << "test_mode = " << (config.isReadFromFile ? "read" : "generate") << std::endl;
        std::cout << "test_data_path = " << config.testDataPath << std::endl;
        std::cout << "test_print_graph = " << (config.isGraphPrinting ? "true" : "false") << std::endl;
        std::cout << "test_sp_start_vertex = " << config.startVertex << std::endl;
    }

    std::cout << "--------------------------------------------\n\n";
}

std::string ConfigParser::toString(const Representation repr) {
    switch (repr) {
        case Representation::LIST: return "list";
        case Representation::MATRIX: return "matrix";
        default: return "unknown";
    }
}

std::string ConfigParser::toString(const Algorithm alg) {
    switch (alg) {
        case Algorithm::PRIM: return "prim";
        case Algorithm::KRUSKAL: return "kruskal";
        case Algorithm::DIJKSTRA: return "dijkstra";
        case Algorithm::BELLMAN_FORD: return "bellman-ford";
        default: return "unknown";
    }
}