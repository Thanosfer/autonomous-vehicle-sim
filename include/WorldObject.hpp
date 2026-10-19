#ifndef WORLDOBJECT_HPP
#define WORLDOBJECT_HPP

#include "Position.hpp"
#include <string>

// Βασική κλάση για όλα τα αντικείμενα στον κόσμο
class WorldObject {
protected:
    std::string id;    // Μοναδικό αναγνωριστικό αντικειμένου (π.χ. "CAR:1")
    char glyph;        // Χαρακτήρας για οπτικοποίηση (π.χ. 'C' για αμάξι)
    Position position; // Θέση του αντικειμένου στον κόσμο

public:
    // Κατασκευαστής
    WorldObject(const std::string& id, char glyph, const Position& pos);
    
    // Εικονικός καταστροφέας για σωστή καταστροφή υποκλάσεων
    virtual ~WorldObject() {}
    
    // Εικονική συνάρτηση ενημέρωσης
    virtual void update() = 0;  // Καθαρά εικονική
    
    // Getter methods
    std::string getId() const;
    virtual char getGlyph() const;  // Αλλαγή: εικονική συνάρτηση
    Position getPosition() const;
    
    // Setter methods
    void setPosition(const Position& pos);
    
    // Επιστρέφει πληροφορίες για το αντικείμενο
    virtual std::string getInfo() const;
};

#endif