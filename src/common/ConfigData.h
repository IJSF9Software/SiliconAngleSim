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

#ifndef SILICONANGLESIM_CONFIGDATA_
#define SILICONANGLESIM_CONFIGDATA_

#include <cmath>
#include <string>
#include <vector>

#include "Physics.h"

enum ConfigVariation {
    None,
    DetectorThickness,
    DetectorNoise,
    DetectorFluence,
    SimulationAngle,
    SimulationOffset,
    SimulationSteps
};

enum ConfigResult {
    All,
    TrackCount,
    PositionBinary,
    PositionWeighted
};

struct ConfigData {
    std::string name;
    std::string file;
    std::string outputDir;
    std::string outputPrefix;
    std::string outputFile;

    bool fields = false;
    bool events = false;
    bool plot = false;
    bool plotSave = false;

    bool demo = false;

    bool detectorDiffusion = true;
    int detectorPitch = 80;
    int detectorThickness = 300;
    int detectorStripWidth = 20;
    int detectorStripHeight = 2;
    int detectorSideStrips = 5;
    int detectorNoise = 500;
    double detectorFluence = 5e14;

    Physics::Particle particle = Physics::Proton;
    double particleEnergy = 12e3;

    double particleBetaE = 3.5e-16;
    double particleBetaH = 4.5e-16;

    long long simulationSteps = 1000;
    double simulationAngle = M_PI / 4;
    double simulationOffset = 0;
    double simulationThreshold = 5;

    ConfigVariation variation1 = ConfigVariation::None;
    std::vector<double> variation1Steps;

    ConfigVariation variation2 = ConfigVariation::None;
    std::vector<double> variation2Steps;

    ConfigVariation variation3 = ConfigVariation::None;
    std::vector<double> variation3Steps;
};

#endif // SILICONANGLESIM_CONFIGDATA_
