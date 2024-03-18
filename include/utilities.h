#pragma once

// Indices (locations) of queue families (if they exist at all)

struct QueueFamilyIndices {
    int graphicsFamily = -1;

    // Check if queue familes are valid
    bool isValid(){
        return graphicsFamily >=0;
    }
};