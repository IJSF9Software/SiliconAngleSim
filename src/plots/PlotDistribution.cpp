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
#include <TFile.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TLine.h>

#include "../common/ConfigData.h"
#include "PlotDistribution.h"
#include "PlotStyle.h"

PlotDistribution::PlotDistribution(ConfigData *config,
                                   TFile *file,
                                   TDirectory *folder)
{
    folder->cd();

    bool save = config->variation1 == ConfigVariation::None && config->plotSave;

    TH1D *plot;

    TCanvas *c1 = new TCanvas("plot_tracks");
    PlotStyle::Canvas(c1);
    plot = (TH1D *)file->Get("tracks");
    PlotStyle::Histogram(plot);
    plot->Scale(1.0 / config->simulationSteps);
    plot->SetTitle("");
    plot->GetXaxis()->SetRangeUser(-0.5, 5.5);
    plot->SetXTitle("number of tracks");
    plot->SetYTitle("fraction");
    plot->DrawCopy("HIST");
    c1->Show();
    c1->Write();
    if (save)
        c1->SaveAs((config->outputPrefix + "_distribution_tracks.eps").c_str());

    TCanvas *c2 = new TCanvas("plot_position_binary");
    PlotStyle::Canvas(c2);
    TLegend *legend2 = new TLegend(0.12, 0.8, 0.55, 0.98);
    PlotStyle::Legend(legend2);
    plot = (TH1D *)file->Get("position_binary_all");
    PlotStyle::Histogram(plot);
    plot->Scale(1.0 / config->simulationSteps);
    plot->SetTitle("");
    plot->SetXTitle("position [#mum]");
    plot->SetYTitle("fraction of events");
    plot->SetLineStyle(2);
    legend2->AddEntry(plot, "all hits");
    plot->DrawCopy("HIST");

    plot = (TH1D *)file->Get("position_binary");
    PlotStyle::Histogram(plot);
    plot->Scale(1.0 / config->simulationSteps);
    legend2->AddEntry(plot, "with additional criteria");
    plot->DrawCopy("HIST SAME");

    legend2->Draw();
    c2->Show();
    c2->Write();
    if (save)
        c2->SaveAs((config->outputPrefix + "_distribution_position_binary.eps").c_str());

    TCanvas *c3 = new TCanvas("plot_position_comparison");
    PlotStyle::Canvas(c3);
    TLegend *legend3 = new TLegend(0.12, 0.8, 0.4, 0.98);
    PlotStyle::Legend(legend3);
    plot = (TH1D *)file->Get("position_binary");
    PlotStyle::Histogram(plot);
    plot->SetTitle("");
    plot->SetXTitle("position [#mum]");
    plot->SetYTitle("fraction of events");
    plot->SetLineStyle(2);
    legend3->AddEntry(plot, "binary");
    plot->DrawCopy("HIST");

    plot = (TH1D *)file->Get("position_weighted");
    PlotStyle::Histogram(plot);
    plot->Scale(1.0 / config->simulationSteps);
    legend3->AddEntry(plot, "weighted");
    plot->DrawCopy("HIST SAME");

    legend3->Draw();
    c3->Show();
    c3->Write();
    if (save)
        c3->SaveAs((config->outputPrefix + "_distribution_position_comparison.eps").c_str());
}
