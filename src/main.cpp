#include <iostream>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "GridWorld.hpp"
#include "StaticObject.hpp"
#include "MovingObject.hpp"
#include "TrafficLight.hpp"
#include "TrafficSign.hpp"
#include "SelfDrivingCar.hpp"
#include "Visualization.hpp"
#include "CommandLineParser.hpp"

using namespace std;

// Κύριο πρόγραμμα προσομοίωσης αυτόνομου οχήματος
int main(int argc, char* argv[]) {
    // Ανάλυση command-line arguments
    SimulationConfig config = CommandLineParser::parse(argc, argv);
    
    if (config.showHelp) {
        CommandLineParser::printHelp();
        return 0;
    }
    
    // Εμφάνιση ρυθμίσεων
    config.print();
    
    // Αρχικοποίηση γεννήτριας τυχαίων αριθμών
    srand(config.seed);
    
    cout << "\n═══════════════════════════════════════════════\n";
    cout << "    AUTONOMOUS VEHICLE - SIMULATION\n";
    cout << "═══════════════════════════════════════════════\n\n";
    
    // Δημιουργία κόσμου
    GridWorld world(config.worldWidth, config.worldHeight);
    
    // Δημιουργία τυχαίων αντικειμένων
    world.generateRandomObjects(config);
    
    // Δημιουργία αυτόνομου οχήματος
    // Τοποθετούμε το όχημα στο κέντρο του κόσμου
    Position startPosition(config.worldWidth / 2, config.worldHeight / 2);
    auto selfDrivingCar = std::make_shared<SelfDrivingCar>("AUTONOMOUS_CAR", startPosition, config.gpsTargets);
    // Προσθήκη του οχήματος στον κόσμο
    world.addObject(selfDrivingCar);
    
    // Εμφάνιση αρχικής κατάστασης
    cout << "\n--- INITIAL STATE ---\n";
    Visualization::renderFull(world);
    Visualization::renderPOV(world, *selfDrivingCar, 5, true);
    
    // Αρχείο log (προαιρετικό)
    ofstream logFile;
    if (config.enableLogging) {
        logFile.open("oopproj_2025.log");
        if (logFile.is_open()) {
            logFile << "=== SIMULATION START ===\n";
            logFile << "Seed: " << config.seed << "\n";
            logFile << "Size: " << config.worldWidth << "x" << config.worldHeight << "\n";
            logFile << "Ticks: " << config.simulationTicks << "\n";
            logFile << "GPS targets: " << config.gpsTargets.size() << "\n\n";
        }
    }
    
    // Κύριος βρόχος προσομοίωσης
    cout << "\n--- STARTING SIMULATION ---\n";
    
    bool simulationEnded = false;
    
    for (int tick = 1; tick <= config.simulationTicks && !simulationEnded; tick++) {
        Visualization::printSimulationStatus(tick, config.simulationTicks, *selfDrivingCar);
        
        // Ενημέρωση όλων των αντικειμένων (κινούμενα, φανάρια, κλπ.)
        world.updateAll();
        
        // Εκτέλεση ενός tick για το αυτόνομο όχημα
        selfDrivingCar->performOneTick(&world);
        
        // Ενημέρωση θέσης οχήματος στον κόσμο
        world.updateCarPosition(selfDrivingCar);
        
        // Οπτικοποίηση
        if (config.fullVisualization) {
            Visualization::renderFull(world);
        }
        Visualization::renderPOV(world, *selfDrivingCar, 5, true);
        
        // Καταγραφή σε αρχείο
        if (config.enableLogging && logFile.is_open()) {
            Position pos = selfDrivingCar->getPosition();
            logFile << "Tick " << tick << ": Position (" << pos.getX() << "," << pos.getY() << ")\n";
        }
        
        // Έλεγχος συνθηκών τερματισμού
        Position carPos = selfDrivingCar->getPosition();
        if (carPos.getX() < 0 || carPos.getX() >= config.worldWidth ||
            carPos.getY() < 0 || carPos.getY() >= config.worldHeight) {
            
            Visualization::printMessage("The car left the grid! Ending simulation.", "WARNING");
            simulationEnded = true;
        }
        
        if (selfDrivingCar->getNavigationSystem()->destinationReached()) {
            Visualization::printMessage("The car reached the final target!", "INFO");
            simulationEnded = true;
        }
        
        // Μικρή παύση για καλύτερη παρακολούθηση (παραλείπεται με --noDelay)
        if (!config.noDelay) {
            #ifdef _WIN32
                system("timeout 1 > nul");
            #else
                system("sleep 0.5");
            #endif
        }
    }
    
    // Τερματισμός προσομοίωσης
    cout << "\n--- END OF SIMULATION ---\n";
    
    // Εμφάνιση τελικής κατάστασης
    Visualization::renderFull(world);
    
    // Στατιστικά
    cout << "\n=== SIMULATION STATS ===\n";
    cout << "Total ticks: " << min(config.simulationTicks, config.simulationTicks) << endl;
    cout << "Final car position: (" << selfDrivingCar->getPosition().getX() << ", "<< selfDrivingCar->getPosition().getY() << ")" << endl;
    cout << "Objects left: " << world.getObjectCount() << endl;
    
    selfDrivingCar->printCarStats();
    
    // Εμφάνιση κατάστασης πλοήγησης 
    cout << "\n=== NAVIGATION STATUS ===\n";
    selfDrivingCar->getNavigationSystem()->printStatus();
    
    // Κλείσιμο αρχείου log
    if (config.enableLogging && logFile.is_open()) {
        logFile << "\n=== SIMULATION END ===\n";
        logFile.close();
        cout << "Log saved to: oopproj_2025.log" << endl;
    }
    
    cout << "\n═══════════════════════════════════════════════\n";
    cout << "    SIMULATION COMPLETED SUCCESSFULLY!\n";
    cout << "═══════════════════════════════════════════════\n";
    
    return 0;
}