#include "CommandLineParser.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

SimulationConfig::SimulationConfig(): seed(time(nullptr)), worldWidth(40), worldHeight(40), numMovingCars(3), numMovingBikes(4), numParkedCars(5), numStopSigns(2), numTrafficLights(2), simulationTicks(100), minConfidenceThreshold(0.4), showHelp(false), enableLogging(false), fullVisualization(false), noDelay(false) {}

void SimulationConfig::print() const {
    std::cout << "\n═══════════════════════════════════════════════\n";
    std::cout << "        SIMULATION SETTINGS\n";
    std::cout << "═══════════════════════════════════════════════\n";
    
    std::cout << "World: " << worldWidth << "x" << worldHeight << "\n";
    std::cout << "Seed: " << seed << "\n";
    std::cout << "\nObjects:\n";
    std::cout << "  Moving cars: " << numMovingCars << "\n";
    std::cout << "  Moving bikes: " << numMovingBikes << "\n";
    std::cout << "  Parked cars: " << numParkedCars << "\n";
    std::cout << "  STOP signs: " << numStopSigns << "\n";
    std::cout << "  Traffic lights: " << numTrafficLights << "\n";
    
    std::cout << "\nSimulation:\n";
    std::cout << "  Max ticks: " << simulationTicks << "\n";
    std::cout << "  Confidence threshold: " << minConfidenceThreshold << "\n";
    
    std::cout << "\nGPS targets (" << gpsTargets.size() << "):\n";
    for (size_t i = 0; i < gpsTargets.size(); i++) {
        std::cout << "  " << i+1 << ": (" << gpsTargets[i].getX() << ", " << gpsTargets[i].getY() << ")\n";
    }
    
    std::cout << "\nOptions:\n";
    std::cout << "  Logging: " << (enableLogging ? "ON" : "OFF") << "\n";
    std::cout << "  Full visualization: " << (fullVisualization ? "YES" : "NO") << "\n";
    std::cout << "  No delay: " << (noDelay ? "YES" : "NO") << "\n";
    
    std::cout << "═══════════════════════════════════════════════\n";
}

SimulationConfig CommandLineParser::parse(int argc, char* argv[]) {
    SimulationConfig config;
    
    // Ανάλυση arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--help") {
            config.showHelp = true;
            return config;
        }
        else if (arg == "--seed" && i + 1 < argc) {
            config.seed = std::stoi(argv[++i]);
        }
        else if (arg == "--dimX" && i + 1 < argc) {
            config.worldWidth = std::stoi(argv[++i]);
        }
        else if (arg == "--dimY" && i + 1 < argc) {
            config.worldHeight = std::stoi(argv[++i]);
        }
        else if (arg == "--numMovingCars" && i + 1 < argc) {
            config.numMovingCars = std::stoi(argv[++i]);
        }
        else if (arg == "--numMovingBikes" && i + 1 < argc) {
            config.numMovingBikes = std::stoi(argv[++i]);
        }
        else if (arg == "--numParkedCars" && i + 1 < argc) {
            config.numParkedCars = std::stoi(argv[++i]);
        }
        else if (arg == "--numStopSigns" && i + 1 < argc) {
            config.numStopSigns = std::stoi(argv[++i]);
        }
        else if (arg == "--numTrafficLights" && i + 1 < argc) {
            config.numTrafficLights = std::stoi(argv[++i]);
        }
        else if (arg == "--simulationTicks" && i + 1 < argc) {
            config.simulationTicks = std::stoi(argv[++i]);
        }
        else if (arg == "--minConfidenceThreshold" && i + 1 < argc) {
            config.minConfidenceThreshold = std::stod(argv[++i]);
        }
        else if (arg == "--enableLogging") {
            config.enableLogging = true;
        }
        else if (arg == "--fullVisualization") {
            config.fullVisualization = true;
        }
        else if (arg == "--noDelay") {
            config.noDelay = true;
        }
        else if (arg == "--gps") {
            // Ανάγνωση στόχων GPS (ζεύγη συντεταγμένων)
            i++;
            while (i + 1 < argc && isNumber(argv[i]) && isNumber(argv[i + 1])) {
                int x = std::stoi(argv[i]);
                int y = std::stoi(argv[i + 1]);
                config.gpsTargets.push_back(Position(x, y));
                i += 2;
            }
            i--;  // Επιστροφή μιας θέσης πίσω
        }
        else {
            std::cerr << "[WARNING] Unknown argument: " << arg << std::endl;
        }
    }
    
    // Έλεγχος για τουλάχιστον έναν στόχο GPS
    if (config.gpsTargets.empty()) {
        std::cerr << "[ERROR] At least one GPS target is required!\n";
        std::cerr << "Usage: --gps <x1> <y1> [x2 y2 ...]\n";
        exit(1);
    }
    
    return config;
}

void CommandLineParser::printHelp() {
    std::cout << "\nAUTONOMOUS VEHICLE - SIMULATION\n";
    std::cout << "Usage:\n";
    std::cout << "  ./oopproj_2025 [options]\n\n";
    
    std::cout << "Options:\n";
    std::cout << "  --seed <n>                Random seed (default: current time)\n";
    std::cout << "  --dimX <n>                World width (default: 40)\n";
    std::cout << "  --dimY <n>                World height (default: 40)\n";
    std::cout << "  --numMovingCars <n>       Number of moving cars (default: 3)\n";
    std::cout << "  --numMovingBikes <n>      Number of moving bikes (default: 4)\n";
    std::cout << "  --numParkedCars <n>       Number of parked cars (default: 5)\n";
    std::cout << "  --numStopSigns <n>        Number of STOP signs (default: 2)\n";
    std::cout << "  --numTrafficLights <n>    Number of traffic lights (default: 2)\n";
    std::cout << "  --simulationTicks <n>     Maximum number of ticks (default: 100)\n";
    std::cout << "  --minConfidenceThreshold <n> Minimum confidence threshold (default: 0.4)\n";
    std::cout << "  --enableLogging           Write a log file\n";
    std::cout << "  --fullVisualization       Print the full map on every tick\n";
    std::cout << "  --noDelay                 Run without the 0.5s pause between ticks\n";
    std::cout << "  --gps <x1> <y1> [x2 y2 ...] GPS targets (required)\n";
    std::cout << "  --help                    Show this message\n";
    
    std::cout << "\nExamples:\n";
    std::cout << "  ./oopproj_2025 --seed 123 --dimX 50 --dimY 50 --gps 10 20 30 40\n";
    std::cout << "  ./oopproj_2025 --numMovingCars 5 --numTrafficLights 3 --gps 5 5 15 15 25 25\n";
}

Position CommandLineParser::parsePosition(const std::string& xStr, const std::string& yStr) {
    int x = std::stoi(xStr);
    int y = std::stoi(yStr);
    return Position(x, y);
}

bool CommandLineParser::isNumber(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}