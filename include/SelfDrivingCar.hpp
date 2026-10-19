#ifndef SELFDRIVINGCAR_HPP
#define SELFDRIVINGCAR_HPP

#include "WorldObject.hpp"
#include "Sensor.hpp"
#include "SensorFusionEngine.hpp"
#include "NavigationSystem.hpp"
#include <vector>
#include <memory>

// Forward declaration για το GridWorld
class GridWorld;

// Το αυτόνομο όχημα με αισθητήρες και σύστημα πλοήγησης
class SelfDrivingCar : public WorldObject {
private:
    std::vector<std::shared_ptr<Sensor>> sensors; // Λίστα με όλους τους αισθητήρες
    std::unique_ptr<SensorFusionEngine> fusionEngine; // Μηχανή συγχώνευσης
    std::unique_ptr<NavigationSystem> navigationSystem; // Σύστημα πλοήγησης
    
    int speed;                // Ταχύτητα οχήματος (0=STOPPED, 1=HALF, 2=FULL)
    std::string direction;    // Κατεύθυνση οχήματος
    
public:
    // Κατασκευαστής
    SelfDrivingCar(const std::string& id, const Position& pos, const std::vector<Position>& gpsTargets);
    
    // Ενημέρωση του οχήματος σε κάθε βήμα προσομοίωσης
    void update() override;
    
    // Προσθήκη αισθητήρα στο όχημα
    void addSensor(std::shared_ptr<Sensor> sensor);
    
    // Εκτέλεση εντολών από το σύστημα πλοήγησης
    void executeCommands(const std::vector<std::string>& commands);
    
    // Επιστρέφει πληροφορίες για το όχημα
    std::string getInfo() const override;
    
    // Εκτυπώνει στατιστικά για το όχημα
    void printCarStats() const;
    
    // Μέθοδος για την εκτέλεση ενός βήματος
    // Εκτέλεση ενός βήματος προσομοίωσης
    void performOneTick(GridWorld* world);
    
    // Επιστρέφει το σύστημα πλοήγησης
    NavigationSystem* getNavigationSystem() { return navigationSystem.get(); }
    
    // Μέθοδος για έλεγχο ορίων
    // Ελέγχει αν το όχημα είναι εντός ορίων
    bool isWithinBounds(int worldWidth, int worldHeight) const;
    
private:
    // Συλλογή δεδομένων από όλους τους αισθητήρες
    std::vector<std::vector<SensorReading>> collectSensorData(const GridWorld* world);
    
    // Μετακίνηση του οχήματος μία θέση προς την κατεύθυνσή του
    void moveOneStep();
    
    // Αλλαγή κατεύθυνσης του οχήματος
    void changeDirection(const std::string& newDir);
    
    // Αλλαγή ταχύτητας του οχήματος
    void changeSpeed(int newSpeed);
};

#endif