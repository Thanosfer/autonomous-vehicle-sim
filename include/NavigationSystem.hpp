#ifndef NAVIGATIONSYSTEM_HPP
#define NAVIGATIONSYSTEM_HPP

#include "Position.hpp"
#include "SensorReading.hpp"
#include <vector>
#include <string>

// Κατάσταση ταχύτητας του αυτόνομου οχήματος
enum CarState {
    STOPPED,      // Σταματημένο
    HALF_SPEED,   // Μισή ταχύτητα
    FULL_SPEED    // Πλήρης ταχύτητα
};

// Σύστημα πλοήγησης που λαμβάνει αποφάσεις βάσει συγχωνευμένων δεδομένων
class NavigationSystem {
private:
    std::vector<Position> gpsTargets; // Λίστα με στόχους GPS
    Position currentTarget;           // Τρέχων στόχος
    CarState currentState;            // Τρέχουσα κατάσταση ταχύτητας
    std::string currentDirection;     // Τρέχουσα κατεύθυνση
    
    int currentTargetIndex;           // Δείκτης στον τρέχοντα στόχο
    bool hasReachedDestination;       // Έχει φτάσει στον τελικό στόχο
    std::string lastStopSignId;       // Η πινακίδα STOP όπου σταμάτησε τελευταία φορά

public:
    // Κατασκευαστής
    NavigationSystem(const std::vector<Position>& targets);
    
    // Λήψη απόφασης βάσει συγχωνευμένων δεδομένων, της θέσης και της ταχύτητας του οχήματος
    std::vector<std::string> makeDecision(const std::vector<SensorReading>& fusedReadings,
                                          const Position& currentPos, int currentSpeed);
    
    // Ενημερώνει τον τρέχοντα στόχο
    void updateTarget();
    
    // Ελέγχει αν το όχημα πρέπει να επιβραδύνει
    bool shouldDecelerate(const std::vector<SensorReading>& fusedReadings) const;
    
    // Ελέγχει αν το όχημα πρέπει να επιταχύνει
    bool shouldAccelerate() const;
    
    // Getter methods
    CarState getCurrentState() const;
    std::string getCurrentDirection() const;
    Position getCurrentTarget() const;
    bool destinationReached() const;
    
    // Επιστρέφει πληροφορίες για το σύστημα πλοήγησης
    void printStatus() const;
    
private:
    // Υπολογισμός απόστασης από τον τρέχοντα στόχο
    int distanceToTarget(const Position& currentPos) const;
    
    // Καθορισμός κατεύθυνσης προς τον στόχο
    std::string calculateDirectionToTarget(const Position& currentPos) const;
};

#endif