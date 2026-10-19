#ifndef LIDARSENSOR_HPP
#define LIDARSENSOR_HPP

#include "Sensor.hpp"

// Αισθητήρας Lidar - Εξαιρετική ακρίβεια απόστασης
class LidarSensor : public Sensor {
public:
    // Κατασκευαστής
    LidarSensor(const std::string& id, const Position& pos);
    
    // Ανίχνευση αντικειμένων με Lidar
    std::vector<SensorReading> detect(const GridWorld* world) override;
    
private:
    //Μετατρέπει τον τύπο αντικειμένου από glyph σε DetectedObjectType
    DetectedObjectType glyphToObjectType(char glyph) const;  // Αλλαγή: DetectedObjectType
};

#endif