#include "ESP.h"
#include "../OurProgram/Config.h"
#include "../Imgui/imgui.h"
#include "../DMA-Library/vmmdll.h" // DMA library
#include <iostream>

// Converts world coordinates to screen coordinates
bool WorldToScreen(float* viewMatrix, float x, float y, float z, float& screenX, float& screenY) {
    float w = viewMatrix[3] * x + viewMatrix[7] * y + viewMatrix[11] * z + viewMatrix[15];
    if (w < 0.01f) return false;

    screenX = (viewMatrix[0] * x + viewMatrix[4] * y + viewMatrix[8] * z + viewMatrix[12]) / w;
    screenY = (viewMatrix[1] * x + viewMatrix[5] * y + viewMatrix[9] * z + viewMatrix[13]) / w;
    return true;
}

void ESP::FetchEntities() {
    entities.clear();

    // Read the view matrix
    float viewMatrix[16];
    uintptr_t viewMatrixBase = config.baseAddress + 0x1A32F50; // Provided offset
    if (!ReadDMA(viewMatrixBase, &viewMatrix, sizeof(viewMatrix))) {
        std::cerr << "Failed to read view matrix." << std::endl;
        return;
    }

    // Iterate through player list
    uintptr_t playerListBase = config.baseAddress + 0x13F2B0; // Provided offset
    for (int i = 0; i < 32; ++i) { // Max 32 players
        uintptr_t playerBase = 0;
        if (!ReadDMA(playerListBase + i * 0x10, &playerBase, sizeof(playerBase))) {
            continue; // Skip invalid player entries
        }

        if (playerBase) {
            Entity entity;

            // Read world position
            float worldX, worldY, worldZ;
            ReadDMA(playerBase + 0x20, &worldX, sizeof(worldX));
            ReadDMA(playerBase + 0x24, &worldY, sizeof(worldY));
            ReadDMA(playerBase + 0x28, &worldZ, sizeof(worldZ));

            if (!WorldToScreen(viewMatrix, worldX, worldY, worldZ, entity.x, entity.y)) {
                continue; // Skip entities not in view
            }

            // Read name
            char nameBuffer[32];
            ReadDMA(playerBase + 0x16C50, nameBuffer, sizeof(nameBuffer));
            entity.name = std::string(nameBuffer);

            // Read health
            ReadDMA(playerBase + 0x14A5E, &entity.health, sizeof(entity.health));

            // Read team ID
            ReadDMA(playerBase + 0x1A1E, &entity.team, sizeof(entity.team));

            // Read visibility
            int visibilityFlag;
            ReadDMA(playerBase + 0x1B2F, &visibilityFlag, sizeof(visibilityFlag));
            entity.visible = (visibilityFlag != 0);

            // Read distance
            ReadDMA(playerBase + 0x1D3C, &entity.distance, sizeof(entity.distance));

            // Push entity to the list
            entities.push_back(entity);
        }
    }
}

void ESP::RenderESP() {
    if (!config.espEnabled) return;

    ImGui::Begin("ESP Overlay", nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
    for (const auto& entity : entities) {
        // Draw entity box
        ImGui::GetWindowDrawList()->AddRect(
            ImVec2(entity.x, entity.y),
            ImVec2(entity.x + entity.width, entity.y + entity.height),
            IM_COL32(entity.visible ? 0 : 255, entity.visible ? 255 : 0, 0, 255)
        );

        // Draw name and distance
        ImGui::SetCursorPos(ImVec2(entity.x, entity.y - 15));
        ImGui::Text("%s [%.1fm]", entity.name.c_str(), entity.distance);

        // Draw health bar
        float healthBarWidth = entity.width * (entity.health / 100.0f);
        ImGui::GetWindowDrawList()->AddRectFilled(
            ImVec2(entity.x, entity.y + entity.height + 5),
            ImVec2(entity.x + healthBarWidth, entity.y + entity.height + 10),
            IM_COL32(0, 255, 0, 255)
        );
    }
    ImGui::End();
}
