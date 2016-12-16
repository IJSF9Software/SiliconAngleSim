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

#include <TApplication.h>

#include "external/cxxopts.h"
#include "common/ConfigManager.h"

int main(int argc, char **argv)
{
    std::string config, output;
    bool all = false, fields = false, events = false, plot = false;

    try {
        cxxopts::Options options("SiliconAngleSim", "SiliconAngleSim - Simulate silicon detectors for different track angles");
        options.add_options()
            ("h,help", "print options")
            ("c,config", "task config", cxxopts::value<std::string>(), "FILE.yaml")
            ("o,output", "output folder", cxxopts::value<std::string>()->default_value("."), "PATH");

        options.add_options("Separate steps")
            ("f,fields", "calculate fields")
            ("e,events", "generate events")
            ("p,plot", "plot results");

        options.parse_positional("config");
        options.parse(argc, argv);

        if (options.count("help")) {
            std::cout << options.help({"", "Separate steps"}) << std::endl;
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
    } catch (const cxxopts::OptionException &e) {
        std::cerr << "error parsing options: " << e.what() << std::endl;
        exit(1);
    }

    TApplication app("SiliconAngleSim", &argc, argv);

    ConfigManager configuration(config, output);
    configuration.setFields(fields || all);
    configuration.setEvents(events || all);
    configuration.setPlot(plot || all);

    app.Run();

    return 0;
}