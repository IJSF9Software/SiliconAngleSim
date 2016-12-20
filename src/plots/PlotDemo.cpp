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
#include "PlotDemo.h"
#include "PlotStyle.h"

PlotDemo::PlotDemo(ConfigData *config,
                   TFile *file)
{
    TH1D *plot = (TH1D *)file->Get("demo_ratio");
    if (!plot) {
        std::cerr << "error plotting: no calculated events" << std::endl;
        exit(3);
    }

    TDirectory *folder = file->GetDirectory("plots");
    if (!folder)
        folder = file->mkdir("plots");
    folder->cd();
    folder->Delete("*;*");

    TCanvas *c1 = new TCanvas("plot_demo_simulation");
    TLegend *legend = new TLegend(0.72, 0.65, 0.99, 0.98);

    PlotStyle::Canvas(c1);
    PlotStyle::Legend(legend);

    plot = (TH1D *)file->Get("demo_initial");
    PlotStyle::Histogram(plot);
    plot->Scale(1e-3);
    plot->SetLineStyle(3);
    plot->SetXTitle("x [#mum]");
    plot->SetYTitle("signal [1000 e_{0}]");
    plot->GetYaxis()->SetTitleOffset(0.75);
    plot->SetMinimum(-0.8);
    plot->SetMaximum(8.400);
    plot->DrawCopy("HIST");
    legend->AddEntry(plot, "#splitline{initial}{signal}");

    plot = (TH1D *)file->Get("demo_initial_landau");
    PlotStyle::Histogram(plot);
    plot->Scale(1e-3);
    plot->SetLineStyle(2);
    plot->DrawCopy("HIST SAME");
    legend->AddEntry(plot, "+ e-h pairs");

    plot = (TH1D *)file->Get("demo_initial_landau_noise");
    PlotStyle::Histogram(plot);
    plot->Scale(1e-3);
    plot->SetLineStyle(1);
    plot->DrawCopy("HIST SAME");
    legend->AddEntry(plot, "+ noise");

    legend->Draw();
    c1->Show();
    c1->Write();
    if (config->plotSave)
        c1->SaveAs((config->outputPrefix + "_demo_simulation.eps").c_str());

    TCanvas *c2 = new TCanvas("plot_demo_ratio");
    PlotStyle::Canvas(c2);

    plot = (TH1D *)file->Get("demo_ratio");
    PlotStyle::Histogram(plot);
    plot->SetXTitle("x [#mum]");
    plot->SetYTitle("signal/noise ratio");
    plot->GetYaxis()->SetTitleOffset(0.75);
    plot->DrawCopy("HIST");

    TLine *threshold = new TLine(0, config->simulationThreshold, (2 * config->detectorSideStrips + 1) * config->detectorPitch, config->simulationThreshold);
    threshold->SetLineColor(kRed);
    threshold->SetLineWidth(2);
    threshold->Draw();

    TLine *zero = new TLine(0, 0, 880, 0);
    zero->SetLineColor(kBlack);
    zero->Draw();

    c2->Show();
    c2->Write();
    if (config->plotSave)
        c2->SaveAs((config->outputPrefix + "_demo_ratio.eps").c_str());
}
