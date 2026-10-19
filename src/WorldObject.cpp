#include "WorldObject.hpp"
#include <sstream>

WorldObject::WorldObject(const std::string& id, char glyph, const Position& pos) : id(id), glyph(glyph), position(pos) {}

std::string WorldObject::getId() const { return id; }
char WorldObject::getGlyph() const { return glyph; }  // Βασική υλοποίηση
Position WorldObject::getPosition() const { return position; }

void WorldObject::setPosition(const Position& pos) { position = pos; }

std::string WorldObject::getInfo() const {
    std::stringstream ss;
    ss << "ID: " << id << ", Glyph: " << glyph << ", Position: (" << position.getX() << "," << position.getY() << ")";
    return ss.str();
}