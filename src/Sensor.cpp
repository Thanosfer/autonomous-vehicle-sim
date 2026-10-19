#include "Sensor.hpp"
#include "GridWorld.hpp"
#include <cmath>

Sensor::Sensor(const std::string& id, const std::string& type, int range, const Position& pos, double accuracy) : id(id), type(type), range(range), position(pos), baseAccuracy(accuracy) {}

double Sensor::calculateConfidence(double distance) const {
    // Αν η απόσταση είναι μεγαλύτερη από την εμβέλεια, βεβαιότητα 0
    if (distance > range) {
        return 0.0;
    }
    
    // Γραμμική μείωση βάσει απόστασης
    double distanceFactor = 1.0 - (distance / range);
    
    // Προσθήκη τυχαίας μεταβολής ±0.05
    double randomNoise = ((rand() % 100) / 1000.0) - 0.05; // -0.05 έως +0.05
    
    // Τελική βεβαιότητα
    double confidence = baseAccuracy * distanceFactor + randomNoise;
    
    // Περικοπή στο εύρος [0.0, 1.0]
    if (confidence < 0.0) confidence = 0.0;
    if (confidence > 1.0) confidence = 1.0;
    
    return confidence;
}

std::string Sensor::getId() const { return id; }
std::string Sensor::getType() const { return type; }
int Sensor::getRange() const { return range; }
Position Sensor::getPosition() const { return position; }
double Sensor::getBaseAccuracy() const { return baseAccuracy; }

void Sensor::setPosition(const Position& newPos) {
    position = newPos;
}