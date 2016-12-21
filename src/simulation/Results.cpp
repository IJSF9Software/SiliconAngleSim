/*
* SiliconAngleSim - Simulate silicon detectors for different track angles
* Copyright (C) 2016 Tadej Novak
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>

#include "../common/ConfigData.h"
#include "Results.h"

void Results::calculatePositions(ConfigData *config,
                                 double offset)
{
    double center = ((double)config->detectorSideStrips + 0.5 + offset) * config->detectorPitch;
    double start = 0;
    double end = 0;

    double numerator = 0;
    double denominator = 0;

    std::vector<double> lengths;
    // Calculate longest "track"
    for (int i = 0; i < 2 * config->detectorSideStrips + 1; i++) {
        if (hitsBool[i]) {
            if (!start)
                start = i + 1;
        } else {
            if (start && !end) {
                end = i;
            }
        }

        if (start && end) {
            lengths.push_back(end - start);
            start = 0;
            end = 0;
        }
    }

    if (start && !end) {
        end = 2 * config->detectorSideStrips + 1;
    }

    if (start && end) {
        lengths.push_back(end - start);
        start = 0;
        end = 0;
    }

    int longest = -1;
    double longestLength = 0;
    for (size_t i = 0; i < lengths.size(); i++) {
        if (lengths[i] >= longestLength) {
            longest = i;
            longestLength = lengths[i];
        }
    }

    start = 0;
    end = 0;

    // Calculate positions
    for (int i = 0; i <= 2 * config->detectorSideStrips + 1; i++) {
        if (hitsBool[i]) {
            double value = ((double)i + 0.5) * config->detectorPitch;

            if (!start) {
                start = value;
            }

            numerator += value * hitsValue[i];
            denominator += hitsValue[i];
        } else {
            if (start && !end) {
                end = ((double)(i - 1) + 0.5) * config->detectorPitch;
            }
        }

        if (start && end) {
            positionsBinary.push_back((double)(start + end) / 2 - center);
            positionsWeighted.push_back(numerator / denominator - center);
            start = 0;
            end = 0;
            numerator = 0;
            denominator = 0;
        }
    }

    if (start && !end) {
        end = ((double)(2 * config->detectorSideStrips) + 0.5) * config->detectorPitch;
    }

    if (start && end) {
        positionsBinary.push_back((double)(start + end) / 2 - center);
        positionsWeighted.push_back(numerator / denominator - center);
    }

    if (longest >= 0 && tracks < 3) {
        positionLongestBinary = positionsBinary[longest];
        positionLongestWeighted = positionsWeighted[longest];
    } else {
        positionLongestBinary = -1e21;
        positionLongestWeighted = -1e21;
    }
}

void Results::printHits()
{
    for (bool hit : hitsBool) {
        if (hit) {
            std::cout << "o";
        } else {
            std::cout << "-";
        }
    }
    std::cout << "\t";
}

void Results::printStats()
{
    std::cout << "tracks: " << tracks << "\t"
              << "gaps: " << gaps << std::endl << "\t"
              << "Positions:" << "\t"
              << "binary: " << positionLongestBinary << "\t"
              << "weighted: " << positionLongestBinary << "\t"
              << "strips: ";
    printPositions();
    std::cout << std::endl;
}

void Results::printPositions()
{
    std::cout << "[ ";
    for (size_t i = 0; i < positionsBinary.size(); i++) {
        if (i != 0)
            std::cout << ", ";
        std::cout << positionsBinary[i];
    }
    std::cout << " ]";
}
