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

#ifndef SILICONANGLESIM_MCRUNNER_
#define SILICONANGLESIM_MCRUNNER_

#include <string>

class TFile;

struct ConfigData;
class StripSimulator;

class MCRunner
{
public:
    MCRunner(ConfigData *config,
             TFile *file);

    void calculate();

private:
    void calculateVariation(double variation1 = -1,
                            double variation2 = -1,
                            double variation3 = -1);

    ConfigData *_config;
    TFile *_file;

    long long _steps;

    StripSimulator *_sim;
};

#endif // SILICONANGLESIM_MCRUNNER_
