#ifndef MOVINGOBJECT_HPP
#define MOVINGOBJECT_HPP

#include "WorldObject.hpp"

// Κατευθύνσεις για τα κινούμενα αντικείμενα
enum Direction {
    NORTH,  // Βόρεια
    EAST,   // Ανατολικά
    SOUTH,  // Νότια
    WEST    // Δυτικά
};

// Αντιπροσωπεύει κινούμενα αντικείμενα (αμάξια, ποδήλατα)
class MovingObject : public WorldObject {
protected:
    int speed;          // Ταχύτητα (θέσεις ανά βήμα)
    Direction direction; // Κατεύθυνση κίνησης
    
public:
    // Κατασκευαστής
    MovingObject(const std::string& id, char glyph, const Position& pos, int speed = 1, Direction dir = NORTH);
    
    // Υλοποίηση της εικονικής συνάρτησης update
    void update() override;
    
    // Κινεί το αντικείμενο μία θέση προς την τρέχουσα κατεύθυνση
    virtual void move();
    
    // Αλλάζει την κατεύθυνση τυχαία
    virtual void changeDirection();
    
    // Getter methods
    int getSpeed() const;
    Direction getDirection() const;
    std::string getDirectionString() const;
    
    // Επιστρέφει πληροφορίες για το κινούμενο αντικείμενο
    std::string getInfo() const override;
};

#endif