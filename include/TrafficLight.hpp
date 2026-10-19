#ifndef TRAFFICLIGHT_HPP
#define TRAFFICLIGHT_HPP

#include "StaticObject.hpp"
#include <string>

// Χρώματα φαναριού
enum LightColor {
    RED,
    YELLOW,
    GREEN
};

// Αντιπροσωπεύει ένα φανάρι κυκλοφορίας
class TrafficLight : public StaticObject {
private:
    LightColor currentColor;  // Τρέχον χρώμα
    int cycleTimer;           // Χρονόμετρο για αλλαγή χρώματος
    int redDuration;          // Διάρκεια κόκκινου (ticks)
    int yellowDuration;       // Διάρκεια κίτρινου (ticks)
    int greenDuration;        // Διάρκεια πράσινου (ticks)
    
public:
    // Κατασκευαστής
    TrafficLight(const std::string& id, const Position& pos, int red = 4, int green = 8, int yellow = 2);
    
    // Ενημέρωση κατάστασης φαναριού
    void update() override;
    
    // Αλλαγή χρώματος φαναριού
    void changeColor();
    
    // Επιστρέφει το τρέχον χρώμα ως string
    std::string getColorString() const;
    
    // Επιστρέφει το γλύφο ανάλογα με το χρώμα
    char getGlyph() const override;  
    
    // Ελέγχει αν το φανάρι επηρεάζει την κίνηση (κόκκινο ή κίτρινο)
    bool affectsMovement() const;
    
    // Επιστρέφει πληροφορίες για το φανάρι
    std::string getInfo() const override;
};

#endif