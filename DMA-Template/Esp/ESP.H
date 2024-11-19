#ifndef ESP_H
#define ESP_H

#include <vector>
#include "Entity.h"

class ESP {
public:
    void FetchEntities(); // Fetch entity data using provided offsets
    void RenderESP();     // Render ESP elements with enhancements

private:
    std::vector<Entity> entities; // List of entities to render
};

#endif // ESP_H
