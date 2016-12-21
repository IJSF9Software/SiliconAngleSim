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

#include "external/cxxopts.h"
#include "common/Common.h"
#include "common/ConfigManager.h"
#include "simulation/Simulation.h"

#include "Application.h"

int main(int argc, char **argv)
{
    std::string config, output;
    bool all = false, fields = false, events = false, plot = false,
         interactive = false, save = false, debug = false;

    std::cout << "SiliconAngleSim" << " v" << SiliconAngleSim::version();
    if (SiliconAngleSim::version_vcs().size())
        std::cout << " (" << SiliconAngleSim::version_vcs() << ")";
    std::cout << std::endl;

    try {
        cxxopts::Options options("SiliconAngleSim", "SiliconAngleSim - Simulate silicon detectors for different track angles");
        options.add_options()
            ("h,help", "print options")
            ("c,config", "task config", cxxopts::value<std::string>(), "FILE.yaml")
            ("o,output", "output folder", cxxopts::value<std::string>()->default_value("."), "PATH")
            ("d,debug", "debug output - WARNING: may be quite verbose");

        options.add_options("Separate steps")
            ("f,fields", "calculate fields")
            ("e,events", "generate events")
            ("p,plot", "plot results");

        options.add_options("Plots")
            ("i,interactive", "display canvases after plotting")
            ("s,save", "save plots into file");

        options.parse_positional("config");
        options.parse(argc, argv);

        if (options.count("help")) {
            std::cout << options.help({"", "Separate steps", "Plots"}) << std::endl;
            exit(0);
        }

        if (!options.count("config")) {
            throw cxxopts::missing_option_exception("config");
        } else {
            config = options["config"].as<std::string>();
        }

        output = options["output"].as<std::string>();
        fields = options.count("fields");
        events = options.count("events");
        plot = options.count("plot");
        all = (fields && events && plot) || (!fields && !events && !plot);

        interactive = options.count("interactive");
        save = options.count("save");
        debug = options.count("debug");
    } catch (const cxxopts::OptionException &e) {
        std::cerr << "error parsing options: " << e.what() << std::endl;
        exit(1);
    }

    Application app(&argc, argv, !interactive);

    ConfigManager configuration(config, output);
    configuration.setFields(fields || all);
    configuration.setEvents(events || all);
    configuration.setPlot(plot || all);
    configuration.setPlotSave(save);
    configuration.setInteractive(interactive);
    configuration.setDebug(debug);

    Simulation simulation(configuration.config());
    simulation.start();

    if (interactive) {
        app.run();
    }

    return 0;
}
