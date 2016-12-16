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

#include <TCanvas.h>
#include <TFile.h>
#include <TH1D.h>
#include <TLegend.h>

#include "../common/ConfigData.h"
#include "../detectors/StripSimulator.h"

#include "MCRunner.h"
#include "Results.h"

MCRunner::MCRunner(ConfigData *config,
                   TFile *file)
    : _config(config),
      _file(file),
      _steps(config->demo ? 1 : config->simulationSteps)
{
    _sim = new StripSimulator(config);
    _sim->setTrackAngle(config->simulationAngle);
}

void MCRunner::calculate()
{
    if (_config->variation3 != None) {
        int total = _config->variation1Steps.size() * _config->variation2Steps.size() * _config->variation3Steps.size();
        for (size_t k = 0; k < _config->variation3Steps.size(); k++) {
            for (size_t j = 0; j < _config->variation2Steps.size(); j++) {
                for (size_t i = 0; i < _config->variation1Steps.size(); i++) {
                    std::cout << "  variation " << ((k * _config->variation2Steps.size() + j) * _config->variation1Steps.size() + i + 1) << " of " << total << std::endl;
                    calculateVariation(_config->variation1Steps[i], _config->variation2Steps[j], _config->variation3Steps[k]);
                }
            }
        }
    } else if (_config->variation2 != None) {
        int total = _config->variation1Steps.size() * _config->variation2Steps.size();
        for (size_t j = 0; j < _config->variation2Steps.size(); j++) {
            for (size_t i = 0; i < _config->variation1Steps.size(); i++) {
                std::cout << "  variation " << (j * _config->variation1Steps.size() + i + 1) << " of " << total << std::endl;
                calculateVariation(_config->variation1Steps[i], _config->variation2Steps[j]);
            }
        }
    } else if (_config->variation1 != None) {
        int total = _config->variation1Steps.size();
        for (size_t i = 0; i < _config->variation1Steps.size(); i++) {
            std::cout << "  variation " << i + 1 << " of " << total << std::endl;
            calculateVariation(_config->variation1Steps[i]);
        }
    } else {
        calculateVariation();
    }
}

void MCRunner::calculateVariation(double variation1,
                                  double variation2,
                                  double variation3)
{
    std::string name;
    if (variation1 != -1) {
        if (_config->variation1 == DetectorThickness) {
            _sim->setDetectorThickness(variation1);
        } else if (_config->variation1 == DetectorNoise) {
            _sim->setNoise(variation1);
        } else if (_config->variation1 == DetectorFluence) {
            _sim->setFluence(variation1);
        } else if (_config->variation1 == SimulationAngle) {
            _sim->setTrackAngle(variation1);
        } else if (_config->variation1 == SimulationSteps) {
            _steps = variation1;
        } else if (_config->variation1 == SimulationOffset) {
            _sim->setTrackOffset(variation1);
        }
        name = std::to_string(variation1);
    }

    if (variation2 != -1) {
        if (_config->variation2 == DetectorThickness) {
            _sim->setDetectorThickness(variation2);
        } else if (_config->variation2 == DetectorNoise) {
            _sim->setNoise(variation2);
        } else if (_config->variation2 == DetectorFluence) {
            _sim->setFluence(variation2);
        } else if (_config->variation2 == SimulationAngle) {
            _sim->setTrackAngle(variation2);
        } else if (_config->variation2 == SimulationOffset) {
            _sim->setTrackOffset(variation2);
        }
        name += "_" + std::to_string(variation2);
    }

    if (variation3 != -1) {
        if (_config->variation3 == DetectorThickness) {
            _sim->setDetectorThickness(variation3);
        } else if (_config->variation3 == DetectorNoise) {
            _sim->setNoise(variation3);
        } else if (_config->variation3 == DetectorFluence) {
            _sim->setFluence(variation3);
        } else if (_config->variation3 == SimulationAngle) {
            _sim->setTrackAngle(variation3);
        } else if (_config->variation3 == SimulationOffset) {
            _sim->setTrackOffset(variation3);
        }
        name += "_" + std::to_string(variation3);
    }

    TDirectory *folder;
    if (name.size()) {
        folder = !_config->fields ? _file->GetDirectory(name.c_str()) : _file->mkdir(name.c_str());
        if (folder)
            folder->cd();
    }

    _sim->resetRandom();

    if (variation1 != -1 && _config->fields) {
        _sim->initDetector();
    }

    int positionBinSize = _config->detectorPitch / 2;
    int positionBinCount = 2 * _config->detectorSideStrips + 1;
    int positionBinRange = (0.5 + positionBinCount / 2) * positionBinSize;

    TH1D *initialSignal;
    if (_config->fields) {
        initialSignal = _sim->calculateInitialTrack();
        initialSignal->SetName("initial");
        if (_file)
            initialSignal->Write();
    } else if (!_config->fields && _config->events) {
        initialSignal = folder ? (TH1D *)folder->Get("initial") : 0;
        if (!initialSignal) {
            std::cerr << "error calculating events: no initial signal available for variation" << std::endl;
            return;
        }
    }

    if (!_config->events)
        return;

    TH1D *tracks = new TH1D("tracks", "Number of tracks", 10, -0.5, 9.5);
    TH1D *positionBinary = new TH1D("position_binary", "Position of track (binary)", positionBinCount, -positionBinRange, positionBinRange);
    TH1D *positionWeighted = new TH1D("position_weighted", "Position of track (weighted)", positionBinCount, -positionBinRange, positionBinRange);
    TH1D *positionBinaryAll = new TH1D("position_binary_all", "Position of track (binary, all)", positionBinCount, -positionBinRange, positionBinRange);
    TH1D *positionWeightedAll = new TH1D("position_weighted_all", "Position of track (weighted, all)", positionBinCount, -positionBinRange, positionBinRange);

    for (long long i = 0; i < _steps; i++) {
        Results *r = _sim->calculateSimulatedTrack(initialSignal);
        if (_steps <= 100) {
            std::cout << i << ":\t";
            r->printHits();
            r->printStats();
        }

        tracks->Fill(r->tracks);

        if (r->positionLongestBinary > -1e20) {
            positionBinary->Fill(r->positionLongestBinary);
            positionWeighted->Fill(r->positionLongestWeighted);
        }

        for (double p : r->positionsBinary) {
            positionBinaryAll->Fill(p);
        }

        for (double p : r->positionsWeighted) {
            positionWeightedAll->Fill(p);
        }

        delete r;
    }

    if (_config->demo)
        return;

    tracks->Write();
    positionBinary->Write();
    positionWeighted->Write();
    positionBinaryAll->Write();
    positionWeightedAll->Write();

    delete tracks;
    delete positionBinary;
    delete positionWeighted;
    delete positionBinaryAll;
    delete positionWeightedAll;
}
