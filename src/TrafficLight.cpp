#include "TrafficLight.hpp"
#include <iostream>

TrafficLight::TrafficLight(const std::string& id, const Position& pos, int red, int green, int yellow)
    : StaticObject(id, 'R', pos, "Traffic Light"),  // Αρχικά κόκκινο
      currentColor(RED), cycleTimer(0),
      redDuration(red), yellowDuration(yellow), greenDuration(green) {  
    
    std::cout << "[+] Created traffic light " << id << " at (" << pos.getX() << "," << pos.getY() << "), initial colour: RED" << std::endl;
}

void TrafficLight::update() {
    cycleTimer++;
    
    // Έλεγχος αν πρέπει να αλλάξει χρώμα
    switch (currentColor) {
        case RED:
            if (cycleTimer >= redDuration) {
                changeColor();
            }
            break;
        case GREEN:
            if (cycleTimer >= greenDuration) {
                changeColor();
            }
            break;
        case YELLOW:
            if (cycleTimer >= yellowDuration) {
                changeColor();
            }
            break;
    }
}

void TrafficLight::changeColor() {
    cycleTimer = 0;
    
    switch (currentColor) {
        case RED:
            currentColor = GREEN;
            std::cout << "[LIGHT] " << id << " changed to GREEN" << std::endl;
            break;
        case GREEN:
            currentColor = YELLOW;
            std::cout << "[LIGHT] " << id << " changed to YELLOW" << std::endl;
            break;
        case YELLOW:
            currentColor = RED;
            std::cout << "[LIGHT] " << id << " changed to RED" << std::endl;
            break;
    }
}

std::string TrafficLight::getColorString() const {
    switch (currentColor) {
        case RED: return "RED";
        case YELLOW: return "YELLOW";
        case GREEN: return "GREEN";
        default: return "UNKNOWN";
    }
}

char TrafficLight::getGlyph() const {
    switch (currentColor) {
        case RED: return 'R';
        case YELLOW: return 'Y';
        case GREEN: return 'G';
        default: return '?';
    }
}

bool TrafficLight::affectsMovement() const {
    // Το φανάρι επηρεάζει την κίνηση αν είναι κόκκινο ή κίτρινο
    return currentColor == RED || currentColor == YELLOW;
}

std::string TrafficLight::getInfo() const {
    std::string info = StaticObject::getInfo();
    info += ", Colour: " + getColorString();
    info += ", Timer: " + std::to_string(cycleTimer);
    return info;
}