#ifndef SENSORFUSIONENGINE_HPP
#define SENSORFUSIONENGINE_HPP

#include "SensorReading.hpp"
#include <vector>
#include <map>

// Μηχανή συγχώνευσης δεδομένων από πολλούς αισθητήρες
class SensorFusionEngine {
private:
    double minConfidenceThreshold; // Ελάχιστο όριο βεβαιότητας
    int fusionCycle; // Αριθμός κύκλων συγχώνευσης
    
public:
    // Κατασκευαστής
    SensorFusionEngine(double threshold = 0.4);
    
    // Συγχώνευση δεδομένων από πολλαπλούς αισθητήρες
    std::vector<SensorReading> fuseSensorData(
        const std::vector<std::vector<SensorReading>>& allReadings);
    
    // Ενημερώνει το όριο βεβαιότητας
    void setMinConfidenceThreshold(double threshold);
    
    // Επιστρέφει πληροφορίες για την μηχανή συγχώνευσης
    void printStats() const;
    
private:
    // Ομαδοποίηση αναγνώσεων με βάση το ID του αντικειμένου
    std::map<std::string, std::vector<SensorReading>> groupByObjectId(
        const std::vector<std::vector<SensorReading>>& allReadings) const;
    
    // Συγχώνευση αναγνώσεων για ένα συγκεκριμένο αντικείμενο
    SensorReading fuseObjectReadings(
        const std::vector<SensorReading>& readings) const;
    
    // Υπολογισμός σταθμισμένου μέσου όρου
    double weightedAverage(const std::vector<double>& values, const std::vector<double>& confidences) const;
};

#endif