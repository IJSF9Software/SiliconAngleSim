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

#ifndef SILICONANGLESIM_CONFIGMANAGER_
#define SILICONANGLESIM_CONFIGMANAGER_

#include "ConfigData.h"

class ConfigManager
{
public:
    ConfigManager(const std::string &file,
                  const std::string &output);

    ConfigData *config() { return _data; }

    void setFields(bool fields = true);
    void setEvents(bool events = true);
    void setPlot(bool plot = true);
    void setPlotSave(bool save = true);

    void setInteractive(bool interactive = true);
    void setDebug(bool debug = true);

private:
    std::string variationString(ConfigVariation parameter);
    ConfigVariation variationEnum(std::string parameter);
    std::string resultString(ConfigResult result);
    ConfigResult resultEnum(std::string result);

    void outputSteps(const std::vector<double> &list);

    ConfigData *_data;
};

#endif // SILICONANGLESIM_CONFIGMANAGER_
