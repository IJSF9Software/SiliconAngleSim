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

#ifndef SILICONANGLESIM_PLOTSTYLE_
#define SILICONANGLESIM_PLOTSTYLE_

class TCanvas;
class TGraph;
class TH1;
class TLegend;
class TMultiGraph;

namespace PlotStyle
{
    void Canvas(TCanvas *c);
    void Graph(TGraph *graph,
               int index);
    void Histogram(TH1 *plot);
    void Legend(TLegend *legend);
    void MultiGraph(TMultiGraph *plot);
}

#endif // SILICONANGLESIM_PLOTSTYLE_
