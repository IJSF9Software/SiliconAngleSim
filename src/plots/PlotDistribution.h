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

#ifndef SILICONANGLESIM_PLOTDISTRIBUTION_
#define SILICONANGLESIM_PLOTDISTRIBUTION_

#include <string>

class TDirectory;
class TFile;

struct ConfigData;

class PlotDistribution
{
public:
    PlotDistribution(ConfigData *config,
                     TFile *file,
                     TDirectory *folder,
                     std::string name = "");
};

#endif // SILICONANGLESIM_PLOTDISTRIBUTION_
