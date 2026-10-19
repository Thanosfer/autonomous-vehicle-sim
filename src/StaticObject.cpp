#include "StaticObject.hpp"
#include <iostream>

StaticObject::StaticObject(const std::string& id, char glyph, const Position& pos, const std::string& desc): WorldObject(id, glyph, pos), description(desc) {}

// @brief Ενημέρωση στατικού αντικειμένου
void StaticObject::update() {
    // Στατικά αντικείμενα συνήθως δεν αλλάζουν
    // Αλλά εδώ μπορούμε να προσθέσουμε λογική για φανάρια κλπ
    // std::cout << "[STATIC] " << id << " δεν απαιτεί ενημέρωση\n";
}

std::string StaticObject::getDescription() const { return description; }

std::string StaticObject::getInfo() const {
    std::string info = WorldObject::getInfo();
    return info + ", Type: Static, Desc: " + description;
}