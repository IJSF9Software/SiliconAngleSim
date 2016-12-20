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
#include <TLine.h>

#include "../common/ConfigData.h"
#include "PlotDistribution.h"
#include "PlotStyle.h"

PlotDistribution::PlotDistribution(ConfigData *config,
                                   TFile *file,
                                   TDirectory *folder,
                                   std::string name)
{
    if (!folder) {
        folder = file->GetDirectory("plots");
        if (!folder)
            folder = file->mkdir("plots");
    }
    folder->cd();
    folder->Delete("*;*");

    bool save = config->variation1 == ConfigVariation::None && config->plotSave;

    TH1D *plot;

    TCanvas *c1 = new TCanvas("plot_tracks");
    PlotStyle::Canvas(c1);
    plot = (TH1D *)file->Get(((name.size() ? name + "/" : "") + "tracks").c_str());
    if (!plot) {
        std::cerr << "error plotting: no calculated events" << std::endl;
        return;
    }

    PlotStyle::Histogram(plot);
    plot->Scale(1.0 / config->simulationSteps);
    plot->GetXaxis()->SetRangeUser(-0.5, 5.5);
    plot->SetXTitle("number of tracks");
    plot->SetYTitle("fraction");
    plot->DrawCopy("HIST");
    c1->Write();
    c1->Show();
    if (save) {
        c1->SaveAs((config->outputPrefix + "_distribution_tracks.eps").c_str());
    } else if (!config->interactive) {
        delete c1;
    }

    TCanvas *c2 = new TCanvas("plot_position_binary");
    PlotStyle::Canvas(c2);
    TLegend *legend2 = new TLegend(0.12, 0.8, 0.55, 0.98);
    PlotStyle::Legend(legend2);
    plot = (TH1D *)file->Get(((name.size() ? name + "/" : "") + "position_binary_all").c_str());
    PlotStyle::Histogram(plot);
    plot->Scale(1.0 / config->simulationSteps);
    plot->SetXTitle("position [#mum]");
    plot->SetYTitle("fraction of events");
    plot->SetLineStyle(2);
    legend2->AddEntry(plot, "all hits");
    plot->DrawCopy("HIST");

    plot = (TH1D *)file->Get(((name.size() ? name + "/" : "") + "position_binary").c_str());
    PlotStyle::Histogram(plot);
    plot->Scale(1.0 / config->simulationSteps);
    legend2->AddEntry(plot, "with additional criteria");
    plot->DrawCopy("HIST SAME");

    legend2->Draw();
    c2->Write();
    c2->Show();
    if (save) {
        c2->SaveAs((config->outputPrefix + "_distribution_position_binary.eps").c_str());
    } else if (!config->interactive) {
        delete legend2;
        delete c2;
    }

    TCanvas *c3 = new TCanvas("plot_position_comparison");
    PlotStyle::Canvas(c3);
    TLegend *legend3 = new TLegend(0.12, 0.8, 0.4, 0.98);
    PlotStyle::Legend(legend3);
    plot = (TH1D *)file->Get(((name.size() ? name + "/" : "") + "position_binary").c_str());
    PlotStyle::Histogram(plot);
    plot->SetXTitle("position [#mum]");
    plot->SetYTitle("fraction of events");
    plot->SetLineStyle(2);
    legend3->AddEntry(plot, "binary");
    plot->DrawCopy("HIST");

    plot = (TH1D *)file->Get(((name.size() ? name + "/" : "") + "position_weighted").c_str());
    PlotStyle::Histogram(plot);
    plot->Scale(1.0 / config->simulationSteps);
    legend3->AddEntry(plot, "weighted");
    plot->DrawCopy("HIST SAME");

    legend3->Draw();
    c3->Write();
    c3->Show();
    if (save) {
        c3->SaveAs((config->outputPrefix + "_distribution_position_comparison.eps").c_str());
    } else if (!config->interactive) {
        delete legend3;
        delete c3;
    }
}
