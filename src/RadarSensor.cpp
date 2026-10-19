#include "RadarSensor.hpp"
#include "GridWorld.hpp"
#include <cmath>

RadarSensor::RadarSensor(const std::string& id, const Position& pos, RadarDirection dir)
    : Sensor(id, "RADAR", 12, pos, 0.95), // Εμβέλεια 12, ακρίβεια 95%
      facingDirection(dir) {}

std::vector<SensorReading> RadarSensor::detect(const GridWorld* world) {
    std::vector<SensorReading> readings;
    
    int startX = position.getX();
    int startY = position.getY();
    
    // Προσδιορισμός κατεύθυνσης σάρωσης
    int deltaX = 0, deltaY = 0;
    switch (facingDirection) {
        case RADAR_NORTH: deltaY = -1; break;
        case RADAR_EAST:  deltaX = 1;  break;
        case RADAR_SOUTH: deltaY = 1;  break;
        case RADAR_WEST:  deltaX = -1; break;
    }
    
    // Σάρωση ευθεία μπροστά για range κελιά
    for (int i = 1; i <= range; i++) {
        int x = startX + (deltaX * i);
        int y = startY + (deltaY * i);
        
        auto objectPtr = world->getObjectAt(x, y);
        if (objectPtr != nullptr) {
            char glyph = objectPtr->getGlyph();
            
            // Το Radar ανιχνεύει μόνο κινούμενα αντικείμενα (C, B)
            if (glyph == 'C' || glyph == 'B') {
                SensorReading reading;
                reading.sensorId = id;
                reading.objectId = objectPtr->getId();
                reading.type = (glyph == 'C') ? DETECTED_MOVING_CAR : DETECTED_MOVING_BIKE;  // Αλλαγή
                reading.position = Position(x, y);
                reading.distance = i;  // Απόσταση σε κελιά
                reading.confidence = calculateConfidence(i);
                reading.speed = 1.0;  // Υποθέτουμε σταθερή ταχύτητα για απλότητα
                reading.direction = "FORWARD";  // Υποθέτουμε προς τα εμπρός
                
                readings.push_back(reading);
                
                // Το Radar βλέπει μόνο το πρώτο κινούμενο αντικείμενο
                break;
            }
        }
    }
    
    return readings;
}

void RadarSensor::setDirection(RadarDirection newDir) {
    facingDirection = newDir;
}