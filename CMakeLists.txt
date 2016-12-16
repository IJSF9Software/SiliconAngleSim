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

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(SiliconAngleSim)

# Various compiler configs
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")

# Make final release build smaller
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s")
endif()

set(ROOTSYS "" CACHE STRING "ROOT path")

# Tell CMake where to find the ROOT installation.
# This requires ROOT built with CMake.
list(APPEND CMAKE_PREFIX_PATH ${ROOTSYS})

# Locate the ROOT and yaml-cpp dependencies
find_package(ROOT REQUIRED)
find_package(yaml-cpp REQUIRED)
# find_package(KDetSim REQUIRED)

# Define ROOT functions and macros
include(${ROOT_USE_FILE})

# Define sources
set(Project_Sources
    src/main.cpp

    src/common/ConfigData.h
    src/common/ConfigManager.cpp
    src/common/Physics.cpp

    src/detectors/StripDetector.cpp
    src/detectors/StripSimulator.cpp

    src/simulation/MCRunner.cpp
    src/simulation/Results.cpp
    src/simulation/Simulation.cpp
    src/simulation/Track.h
)

# Create the SiliconAngleSim main executable
add_executable(${PROJECT_NAME} ${Project_Sources})
target_link_libraries(${PROJECT_NAME} ${ROOT_LIBRARIES} KDetSim yaml-cpp)
