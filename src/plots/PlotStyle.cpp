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

#include <vector>

#include <TCanvas.h>
#include <TColor.h>
#include <TH1.h>
#include <TLegend.h>

#import "PlotStyle.h"

std::vector<int> markers = {20, 21, 22, 23, 29, 28};
std::vector<int> colors = {TColor::GetColorDark(1), 100, TColor::GetColorDark(8), TColor::GetColorDark(9), 95, TColor::GetColorDark(6)};

namespace PlotStyle
{
    void Canvas(TCanvas *c)
    {
        c->SetMargin(0.12, 0.01, 0.13, 0.02);
    }

    void Histogram(TH1 *plot)
    {
        plot->SetStats(false);
        plot->SetTitle("");
        plot->SetLineWidth(2);

        float size = 0.065;
        plot->GetXaxis()->SetLabelSize(size);
        plot->GetXaxis()->SetTitleSize(size);
        plot->GetYaxis()->SetLabelSize(size);
        plot->GetYaxis()->SetTitleSize(size);
    }

    void Legend(TLegend *legend)
    {
        legend->SetTextSize(0.05);
    }
}
