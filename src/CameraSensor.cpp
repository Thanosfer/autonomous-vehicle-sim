#include "CameraSensor.hpp"
#include "GridWorld.hpp"

CameraSensor::CameraSensor(const std::string& id, const Position& pos)
    : Sensor(id, "CAMERA", 7, pos, 0.87), // Εμβέλεια 7, ακρίβεια 87%
      facingDirection("NORTH") {}

std::vector<SensorReading> CameraSensor::detect(const GridWorld* world) {
    std::vector<SensorReading> readings;
    
    int centerX = position.getX();
    int centerY = position.getY();
    
    // Οπτικό πεδίο: τετράγωνο 7x7 μπροστά από την Camera
    // (Υποθέτουμε ότι η Camera κοιτάει προς τα πάνω)
    for (int y = centerY - 3; y <= centerY + 3; y++) {
        for (int x = centerX - 3; x <= centerX + 3; x++) {
            // Παράβλεψε το κεντρικό κελί
            if (x == centerX && y == centerY) continue;
            
            // Υπολόγισε απόσταση Manhattan
            double distance = abs(x - centerX) + abs(y - centerY);
            
            // Εάν η απόσταση είναι μέσα στην εμβέλεια
            if (distance <= range) {
                auto objectPtr = world->getObjectAt(x, y);
                if (objectPtr != nullptr) {
                    char glyph = objectPtr->getGlyph();
                    
                    SensorReading reading;
                    reading.sensorId = id;
                    reading.objectId = objectPtr->getId();
                    
                    // Αναγνώριση τύπου από το glyph
                    switch (glyph) {
                        case 'C': reading.type = DETECTED_MOVING_CAR; break;
                        case 'B': reading.type = DETECTED_MOVING_BIKE; break;
                        case 'P': reading.type = DETECTED_PARKED_CAR; break;
                        case 'S': reading.type = DETECTED_STOP_SIGN; break;
                        case 'R':
                        case 'Y':
                        case 'G': reading.type = DETECTED_TRAFFIC_LIGHT; break;
                        default: reading.type = DETECTED_STATIC; break;
                    }
                    
                    reading.position = Position(x, y);
                    reading.distance = distance;
                    reading.confidence = calculateConfidence(distance);
                    reading.speed = 0.0;  // Η Camera δεν μετρά ταχύτητα με ακρίβεια
                    
                    // Ειδικές λειτουργίες Camera
                    reading.lightColor = detectLightColor(glyph);
                    reading.signText = detectSignText(glyph);
                    
                    readings.push_back(reading);
                }
            }
        }
    }
    
    return readings;
}

std::string CameraSensor::detectLightColor(char glyph) const {
    switch (glyph) {
        case 'R': return "RED";
        case 'Y': return "YELLOW";
        case 'G': return "GREEN";
        default: return "";
    }
}

std::string CameraSensor::detectSignText(char glyph) const {
    switch (glyph) {
        case 'S': return "STOP";
        default: return "";
    }
}