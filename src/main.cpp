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

int main(int argc, char **argv)
{
    try {
        cxxopts::Options options("SiliconAngleSim", "SiliconAngleSim - Simulate silicon detectors for different track angles");
        options.add_options()
            ("h,help", "print options")
            ("c,config", "task config", cxxopts::value<std::string>(), "FILE.yml")
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
        }
    } catch (const cxxopts::OptionException &e) {
        std::cerr << "error parsing options: " << e.what() << std::endl;
        exit(1);
    }

    TApplication app("SiliconAngleSim", &argc, argv);

    app.Run();

    return 0;
}
