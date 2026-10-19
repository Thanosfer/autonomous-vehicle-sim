#include "Visualization.hpp"
#include <iostream>
#include <iomanip>

void Visualization::renderFull(const GridWorld& world) {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════╗\n";
    std::cout << "║            FULL VISUALIZATION            ║\n";
    std::cout << "╚══════════════════════════════════════════╝\n";
    
    world.render();
}

void Visualization::renderPOV(const GridWorld& world, const SelfDrivingCar& car, int radius, bool centered) {
    
    Position carPos = car.getPosition();
    int carX = carPos.getX();
    int carY = carPos.getY();
    
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════╗\n";
    std::cout << "║            CAR FIELD OF VIEW             ║\n";
    std::cout << "╚══════════════════════════════════════════╝\n";
    
    std::cout << "Car position: (" << carX << ", " << carY << ")\n";
    std::cout << "Radius: " << radius << " cells\n";
    std::cout << "Mode: " << (centered ? "Centered" : "Forward") << "\n\n";
    
    // Υπολογισμός ορίων
    int startX, endX, startY, endY;
    
    if (centered) {
        startX = carX - radius;
        endX = carX + radius;
        startY = carY - radius;
        endY = carY + radius;
    } else {
        // Μπροστινή όψη (θεωρούμε ότι κοιτάει βόρεια)
        startX = carX - radius;
        endX = carX + radius;
        startY = carY - radius;
        endY = carY;
    }
    
    // Εμφάνιση οπτικοποίησης
    for (int y = startY; y <= endY; y++) {
        // Εμφάνιση αριθμού γραμμής
        std::cout << std::setw(3) << y << ": ";
        
        for (int x = startX; x <= endX; x++) {
            // Έλεγχος αν είναι η θέση του οχήματος
            if (x == carX && y == carY) {
                std::cout << '@';
            } else {
                // Έλεγχος ορίων
                if (x < 0 || x >= world.getWidth() || 
                    y < 0 || y >= world.getHeight()) {
                    std::cout << 'X';
                } else {
                    auto obj = world.getObjectAt(x, y);
                    if (obj != nullptr) {
                        std::cout << obj->getGlyph();
                    } else {
                        std::cout << '.';
                    }
                }
            }
        }
        std::cout << "\n";
    }
    
    std::cout << "\n";
    printLegend();
}

void Visualization::printLegend() {
    std::cout << "LEGEND:\n";
    std::cout << "  @ = Autonomous car\n";
    std::cout << "  . = Empty cell\n";
    std::cout << "  X = Out of bounds\n";
    std::cout << "  R = Red light\n";
    std::cout << "  Y = Yellow light\n";
    std::cout << "  G = Green light\n";
    std::cout << "  S = STOP sign\n";
    std::cout << "  B = Bike\n";
    std::cout << "  C = Car\n";
    std::cout << "  P = Parked car\n";
    std::cout << "  ? = Unknown object\n";
}

void Visualization::printMessage(const std::string& message, const std::string& level) {
    
    std::string prefix;
    if (level == "INFO") {
        prefix = "[INFO] ";
    } else if (level == "WARNING") {
        prefix = "[WARNING] ";
    } else if (level == "ERROR") {
        prefix = "[ERROR] ";
    } else {
        prefix = "[DEBUG] ";
    }
    
    std::cout << prefix << message << std::endl;
}

void Visualization::printSimulationStatus(int tick, int maxTicks, const SelfDrivingCar& car) {
    
    std::cout << "\n═══════════════════════════════════════════════\n";
    std::cout << "SIMULATION - TICK " << tick << " / " << maxTicks << "\n";
    std::cout << "═══════════════════════════════════════════════\n";
    
    Position pos = car.getPosition();
    std::cout << "Car position: (" << pos.getX() << ", " << pos.getY() << ")\n";
}