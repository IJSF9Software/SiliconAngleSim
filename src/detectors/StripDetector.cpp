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

#include <TCanvas.h>
#include <TGraph.h>
#include <TRandom3.h>

#include <KDetSim/KStrip.h>

#include "../common/ConfigData.h"

#include "StripDetector.h"

StripDetector::StripDetector(ConfigData *config)
    : _det(0),
      _sideStrips(config->detectorSideStrips),
      _diff(config->detectorDiffusion),
      _sign(-1),
      _neff(0),
      _averageField(3),
      _pitch(config->detectorPitch),
      _thickness(config->detectorThickness),
      _electrodeWidth(config->detectorStripWidth),
      _electrodeDepth(config->detectorStripHeight)
{
    if (config->variation1 == ConfigVariation::None)
        initDetector();
}

void StripDetector::initDetector()
{
    if (_det)
        delete _det;

    std::cout << "\t"
              << "Calculating detector fields..." << std::endl;

    TF3 *neff = new TF3("neff", "[0]", 0, 3000, 0, 3000, 0, 3000);
    neff->SetParameter(0, _sign * _neff);

    _det = new KStrip(_pitch, _electrodeWidth, _electrodeDepth, _sideStrips * 2 + 1, _thickness);
    _det->Voltage = _sign * _averageField * _thickness;
    _det->SetUpVolume(2);
    _det->SetUpElectrodes();
    _det->SetBoundaryConditions();
    _det->SetUpMaterial(0);
    _det->NeffF = neff;
    _det->CalField(0);
    _det->CalField(1);
    _det->diff = _diff ? 1 : 0;
    _det->Temperature = 263;
}

void StripDetector::setDetectorThickness(int thickness)
{
    _thickness = thickness;
}

void StripDetector::setElectrodeGeometry(int width,
                                         int depth)
{
    _electrodeWidth = width;
    _electrodeDepth = depth;
}
