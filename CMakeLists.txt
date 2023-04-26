cmake_minimum_required(VERSION 3.25)
project(TomasuloSimulator)

set(CMAKE_CXX_STANDARD 17)

add_executable(TomasuloSimulator src/main.cpp src/InstructionQueue.cpp src/headerFiles/instruction_queue.h src/ROBEntry.cpp src/headerFiles/ROBEntry.h src/ROB.cpp src/headerFiles/ROB.h src/ReservationStation.cpp src/headerFiles/ReservationStation.h src/ReservationStationEntry.cpp src/headerFiles/ReservationStationEntry.h src/DecodedInstruction.cpp src/headerFiles/DecodedInstruction.h src/RAT.cpp src/headerFiles/RAT.h src/RegisterFile.cpp src/headerFiles/RegisterFile.h src/InstructionQueue.cpp src/InstructionQueue.cpp src/InstructionQueue.cpp src/InstructionQueue.cpp src/main.cpp src/main.cpp src/main.cpp src/main.cpp src/IntFunctionalUnit.cpp src/headerFiles/IntFunctionalUnit.h src/FPAddFunctionalUnit.cpp src/headerFiles/FPAddFunctionalUnit.h src/FPMulFunctionalUnit.cpp src/headerFiles/FPMulFunctionalUnit.h src/FPDivFunctionalUnit.cpp src/headerFiles/FPDivFunctionalUnit.h)

target_include_directories(TomasuloSimulator PUBLIC src)
