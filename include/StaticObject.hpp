#ifndef STATICOBJECT_HPP
#define STATICOBJECT_HPP

#include "WorldObject.hpp"
#include <string>

// Αντιπροσωπεύει στατικά αντικείμενα που δεν κινούνται
class StaticObject : public WorldObject {
protected:
    std::string description; // Περιγραφή του αντικειμένου

public:
    // Κατασκευαστής
    StaticObject(const std::string& id, char glyph, const Position& pos, const std::string& desc = "");
    
    // Υλοποίηση της εικονικής συνάρτησης update
    void update() override;
    
    // Getter για την περιγραφή
    std::string getDescription() const;
    
    // Επιστρέφει πληροφορίες για το στατικό αντικείμενο
    std::string getInfo() const override;
};

#endif