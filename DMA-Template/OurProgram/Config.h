#pragma once
#include <string>
#include <fstream>
#include <iostream>

// Configuration structure
struct Config
{
    // KMbox-related settings
    std::string kmboxIP;       // KMbox IP address for Net type
    int kmboxPort;             // KMbox port for Net type
    std::string kmboxUUID;     // KMbox UUID for Net type
    int KmboxComPort;          // COM port for KMbox BPro type
    std::string kmboxType;     // Type of KMbox: "Net" or "BPro"

    // Screen settings
    int screenWidth;           // Width of the screen
    int screenHeight;          // Height of the screen

    // Example settings
    bool exampleBool;          // Example boolean flag
    float exampleFloat;        // Example floating-point value
    int exampleInt;            // Example integer value

    // ESP-related settings
    bool espEnabled = true;               // Enable or disable ESP
    float espBoxColor[3] = {1.0f, 0.0f, 0.0f}; // Default ESP box color (red)

    // Process-specific settings
    std::string targetProcessName = "cod.exe"; // Process name for the game
    uintptr_t baseAddress = 0;                // Base address of the process
};

// Function to read configuration from a file
bool ReadConfig(Config& config)
{
    std::ifstream configFile("config.cfg");
    if (!configFile.is_open()) {
        std::cerr << "Failed to open config.cfg" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        size_t pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        // Parse KMbox-related settings
        if (key == "kmboxIP") config.kmboxIP = value;
        else if (key == "kmboxPort") config.kmboxPort = std::stoi(value);
        else if (key == "kmboxUUID") config.kmboxUUID = value;
        else if (key == "KmboxComPort") config.KmboxComPort = std::stoi(value);
        else if (key == "kmboxType") config.kmboxType = value;

        // Parse screen settings
        else if (key == "screenWidth") config.screenWidth = std::stoi(value);
        else if (key == "screenHeight") config.screenHeight = std::stoi(value);

        // Parse example settings
        else if (key == "exampleBool") config.exampleBool = (value == "1");
        else if (key == "exampleFloat") config.exampleFloat = std::stof(value);
        else if (key == "exampleInt") config.exampleInt = std::stoi(value);

        // Parse ESP-related settings
        else if (key == "espEnabled") config.espEnabled = (value == "1");
        else if (key == "espBoxColor") {
            size_t rPos = value.find(',');
            size_t gPos = value.find(',', rPos + 1);
            if (rPos != std::string::npos && gPos != std::string::npos) {
                config.espBoxColor[0] = std::stof(value.substr(0, rPos));
                config.espBoxColor[1] = std::stof(value.substr(rPos + 1, gPos - rPos - 1));
                config.espBoxColor[2] = std::stof(value.substr(gPos + 1));
            }
        }

        // Parse process-specific settings
        else if (key == "targetProcessName") config.targetProcessName = value;
        else if (key == "baseAddress") config.baseAddress = std::stoull(value, nullptr, 16); // Hexadecimal value
    }

    return true;
}

// Function to save configuration to a file
bool SaveConfig(const Config& config)
{
    std::ofstream configFile("config.cfg");
    if (!configFile.is_open()) {
        std::cerr << "Failed to open config.cfg for writing" << std::endl;
        return false;
    }

    // Write KMbox-related settings
    configFile << "kmboxIP=" << config.kmboxIP << std::endl;
    configFile << "kmboxPort=" << config.kmboxPort << std::endl;
    configFile << "kmboxUUID=" << config.kmboxUUID << std::endl;
    configFile << "KmboxComPort=" << config.KmboxComPort << std::endl;
    configFile << "kmboxType=" << config.kmboxType << std::endl;

    // Write screen settings
    configFile << "screenWidth=" << config.screenWidth << std::endl;
    configFile << "screenHeight=" << config.screenHeight << std::endl;

    // Write example settings
    configFile << "exampleBool=" << (config.exampleBool ? "1" : "0") << std::endl;
    configFile << "exampleFloat=" << config.exampleFloat << std::endl;
    configFile << "exampleInt=" << config.exampleInt << std::endl;

    // Write ESP-related settings
    configFile << "espEnabled=" << (config.espEnabled ? "1" : "0") << std::endl;
    configFile << "espBoxColor=" << config.espBoxColor[0] << "," << config.espBoxColor[1] << "," << config.espBoxColor[2] << std::endl;

    // Write process-specific settings
    configFile << "targetProcessName=" << config.targetProcessName << std::endl;
    configFile << "baseAddress=" << std::hex << config.baseAddress << std::endl;

    return true;
}
