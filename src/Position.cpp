#include "Position.hpp"
#include <cstdlib>  // Προσθήκη για την abs()

// Κατασκευαστής θέσης
Position::Position(int x, int y) : x(x), y(y) {}

int Position::getX() const { return x; }
int Position::getY() const { return y; }

void Position::setX(int x) { this->x = x; }
void Position::setY(int y) { this->y = y; }

// Ελέγχει αν δύο θέσεις είναι ίδιες
bool Position::operator==(const Position& other) const {
    return (x == other.x) && (y == other.y);
}

// Υπολογίζει την απόσταση Manhattan μεταξύ δύο θέσεων
int Position::manhattanDistance(const Position& other) const {
    return abs(x - other.x) + abs(y - other.y);
}