#include "LidarSensor.hpp"
#include "GridWorld.hpp"
#include <cmath>

LidarSensor::LidarSensor(const std::string& id, const Position& pos) : Sensor(id, "LIDAR", 9, pos, 0.99) {} // Εμβέλεια 9, ακρίβεια 99%

std::vector<SensorReading> LidarSensor::detect(const GridWorld* world) {
    std::vector<SensorReading> readings;
    
    int centerX = position.getX();
    int centerY = position.getY();
    
    // Οπτικό πεδίο: τετράγωνο 9x9 γύρω από το αισθητήρα
    for (int y = centerY - 4; y <= centerY + 4; y++) {
        for (int x = centerX - 4; x <= centerX + 4; x++) {
            // Παράβλεψε το κεντρικό κελί (το ίδιο το όχημα)
            if (x == centerX && y == centerY) continue;
            
            // Υπολόγισε απόσταση Manhattan
            double distance = abs(x - centerX) + abs(y - centerY);
            
            // Εάν η απόσταση είναι μέσα στην εμβέλεια
            if (distance <= range) {
                // Ζήτησε από τον κόσμο το αντικείμενο σε αυτή τη θέση
                auto objectPtr = world->getObjectAt(x, y);
                if (objectPtr != nullptr) {
                    SensorReading reading;
                    reading.sensorId = id;
                    reading.objectId = objectPtr->getId();
                    reading.type = glyphToObjectType(objectPtr->getGlyph());
                    reading.position = Position(x, y);
                    reading.distance = distance;
                    reading.confidence = calculateConfidence(distance);
                    reading.speed = 0.0;  // Το Lidar δεν μετρά ταχύτητα
                    reading.direction = "";
                    
                    readings.push_back(reading);
                }
            }
        }
    }
    
    return readings;
}

DetectedObjectType LidarSensor::glyphToObjectType(char glyph) const {
    switch (glyph) {
        case 'C': return DETECTED_MOVING_CAR;
        case 'B': return DETECTED_MOVING_BIKE;
        case 'P': return DETECTED_PARKED_CAR;
        case 'S': return DETECTED_STOP_SIGN;
        case 'R':
        case 'Y':
        case 'G': return DETECTED_TRAFFIC_LIGHT;
        default: return DETECTED_STATIC;
    }
}