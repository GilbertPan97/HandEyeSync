// ProfileParser.cpp
#include "ProfileParser.h"

#include <algorithm>
#include <iostream>
#include <regex>

ProfileParser::ProfileParser(const std::string& folderPath, const std::string& type) {
    // Normalize the file extension to lowercase
    normalizedType_ = type;
    std::transform(normalizedType_.begin(), normalizedType_.end(), normalizedType_.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    // Iterate through the directory using std::filesystem
    filePaths_.clear();
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c) {
                return std::tolower(c);
            });
            if (extension == ("." + normalizedType_)) {
                filePaths_.push_back(entry.path());
            }
        }
    }

    // Check if filePaths is empty and throw an exception
    if (filePaths_.empty()) {
        throw std::runtime_error("No files with the specified type (" + normalizedType_ +
            ") found in the directory: " + folderPath);
    }

    // Sort the file paths by numeric value extracted from filenames
    std::sort(filePaths_.begin(), filePaths_.end(), [this](const fs::path& a, const fs::path& b) {
        double numA = extractNumericValueFromFilename(a.filename().string());
        double numB = extractNumericValueFromFilename(b.filename().string());
        return numA < numB;
    });
}

ProfileParser::~ProfileParser() {}

double ProfileParser::extractNumericValueFromFilename(const std::string& filename) {
    std::regex numberRegex("\\d+(\\.\\d+)?");
    std::smatch match;
    if (std::regex_search(filename, match, numberRegex)) {
        return std::stod(match.str());
    }
    return 0.0;
}

std::vector<std::vector<std::pair<double, double>>> ProfileParser::parseProfileFiles(
    const std::string& scanLineNodeName,
    const std::function<void(int)>& progressCallback
) {
    std::vector<std::vector<std::pair<double, double>>> pointsList;

    // Process each file
    for (size_t i = 0; i < filePaths_.size(); ++i) {
        const auto& filePath = filePaths_[i];

        // Notify progress if progressCallback is provided
        if (progressCallback) {
            int progress = static_cast<int>((static_cast<double>(i + 1) / filePaths_.size()) * 100);
            progressCallback(progress);
        }

        cv::FileStorage fs(filePath.string(), cv::FileStorage::READ);
        if (!fs.isOpened()) {
            std::cerr << "Failed to open file: " << filePath.string() << std::endl;
            continue;
        }

        // Use the scanLineNodeName parameter to find the node
        cv::FileNode scanLineNode = fs[scanLineNodeName];
        if (scanLineNode.empty()) {
            // Node not found, throw an exception and exit the function
            throw std::runtime_error("Node '" + scanLineNodeName + "' not found in file: " + filePath.string());
        }

        std::vector<std::pair<double, double>> points;
        for (int i = 0; i < scanLineNode.size(); i += 3) {
            double x = static_cast<double>(scanLineNode[i]);
            double z = static_cast<double>(scanLineNode[i + 2]);
            points.emplace_back(x, z);
        }

        // Stack profiles type
        profilesType_.push_back(fs["type"]);

        pointsList.push_back(points);
        fs.release();
    }

    return pointsList;
}

std::vector<cv::Point3f> ProfileParser::parseFeatureFiles( const std::string& scanLineNodeName ) {
    std::vector<cv::Point3f> pointsList;

    // Normalize the file extension to lowercase
    std::transform(normalizedType_.begin(), normalizedType_.end(), normalizedType_.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    // Process each file
    for (size_t i = 0; i < filePaths_.size(); ++i) {
        const auto& filePath = filePaths_[i];

        cv::FileStorage fs(filePath.string(), cv::FileStorage::READ);
        if (!fs.isOpened()) {
            std::cerr << "Failed to open file: " << filePath.string() << std::endl;
            continue;
        }

        // Use the scanLineNodeName parameter to find the node
        cv::FileNode scanLineNode = fs[scanLineNodeName];
        if (scanLineNode.empty()) {
            // Node not found, throw an exception and exit the function
            throw std::runtime_error("Node '" + scanLineNodeName + "' not found in file: " + filePath.string());
        }

        float x = static_cast<float>(scanLineNode[0]);
        float y = static_cast<float>(scanLineNode[1]);
        float z = static_cast<float>(scanLineNode[2]);

        pointsList.push_back(cv::Point3f(x, y, z));
        fs.release();
    }

    return pointsList;
}

std::vector<std::string> ProfileParser::getFilePaths() {
    std::vector<std::string> filePathsStr; // Vector to store the string paths

    // Iterate over each path in filePaths_ and convert it to string
    for (const auto& path : filePaths_) {
        filePathsStr.push_back(path.string()); // Convert fs::path to string and add to the vector
    }

    return filePathsStr; // Return the vector of file paths as strings
}

std::string ProfileParser::getProfileType() {
    return profilesType_[0];
};


