// Smart Bus CO4 CPU Simulator
// Educational 5-stage pipeline model: IF, ID, EX, MEM, WB.
// This is a teaching simulator, not a cycle-accurate Intel 80386 emulator.

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

struct Instruction {
    string text, op, dst, src1, src2;
    int immediate = 0;
    bool hasImmediate = false;
};

struct ControlSignals {
    int regRead = 0, regWrite = 0, memRead = 0, memWrite = 0;
    int aluSrc = 0, branch = 0;
    string aluOp = "NONE";
};

ControlSignals decode(const Instruction& in) {
    ControlSignals c;
    if (in.op == "LOADI") {
        c.regWrite = 1; c.aluSrc = 1; c.aluOp = "PASS IMM";
    } else if (in.op == "ADD" || in.op == "MUL" || in.op == "SUB") {
        c.regRead = 1; c.regWrite = 1; c.aluOp = in.op;
    } else if (in.op == "STORE") {
        c.regRead = 1; c.memWrite = 1; c.aluOp = "PASS";
    } else if (in.op == "SEARCH") {
        c.regRead = 1; c.branch = 1; c.aluOp = "COMPARE";
    }
    return c;
}

int main() {
    cout << "\n========== SMART BUS : CO4 CPU SIMULATOR ==========\n";
    cout << "CEO1: 80386 architecture concepts (registers, memory, instructions)\n";
    cout << "CEO2: Assembly-language instruction sequence\n";
    cout << "CEO3: ALU arithmetic operations\n";
    cout << "CEO4: Control Unit and 5-stage instruction pipelining\n\n";

    int distance, rate, passengers;
    cout << "Enter journey distance (KM): ";
    if (!(cin >> distance) || distance <= 0) {
        cout << "Invalid distance. Enter a positive number.\n"; return 1;
    }
    cout << "Enter fare rate per KM (Rs): ";
    if (!(cin >> rate) || rate <= 0) {
        cout << "Invalid rate. Enter a positive number.\n"; return 1;
    }
    cout << "Enter passenger count: ";
    if (!(cin >> passengers) || passengers <= 0) {
        cout << "Invalid passenger count. Enter a positive number.\n"; return 1;
    }

    // Register file and memory used by the teaching CPU model.
    unordered_map<string, int> R;
    unordered_map<string, int> memory;
    int fare = distance * rate;
    int total = fare * passengers;

    vector<Instruction> program = {
        {"LOADI R1, distance", "LOADI", "R1", "", "", distance, true},
        {"LOADI R2, rate", "LOADI", "R2", "", "", rate, true},
        {"MUL R3, R1, R2", "MUL", "R3", "R1", "R2"},
        {"LOADI R4, passengers", "LOADI", "R4", "", "", passengers, true},
        {"MUL R5, R3, R4", "MUL", "R5", "R3", "R4"},
        {"STORE total, R5", "STORE", "", "R5", ""},
        {"SEARCH R5, R5", "SEARCH", "", "R5", ""}
    };

    cout << "\n--- Control Unit: Instruction Decode & Control Signals ---\n";
    cout << left << setw(24) << "Instruction" << setw(9) << "RegRead"
         << setw(10) << "RegWrite" << setw(9) << "MemRead"
         << setw(10) << "MemWrite" << setw(8) << "ALUSrc"
         << setw(9) << "Branch" << "ALU Operation\n";
    cout << string(100, '-') << "\n";
    for (const auto& in : program) {
        auto c = decode(in);
        cout << left << setw(24) << in.text << setw(9) << c.regRead
             << setw(10) << c.regWrite << setw(9) << c.memRead
             << setw(10) << c.memWrite << setw(8) << c.aluSrc
             << setw(9) << c.branch << c.aluOp << "\n";
    }

    cout << "\n--- ALU Execution ---\n";
    R["R1"] = distance;
    R["R2"] = rate;
    R["R3"] = R["R1"] * R["R2"];
    R["R4"] = passengers;
    R["R5"] = R["R3"] * R["R4"];
    memory["total"] = R["R5"];
    cout << "R1 (distance) = " << R["R1"] << "\n";
    cout << "R2 (rate) = " << R["R2"] << "\n";
    cout << "R3 = R1 * R2 = " << R["R3"] << " (fare per passenger)\n";
    cout << "R4 (passengers) = " << R["R4"] << "\n";
    cout << "R5 = R3 * R4 = " << R["R5"] << " (total fare)\n";
    cout << "Memory[total] = " << memory["total"] << "\n";

    // Simple in-order pipeline schedule with dependency stalls.
    // A consumer waits until the producer reaches WB; no forwarding is modeled.
    const vector<string> stages = {"IF", "ID", "EX", "MEM", "WB"};
    int n = (int)program.size();
    vector<int> start(n, 0);
    for (int i = 0; i < n; ++i) {
        start[i] = i;
        for (int j = 0; j < i; ++j) {
            bool depends = false;
            if (!program[j].dst.empty()) {
                depends = (program[i].src1 == program[j].dst ||
                           program[i].src2 == program[j].dst);
            }
            if (depends)
                start[i] = max(start[i], start[j] + 5);
        }
        if (i > 0)
            start[i] = max(start[i], start[i - 1] + 1); // in-order issue
    }
    int lastCycle = 0;
    for (int i = 0; i < n; ++i) lastCycle = max(lastCycle, start[i] + 4);

    cout << "\n--- 5-Stage Pipeline Timing (dependency stalls included) ---\n";
    cout << left << setw(27) << "Instruction";
    for (int c = 0; c <= lastCycle; ++c) cout << setw(6) << ("C" + to_string(c + 1));
    cout << "\n" << string(27 + (lastCycle + 1) * 6, '-') << "\n";
    for (int i = 0; i < n; ++i) {
        cout << left << setw(27) << program[i].text;
        for (int c = 0; c <= lastCycle; ++c) {
            string cell = (c >= start[i] && c < start[i] + 5)
                        ? stages[c - start[i]] : "";
            cout << setw(6) << cell;
        }
        cout << "\n";
    }
    cout << "\nPipeline stages: IF=Fetch, ID=Decode, EX=Execute, MEM=Memory, WB=Write Back\n";
    cout << "Stall policy: dependent instruction waits until producer completes WB (no forwarding).\n";

    cout << "\n--- Final Smart Bus Result ---\n";
    cout << "Distance              : " << distance << " KM\n";
    cout << "Fare rate             : Rs " << rate << " / KM\n";
    cout << "Fare per passenger    : Rs " << fare << "\n";
    cout << "Passenger count       : " << passengers << "\n";
    cout << "TOTAL FARE            : Rs " << total << "\n";
    cout << "\nNote: This is an educational CPU/pipeline model; it does not emulate the full 80386.\n";
    return 0;
}
