#ifndef CAMERASENSOR_HPP
#define CAMERASENSOR_HPP

#include "Sensor.hpp"

// Αισθητήρας Camera - Υψηλή ακρίβεια αναγνώρισης
class CameraSensor : public Sensor {
private:
    std::string facingDirection; // Κατεύθυνση που κοιτάει η Camera
    
public:
    // Κατασκευαστής
    CameraSensor(const std::string& id, const Position& pos);
    
    // Ανίχνευση αντικειμένων με Camera
    std::vector<SensorReading> detect(const GridWorld* world) override;
    
    // Ανίχνευση χρώματος φαναριού
    std::string detectLightColor(char glyph) const;
    
    // Ανίχνευση κειμένου πινακίδας
    std::string detectSignText(char glyph) const;
};

#endif