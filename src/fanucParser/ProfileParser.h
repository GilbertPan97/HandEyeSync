// ProfileParser.h
#ifndef PROFILEPARSER_H
#define PROFILEPARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <filesystem>
#include <stdexcept>

#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

class ProfileParser {
private:
    std::vector<fs::path> filePaths_;
    std::string normalizedType_;

public:
    // Constructor
    ProfileParser(const std::string& folderPath, const std::string& type);

    // Destructor
    ~ProfileParser();

    // Parse profile files in a directory and extract points (X, Z)
    // @param folderPath: Path to the folder containing profile files
    // @param type: File type (e.g., "yml")
    // @return: Parsed data as a vector of vectors of pairs of doubles
    std::vector<std::vector<std::pair<double, double>>> parseProfileFiles(
        const std::string& scanLineNodeName,
        const std::function<void(int)>& progressCallback = nullptr);

    std::vector<cv::Point3f> ProfileParser::parseFeatureFiles( const std::string& scanLineNodeName);

    std::vector<std::string> getFilePaths();

private:
    // Helper functions can be added here if necessary
    double extractNumericValueFromFilename(const std::string& filename);
};

#endif // PROFILEPARSER_H