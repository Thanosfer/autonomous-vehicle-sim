#ifndef POSITION_HPP
#define POSITION_HPP

// Απλή κλάση που αποθηκεύει συντεταγμένες και παρέχει βασικές λειτουργίες.
class Position {
private:
    int x;  // Συντεταγμένη x (οριζόντια)
    int y;  // Συντεταγμένη y (κατακόρυφη)

public:
    // Κατασκευαστής
    Position(int x = 0, int y = 0);
    
    // Getter methods
    int getX() const;
    int getY() const;
    
    // Setter methods
    void setX(int x);
    void setY(int y);
    
    // Υπερφόρτωση τελεστή για σύγκριση θέσεων
    bool operator==(const Position& other) const;
    
    // Υπολογισμός απόστασης Manhattan μεταξύ δύο θέσεων
    int manhattanDistance(const Position& other) const;
};

#endif