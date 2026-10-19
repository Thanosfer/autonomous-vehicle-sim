#include "GridWorld.hpp"
#include "TrafficLight.hpp"       
#include "TrafficSign.hpp"        
#include "CommandLineParser.hpp"  
#include "SelfDrivingCar.hpp"     
#include <iostream>
#include <algorithm>

GridWorld::GridWorld(int w, int h) : width(w), height(h) {
    // Αρχικοποίηση του πίνακα grid με κενά κελιά
    grid.resize(height, std::vector<char>(width, '.'));
}

bool GridWorld::addObject(std::shared_ptr<WorldObject> obj) {
    Position pos = obj->getPosition();
    int x = pos.getX();
    int y = pos.getY();
    
    // Έλεγχος ορίων
    if (!isWithinBounds(x, y)) {
        std::cerr << "[ERROR] Object " << obj->getId() << " out of bounds: (" << x << "," << y << ")\n";
        return false;
    }
    
    // Έλεγχος αν η θέση είναι κατειλημμένη 
    if (grid[y][x] != '.') {
        std::cerr << "[WARNING] Position (" << x << "," << y << ") occupied by " << grid[y][x] << "\n";
        // Εδώ μπορούμε να επιλέξουμε να μην προστεθεί ή να αντικατασταθεί
        return false;
    }
    
    // Προσθήκη στη λίστα αντικειμένων
    objects.push_back(obj);
    
    // Ενημέρωση του πίνακα grid
    grid[y][x] = obj->getGlyph();
    
    std::cout << "[+] Added object: " << obj->getInfo() << "\n";
    return true;
}

bool GridWorld::removeObject(const std::string& id) {
    auto it = std::remove_if(objects.begin(), objects.end(),[&id](const std::shared_ptr<WorldObject>& obj) {
            return obj->getId() == id;
        });
    
    if (it != objects.end()) {
        // Αφαίρεση από το grid
        Position pos = (*it)->getPosition();
        grid[pos.getY()][pos.getX()] = '.';
        
        objects.erase(it, objects.end());
        std::cout << "[-] Removed object: " << id << "\n";
        return true;
    }
    
    return false;
}

void GridWorld::updateAll() {
    // Αρχικά, αφαιρούμε όλα τα αντικείμενα από το grid
    for (auto& row : grid) {
        std::fill(row.begin(), row.end(), '.');
    }
    
    // Ενημερώνουμε κάθε αντικείμενο
    for (auto& obj : objects) {
        obj->update();
        
        // Ενημέρωση θέσης στο grid
        Position pos = obj->getPosition();
        int x = pos.getX();
        int y = pos.getY();
        
        // Αν το αντικείμενο είναι εκτός ορίων, το αφαιρούμε
        if (!isWithinBounds(x, y)) {
            std::cout << "[!] Object " << obj->getId() << " left the grid and will be removed\n";
            // Θα το αφαιρέσουμε στον επόμενο κύκλο
            continue;
        }
        
        grid[y][x] = obj->getGlyph();
    }
    
    // Αφαίρεση αντικειμένων εκτός ορίων
    objects.erase(std::remove_if(objects.begin(), objects.end(), [this](const std::shared_ptr<WorldObject>& obj) {
            Position pos = obj->getPosition();
            return !isWithinBounds(pos.getX(), pos.getY());
        }), objects.end());
}

std::shared_ptr<WorldObject> GridWorld::getObjectAt(int x, int y) const {
    if (!isWithinBounds(x, y)) return nullptr;
    
    for (const auto& obj : objects) {
        Position pos = obj->getPosition();
        if (pos.getX() == x && pos.getY() == y) {
            return obj;
        }
    }
    
    return nullptr;
}

void GridWorld::render() const {
    std::cout << "\n=== GRID WORLD (" << width << "x" << height << ") ===\n";
    
    // Εμφάνιση των αριθμών των στηλών
    std::cout << "   ";
    for (int x = 0; x < width; x++) {
        std::cout << x % 10;  // Μόνο το τελευταίο ψηφίο για οικονομία χώρου
    }
    std::cout << "\n";
    
    // Εμφάνιση του grid
    for (int y = 0; y < height; y++) {
        std::cout << y % 10 << ": ";  // Αριθμός γραμμής
        for (int x = 0; x < width; x++) {
            std::cout << grid[y][x];
        }
        std::cout << "\n";
    }
    
    std::cout << "Legend: .=Empty, S=STOP, P=Parked, C=Car, B=Bike\n";
}

void GridWorld::printObjectsInfo() const {
    std::cout << "\n=== OBJECT INFO ===\n";
    std::cout << "Total: " << objects.size() << "\n";
    
    for (const auto& obj : objects) {
        std::cout << "  " << obj->getInfo() << "\n";
    }
}

int GridWorld::getWidth() const { return width; }
int GridWorld::getHeight() const { return height; }
int GridWorld::getObjectCount() const { return objects.size(); }

void GridWorld::updateGrid() {
    // Αρχικοποίηση με κενά
    for (auto& row : grid) {
        std::fill(row.begin(), row.end(), '.');
    }
    
    // Τοποθέτηση αντικειμένων
    for (const auto& obj : objects) {
        Position pos = obj->getPosition();
        if (isWithinBounds(pos.getX(), pos.getY())) {
            grid[pos.getY()][pos.getX()] = obj->getGlyph();
        }
    }
}

bool GridWorld::isWithinBounds(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

// Δημιουργία τυχαίων αντικειμένων βάσει ρυθμίσεων
void GridWorld::generateRandomObjects(const SimulationConfig& config) {
    std::cout << "\n--- GENERATING RANDOM OBJECTS ---\n";
    
    srand(config.seed);
    
    // Δημιουργία κινούμενων αυτοκινήτων
    for (int i = 0; i < config.numMovingCars; i++) {
        int x = rand() % width;
        int y = rand() % height;
        Direction dir = static_cast<Direction>(rand() % 4);
        
        std::string id = "CAR:" + std::to_string(i + 1);
        auto car = std::make_shared<MovingObject>(id, 'C', Position(x, y), 1, dir);
        
        if (!addObject(car)) {
            i--;  // Δοκιμή ξανά αν η θέση ήταν κατειλημμένη
        }
    }
    
    // Δημιουργία κινούμενων ποδηλάτων
    for (int i = 0; i < config.numMovingBikes; i++) {
        int x = rand() % width;
        int y = rand() % height;
        Direction dir = static_cast<Direction>(rand() % 4);
        
        std::string id = "BIKE:" + std::to_string(i + 1);
        auto bike = std::make_shared<MovingObject>(id, 'B', Position(x, y), 1, dir);
        
        if (!addObject(bike)) {
            i--;
        }
    }
    
    // Δημιουργία σταθμευμένων αυτοκινήτων
    for (int i = 0; i < config.numParkedCars; i++) {
        int x = rand() % width;
        int y = rand() % height;
        
        std::string id = "PARKED:" + std::to_string(i + 1);
        auto parked = std::make_shared<StaticObject>(
            id, 'P', Position(x, y), "Parked car");
        
        if (!addObject(parked)) {
            i--;
        }
    }
    
    // Δημιουργία πινακίδων STOP
    for (int i = 0; i < config.numStopSigns; i++) {
        int x = rand() % width;
        int y = rand() % height;
        
        std::string id = "STOP:" + std::to_string(i + 1);
        auto stopSign = std::make_shared<TrafficSign>(
            id, Position(x, y), STOP_SIGN, "STOP");
        
        if (!addObject(stopSign)) {
            i--;
        }
    }
    
    // Δημιουργία φανάριων
    for (int i = 0; i < config.numTrafficLights; i++) {
        int x = rand() % width;
        int y = rand() % height;
        
        std::string id = "LIGHT:" + std::to_string(i + 1);
        auto light = std::make_shared<TrafficLight>(id, Position(x, y));
        
        if (!addObject(light)) {
            i--;
        }
    }
    
    std::cout << "Created " << objects.size() << " objects.\n";
}

// Ενημέρωση του αυτόνομου οχήματος με νέα θέση
void GridWorld::updateCarPosition(const std::shared_ptr<SelfDrivingCar>& car) {
    // Το όχημα είναι ήδη στη λίστα objects, οπότε θα εμφανιστεί στο render()
    updateGrid();
    
    // Επιπλέον, μπορούμε να ενημερώσουμε τη θέση του οχήματος στον πίνακα
    if (car != nullptr) {
        Position carPos = car->getPosition();
        if (isWithinBounds(carPos.getX(), carPos.getY())) {
            grid[carPos.getY()][carPos.getX()] = car->getGlyph();
        }
    }
}