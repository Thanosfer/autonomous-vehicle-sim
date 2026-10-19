#ifndef GRIDWORLD_HPP
#define GRIDWORLD_HPP

#include "WorldObject.hpp"
#include "StaticObject.hpp"
#include "MovingObject.hpp"
#include <vector>
#include <memory>  // για std::shared_ptr

// Forward declarations
struct SimulationConfig;  
class SelfDrivingCar;     

// Αντιπροσωπεύει τον κόσμο-πλέγμα όπου γίνεται η προσομοίωση
class GridWorld {
private:
    int width;      // Πλάτος κόσμου
    int height;     // Ύψος κόσμου
    
    // Δισδιάστατος πίνακας για τα γλύφη (για γρήγορη οπτικοποίηση)
    std::vector<std::vector<char>> grid;
    
    // Λίστα με όλα τα αντικείμενα (χρησιμοποιούμε smart pointers)
    std::vector<std::shared_ptr<WorldObject>> objects;
    
public:
    // Κατασκευαστής
    GridWorld(int w = 40, int h = 40);
    
    // Προσθέτει ένα αντικείμενο στον κόσμο
    bool addObject(std::shared_ptr<WorldObject> obj);
    
    // Αφαιρεί ένα αντικείμενο από τον κόσμο
    bool removeObject(const std::string& id);
    
    // Ενημερώνει όλα τα αντικείμενα του κόσμου
    void updateAll();
    
    // Επιστρέφει το αντικείμενο σε συγκεκριμένη θέση
    std::shared_ptr<WorldObject> getObjectAt(int x, int y) const;
    
    // Εμφανίζει τον κόσμο στην οθόνη
    void render() const;
    
    // Εκτυπώνει πληροφορίες για όλα τα αντικείμενα
    void printObjectsInfo() const;
    
    // Getter methods
    int getWidth() const;
    int getHeight() const;
    int getObjectCount() const;
    
    // Δημιουργεί τυχαία αντικείμενα βάσει ρυθμίσεων
    void generateRandomObjects(const SimulationConfig& config);
    
    // Ενημερώνει τη θέση του αυτόνομου οχήματος
    void updateCarPosition(const std::shared_ptr<SelfDrivingCar>& car);
    
private:
    // Ενημερώνει τον εσωτερικό πίνακα grid
    void updateGrid();
    
    // Ελέγχει αν μια θέση είναι εντός των ορίων
    bool isWithinBounds(int x, int y) const;
};

#endif