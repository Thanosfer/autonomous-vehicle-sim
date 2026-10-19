#ifndef RADARSENSOR_HPP
#define RADARSENSOR_HPP

#include "Sensor.hpp"

// Κατευθύνσεις για το Radar (προς τα εμπρός)
enum RadarDirection {
    RADAR_NORTH,
    RADAR_EAST,
    RADAR_SOUTH,
    RADAR_WEST
};

// Αισθητήρας Radar - Υψηλή ακρίβεια για κινούμενα αντικείμενα
class RadarSensor : public Sensor {
private:
    RadarDirection facingDirection; // Κατεύθυνση που κοιτάει το Radar
    
public:
    // Κατασκευαστής
    RadarSensor(const std::string& id, const Position& pos, RadarDirection dir = RADAR_NORTH);
    
    // Ανίχνευση κινούμενων αντικειμένων με Radar
    std::vector<SensorReading> detect(const GridWorld* world) override;
    
    // Αλλάζει την κατεύθυνση του Radar
    void setDirection(RadarDirection newDir);
};

#endif