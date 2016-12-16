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

#include <cmath>
#include <iostream>

#include <KDetSim/fizika.h>

#include "Physics.h"

using namespace std;

double Physics::mass(Particle particle)
{
    switch (particle) {
    case Electron:
        return 0.510999;
    case Muon:
        return 105.658;
    case Proton:
        return 938.272;
    case ChargedPion:
        return 139.570;
    case NeutralPion:
        return 134.977;
    default:
        return -1;
    }
}

double Physics::betaSquared(Particle particle, double E)
{
    double gamma = Physics::gamma(particle, E);
    return 1. - (1. / (gamma * gamma));
}

double Physics::gamma(Particle particle, double E)
{
    return E / mass(particle);
}

Physics::LandauInfo::LandauInfo(Particle particle, double E, double x)
{
    double betaSquared = Physics::betaSquared(particle, E);
    double gamma = Physics::gamma(particle, E);
    double I = 173;

    xi = 0.153538 * SiZ * SiRho / (SiA * betaSquared) * x * 1e-4;
    mpv = xi * (log(2. * mass(Electron) * 1e6 * betaSquared * gamma * gamma / I) + log(xi * 1e6 / I) + 0.2 - betaSquared);
}
