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

#ifndef SILICONANGLESIM_STRIPSIMULATOR_
#define SILICONANGLESIM_STRIPSIMULATOR_

class TF1;
class TH1D;
class TRandom3;

#include "../common/Physics.h"
#include "StripDetector.h"

struct ConfigData;
struct Results;
struct Track;

class StripSimulator : public StripDetector
{
public:
    StripSimulator(ConfigData *config);

    void resetRandom();

    void setTrackAngle(double angle);
    void setTrackOffset(double offset);
    void setNoise(double noise);
    void setFluence(double fluence);
    void setTrapping(double taue,
                     double tauh);
    void setEnergyLossFitted(bool enabled);

    TH1D *calculateInitialTrack();
    Results *calculateSimulatedTrack(TH1D *initial);

private:
    double calculateTrackForStrip(int strip);

    TH1D *calculateNoise();

    TH1D *calculateLandauHist();
    double calculateLandau();

    Results *resultsForSignal(TH1D *signal);

    int stripCenter(int strip);
    Track *trackForStrip(int strip, double offset = 0.0);

    ConfigData *_config;

    unsigned int _seed;
    TRandom3 *_rnd;
    double _threshold;

    TF1 *_trappingE;
    TF1 *_trappingH;

    double _angle;
    double _offset;
    double _noise;
    Physics::Particle _particle;
    double _E;
};

#endif // SILICONANGLESIM_STRIPSIMULATOR_
