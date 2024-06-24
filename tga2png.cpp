#include <iostream>
#include <vector>
#include <filesystem>
#include <string>
#include <chrono>
#include <sys/stat.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

namespace fs = std::filesystem;

void processImage(const std::string& inputPath, const std::string& outputPath) {
    int width, height, channels;
    // Load the image
    unsigned char* img = stbi_load(inputPath.c_str(), &width, &height, &channels, 0);

    if (img == nullptr) {
        std::cerr << "Error loading image: " << inputPath << std::endl;
        return;
    }

    // Check if the image has an alpha channel
    if (channels == 4) {
        // Create a buffer for the new image without the alpha channel
        std::vector<unsigned char> imgWithoutAlpha(width * height * 3);

        // Copy the RGB values, ignoring the alpha channel
        for (int i = 0; i < width * height; ++i) {
            imgWithoutAlpha[i * 3 + 0] = img[i * 4 + 0];
            imgWithoutAlpha[i * 3 + 1] = img[i * 4 + 1];
            imgWithoutAlpha[i * 3 + 2] = img[i * 4 + 2];
        }

        // Save the new image as PNG
        stbi_write_png(outputPath.c_str(), width, height, 3, imgWithoutAlpha.data(), width * 3);
    }
    else if (channels == 3) {
        // If the image doesn't have an alpha channel, save it directly as PNG
        stbi_write_png(outputPath.c_str(), width, height, channels, img, width * channels);
    }
    else {
        std::cerr << "Unsupported image format: " << inputPath << std::endl;
    }

    // Free the image memory
    stbi_image_free(img);
}

void copyFileModificationTime(const fs::path& source, const fs::path& destination) {
    auto ftime = fs::last_write_time(source);
    fs::last_write_time(destination, ftime);
}

int main(int argc, char* argv[]) {
    std::string directory;

    if (argc == 2) {
        directory = argv[1];
    }
    else {
        directory = fs::current_path().string();
    }

    // Iterate through the directory
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".tga") {
            std::string inputPath = entry.path().string();
            std::string outputPath = entry.path().stem().string() + ".png";

            // Skip if the output file already exists
            if (fs::exists(outputPath)) {
                std::cout << "Skipping (already exists): " << outputPath << std::endl;
                continue;
            }

            processImage(inputPath, outputPath);
            copyFileModificationTime(inputPath, outputPath);
            std::cout << "Processed: " << inputPath << " -> " << outputPath << std::endl;
        }
    }

    return 0;
}
