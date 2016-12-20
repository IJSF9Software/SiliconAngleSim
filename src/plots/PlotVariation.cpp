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

#include <TAxis.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TLegend.h>
#include <TMultiGraph.h>

#include "PlotStyle.h"
#include "PlotVariation.h"

PlotVariation::PlotVariation(ConfigData *config,
                             TFile *file)
    : _config(config),
      _file(file)

{
    _plots = _file->GetDirectory("plots");
    if (!_plots)
        _plots = _file->mkdir("plots");
    _plots->Delete("*;*");

    if (process())
        draw();
}

bool PlotVariation::process()
{
    bool valid = true;
    if (_config->variation2 != None) {
        for (size_t j = 0; j < _config->variation2Steps.size(); j++) {
            for (size_t i = 0; i < _config->variation1Steps.size(); i++) {
                std::string folder = std::to_string(_config->variation1Steps[i]) + "_" + std::to_string(_config->variation2Steps[j]);

                valid = writeTracks(folder);
                valid = writePositionBinary(folder);
                valid = writePositionWeighted(folder);

                if (!valid)
                    return false;
            }
        }
    } else if (_config->variation1 != None) {
        for (size_t i = 0; i < _config->variation1Steps.size(); i++) {
            std::string folder = std::to_string(_config->variation1Steps[i]);

            valid = writeTracks(folder);
            valid = writePositionBinary(folder);
            valid = writePositionWeighted(folder);

            if (!valid)
                return false;
        }
    }

    if (_config->variation1 == SimulationAngle) {
        for (size_t i = 0; i < _config->variation1Steps.size(); i++) {
            _config->variation1Steps[i] = radToDeg(_config->variation1Steps[i]);
        }
    }
    if (_config->variation2 == SimulationAngle) {
        for (size_t i = 0; i < _config->variation2Steps.size(); i++) {
            _config->variation2Steps[i] = radToDeg(_config->variation2Steps[i]);
        }
    }
    if (_config->variation1 == SimulationOffset) {
        for (size_t i = 0; i < _config->variation1Steps.size(); i++) {
            _config->variation1Steps[i] = _config->variation1Steps[i] * _config->detectorPitch;
        }
    }

    return true;
}

void PlotVariation::draw()
{
    drawPlot(TrackCount);
    drawPlot(PositionBinary);
    drawPlot(PositionBinaryStdDeviation);
    drawPlot(PositionWeighted);
    drawPlot(PositionWeightedStdDeviation);
}

void PlotVariation::drawPlot(ConfigResult result)
{
    std::vector<double> results;
    switch (result) {
    case TrackCount:
        results = _resultsTracks;
        break;
    case PositionBinary:
        results = _resultsPositionBinary;
        break;
    case PositionBinaryStdDeviation:
        results = _resultsPositionBinaryErr;
        break;
    case PositionWeighted:
        results = _resultsPositionWeighted;
        break;
    case PositionWeightedStdDeviation:
        results = _resultsPositionWeightedErr;
        break;
    default:
        break;
    }

    std::string plot_name = name(result);
    TCanvas *c = new TCanvas(("plot_" + plot_name).c_str());
    PlotStyle::Canvas(c);
    if (result == TrackCount) {
        if (_config->tracksMin != -1 && _config->tracksMin > 0.5) {
            c->SetLeftMargin(0.14);
        }
    }

    TMultiGraph *mg = new TMultiGraph(plot_name.c_str(), "");
    std::vector<TGraph *> graphs;

    if (_config->variation1 == SimulationSteps) {
        c->SetLogx();
    }

    if (_config->variation2 != None) {
        int total = _config->variation1Steps.size();
        for (size_t j = 0; j < _config->variation2Steps.size(); j++) {
            std::string name = "graph_" + std::to_string(j);
            TGraph *graph = new TGraph(total, &_config->variation1Steps[0], &results[j * total]);
            PlotStyle::Graph(graph, j);
            graph->SetName(name.c_str());
            graph->SetTitle(label(_config->variation2, _config->variation2Steps[j]).c_str());
            graphs.push_back(graph);
            mg->Add(graph);
        }
    } else if (_config->variation1 != None) {
        int total = _config->variation1Steps.size();
        std::string name = "graph_single";
        TGraph *graph = new TGraph(total, &_config->variation1Steps[0], &results[0]);
        PlotStyle::Graph(graph, 0);
        graph->SetName(name.c_str());
        graph->SetTitle(name.c_str());
        graphs.push_back(graph);
        mg->Add(graph);
    }

    mg->Draw("ALP");
    mg->GetXaxis()->SetTitle(axis(_config->variation1).c_str());
    mg->GetYaxis()->SetTitle(axis(result).c_str());

    TLegend *legend = 0;
    if (result == TrackCount) {
        if (_config->tracksMin != -1) {
            mg->SetMinimum(_config->tracksMin);
            if (_config->tracksMin > 0.5) {
                c->SetLeftMargin(0.14);
                mg->GetYaxis()->SetTitleOffset(1.15);
            }
        }
        mg->SetMaximum(_config->tracksMax != -1 ? _config->tracksMax : 1);

        if (_config->tracksLegendPosition) {
            double lx1, lx2, ly1, ly2;
            if (_config->tracksLegendPosition == 2 || _config->tracksLegendPosition == 3) {
                lx1 = c->GetLeftMargin();
                lx2 = lx1 + 0.4;
            } else {
                lx2 = 1 - c->GetRightMargin();
                lx1 = lx2 - 0.4;
            }
            if (_config->tracksLegendPosition == 1 || _config->tracksLegendPosition == 2) {
                ly2 = 1 - c->GetTopMargin();
                ly1 = ly2 - (0.1 * _config->variation2Steps.size());
            } else {
                ly1 = c->GetBottomMargin();
                ly2 = ly1 + (0.1 * _config->variation2Steps.size());
            }
            legend = c->BuildLegend(lx1, ly1, lx2, ly2);
        }
    } else {
        if (_config->variation1 != SimulationOffset && _config->variation1 != SimulationSteps) {
            mg->SetMinimum(_config->positionMin != -1 ? _config->positionMin : -20);
            mg->SetMaximum(_config->positionMax != -1 ? _config->positionMax : 80);
        }

        if (_config->positionLegendPosition) {
            double lx1, lx2, ly1, ly2;
            if (_config->positionLegendPosition == 2 || _config->positionLegendPosition == 3) {
                lx1 = c->GetLeftMargin();
                lx2 = lx1 + 0.4;
            } else {
                lx2 = 1 - c->GetRightMargin();
                lx1 = lx2 - 0.4;
            }
            if (_config->positionLegendPosition == 1 || _config->positionLegendPosition == 2) {
                ly2 = 1 - c->GetTopMargin();
                ly1 = ly2 - (0.1 * _config->variation2Steps.size());
            } else {
                ly1 = c->GetBottomMargin();
                ly2 = ly1 + (0.1 * _config->variation2Steps.size());
            }
            legend = c->BuildLegend(lx1, ly1, lx2, ly2);
        }
    }

    PlotStyle::MultiGraph(mg);
    if (legend) {
        PlotStyle::Legend(legend);
    }

    _plots->cd();

    c->Show();
    c->Write();
    if (_config->plotSave)
        c->SaveAs((_config->outputPrefix + "_" + plot_name + ".eps").c_str());
}

bool PlotVariation::writeTracks(std::string folder)
{
    TH1D *histogram = (TH1D *)_file->Get((folder + "/" + "tracks").c_str());
    if (!histogram) {
        std::cerr << "error plotting: no calculated events" << std::endl;
        return false;
    }

    _resultsTracks.push_back(histogram->GetBinContent(2) / histogram->Integral());
    return true;
}

bool PlotVariation::writePositionBinary(std::string folder)
{
    TH1D *histogram = (TH1D *)_file->Get((folder + "/" + "position_binary").c_str());
    if (!histogram) {
        std::cerr << "error plotting: no calculated events" << std::endl;
        return false;
    }

    _resultsPositionBinary.push_back(histogram->GetMean());
    _resultsPositionBinaryErr.push_back(histogram->GetStdDev());
    return true;
}

bool PlotVariation::writePositionWeighted(std::string folder)
{
    TH1D *histogram = (TH1D *)_file->Get((folder + "/" + "position_weighted").c_str());
    if (!histogram) {
        std::cerr << "error plotting: no calculated events" << std::endl;
        return false;
    }

    _resultsPositionWeighted.push_back(histogram->GetMean());
    _resultsPositionWeightedErr.push_back(histogram->GetStdDev());
    return true;
}

std::string PlotVariation::name(ConfigResult results)
{
    switch (results) {
    case TrackCount:
        return "tracks";
    case PositionBinary:
        return "position_binary";
    case PositionWeighted:
        return "position_weighted";
    case PositionBinaryStdDeviation:
        return "position_binary_stddev";
    case PositionWeightedStdDeviation:
        return "position_weighted_stddev";
    default:
        return "";
    }
}

std::string PlotVariation::axis(ConfigVariation variation)
{
    switch (variation) {
    case DetectorThickness:
        return "detector thickness [#mum]";
    case SimulationAngle:
        return "angle [#circ]";
    case SimulationOffset:
        return "shift from center of the strip [#mum]";
    case SimulationSteps:
        return "# of events";
    default:
        return "";
    }
}

std::string PlotVariation::axis(ConfigResult results)
{
    switch (results) {
    case TrackCount:
        return "fraction of single detected tracks";
    case PositionBinary:
    case PositionWeighted:
        return "difference from correct position [#mum]";
    case PositionBinaryStdDeviation:
    case PositionWeightedStdDeviation:
        return "#sigma of position difference [#mum]";
    default:
        return "";
    }
}

std::string PlotVariation::label(ConfigVariation variation,
                                 double value)
{
    switch (variation) {
    case DetectorThickness:
        return "D = " + std::to_string((int)round(value)) + " #mum";
    case DetectorNoise:
        return "ENC = " + std::to_string((int)round(value));
    case SimulationAngle:
        return "#alpha = " + std::to_string((int)value) + "#circ";
    case SimulationOffset:
        return "track shift [#mum]";
    case DetectorFluence:
        return "#phi_{eq} = " + fluenceString(value) + " cm^{-2}";
    default:
        return "";
    }
}

int PlotVariation::radToDeg(double angle)
{
    return angle / M_PI * 180;
}

std::string PlotVariation::fluenceString(double fluence)
{
    std::string f;
    if (fluence < 1e15) {
        f = "0." + std::to_string((int)(fluence / 1e14));
    } else {
        f = std::to_string((int)(fluence / 1e15)) + ".0";
    }
    return f + " #times 10^{15}";
}
