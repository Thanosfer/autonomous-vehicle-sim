#include "MovingObject.hpp"
#include <iostream>
#include <cstdlib>  // για rand()
#include <ctime>    // για time()

MovingObject::MovingObject(const std::string& id, char glyph, const Position& pos, int speed, Direction dir): WorldObject(id, glyph, pos), speed(speed), direction(dir) {}

// @brief Ενημέρωση κινούμενου αντικειμένου
void MovingObject::update() {
    move();
    
    // 10% πιθανότητα να αλλάξει κατεύθυνση σε κάθε βήμα
    if (rand() % 10 == 0) {
        changeDirection();
    }
}

// Κινεί το αντικείμενο μία θέση
void MovingObject::move() {
    int x = position.getX();
    int y = position.getY();
    
    switch (direction) {
        case NORTH: y -= speed; break;
        case EAST:  x += speed; break;
        case SOUTH: y += speed; break;
        case WEST:  x -= speed; break;
    }
    
    position.setX(x);
    position.setY(y);
}

// Αλλάζει την κατεύθυνση τυχαία
void MovingObject::changeDirection() {
    direction = static_cast<Direction>(rand() % 4);
}

int MovingObject::getSpeed() const { return speed; }
Direction MovingObject::getDirection() const { return direction; }

std::string MovingObject::getDirectionString() const {
    switch (direction) {
        case NORTH: return "NORTH";
        case EAST:  return "EAST";
        case SOUTH: return "SOUTH";
        case WEST:  return "WEST";
        default:    return "UNKNOWN";
    }
}

std::string MovingObject::getInfo() const {
    std::string info = WorldObject::getInfo();
    return info + ", Type: Moving, Speed: " + std::to_string(speed) + ", Direction: " + getDirectionString();
}