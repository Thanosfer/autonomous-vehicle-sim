#include "SensorFusionEngine.hpp"
#include <iostream>
#include <algorithm>
#include <numeric>

SensorFusionEngine::SensorFusionEngine(double threshold) : minConfidenceThreshold(threshold), fusionCycle(0) {}

std::vector<SensorReading> SensorFusionEngine::fuseSensorData(
    const std::vector<std::vector<SensorReading>>& allReadings) {
    
    std::vector<SensorReading> fusedReadings;
    fusionCycle++;
    
    std::cout << "[FUSION] Cycle " << fusionCycle << ", readings to process: ";
    
    // Μέτρηση συνολικών αναγνώσεων
    int totalReadings = 0;
    for (const auto& readings : allReadings) {
        totalReadings += readings.size();
    }
    std::cout << totalReadings << std::endl;
    
    // Ομαδοποίηση αναγνώσεων ανά αντικείμενο
    auto groupedReadings = groupByObjectId(allReadings);
    
    // Συγχώνευση κάθε ομάδας
    for (const auto& group : groupedReadings) {
        const std::string& objectId = group.first;
        const auto& readings = group.second;
        
        // Απόρριψη ομάδων με μέση βεβαιότητα κάτω από το όριο
        double avgConfidence = 0.0;
        for (const auto& reading : readings) {
            avgConfidence += reading.confidence;
        }
        avgConfidence /= readings.size();
        
        // Εξαίρεση για ποδήλατα (ασφάλεια)
        bool isBike = false;
        for (const auto& reading : readings) {
            if (reading.type == DETECTED_MOVING_BIKE) {  // Αλλαγή
                isBike = true;
                break;
            }
        }
        
        if (avgConfidence >= minConfidenceThreshold || isBike) {
            SensorReading fused = fuseObjectReadings(readings);
            fusedReadings.push_back(fused);
            
            std::cout << "  [RESULT] Object: " << objectId<< ", Confidence: " << fused.confidence<< ", Position: (" << fused.position.getX() << "," << fused.position.getY() << ")" << std::endl;
        } else {
            std::cout << "  [REJECTED] Object: " << objectId<< ", Average confidence: " << avgConfidence << " < " << minConfidenceThreshold << std::endl;
        }
    }
    
    std::cout << "[FUSION] Done. Fused readings: " << fusedReadings.size() << std::endl;
    
    return fusedReadings;
}

void SensorFusionEngine::setMinConfidenceThreshold(double threshold) {
    minConfidenceThreshold = threshold;
}

void SensorFusionEngine::printStats() const {
    std::cout << "[STATS] Fusion engine:" << std::endl;
    std::cout << "  - Fusion cycles: " << fusionCycle << std::endl;
    std::cout << "  - Minimum confidence threshold: " << minConfidenceThreshold << std::endl;
}

std::map<std::string, std::vector<SensorReading>> 
SensorFusionEngine::groupByObjectId(
    const std::vector<std::vector<SensorReading>>& allReadings) const {
    
    std::map<std::string, std::vector<SensorReading>> grouped;
    
    for (const auto& sensorReadings : allReadings) {
        for (const auto& reading : sensorReadings) {
            grouped[reading.objectId].push_back(reading);
        }
    }
    
    return grouped;
}

SensorReading SensorFusionEngine::fuseObjectReadings(
    const std::vector<SensorReading>& readings) const {
    
    if (readings.empty()) {
        return SensorReading();
    }
    
    SensorReading result = readings[0];  // Αρχικοποίηση με την πρώτι ανάγνωση
    
    // Στατιστικά για συγχώνευση
    std::vector<double> distances;
    std::vector<double> distanceConfidences;
    std::vector<double> positionsX;
    std::vector<double> positionsY;
    std::vector<double> positionConfidences;
    
    for (const auto& reading : readings) {
        distances.push_back(reading.distance);
        distanceConfidences.push_back(reading.confidence);
        
        positionsX.push_back(reading.position.getX());
        positionsY.push_back(reading.position.getY());
        positionConfidences.push_back(reading.confidence);
    }
    
    // Σταθμισμένος μέσος όρος για απόσταση
    result.distance = weightedAverage(distances, distanceConfidences);
    
    // Σταθμισμένος μέσος όρος για θέση
    int fusedX = static_cast<int>(weightedAverage(positionsX, positionConfidences) + 0.5);
    int fusedY = static_cast<int>(weightedAverage(positionsY, positionConfidences) + 0.5);
    result.position = Position(fusedX, fusedY);
    
    // Υπολογισμός συνολικής βεβαιότητας (μέγιστη)
    double maxConfidence = 0.0;
    for (const auto& reading : readings) {
        if (reading.confidence > maxConfidence) {
            maxConfidence = reading.confidence;
        }
    }
    result.confidence = maxConfidence;
    
    // Πρόσθεση πληροφοριών από Camera αν υπάρχουν
    for (const auto& reading : readings) {
        if (!reading.lightColor.empty()) {
            result.lightColor = reading.lightColor;
        }
        if (!reading.signText.empty()) {
            result.signText = reading.signText;
        }
    }
    
    return result;
}

double SensorFusionEngine::weightedAverage(const std::vector<double>& values, const std::vector<double>& confidences) const {
    
    if (values.empty()) return 0.0;
    
    double weightedSum = 0.0;
    double confidenceSum = 0.0;
    
    for (size_t i = 0; i < values.size(); i++) {
        weightedSum += values[i] * confidences[i];
        confidenceSum += confidences[i];
    }
    
    if (confidenceSum == 0.0) return values[0];
    return weightedSum / confidenceSum;
}