#include "SensorReading.hpp"
#include <iostream>

SensorReading::SensorReading() : sensorId(""), objectId(""), type(DETECTED_UNKNOWN), position(Position(0,0)), distance(0.0), confidence(0.0), speed(0.0), direction(""), signText(""), lightColor("") {}

void SensorReading::print() const {
    std::cout << "[Reading] Sensor: " << sensorId << ", Object: " << objectId << ", Type: ";
    
    switch (type) {
        case DETECTED_UNKNOWN: std::cout << "Unknown"; break;
        case DETECTED_STATIC: std::cout << "Static"; break;
        case DETECTED_MOVING_CAR: std::cout << "Moving Car"; break;
        case DETECTED_MOVING_BIKE: std::cout << "Moving Bike"; break;
        case DETECTED_PARKED_CAR: std::cout << "Parked Car"; break;
        case DETECTED_STOP_SIGN: std::cout << "STOP Sign"; break;
        case DETECTED_TRAFFIC_LIGHT: std::cout << "Traffic Light"; break;
    }
    
    std::cout << ", Position: (" << position.getX() << "," << position.getY() << "), Distance: " << distance << ", Confidence: " << confidence << std::endl;
}