#ifndef VISUALIZATION_HPP
#define VISUALIZATION_HPP

#include "GridWorld.hpp"
#include "SelfDrivingCar.hpp"
#include <vector>
#include <string>

// Υπεύθυνη για την οπτικοποίηση της προσομοίωσης
class Visualization {
public:
    // Πλήρης οπτικοποίηση του κόσμου
    static void renderFull(const GridWorld& world);
    
    // Μερική οπτικοποίηση γύρω από το όχημα
    static void renderPOV(const GridWorld& world, const SelfDrivingCar& car, int radius = 5, bool centered = true);
    
    // Εμφανίζει τη λεζάντα των συμβόλων
    static void printLegend();
    
    // Εμφανίζει μήνυμα εξόδου με μορφοποίηση
    static void printMessage(const std::string& message, const std::string& level = "INFO");
    
    // Εμφανίζει την κατάσταση της προσομοίωσης
    static void printSimulationStatus(int tick, int maxTicks, const SelfDrivingCar& car);
};

#endif