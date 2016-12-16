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

#ifndef SILICONANGLESIM_RESULTS_
#define SILICONANGLESIM_RESULTS_

#include <vector>

struct ConfigData;

struct Results {
    std::vector<double> hitsValue;
    std::vector<bool> hitsBool;
    int tracks = 0;
    int gaps = 0;

    double positionLongestBinary;
    double positionLongestWeighted;
    std::vector<double> positionsBinary;
    std::vector<double> positionsWeighted;

    void calculatePositions(ConfigData *config,
                            double offset = 0);

    void printHits();
    void printStats();
    void printPositions();
};

#endif // SILICONANGLESIM_RESULTS_
