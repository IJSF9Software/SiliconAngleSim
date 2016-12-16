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

#include <TF1.h>
#include <TRandom3.h>

#include <KDetSim/KStrip.h>

#include "../common/ConfigData.h"
#include "../simulation/Results.h"
#include "../simulation/Track.h"
#include "StripSimulator.h"

StripSimulator::StripSimulator(ConfigData *config)
    : StripDetector(config),
      _config(config),
      _seed(42),
      _rnd(0),
      _threshold(5),
      _trappingE(new TF1("te", "exp(-x/[0])", 0, 1)),
      _trappingH(new TF1("th", "exp(-x/[0])", 0, 1))
{
    resetRandom();

    // Set observed particle
    _particle = _config->particle;
    _E = _config->particleEnergy;

    // Set initial offset
    setTrackOffset(_config->simulationOffset);

    // Set initial noise
    setNoise(_config->detectorNoise);

    // Init trapping functions
    setFluence(_config->detectorFluence);
}

void StripSimulator::resetRandom()
{
    if (_rnd)
        delete _rnd;

    _rnd = new TRandom3(_seed);
}

void StripSimulator::setTrackAngle(double angle)
{
    _angle = angle;
}

void StripSimulator::setTrackOffset(double offset)
{
    _offset = offset;
}

void StripSimulator::setNoise(double noise)
{
    _noise = noise;
}

void StripSimulator::setFluence(double fluence)
{
    setTrapping(1. / _config->particleBetaE / fluence * 1e-9,
                1. / _config->particleBetaH / fluence * 1e-9);
}

void StripSimulator::setTrapping(double taue,
                                 double tauh)
{
    _trappingE->SetParameter(0, taue);
    _trappingH->SetParameter(0, tauh);
}

Results *StripSimulator::calculateSimulatedTrack(TH1D *initial)
{
    // Calculate average e-h pairs created per um
    TH1D *landau = calculateLandauHist();

    // Calculate noise
    TH1D *noise = calculateNoise();

    // Calculate real signal
    TH1D *signal = (TH1D *)initial->Clone();
    signal->Multiply(landau);
    signal->Add(noise);

    // Calculate signal/noise ratio
    signal->Scale(1.0 / _noise);

    delete landau;
    delete noise;

    return resultsForSignal(signal);
}

Results *StripSimulator::resultsForSignal(TH1D *signal)
{
    Results *results = new Results();
    bool track = false;
    bool gap = false;
    for (int i = 1; i <= 2 * _sideStrips + 1; i++) {
        double value = signal->GetBinContent(i);

        results->hitsValue.push_back(value > _threshold ? value : 0);
        results->hitsBool.push_back(value > _threshold);

        if (value > _threshold) {
            if (gap) {
                gap = false;
                results->gaps++;
            }

            if (!track) {
                track = true;
                results->tracks++;
            }
        } else {
            if (track && gap) {
                track = false;
            }
            gap = true;
        }
    }

    results->calculatePositions(_config, _offset);

    delete signal;

    return results;
}

int StripSimulator::stripCenter(int strip)
{
    return (_sideStrips + strip) * _pitch + _pitch / 2;
}

Track *StripSimulator::trackForStrip(int strip,
                                     double offset)
{
    Track *track = new Track();

    double centerX = ((double)(_sideStrips - strip) + 0.5 + offset) * _pitch;
    double centerY = (double)_thickness / 2;

    double diffX = centerY * tan(_angle);

    track->x1 = centerX - diffX;
    track->x2 = centerX + diffX;
    track->y1 = 0;
    track->y2 = _thickness;

    return track;
}

TH1D *StripSimulator::calculateInitialTrack()
{
    TH1D *trackSignal = new TH1D("TS", "Track Signal", 2 * _sideStrips + 1, 0, (_sideStrips * 2 + 1) * _pitch);
    for (int i = -_sideStrips; i <= _sideStrips; i++) {
        trackSignal->Fill(stripCenter(i), calculateTrackForStrip(i));
    }
    return trackSignal;
}

double StripSimulator::calculateTrackForStrip(int strip)
{
    _det->ResetRnd(_seed);

    Track *track = trackForStrip(strip, _offset);

    _det->SetEntryPoint(track->x1, track->y1, 0.5);
    _det->SetExitPoint(track->x2, track->y2, 0.5);

    _det->MipIR(track->length(), 0);

    _det->neg->Multiply(_trappingE, _sign);
    _det->pos->Multiply(_trappingH, _sign);

    delete track;

    return _det->neg->Integral() + _det->pos->Integral();
}

TH1D *StripSimulator::calculateLandauHist()
{
    TH1D *trackLandau = new TH1D("TLD", "Landau Distribution", 2 * _sideStrips + 1, 0, (_sideStrips * 2 + 1) * _pitch);
    for (int i = -_sideStrips; i <= _sideStrips; i++) {
        trackLandau->Fill(stripCenter(i), calculateLandau());
    }
    return trackLandau;
}

double StripSimulator::calculateLandau()
{
    // ROOT wierdness
    double shift = -0.22278298;

    // Calculate track length under one strip
    Track *track = trackForStrip(0);

    double center = stripCenter(0);
    double xmin = std::max(center - _pitch / 2, track->x1);
    double xmax = std::min(center + _pitch / 2, track->x2);
    double len = _angle == 0 ? _thickness : (xmax - xmin) / sin(_angle);

    delete track;

    // Calculate properties of landau distribution for specific particle and E
    Physics::LandauInfo info(_particle, _E, len);
    double mean = (info.mpv - info.xi * shift) * 1e6;
    double sigma = info.xi * 1e6;

    // e-h pairs per um
    double eh = _rnd->Landau(mean, sigma) / 3.63 / len;
    return eh;
}

TH1D *StripSimulator::calculateNoise()
{
    double mean = 0;
    double sigma = _noise;

    TH1D *trackNoise = new TH1D("TN", "Track Noise", 2 * _sideStrips + 1, 0, (_sideStrips * 2 + 1) * _pitch);
    for (int i = -_sideStrips; i <= _sideStrips; i++) {
        trackNoise->Fill(stripCenter(i), _rnd->Gaus(mean, sigma));
    }

    return trackNoise;
}
