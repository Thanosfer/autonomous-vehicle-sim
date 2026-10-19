#include "SelfDrivingCar.hpp"
#include "GridWorld.hpp"
#include "LidarSensor.hpp"
#include "RadarSensor.hpp"
#include "CameraSensor.hpp"
#include <iostream>

SelfDrivingCar::SelfDrivingCar(const std::string& id, const Position& pos, const std::vector<Position>& gpsTargets) : WorldObject(id, '@', pos), speed(0), direction("NORTH") {
    
    // Δημιουργία μηχανής συγχώνευσης
    fusionEngine = std::make_unique<SensorFusionEngine>(0.4);
    
    // Δημιουργία συστήματος πλοήγησης
    navigationSystem = std::make_unique<NavigationSystem>(gpsTargets);
    
    std::cout << "[CAR] Created autonomous car " << id << " at (" << pos.getX() << "," << pos.getY() << ")" << std::endl;
    
    // Προσθήκη προκαθορισμένων αισθητήρων
    addSensor(std::make_shared<LidarSensor>("LIDAR_1", pos));
    addSensor(std::make_shared<RadarSensor>("RADAR_1", pos));
    addSensor(std::make_shared<CameraSensor>("CAMERA_1", pos));
}

void SelfDrivingCar::update() {
    // Αυτή η συνάρτηση θα καλείται από την κύρια προσομοίωση
    // Θα υλοποιηθεί πλήρως όταν ενσωματωθεί με το GridWorld
    std::cout << "[CAR] Updating autonomous car " << id << " at (" << position.getX() << "," << position.getY() << ")" << std::endl;
}

void SelfDrivingCar::addSensor(std::shared_ptr<Sensor> sensor) {
    sensors.push_back(sensor);
    std::cout << "[CAR] Added sensor " << sensor->getId() << " (" << sensor->getType() << ")" << std::endl;
}

void SelfDrivingCar::executeCommands(const std::vector<std::string>& commands) {
    for (const auto& command : commands) {
        if (command == "MOVE_NORTH") {
            direction = "NORTH";
            moveOneStep();
        } else if (command == "MOVE_EAST") {
            direction = "EAST";
            moveOneStep();
        } else if (command == "MOVE_SOUTH") {
            direction = "SOUTH";
            moveOneStep();
        } else if (command == "MOVE_WEST") {
            direction = "WEST";
            moveOneStep();
        } else if (command == "ACCELERATE_HALF") {
            changeSpeed(1);
        } else if (command == "ACCELERATE_FULL") {
            changeSpeed(2);
        } else if (command == "DECELERATE") {
            if (speed > 0) changeSpeed(speed - 1);
        } else if (command == "STOP") {
            changeSpeed(0);
        } else if (command.find("TURN_") == 0) {
            std::string newDir = command.substr(5);
            changeDirection(newDir);
        }
    }
}

std::string SelfDrivingCar::getInfo() const {
    std::string info = WorldObject::getInfo();
    info += ", Type: Autonomous Car";
    info += ", Speed: " + std::to_string(speed);
    info += ", Direction: " + direction;
    info += ", Sensors: " + std::to_string(sensors.size());
    return info;
}

void SelfDrivingCar::printCarStats() const {
    std::cout << "\n=== AUTONOMOUS CAR STATS ===" << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "Position: (" << position.getX() << "," << position.getY() << ")" << std::endl;
    std::cout << "Speed: " << speed << " (0=STOPPED, 1=HALF, 2=FULL)" << std::endl;
    std::cout << "Direction: " << direction << std::endl;
    std::cout << "Number of sensors: " << sensors.size() << std::endl;
    
    for (const auto& sensor : sensors) {
        std::cout << "  - " << sensor->getId() << " (" << sensor->getType() << "), Range: " << sensor->getRange() << ", Accuracy: " << sensor->getBaseAccuracy() << std::endl;
    }
}

void SelfDrivingCar::performOneTick(GridWorld* world) {
    std::cout << "\n[CAR] Running tick for " << id << std::endl;
    
    // Συλλογή δεδομένων από αισθητήρες
    auto allReadings = collectSensorData(world);
    
    // Συγχώνευση δεδομένων
    auto fusedReadings = fusionEngine->fuseSensorData(allReadings);
    
    // Λήψη αποφάσεων πλοήγησης
    auto commands = navigationSystem->makeDecision(fusedReadings, position, speed);
    
    // Εκτέλεση εντολών
    executeCommands(commands);
    
    // Ενημέρωση σύστηματος πλοήγησης (έλεγχος στόχων)
    Position currentPos = getPosition();
    Position target = navigationSystem->getCurrentTarget();
    
    int distance = currentPos.manhattanDistance(target);
    if (distance <= 2) {  // Κοντά στον στόχο
        navigationSystem->updateTarget();
        
        if (navigationSystem->destinationReached()) {
            std::cout << "[CAR] Final target reached!" << std::endl;
            changeSpeed(0);  // Στάση
        }
    }
    
    // Εκτύπωση κατάστασης
    std::cout << "[CAR] Current state: " << std::endl;
    std::cout << "  - Position: (" << currentPos.getX() << ", " << currentPos.getY() << ")" << std::endl;
    std::cout << "  - Speed: " << speed << std::endl;
    std::cout << "  - Direction: " << direction << std::endl;
}

bool SelfDrivingCar::isWithinBounds(int worldWidth, int worldHeight) const {
    return position.getX() >= 0 && position.getX() < worldWidth && position.getY() >= 0 && position.getY() < worldHeight;
}

std::vector<std::vector<SensorReading>> SelfDrivingCar::collectSensorData(const GridWorld* world) {
    std::vector<std::vector<SensorReading>> allReadings;
    
    // Ενημέρωση θέσης όλων των αισθητήρων
    for (auto& sensor : sensors) {
        sensor->setPosition(position);
    }
    
    // Συλλογή δεδομένων από κάθε αισθητήρα
    for (const auto& sensor : sensors) {
        std::vector<SensorReading> readings = sensor->detect(world);
        allReadings.push_back(readings);
        
        std::cout << "[SENSE] Sensor " << sensor->getId() << " found " << readings.size() << " objects" << std::endl;
    }
    
    return allReadings;
}

void SelfDrivingCar::moveOneStep() {
    if (speed == 0) return;
    
    int steps = speed;  // FULL_SPEED = 2 βήματα
    
    for (int i = 0; i < steps; i++) {
        if (direction == "NORTH") {
            position.setY(position.getY() - 1);
        } else if (direction == "EAST") {
            position.setX(position.getX() + 1);
        } else if (direction == "SOUTH") {
            position.setY(position.getY() + 1);
        } else if (direction == "WEST") {
            position.setX(position.getX() - 1);
        }
    }
    
    std::cout << "[MOVE] Car moved " << steps << " cell(s) " << direction << ", new position: (" << position.getX() << "," << position.getY() << ")" << std::endl;
}

void SelfDrivingCar::changeDirection(const std::string& newDir) {
    if (direction != newDir) {
        std::cout << "[TURN] Direction changed from " << direction << " to " << newDir << std::endl;
        direction = newDir;
    }
}

void SelfDrivingCar::changeSpeed(int newSpeed) {
    if (newSpeed < 0) newSpeed = 0;
    if (newSpeed > 2) newSpeed = 2;
    
    if (speed != newSpeed) {
        std::string speedNames[] = {"STOPPED", "HALF_SPEED", "FULL_SPEED"};
        std::cout << "[SPEED] Changed from " << speedNames[speed] << " to " << speedNames[newSpeed] << std::endl;
        speed = newSpeed;
    }
}