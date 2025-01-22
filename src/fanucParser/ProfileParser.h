// ProfileParser.h
#ifndef PROFILEPARSER_H
#define PROFILEPARSER_H

#include <string>
#include <vector>
#include <utility>

class ProfileParser {
public:
    // Constructor
    ProfileParser();

    // Destructor
    ~ProfileParser();

    // Parse profile files in a directory and extract points (X, Z)
    // @param folderPath: Path to the folder containing profile files
    // @param type: File type (e.g., "yml")
    // @return: Parsed data as a vector of vectors of pairs of doubles
    std::vector<std::vector<std::pair<double, double>>> parseProfileFiles(const std::string& folderPath, const std::string& type);

private:
    // Helper functions can be added here if necessary
    double extractNumericValueFromFilename(const std::string& filename);
};

#endif // PROFILEPARSER_H