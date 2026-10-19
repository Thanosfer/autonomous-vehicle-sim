#ifndef SENSOR_HPP
#define SENSOR_HPP

#include "SensorReading.hpp"
#include "Position.hpp"
#include <vector>
#include <memory>

// Προσθήκη forward declaration για το GridWorld
class GridWorld;

// Βασική αφηρημένη κλάση για όλους τους αισθητήρες
class Sensor {
protected:
    std::string id;         // Μοναδικό ID αισθητήρα
    std::string type;       // Τύπος αισθητήρα (Lidar, Radar, Camera)
    int range;              // Εμβέλεια αισθητήρα (σε κελιά)
    Position position;      // Θέση του αισθητήρα στον κόσμο
    double baseAccuracy;    // Βασική ακρίβεια (0.0-1.0)
    
public:
    // Κατασκευαστής
    Sensor(const std::string& id, const std::string& type, 
           int range, const Position& pos, double accuracy);
    
    // Εικονικός καταστροφέας
    virtual ~Sensor() {}
    
    // Εικονική συνάρτηση ανίχνευσης
    virtual std::vector<SensorReading> detect(const GridWorld* world) = 0;
    
    // Υπολογίζει την βεβαιότητα ανίχνευσης βάσει απόστασης
    virtual double calculateConfidence(double distance) const;
    
    // Getter methods
    std::string getId() const;
    std::string getType() const;
    int getRange() const;
    Position getPosition() const;
    double getBaseAccuracy() const;
    
    // Ενημερώνει τη θέση του αισθητήρα
    void setPosition(const Position& newPos);
};

#endif