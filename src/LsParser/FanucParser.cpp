// FanucParser.cpp
#include "fanucParser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

FanucParser::FanucParser() {}

FanucParser::~FanucParser() {}

std::vector<std::vector<double>> FanucParser::parseRobFile(const std::string& filePath, const std::string& type) {
    // Vector to store the data
    std::vector<std::vector<double>> data;

    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open the file" << std::endl;
        return data; // Return an empty data vector
    }

    if (type == "TXT" || type == "txt") {
        std::ifstream infile(filePath);
        std::string txt_line;
        while (std::getline(infile, txt_line)) {
            // Read as xyzwpr
            double x, y, z, w, p, r;

            std::istringstream iss(txt_line);

            iss >> x >> y >> z >> w >> p >> r;

            data.push_back({x, y, z, w, p, r});
        }
    } else if (type == "LS" || type == "ls") {
        std::regex pattern_xyz("X=([-+]?[0-9]*\\.?[0-9]+)mm,Y=([-+]?[0-9]*\\.?[0-9]+)mm,Z=([-+]?[0-9]*\\.?[0-9]+)mm");
        std::regex pattern_wpr("W=([-+]?[0-9]*\\.?[0-9]+)deg,P=([-+]?[0-9]*\\.?[0-9]+)deg,R=([-+]?[0-9]*\\.?[0-9]+)deg");
        std::regex pattern_ex("E1=([-+]?[0-9]*\\.?[0-9]+)mm");

        double ex; // point data and ex-axis data
        std::string line; // line string
        double x = 0, y = 0, z = 0, w = 0, p = 0, r = 0;
        bool parsingEnabled = false; // parse enable signal
        while (std::getline(inputFile, line)) {

            if (line.find("/POS") != std::string::npos) {
                parsingEnabled = true;
                continue;
            }
            if (line.find("/END") != std::string::npos) {
                parsingEnabled = false;
                break;
            }

            // clear every line string \t and \s
            std::regex spaceRegex("\\s+");
            line = std::regex_replace(line, spaceRegex, "");

            std::smatch match;
            if (parsingEnabled) {
                if (std::regex_search(line, match, pattern_xyz)) {
                    x = std::stod(match[1]);
                    y = std::stod(match[2]);
                    z = std::stod(match[3]);
                } else if (std::regex_search(line, match, pattern_wpr)) {
                    w = std::stod(match[1]);
                    p = std::stod(match[2]);
                    r = std::stod(match[3]);
                    data.push_back({x, y, z, w, p, r});
                } else if (std::regex_search(line, match, pattern_ex)) {
                    ex = std::stod(match[1]);
                    x -= ex; // Adjust x value based on ex
                    data.push_back({x, y, z, w, p, r});
                }
            }
        }

        inputFile.close();
    }

    return data;
}
