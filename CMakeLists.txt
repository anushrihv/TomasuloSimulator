cmake_minimum_required(VERSION 3.25)
project(TomasuloSimulator)

set(CMAKE_CXX_STANDARD 17)

add_executable(TomasuloSimulator src/main.cpp src/InstructionQueue.cpp src/instruction_queue.h src/ROBEntry.cpp src/ROBEntry.h src/ROB.cpp src/ROB.h src/ReservationStation.cpp src/ReservationStation.h src/ReservationStationEntry.cpp src/ReservationStationEntry.h src/DecodedInstruction.cpp src/DecodedInstruction.h src/RAT.cpp src/RAT.h src/RegisterFile.cpp src/RegisterFile.h src/InstructionQueue.cpp src/InstructionQueue.cpp src/InstructionQueue.cpp src/InstructionQueue.cpp src/main.cpp src/main.cpp src/main.cpp src/main.cpp)

target_include_directories(TomasuloSimulator PUBLIC src)
