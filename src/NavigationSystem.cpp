#include "NavigationSystem.hpp"
#include <iostream>
#include <algorithm>

NavigationSystem::NavigationSystem(const std::vector<Position>& targets) : gpsTargets(targets), currentState(STOPPED), currentDirection("NORTH"), currentTargetIndex(0), hasReachedDestination(false) {
    
    if (!gpsTargets.empty()) {
        currentTarget = gpsTargets[0];
        std::cout << "[NAVIGATION] Initial target: (" << currentTarget.getX() << "," << currentTarget.getY() << ")" << std::endl;
    } else {
        std::cerr << "[NAVIGATION] Warning: no GPS targets given!" << std::endl;
    }
}

std::vector<std::string> NavigationSystem::makeDecision(
    const std::vector<SensorReading>& fusedReadings, const Position& currentPos, int currentSpeed) {

    std::vector<std::string> commands;

    // Η ταχύτητα του οχήματος είναι η μόνη "αλήθεια": την παίρνουμε από το όχημα σε κάθε βήμα
    currentState = static_cast<CarState>(currentSpeed);

    // Έλεγχος για φανάρια και εμπόδια
    for (const auto& reading : fusedReadings) {
        // Εμπόδιο (κινούμενο αντικείμενο) σε απόσταση 2 κελιών
        if ((reading.type == DETECTED_MOVING_CAR || reading.type == DETECTED_MOVING_BIKE) 
            && reading.distance <= 2.0) {
            commands.push_back("DECELERATE");
            commands.push_back("STOP_IF_NEEDED");
            std::cout << "[DECISION] Obstacle at distance " << reading.distance << ", slowing down!" << std::endl;
        }
        
        // Κόκκινο ή κίτρινο φανάρι σε απόσταση 3 κελιών
        if (reading.type == DETECTED_TRAFFIC_LIGHT && reading.distance <= 3.0) {
            if (reading.lightColor == "RED" || reading.lightColor == "YELLOW") {
                commands.push_back("DECELERATE");
                std::cout << "[DECISION] " << reading.lightColor << " light at distance " << reading.distance << ", slowing down!" << std::endl;
            }
        }
        
        // Πινακίδα STOP σε απόσταση 2 κελιών (σε πλήρη ταχύτητα το όχημα κινείται 2 κελιά,
        // οπότε με όριο 1 μπορούσε να την προσπεράσει χωρίς να σταματήσει).
        // Σε κάθε πινακίδα σταματάει μία φορά και μετά συνεχίζει (αλλιώς θα έμενε για πάντα εκεί).
        if (reading.type == DETECTED_STOP_SIGN && reading.distance <= 2.0
            && reading.objectId != lastStopSignId) {
            lastStopSignId = reading.objectId;
            commands.push_back("DECELERATE");
            commands.push_back("STOP");
            std::cout << "[DECISION] STOP sign, full stop!" << std::endl;
        }
    }
    
    // Αν βρέθηκε εμπόδιο, φανάρι ή STOP, δεν επιταχύνουμε σε αυτό το βήμα
    bool mustSlowDown = !commands.empty();

    // Στροφή προς τον τρέχοντα στόχο GPS (πρώτα στον άξονα με τη μεγαλύτερη απόσταση)
    std::string newDirection = calculateDirectionToTarget(currentPos);
    if (newDirection != currentDirection) {
        commands.push_back("TURN_" + newDirection);
        currentDirection = newDirection;
    }

    // Επιτάχυνση αν δεν υπάρχουν εμπόδια
    if (shouldAccelerate() && !mustSlowDown) {
        if (currentState == STOPPED) {
            currentState = HALF_SPEED;
            commands.push_back("ACCELERATE_HALF");
        } else if (currentState == HALF_SPEED) {
            currentState = FULL_SPEED;
            commands.push_back("ACCELERATE_FULL");
        }
    }
    
    // Κίνηση προς την τρέχουσα κατεύθυνση σε κάθε βήμα, εκτός αν πρέπει να σταματήσει
    if (std::find(commands.begin(), commands.end(), "STOP") == commands.end()) {
        commands.push_back("MOVE_" + currentDirection);
    }
    
    return commands;
}

void NavigationSystem::updateTarget() {
    if (hasReachedDestination) return;
    
    currentTargetIndex++;
    if (currentTargetIndex < static_cast<int>(gpsTargets.size())) {
        currentTarget = gpsTargets[currentTargetIndex];
        std::cout << "[NAVIGATION] New target: (" << currentTarget.getX() << "," << currentTarget.getY() << ")" << std::endl;
    } else {
        hasReachedDestination = true;
        std::cout << "[NAVIGATION] Final target reached!" << std::endl;
    }
}

bool NavigationSystem::shouldDecelerate(
    const std::vector<SensorReading>& fusedReadings) const {
    
    for (const auto& reading : fusedReadings) {
        // Κοντινό εμπόδιο
        if (reading.distance <= 2.0) return true;
        
        // Κοντινό φανάρι
        if (reading.type == DETECTED_TRAFFIC_LIGHT && reading.distance <= 3.0) {
            if (reading.lightColor == "RED" || reading.lightColor == "YELLOW") {
                return true;
            }
        }
    }
    
    return false;
}

bool NavigationSystem::shouldAccelerate() const {
    return currentState != FULL_SPEED && !hasReachedDestination;
}

CarState NavigationSystem::getCurrentState() const {
    return currentState;
}

std::string NavigationSystem::getCurrentDirection() const {
    return currentDirection;
}

Position NavigationSystem::getCurrentTarget() const {
    return currentTarget;
}

bool NavigationSystem::destinationReached() const {
    return hasReachedDestination;
}

void NavigationSystem::printStatus() const {
    std::cout << "[NAVIGATION STATUS]" << std::endl;
    std::cout << "  - Speed: ";
    switch (currentState) {
        case STOPPED: std::cout << "STOPPED"; break;
        case HALF_SPEED: std::cout << "HALF SPEED"; break;
        case FULL_SPEED: std::cout << "FULL SPEED"; break;
    }
    std::cout << std::endl;
    std::cout << "  - Direction: " << currentDirection << std::endl;
    std::cout << "  - Current target: (" << currentTarget.getX() << "," << currentTarget.getY() << ")" << std::endl;
    // Μετατροπή σε int: με size_t η αφαίρεση έβγαζε τεράστιο αριθμό όταν τελείωναν οι στόχοι
    int remaining = static_cast<int>(gpsTargets.size()) - currentTargetIndex - 1;
    if (remaining < 0) remaining = 0;
    std::cout << "  - Targets left: " << remaining << std::endl;
}

int NavigationSystem::distanceToTarget(const Position& currentPos) const {
    return abs(currentPos.getX() - currentTarget.getX()) + abs(currentPos.getY() - currentTarget.getY());
}

std::string NavigationSystem::calculateDirectionToTarget(
    const Position& currentPos) const {
    
    int dx = currentTarget.getX() - currentPos.getX();
    int dy = currentTarget.getY() - currentPos.getY();
    
    // Προτεραιότητα στις κατακόρυφες μετακινήσεις
    if (abs(dy) > abs(dx)) {
        if (dy > 0) return "SOUTH";
        else return "NORTH";
    } else {
        if (dx > 0) return "EAST";
        else return "WEST";
    }
}