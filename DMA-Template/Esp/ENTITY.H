#ifndef ENTITY_H
#define ENTITY_H

#include <string>

struct Entity {
    float x;          // Screen X coordinate
    float y;          // Screen Y coordinate
    float width;      // Width of ESP box
    float height;     // Height of ESP box
    std::string name; // Entity's name
    float health;     // Health percentage (0 to 100)
    int team;         // Team ID (e.g., 1 for allies, 2 for enemies)
    bool visible;     // Visibility status (true if visible)
    float distance;   // Distance from the player
};

#endif // ENTITY_H
