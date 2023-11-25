#ifndef UBITMAP_H
#define UBITMAP_H

#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <stdio.h>

struct uPixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

class uBitmap {
    
    public:

    std::vector<uPixel> data;

    unsigned int width;
    unsigned int height;


    uBitmap(unsigned int initWidth, unsigned int initHeight, uint8_t color) {
        width = initWidth;
        height = initHeight;
        data.resize(width * height);
        std::fill(data.begin(), data.end(),  uPixel{ 255, 255, 0, 255});
 

    }

    void Report() {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                printf("%d %d : %u %u %u %u\n", x, y, data[(y * width) + x].r, data[(y * width) + x].g, data[(y * width) + x].b, data[(y * width) + x].a);
            }
        }
    }

    void WriteToFile(const std::string& filename) {
        // BMP File Header
        unsigned char fileHeader[14] = {
            'B','M',    // Magic Identifier
            0,0,0,0,    // File size in bytes (to be filled later)
            0,0,0,0,    // Reserved
            54,0,0,0    // Pixel data offset
        };

        // BMP Info Header
        unsigned char infoHeader[40] = {
            40,0,0,0,   // Info header size
            0,0,0,0,    // Image width (to be filled later)
            0,0,0,0,    // Image height (to be filled later)
            1,0,        // Number of color planes
            32,0,       // Bits per pixel
            0,0,0,0,    // Compression
            0,0,0,0,    // Image size (can be set to 0 for uncompressed)
            0,0,0,0,    // X pixels per meter (not specified)
            0,0,0,0,    // Y pixels per meter (not specified)
            0,0,0,0,    // Total colors (color palette not used)
            0,0,0,0     // Important colors (all colors are important)
        };

        int fileSize = 54 + width * height * 4; // Header size + pixel data size

        // Filling the file size in file header
        fileHeader[2] = fileSize;
        fileHeader[3] = fileSize >> 8;
        fileHeader[4] = fileSize >> 16;
        fileHeader[5] = fileSize >> 24;

        // Filling width and height in info header
        infoHeader[4] = width;
        infoHeader[5] = width >> 8;
        infoHeader[6] = width >> 16;
        infoHeader[7] = width >> 24;
        infoHeader[8] = height;
        infoHeader[9] = height >> 8;
        infoHeader[10] = height >> 16;
        infoHeader[11] = height >> 24;

        std::ofstream file;
        file.open(filename, std::ios::out | std::ios::binary);
        if (!file) {
            throw std::runtime_error("Unable to open the file.");
        }

        // Write headers
        file.write(reinterpret_cast<const char*>(fileHeader), sizeof(fileHeader));
        file.write(reinterpret_cast<const char*>(infoHeader), sizeof(infoHeader));

        // Write pixel data
        file.write(reinterpret_cast<const char*>(data.data()), width * height * 4);

        file.close();
    }

};

#endif // UBITMAP_H