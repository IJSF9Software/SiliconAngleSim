# SiliconAngleSim - Simulate silicon detectors for different track angles
# Copyright (C) 2016 Tadej Novak
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/config
)

include(ParseVersion)

project(SiliconAngleSim VERSION ${SiliconAngleSim_VERSION})

INCLUDE(GNUInstallDirs)
include(CompilerConfig)
include(ROOT)

# Locate dependencies
find_package(yaml-cpp REQUIRED)
find_package(KDetSim REQUIRED)

# Include dependencies headers
include_directories(${YAML_CPP_INCLUDE_DIR})

configure_file(scripts/setup_SiliconAngleSim.sh.in
    scripts/setup_SiliconAngleSim.sh
    @ONLY
)

# Define sources
set(Project_Sources
    src/main.cpp
    src/Application.cpp

    src/common/ConfigData.h
    src/common/ConfigManager.cpp
    src/common/Physics.cpp

    src/detectors/StripDetector.cpp
    src/detectors/StripSimulator.cpp

    src/plots/PlotDemo.cpp
    src/plots/PlotDistribution.cpp
    src/plots/PlotStyle.cpp
    src/plots/PlotVariation.cpp

    src/simulation/MCRunner.cpp
    src/simulation/Results.cpp
    src/simulation/Simulation.cpp
    src/simulation/Track.h
)

# Create the SiliconAngleSim main executable
add_executable(${PROJECT_NAME} ${Project_Sources})
target_link_libraries(${PROJECT_NAME} ${ROOT_LIBRARIES} KDetSim yaml-cpp)

# Install SiliconAngleSim
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(PROGRAMS
    ${CMAKE_SOURCE_DIR}/scripts/SiliconAngleSim_batch.sh
    ${CMAKE_BINARY_DIR}/scripts/setup_SiliconAngleSim.sh
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
