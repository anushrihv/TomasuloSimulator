#include <iostream>
#include <fstream>
#include "headerFiles/instruction_queue.h"
#include "headerFiles/ROB.h"
#include "headerFiles/ReservationStation.h"
#include "sstream"
#include <string>
#include "headerFiles/DecodedInstruction.h"
#include "headerFiles/RAT.h"
#include <filesystem>

using namespace std;

int NF = 0, NI = 0, NW = 0, NR = 0, NB = 0;
string file_name;
filesystem::path file_path;
ReservationStation intReservationStation(4);
ReservationStation loadBuffer(2);
ReservationStation storeBuffer(2);
ReservationStation fpAdd(3);
ReservationStation fpMul(4);
ReservationStation fpDiv(2);
ReservationStation bu(1); // TODO mostly not gonna use this
InstructionQueue instruction_queue;
RAT registerAllocationTable;

bool fileExists(string filePath) {
    return filesystem::exists(filePath);
}

/**
 * fetch NF number of instructions from the input file and place it in InstructionQueue
 */
bool fetchInstructions(int startLineNumber) {
    string instruction;
    int lineNumber = 1;
    bool linesRead = false;

    ifstream infile(file_name);
    if (infile.fail()) {
        cout << "Failed to open file " << file_name << endl;
        return false;
    }

    while (lineNumber < startLineNumber) {
        getline(infile, instruction);
        lineNumber++;
    }

    cout << "EXECUTING FETCH " << std::endl;
    while (lineNumber < startLineNumber + NF) {
        // get NF instructions from the file
        getline(infile, instruction);
        cout << instruction << std::endl;

        if (infile.eof()) {
            break;
        }
        // put it in the instruction queue
        instruction_queue.addNewInstruction(instruction);
        linesRead = true;
        lineNumber++;
    }

    return linesRead;
}

bool finishProcessing() {
    return true;
}

bool is_integer(string s) {
    try {
        int i = stoi(s);
        return true;
    } catch (invalid_argument &e) {
        return false;
    }
}

bool isOperandReady(string register_name) {
    if(is_integer(register_name)) {
        return true;
    } else if (registerAllocationTable.mappings[register_name] == register_name){
        return true;
    } else {
        return false;
    }
}

DecodedInstruction swapSourceAndDest(DecodedInstruction decodedInstruction) {
    if(decodedInstruction.instruction_type == "fsd") {
        // this changes source and destination for store instruction
        string destination = decodedInstruction.source2;
        decodedInstruction.source2 = decodedInstruction.destination;
        decodedInstruction.destination = destination;
    }

    return decodedInstruction;
}

void renameRegisters(DecodedInstruction decodedInstruction) {
    // get the destination register
    // get a free physical register
    // update RAT table to use
}

void decodeInstructions(ROB reorder_buffer) {
    for (int i = 0; i < NR; i++) {
        if (!instruction_queue.hasNextInstruction()) {
            return;
        }

        if (reorder_buffer.isFull()) {
            cout << "ROB is full. Cannot decode any new instructions";
            return;
        }

        // based on the instruction type, check if the corresponding reservation station is free
        string next_instruction = instruction_queue.peekNextInstruction();
        cout << "Next instruction available to decode " << next_instruction << std::endl;

        istringstream stringstream(next_instruction);
        string operation, instructionID;

        stringstream >> instructionID;
        stringstream >> operation;

        ReservationStation *reservationStationToAdd;
        if (operation == "fld" && loadBuffer.isFull()) {
            cout << "Load buffer is full";
            return;
        } else if (operation == "fld"){
            reservationStationToAdd = &loadBuffer;
        }

        if (operation == "fsd" && storeBuffer.isFull()) {
            cout << "Store buffer is full";
            return;
        } else if(operation == "fsd"){
            reservationStationToAdd = &storeBuffer;
        }

        if ((operation == "add" || operation == "addi") && intReservationStation.isFull()) {
            cout << "INT reservation station is full";
            return;
        } else if (operation == "add" || operation == "addi"){
            reservationStationToAdd = &intReservationStation;
        }

        if ((operation == "fadd" || operation == "fsub") && fpAdd.isFull()) {
            cout << "FPADD reservation station is full";
            return;
        } else if(operation == "fadd" || operation == "fsub"){
            reservationStationToAdd = &fpAdd;
        }

        if (operation == "fmul" && fpMul.isFull()) {
            cout << "FPMUL reservation station is full";
            return;
        } else if(operation == "fmul"){
            reservationStationToAdd = &fpMul;
        }

        if (operation == "fdiv" && fpDiv.isFull()) {
            cout << "FPDIV reservation station is full";
            return;
        } else if(operation == "fdiv"){
            reservationStationToAdd = &fpDiv;
        }

        if (operation == "bne" && bu.isFull()) {
            cout << "BU reservation station is full";
            return;
        } else if(operation == "bne"){
            reservationStationToAdd = &bu;
        }

        // fetch the next instruction
        string instruction = instruction_queue.getNextInstruction();
        cout << "EXECUTING DECODE " + instruction << std::endl;
        DecodedInstruction decodedInstruction(instruction);
        decodedInstruction = swapSourceAndDest(decodedInstruction); // for fsd


        // add to ROB
        int robEntryID = reorder_buffer.addROBEntry(
                ROBEntry(decodedInstruction.instruction_id, decodedInstruction.destination,
                         0.0, false));
        string robRef = "ROB" + to_string(robEntryID);

        // add to corresponding reservation station. see if any of the operands are ready
        ReservationStationEntry reservationStationEntry = ReservationStationEntry(decodedInstruction.instruction_id,
                                                                                  decodedInstruction.instruction_type,
                                                                                  "", "", decodedInstruction.source1,
                                                                                  decodedInstruction.source2,
                                                                                  robRef, true);

        // first operand
        if (isOperandReady(decodedInstruction.source1)) {
            reservationStationEntry.vj = decodedInstruction.source1;
        } else {
            // result is coming from ROB
            reservationStationEntry.qj = registerAllocationTable.mappings[decodedInstruction.source1];
        }

        // second operand
        if (isOperandReady(decodedInstruction.source2)) {
            reservationStationEntry.vk = decodedInstruction.source2;
        } else {
            reservationStationEntry.qk = decodedInstruction.source2;
        }

        reservationStationToAdd->addReservationStationEntry(reservationStationEntry);

        // update RAT entry for this instruction's destination register
        registerAllocationTable.mappings[decodedInstruction.destination] = robRef;
    }

}

void startProcessing() {
    int cycle = 1;
    ROB reorder_buffer(NR);

    while (true) {
        cout << "******** Executing cycle " + to_string(cycle) + " *********" << std::endl;
        // decode instructions fetched during the previous cycle's fetch stage
        decodeInstructions(reorder_buffer);

        // fetch stage
        fetchInstructions(((cycle - 1) * NF) + 1);

        if (cycle == 2) {
            break; // TODO change exit condition
        }

        cycle++;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 7) {
        cout << "Usage: ./simulator {file_name} {NF} {NI} {NW} {NR} {NB}";
        return 1;
    } else {
        // read command line args
        file_name = argv[1];
        if (!fileExists(file_name)) {
            cout << "Error: file not found: " << file_name << std::endl;
            cout << "Place the input file under src directory and provide the file name as an argument";
            return 1;
        }
        NF = stoi(argv[2]);
        NI = stoi(argv[3]);
        NW = stoi(argv[4]);
        NR = stoi(argv[5]);
        NB = stoi(argv[6]);

        startProcessing();
    }
    return 0;
}