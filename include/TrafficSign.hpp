#ifndef TRAFFICSIGN_HPP
#define TRAFFICSIGN_HPP

#include "StaticObject.hpp"
#include <string>

// Τύποι πινακίδων
enum SignType {
    STOP_SIGN,
    SPEED_LIMIT,
    YIELD,
    OTHER
};

// Αντιπροσωπεύει μια οδική πινακίδα
class TrafficSign : public StaticObject {
private:
    SignType type;      // Τύπος πινακίδας
    std::string text;   // Κείμενο πινακίδας
    
public:
    // Κατασκευαστής
    TrafficSign(const std::string& id, const Position& pos, SignType signType, const std::string& signText = "");
    
    // Επιστρέφει τον τύπο της πινακίδας
    SignType getSignType() const;
    
    // Επιστρέφει το κείμενο της πινακίδας
    std::string getText() const;
    
    // Επιστρέφει το γλύφο ανάλογα με τον τύπο
    char getGlyph() const; 
    
    // Ελέγχει αν η πινακίδα επηρεάζει την κίνηση
    bool affectsMovement() const;
    
    // Επιστρέφει πληροφορίες για την πινακίδα
    std::string getInfo() const override;
};

#endif