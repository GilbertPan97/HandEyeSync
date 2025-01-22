// FanucParser.h
#ifndef FANUCPARSER_H
#define FANUCPARSER_H

#include <string>
#include <vector>

class FanucParser {
public:
    // Constructor
    FanucParser();

    // Destructor
    ~FanucParser();

    // Parse the robot file
    // @param filePath: Path to the file
    // @param type: File type (e.g., "TXT" or "LS")
    // @return: Parsed data as a vector of vectors of doubles
    std::vector<std::vector<double>> parseRobFile(const std::string& filePath, const std::string& type);

private:
    // Helper functions can be added here if necessary
};

#endif // FANUCPARSER_H