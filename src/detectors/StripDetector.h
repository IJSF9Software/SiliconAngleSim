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

#ifndef SILICONANGLESIM_STRIPDETECTOR_
#define SILICONANGLESIM_STRIPDETECTOR_

class KStrip;

struct ConfigData;

class StripDetector
{
public:
    StripDetector(ConfigData *config);

    void initDetector();

    void setDetectorThickness(int thickness);
    void setElectrodeGeometry(int width,
                              int depth);

protected:
    KStrip *_det;
    int _sideStrips;

    bool _diff;
    int _sign;
    int _neff;
    int _averageField;

    int _pitch;
    int _thickness;

    int _electrodeWidth;
    int _electrodeDepth;
};

#endif // SILICONANGLESIM_STRIPDETECTOR_
