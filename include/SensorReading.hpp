#ifndef SENSORREADING_HPP
#define SENSORREADING_HPP

#include "Position.hpp"
#include <string>

// Τύποι αντικειμένων που μπορούν να ανιχνευθούν από αισθητήρες
enum DetectedObjectType {
    DETECTED_UNKNOWN,        // Άγνωστο αντικείμενο
    DETECTED_STATIC,         // Στατικό αντικείμενο
    DETECTED_MOVING_CAR,     // Κινούμενο αυτοκίνητο
    DETECTED_MOVING_BIKE,    // Κινούμενο ποδήλατο
    DETECTED_PARKED_CAR,     // Σταθμευμένο αυτοκίνητο
    DETECTED_STOP_SIGN,      // Πινακίδα STOP (με DETECTED_ prefix)
    DETECTED_TRAFFIC_LIGHT   // Φανάρι
};

// Δομή που περιέχει τα δεδομένα μιας ανάγνωσης αισθητήρα
struct SensorReading {
    std::string sensorId;        // ID του αισθητήρα που πήρε την μέτρηση
    std::string objectId;        // ID του ανιχνευθέντος αντικειμένου
    DetectedObjectType type;     // Τύπος αντικειμένου 
    Position position;           // Θέση αντικειμένου
    double distance;             // Απόσταση από το όχημα
    double confidence;           // Βεβαιότητα ανίχνευσης (0.0 έως 1.0)
    double speed;                // Ταχύτητα αντικειμένου (για κινούμενα)
    std::string direction;       // Κατεύθυνση κίνησης
    std::string signText;        // Κείμενο πινακίδας
    std::string lightColor;      // Χρώμα φαναριού
    
    // Κατασκευαστής με προκαθορισμένες τιμές
    SensorReading();
    
    // Εκτυπώνει τις πληροφορίες της ανάγνωσης
    void print() const;
};

#endif