#ifndef COMMANDLINEPARSER_HPP
#define COMMANDLINEPARSER_HPP

#include "Position.hpp"
#include <vector>
#include <string>
#include <map>

// Δομή με όλες τις παραμέτρους προσομοίωσης
struct SimulationConfig {
    // Παράμετροι κόσμου
    int seed;                      // Seed για γεννήτρια τυχαίων αριθμών
    int worldWidth;                // Πλάτος κόσμου
    int worldHeight;               // Ύψος κόσμου
    
    // Αριθμός αντικειμένων
    int numMovingCars;             // Αριθμός κινούμενων αυτοκινήτων
    int numMovingBikes;            // Αριθμός κινούμενων ποδηλάτων
    int numParkedCars;             // Αριθμός σταθμευμένων αυτοκινήτων
    int numStopSigns;              // Αριθμός πινακίδων STOP
    int numTrafficLights;          // Αριθμός φανάριων
    
    // Παράμετροι προσομοίωσης
    int simulationTicks;           // Μέγιστος αριθμός βημάτων
    double minConfidenceThreshold; // Ελάχιστο όριο βεβαιότητας
    
    // Στόχοι GPS
    std::vector<Position> gpsTargets;  // Λίστα με στόχους
    
    // Flags
    bool showHelp;                 // Εμφάνιση βοήθειας
    bool enableLogging;            // Ενεργοποίηση logging
    bool fullVisualization;        // Πλήρης οπτικοποίηση σε κάθε βήμα
    bool noDelay;                  // Εκτέλεση χωρίς παύση ανάμεσα στα βήματα

    // Κατασκευαστής με προκαθορισμένες τιμές
    SimulationConfig();
    
    // Εκτυπώνει τις ρυθμίσεις
    void print() const;
};

// Υπεύθυνη για την ανάλυση των command-line arguments
class CommandLineParser {
public:
    // Ανάλυση των command-line arguments
    static SimulationConfig parse(int argc, char* argv[]);
    
    // Εμφανίζει το μήνυμα βοήθειας
    static void printHelp();
    
private:
    // Μετατροπή string σε Position
    static Position parsePosition(const std::string& xStr, const std::string& yStr);
    
    // Έλεγχος αν ένα string είναι αριθμός
    static bool isNumber(const std::string& str);
};

#endif