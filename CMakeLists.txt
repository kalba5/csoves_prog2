cmake_minimum_required(VERSION 3.26)
project(csovesProg2)

set(CMAKE_CXX_STANDARD 23)

add_executable(csovesProg2 main.cpp
        cso_fajtak/Valve.cpp
        cso_fajtak/Valve.hpp
        cso_fajtak/PipeIdom.cpp
        cso_fajtak/PipeIdom.hpp
        cso_fajtak/SimplePipe.cpp
        cso_fajtak/SimplePipe.hpp
        cso_fajtak/Source.cpp
        cso_fajtak/Source.hpp
        cso_fajtak/Sink.cpp
        cso_fajtak/Sink.hpp
        cso_fajtak/DoubleTurn.hpp
        Simulation.cpp
        Simulation.hpp
)
