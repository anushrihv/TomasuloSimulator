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
#include "headerFiles/IntFunctionalUnit.h"
#include "headerFiles/RegisterFile.h"
#include "headerFiles/FPAddFunctionalUnit.h"
#include "headerFiles/FPMulFunctionalUnit.h"
#include "headerFiles/FPDivFunctionalUnit.h"

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
RegisterFile registerFile;
IntFunctionalUnit intFunctionalUnit(1);
FPAddFunctionalUnit fpAddFunctionalUnit(3);
FPMulFunctionalUnit fpMulFunctionalUnit(4);
FPDivFunctionalUnit fpDivFunctionalUnit(2);
ROB reorder_buffer(0);
int execStalledByRESOccupancy = 0, execStalledDueToROBFull = 0, totalCycles = 0;

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

    if (infile.eof()) {
        return false;
    }

    cout << "FETCH STAGE" << std::endl;
    while (lineNumber < startLineNumber + NF) {
        // get NF instructions from the file
        getline(infile, instruction);
        cout << instruction << std::endl;

        // put it in the instruction queue
        instruction_queue.addNewInstruction(instruction);
        linesRead = true;
        lineNumber++;

        if (infile.eof()) {
            break;
        }
    }

    return linesRead;
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

vector<int> getFPDivExecutedInstructions() {
    vector<int> instructionIDs;
    for (const auto& pair : fpDivFunctionalUnit.results) {
        int instructionID = pair.first;
        instructionIDs.push_back(instructionID);
    }

    fpDivFunctionalUnit.results.clear();
    return instructionIDs;
}

vector<int> getFPMulExecutedInstructions() {
    vector<int> instructionIDs;
    for (const auto& pair : fpMulFunctionalUnit.results) {
        int instructionID = pair.first;
        instructionIDs.push_back(instructionID);
    }

    fpMulFunctionalUnit.results.clear();
    return instructionIDs;
}

vector<int> getFPAddExecutedInstructions() {
    vector<int> instructionIDs;
    for (const auto& pair : fpAddFunctionalUnit.results) {
        int instructionID = pair.first;
        instructionIDs.push_back(instructionID);
    }

    fpAddFunctionalUnit.results.clear();
    return instructionIDs;
}

vector<int> getIntExecutedInstructions() {
    vector<int> instructionIDs;
    for (const auto& pair : intFunctionalUnit.results) {
        int instructionID = pair.first;
        instructionIDs.push_back(instructionID);
    }

    intFunctionalUnit.results.clear();
    return instructionIDs;
}

void clearStateForExecutedInstructions(vector<int> instructionIDs, ReservationStation* reservationStation) {
    for(int instructionID : instructionIDs) {
        // remove instruction from RES
        reservationStation->removeRESEntry(instructionID);

        // get ROB entry for this instr
        ROBEntry* robEntry = reorder_buffer.getROBEntrybyInstructionID(instructionID);
        // mark ROB entry as done
        robEntry->done = true;

        string destination_register = robEntry->destination_register;
    }
}

void print(vector<int> instructions) {
    if(instructions.size() == 0) {
        return;
    }

    cout<< "WRITE BACK STAGE for instructions ";
    for (int i = 0; i < instructions.size(); i++) {
        std::cout << instructions[i] << " ";
    }
    std::cout << std::endl;

}

void writeBackStage() {
    // get instructions that are executed

    vector<int> instructionIDs = getIntExecutedInstructions();
    clearStateForExecutedInstructions(instructionIDs, &intReservationStation);
    print(instructionIDs);

    instructionIDs = getFPAddExecutedInstructions();
    clearStateForExecutedInstructions(instructionIDs, &fpAdd);
    print(instructionIDs);

    instructionIDs = getFPMulExecutedInstructions();
    clearStateForExecutedInstructions(instructionIDs, &fpMul);
    print(instructionIDs);

    instructionIDs = getFPDivExecutedInstructions();
    clearStateForExecutedInstructions(instructionIDs, &fpDiv);
    print(instructionIDs);
}

void decodeInstructions() {
    for (int i = 0; i < NR; i++) {
        if (!instruction_queue.hasNextInstruction()) {
            return;
        }

        if (reorder_buffer.isFull()) {
            execStalledDueToROBFull++;
            cout << "ROB is full. Cannot decode any new instructions";
            return;
        }

        // based on the instruction type, check if the corresponding reservation station is free
        string next_instruction = instruction_queue.peekNextInstruction();
        //cout << "Next instruction available to decode " << next_instruction << std::endl;

        istringstream stringstream(next_instruction);
        string operation, instructionID;

        stringstream >> instructionID;
        stringstream >> operation;

        ReservationStation *reservationStationToAdd;
        if (operation == "fld" && loadBuffer.isFull()) {
            execStalledByRESOccupancy++;
            cout << "Load buffer is full";
            return;
        } else if (operation == "fld"){
            reservationStationToAdd = &loadBuffer;
        }

        if (operation == "fsd" && storeBuffer.isFull()) {
            execStalledByRESOccupancy++;
            cout << "Store buffer is full";
            return;
        } else if(operation == "fsd"){
            reservationStationToAdd = &storeBuffer;
        }

        if ((operation == "add" || operation == "addi") && intReservationStation.isFull()) {
            execStalledByRESOccupancy++;
            cout << "INT reservation station is full";
            return;
        } else if (operation == "add" || operation == "addi"){
            reservationStationToAdd = &intReservationStation;
        }

        if ((operation == "fadd" || operation == "fsub") && fpAdd.isFull()) {
            execStalledByRESOccupancy++;
            cout << "FPADD reservation station is full";
            return;
        } else if(operation == "fadd" || operation == "fsub"){
            reservationStationToAdd = &fpAdd;
        }

        if (operation == "fmul" && fpMul.isFull()) {
            execStalledByRESOccupancy++;
            cout << "FPMUL reservation station is full";
            return;
        } else if(operation == "fmul"){
            reservationStationToAdd = &fpMul;
        }

        if (operation == "fdiv" && fpDiv.isFull()) {
            execStalledByRESOccupancy++;
            cout << "FPDIV reservation station is full";
            return;
        } else if(operation == "fdiv"){
            reservationStationToAdd = &fpDiv;
        }

        if (operation == "bne" && bu.isFull()) {
            execStalledByRESOccupancy++;
            cout << "BU reservation station is full";
            return;
        } else if(operation == "bne"){
            reservationStationToAdd = &bu;
        }

        // fetch the next instruction
        string instruction = instruction_queue.getNextInstruction();
        cout << "DECODE STAGE " + instruction << std::endl;
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
                                                                                  0.0, 0.0, decodedInstruction.source1,
                                                                                  decodedInstruction.source2,
                                                                                  robRef, true);

        // first operand
        if (isOperandReady(decodedInstruction.source1)) {
            reservationStationEntry.vj = registerFile.getRegisterValue(decodedInstruction.source1);
            reservationStationEntry.qj = "";
        } else {
            // result is coming from ROB
            reservationStationEntry.qj = registerAllocationTable.mappings[decodedInstruction.source1];
        }

        // second operand
        if (isOperandReady(decodedInstruction.source2)) {
            if (is_integer(decodedInstruction.source2)) {
                reservationStationEntry.vk = stoi(decodedInstruction.source2);
            } else {
                reservationStationEntry.vk = registerFile.getRegisterValue(decodedInstruction.source2);
            }
            reservationStationEntry.qk = "";
        } else {
            reservationStationEntry.qk = registerAllocationTable.mappings[decodedInstruction.source2];
        }

        reservationStationToAdd->addReservationStationEntry(reservationStationEntry);

        // update RAT entry for this instruction's destination register
        registerAllocationTable.mappings[decodedInstruction.destination] = robRef;
    }
}

void forwardingResult(int instructionID, float value) {
    // get the ROB ID that was associated with this instruction
    int robID = reorder_buffer.getROBIDbyInstructionID(instructionID);
    ROBEntry* robEntry = reorder_buffer.getROBEntrybyInstructionID(instructionID);
    // update the value field in the ROB
    robEntry->value = value;
    // update the RES entries waiting on this ROB entry
    string robRef = "ROB" + to_string(robID);
    intReservationStation.updateRESEntriesWaitingOnROB(robRef, robEntry->value);
    fpAdd.updateRESEntriesWaitingOnROB(robRef, robEntry->value);
    fpMul.updateRESEntriesWaitingOnROB(robRef, robEntry->value);
    fpDiv.updateRESEntriesWaitingOnROB(robRef, robEntry->value);
}

void executeIntInstructions() {
    vector<ReservationStationEntry*> readyRESEntries = intReservationStation.getReadyReservationStationEntries();
    for (ReservationStationEntry* resEntry : readyRESEntries) {
        int operand1 = static_cast<int>(resEntry->vj);
        int operand2 = static_cast<int>(resEntry->vk);
        resEntry->executing = true;
        intFunctionalUnit.scheduleExecution(resEntry->instruction_id, operand1, operand2, resEntry->op);
    }
    vector<int> instructions_executed = intFunctionalUnit.stallOrExecute();
    for (int instructionID : instructions_executed) {
        // forwarding the result
        forwardingResult(instructionID, intFunctionalUnit.getResult(instructionID));
    }
}

void executeFPAddInstructions() {
    vector<ReservationStationEntry*> readyRESEntries = fpAdd.getReadyReservationStationEntries();
    for (ReservationStationEntry* resEntry : readyRESEntries) {
        float operand1 = resEntry->vj;
        float operand2 = resEntry->vk;
        resEntry->executing = true;
        fpAddFunctionalUnit.scheduleExecution(resEntry->instruction_id, operand1, operand2, resEntry->op);
    }
    vector<int> instructions_executed = fpAddFunctionalUnit.stallOrExecute();
    for (int instructionID : instructions_executed) {
        // forwarding the result
        forwardingResult(instructionID, fpAddFunctionalUnit.getResult(instructionID));
    }
}

void executeFPMulInstructions() {
    vector<ReservationStationEntry*> readyRESEntries = fpMul.getReadyReservationStationEntries();
    for (ReservationStationEntry* resEntry : readyRESEntries) {
        float operand1 = resEntry->vj;
        float operand2 = resEntry->vk;
        resEntry->executing = true;
        fpMulFunctionalUnit.scheduleExecution(resEntry->instruction_id, operand1, operand2, resEntry->op);
    }
    vector<int> instructions_executed = fpMulFunctionalUnit.stallOrExecute();
    for (int instructionID : instructions_executed) {
        // forwarding the result
        forwardingResult(instructionID, fpMulFunctionalUnit.getResult(instructionID));
    }
}

void executeFPDivInstructions() {
    vector<ReservationStationEntry*> readyRESEntries = fpDiv.getReadyReservationStationEntries();
    for (ReservationStationEntry* resEntry : readyRESEntries) {
        float operand1 = resEntry->vj;
        float operand2 = resEntry->vk;
        resEntry->executing = true;
        fpDivFunctionalUnit.scheduleExecution(resEntry->instruction_id, operand1, operand2, resEntry->op);
    }
    vector<int> instructions_executed = fpDivFunctionalUnit.stallOrExecute();
    for (int instructionID : instructions_executed) {
        // forwarding the result
        forwardingResult(instructionID, fpDivFunctionalUnit.getResult(instructionID));
    }
}

// for each functional unit
// get ready RES entries
// schedule it for execution
void executeStage() {
    // int functional unit
    executeIntInstructions();

    // FPAdd functional unit
    executeFPAddInstructions();

    // FPMul functional unit
    executeFPMulInstructions();

    // FPDiv functional unit
    executeFPDivInstructions();
}

void commitStage() {
    // move ROB pointers for done instructions in ROB

    ROBEntry robEntry = reorder_buffer.getAndClearNextDoneInstruction();
    while(robEntry.instruction_id != -1) {
        cout << "COMMIT STAGE for instruction " + to_string(robEntry.instruction_id) << std::endl;
        // update RAT
        string destination_register = robEntry.destination_register;
        registerAllocationTable.mappings[destination_register] = destination_register;

        // update register file
        char first_char = destination_register.front();
        if(first_char == 'R') {
            registerFile.intRegisters[destination_register] = robEntry.value;
        } else {
            registerFile.floatRegisters[destination_register] = robEntry.value;
        }

        robEntry = reorder_buffer.getAndClearNextDoneInstruction();
    }
}

void startProcessing() {
    int cycle = 1;
    reorder_buffer = ROB(NR);

    while (true) {
        cout << "******** Executing cycle " + to_string(cycle) + " *********" << std::endl;

        // commit stage
        commitStage();

        // write back
        writeBackStage();

        // execute with forwarding
        executeStage();

        // decode instructions fetched during the previous cycle's fetch stage
        decodeInstructions();

        // fetch stage
        fetchInstructions(((cycle - 1) * NF) + 1);

        if (cycle > 1 && (reorder_buffer.isEmpty() || cycle == 100)) {
            break;
        }

        cycle++;
    }

    cout << "Number of cycles : " + to_string(cycle) << std::endl;
    cout << "Number of executions stalled because the ROB was full " + to_string(execStalledDueToROBFull) << std::endl;
    cout << "Number of executions stalled because the ROB was full " + to_string(execStalledByRESOccupancy) << std::endl;
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