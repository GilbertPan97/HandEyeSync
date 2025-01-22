// ProfileParser.cpp
#include "ProfileParser.h"

#include <filesystem>
#include <algorithm>
#include <iostream>
#include <regex>

#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

ProfileParser::ProfileParser() {}

ProfileParser::~ProfileParser() {}

double ProfileParser::extractNumericValueFromFilename(const std::string& filename) {
    std::regex numberRegex("\\d+(\\.\\d+)?");
    std::smatch match;
    if (std::regex_search(filename, match, numberRegex)) {
        return std::stod(match.str());
    }
    return 0.0;
}

// TODO: add progress callback function
std::vector<std::vector<std::pair<double, double>>> ProfileParser::parseProfileFiles(const std::string& folderPath, const std::string& type) {
    std::vector<std::vector<std::pair<double, double>>> pointsList;

    // Normalize the file extension to lowercase
    std::string normalizedType = type;
    std::transform(normalizedType.begin(), normalizedType.end(), normalizedType.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    // Iterate through the directory using std::filesystem
    std::vector<fs::path> filePaths;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c) {
                return std::tolower(c);
            });
            if (extension == ("." + normalizedType)) {
                filePaths.push_back(entry.path());
            }
        }
    }

    // Check if filePaths is empty and throw an exception
    if (filePaths.empty()) {
        throw std::runtime_error("No files with the specified type (" + normalizedType +
            ") found in the directory: " + folderPath);
    }

    // Sort the file paths by numeric value extracted from filenames
    std::sort(filePaths.begin(), filePaths.end(), [this](const fs::path& a, const fs::path& b) {
        double numA = extractNumericValueFromFilename(a.filename().string());
        double numB = extractNumericValueFromFilename(b.filename().string());
        return numA < numB;
    });

    // Process each file
    for (const auto& filePath : filePaths) {
        cv::FileStorage fs(filePath.string(), cv::FileStorage::READ);
        if (!fs.isOpened()) {
            std::cerr << "Failed to open file: " << filePath.string() << std::endl;
            continue;
        }

        cv::FileNode scanLineNode = fs["scan_line"];
        if (scanLineNode.empty()) {
            std::cerr << "No 'scan_line' node found in file: " << filePath.string() << std::endl;
            continue;
        }

        std::vector<std::pair<double, double>> points;
        for (int i = 0; i < scanLineNode.size(); i += 3) {
            double x = static_cast<double>(scanLineNode[i]);
            double z = static_cast<double>(scanLineNode[i + 2]);
            points.emplace_back(x, z);
        }

        pointsList.push_back(points);
        fs.release();
    }

    return pointsList;
}
