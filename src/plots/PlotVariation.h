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

#ifndef SILICONANGLESIM_PLOTVARIATION_
#define SILICONANGLESIM_PLOTVARIATION_

#include <vector>

class TDirectory;
class TFile;

#import "../common/ConfigData.h"

class PlotVariation
{
public:
    PlotVariation(ConfigData *config,
                  TFile *file);

    bool process();
    void draw();
    void drawPlot(ConfigResult result);

private:
    bool writeTracks(std::string folder);
    bool writePositionBinary(std::string folder);
    bool writePositionWeighted(std::string folder);

    std::string name(ConfigResult results);
    std::string axis(ConfigVariation variation);
    std::string axis(ConfigResult results);
    std::string label(ConfigVariation variation,
                      double value);
    int radToDeg(double angle);
    std::string fluenceString(double fluence);

    ConfigData *_config;
    TDirectory *_plots;
    TFile *_file;

    std::vector<double> _resultsTracks;
    std::vector<double> _resultsPositionBinary;
    std::vector<double> _resultsPositionWeighted;
    std::vector<double> _resultsPositionBinaryErr;
    std::vector<double> _resultsPositionWeightedErr;
};

#endif // SILICONANGLESIM_PLOTVARIATION_
