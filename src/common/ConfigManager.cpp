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
#include <yaml-cpp/yaml.h>

#include "ConfigManager.h"

ConfigManager::ConfigManager(const std::string &file,
                             const std::string &output)
    : _data(new ConfigData)
{
    YAML::Node config;
    try {
        config = YAML::LoadFile(file);
    } catch (const YAML::Exception &e) {
        std::cerr << "error reading config: " << e.what() << std::endl;
        exit(2);
    }

    size_t slash = file.find_last_of('/');
    size_t start = slash != std::string::npos ? slash + 1 : 0;
    size_t end = file.find_last_of(".") - start;
    _data->name = file.substr(start, end);
    _data->file = file;
    _data->outputDir = output;
    _data->outputPrefix = output + "/" + _data->name;
    _data->outputFile = _data->outputPrefix + ".root";

    std::cout << "Loading configuration: " << (config["name"] ? config["name"].as<std::string>() : _data->name)
              << " (" << file.substr(start, file.size()) << ")" << std::endl
              << "Output file: " << _data->outputFile << std::endl;

    if (config["demo"]) {
        _data->demo = config["demo"].as<bool>();
    }

    if (config["detector"] && config["detector"].IsMap()) {
        YAML::Node detector = config["detector"];
        if (detector["pitch"]) {
            _data->detectorPitch = detector["pitch"].as<int>();
        }
        if (detector["strip-width"]) {
            _data->detectorStripWidth = detector["strip-width"].as<int>();
        }
        if (detector["thickness"]) {
            _data->detectorThickness = detector["thickness"].as<int>();
        }
        if (detector["noise"]) {
            _data->detectorNoise = detector["noise"].as<int>();
        }
        if (detector["fluence"]) {
            _data->detectorFluence = detector["fluence"].as<double>();
        }
        if (detector["average-field"]) {
            _data->detectorAverageField = detector["average-field"].as<double>();
        }
        if (detector["neff"]) {
            _data->detectorNeff = detector["neff"].as<double>();
        }
        if (detector["diffusion"]) {
            _data->detectorDiffusion = detector["diffusion"].as<bool>();
        }
    }

    if (config["simulation"] && config["simulation"].IsMap()) {
        YAML::Node simulation = config["simulation"];
        if (simulation["steps"]) {
            _data->simulationSteps = simulation["steps"].as<long long>();
        }
        if (simulation["threshold"]) {
            _data->simulationThreshold = simulation["threshold"].as<int>();
        }
        if (simulation["angle"]) {
            _data->simulationAngle = simulation["angle"].as<double>();
        }
        if (simulation["offset"]) {
            _data->simulationOffset = simulation["offset"].as<double>();
        }
    }

    if (config["plot"] && config["plot"].IsMap()) {
        YAML::Node plot = config["plot"];
        if (plot["tracks"] && plot["tracks"].IsMap()) {
            YAML::Node tracks = plot["tracks"];
            if (tracks["min"]) {
                _data->tracksMin = tracks["min"].as<double>();
            }
            if (tracks["max"]) {
                _data->tracksMax = tracks["max"].as<double>();
            }
            if (tracks["legend"]) {
                _data->tracksLegendPosition = tracks["legend"].as<int>();
            }
        }
        if (plot["position"]) {
            YAML::Node position = plot["position"];
            if (position["min"]) {
                _data->positionMin = position["min"].as<double>();
            }
            if (position["max"]) {
                _data->positionMax = position["max"].as<double>();
            }
            if (position["legend"]) {
                _data->positionLegendPosition = position["legend"].as<int>();
            }
        }
        if (plot["position-error"]) {
            YAML::Node positionError = plot["position-error"];
            if (positionError["min"]) {
                _data->positionErrorMin = positionError["min"].as<double>();
            }
            if (positionError["max"]) {
                _data->positionErrorMax = positionError["max"].as<double>();
            }
            if (positionError["legend"]) {
                _data->positionErrorLegendPosition = positionError["legend"].as<int>();
            }
        }
    }

    if (config["variations"] && config["variations"].IsSequence()) {
        YAML::Node variations = config["variations"];
        for (std::size_t i = 0; i < variations.size(); i++) {
            if (i == 3) {
                std::cout << "warning parsing config: "
                          << "only first three variations will be used" << std::endl;
                break;
            }

            YAML::Node var = variations[i];

            if (!var.IsMap()) {
                continue;
            }

            if (!var["type"] || !((var["start"] && var["end"] && var["steps"]) || var["list"])) {
                std::cout << "warning parsing config: "
                          << "variation #" << (i + 1) << " not valid and will be skipped" << std::endl;
                continue;
            }

            std::string type = var["type"].as<std::string>();
            ConfigVariation typeEnum = variationEnum(type);

            std::vector<double> steps;
            if (var["list"] && var["list"].IsSequence()) {
                for (YAML::Node n : var["list"]) {
                    steps.push_back(n.as<double>());
                }
            } else {
                double start = var["start"].as<double>();
                double end = var["end"].as<double>();
                unsigned long stepsCount = var["steps"].as<unsigned long>();
                double step = (end - start) / (stepsCount - 1);

                for (std::size_t j = 0; j < stepsCount; j++) {
                    steps.push_back(start + j * step);
                }
            }

            if (i == 0) {
                _data->variation1 = typeEnum;
                _data->variation1Steps = steps;
            } else if (i == 1) {
                _data->variation2 = typeEnum;
                _data->variation2Steps = steps;
            } else if (i == 2) {
                _data->variation3 = typeEnum;
                _data->variation3Steps = steps;
            }
        }
    }

    std::cout << "Detector: "
              << "pitch=" << _data->detectorPitch << ", "
              << "strip-width=" << _data->detectorStripWidth << ", "
              << "thickness=" << _data->detectorThickness << ", "
              << "noise=" << _data->detectorNoise << ", "
              << "fluence=" << _data->detectorFluence << ", "
              << "average-field=" << _data->detectorAverageField << ", "
              << "neff=" << _data->detectorNeff << ", "
              << "diffusion=" << _data->detectorDiffusion
              << std::endl;

    std::cout << "Simulation: "
              << "steps=" << _data->simulationSteps << ", "
              << "threshold=" << _data->simulationThreshold << ", "
              << "angle=" << _data->simulationAngle
              << std::endl;

    if (_data->variation1 != ConfigVariation::None) {
        std::cout << "Variation 1 - "
                  << variationString(_data->variation1) << ": ";
        outputSteps(_data->variation1Steps);
        std::cout << std::endl;
    }

    if (_data->variation2 != ConfigVariation::None) {
        std::cout << "Variation 2 - "
                  << variationString(_data->variation2) << ": ";
        outputSteps(_data->variation2Steps);
        std::cout << std::endl;
    }

    if (_data->variation3 != ConfigVariation::None) {
        std::cout << "Variation 3 - "
                  << variationString(_data->variation3) << ": ";
        outputSteps(_data->variation3Steps);
        std::cout << std::endl;
    }
}

void ConfigManager::setFields(bool fields)
{
    _data->fields = fields;
}

void ConfigManager::setEvents(bool events)
{
    _data->events = events;
}

void ConfigManager::setPlot(bool plot)
{
    _data->plot = plot;
}

void ConfigManager::setPlotSave(bool save)
{
    _data->plotSave = save;
}

void ConfigManager::setInteractive(bool interactive)
{
    _data->interactive = interactive;
}

void ConfigManager::setDebug(bool debug)
{
    _data->debug = debug;
}

std::string ConfigManager::variationString(ConfigVariation variation)
{
    switch (variation) {
    case DetectorThickness:
        return "thickness";
    case DetectorNoise:
        return "noise";
    case DetectorFluence:
        return "fluence";
    case SimulationAngle:
        return "angle";
    case SimulationOffset:
        return "offset";
    case SimulationSteps:
        return "steps";
    default:
        return "none";
    }
}

ConfigVariation ConfigManager::variationEnum(std::string variation)
{
    if (variation == "thickness")
        return DetectorThickness;
    else if (variation == "noise")
        return DetectorNoise;
    else if (variation == "fluence")
        return DetectorFluence;
    else if (variation == "angle")
        return SimulationAngle;
    else if (variation == "offset")
        return SimulationOffset;
    else if (variation == "steps")
        return SimulationSteps;
    else
        return None;
}

std::string ConfigManager::resultString(ConfigResult result)
{
    switch (result) {
    case TrackCount:
        return "track count";
    case PositionBinary:
        return "position (binary)";
    case PositionWeighted:
        return "position (weighted)";
    default:
        return "none";
    }
}

void ConfigManager::outputSteps(const std::vector<double> &list)
{
    std::cout << "[ ";
    for (std::size_t i = 0; i < list.size(); i++) {
        if (i != 0)
            std::cout << ", ";
        std::cout << list[i];
    }
    std::cout << " ]";
}
