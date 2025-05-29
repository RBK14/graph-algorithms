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
    config.simulation = false;
    config.representation = Representation::LIST;
    config.algorithms.push_back(Algorithm::PRIM);
    config.vertices = {80, 120, 160, 180, 220, 240, 280};
    config.densities = {0.25, 0.50, 0.99};
    config.read = false;
    config.testDataPath = nullptr;
    config.print = false;

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
                if (value == "simulation") config.simulation = true;
                else if (value == "test") config.simulation = false;
                else {
                    std::cerr << "[ERROR] Invalid value for mode: " << value << std::endl;
                    std::cout << "[INFO] Restoring default values..." << std::endl;
                }
            } else if (key == "representation") {
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
                config.densities.clear();
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
                if (value == "read") config.read = true;
                else if (value == "generate") config.read = false;
                else {
                    std::cerr << "[ERROR] Invalid value for test_mode: " << value << std::endl;
                    std::cout << "[INFO] Restoring default values..." << std::endl;
                }
            } else if (key == "test_data_path") {
                config.testDataPath = value;
            } else if (key == "test_print_graph") {
                if (value == "true") config.print = true;
                else if (value == "false") config.print = false;
                else {
                    std::cerr << "[ERROR] Invalid value for test_print_graph: " << value << std::endl;
                    std::cout << "[INFO] Restoring default values..." << std::endl;
                }
            }
        }
    }
    return config;
}

void ConfigParser::printConfig(const Config &config) {
    std::cout << "\n---------Application Configuration---------\n";

    std::cout << "simulation_mode = " << (config.simulation ? "simulation" : "test") << std::endl;
    std::cout << "representation = " << toString(config.representation) << std::endl;

    std::cout << "algorithms = ";
    for (const auto& alg : config.algorithms) {
        std::cout << toString(alg) << " ";
    }
    std::cout << std::endl;

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

    std::cout << "read_mode = " << (config.read ? "read" : "generate") << std::endl;
    std::cout << "test_data_path = " << config.testDataPath << std::endl;
    std::cout << "test_print_graph = " << (config.print ? "true" : "false") << std::endl;

    std::cout << "--------------------------------------------\n\n";
}

std::string ConfigParser::toString(const Representation repr) {
    switch (repr) {
        case Representation::LIST: return "Adjacency List";
        case Representation::MATRIX: return "Incidence Matrix";
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