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

FILE(READ VERSION SiliconAngleSim_VERSION)
STRING(REGEX REPLACE "\n" "" SiliconAngleSim_VERSION "${SiliconAngleSim_VERSION}") # get rid of the newline at the end
MESSAGE(STATUS "Building SiliconAngleSim ${SiliconAngleSim_VERSION}")

# Find Git Version Patch
FIND_PROGRAM(GIT git)
IF(GIT AND NOT NO_GIT)
    EXECUTE_PROCESS(
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND ${GIT} rev-parse --short HEAD
        OUTPUT_VARIABLE GIT_OUT OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    IF(NOT GIT_OUT STREQUAL "")
        SET(PROJECT_VERSION_VCS "${GIT_OUT}")
        MESSAGE(STATUS "Git revision: ${GIT_OUT}")
    ENDIF()
ELSEIF(GITBUILD)
    SET(PROJECT_VERSION_VCS "${GITBUILD}")
ELSE()
    SET(GIT_OUT 0)
ENDIF()
