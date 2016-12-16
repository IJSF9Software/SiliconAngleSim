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

#include <TFile.h>

#include "../common/ConfigData.h"

#include "MCRunner.h"
#include "Simulation.h"

Simulation::Simulation(ConfigData *config)
    : _config(config)
{
    std::string fileMode = config->fields ? "recreate" : "update";
    _file = new TFile(config->outputFile.c_str(), fileMode.c_str());

    _mc = new MCRunner(config, _file);
}

void Simulation::start()
{
    if (_config->fields || _config->events) {
        std::cout << "Calculating ";
        if (_config->fields) {
            std::cout << "fields";
        }
        if (_config->fields && _config->events) {
            std::cout << " & ";
        }
        if (_config->events) {
            std::cout << "events";
        }
        std::cout << "..." << std::endl;

        _mc->calculate();
    } else {
        std::cout << "Skipping calculation..." << std::endl;
    }
}
