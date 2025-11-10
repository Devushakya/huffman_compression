#include <iostream>
#include <fstream>
#include <unordered_map>
#include <bitset>
#include <string>
using namespace std;

int main() {
    string codeFile = "codes.txt";
    string compressedFile = "compressed.bin";

   
    ifstream codeIn(codeFile);
    if (!codeIn) {
        cerr << "Error: Unable to open " << codeFile << endl;
        return 1;
    }

    unordered_map<string, char> reverseCodes;
    string line;

    while (getline(codeIn, line)) {
        if (line.empty() || line.size() < 4) continue;
        size_t pos = line.find(" : ");
        if (pos == string::npos) continue;

        string symbol = line.substr(0, pos);
        string code = line.substr(pos + 3);

        if (symbol == "\\n") reverseCodes[code] = '\n';
        else if (symbol == "[space]") reverseCodes[code] = ' ';
        else reverseCodes[code] = symbol[0];
    }
    codeIn.close();


    ifstream in(compressedFile, ios::binary);
    if (!in) {
        cerr << "Error: Unable to open " << compressedFile << endl;
        return 1;
    }

    string bitString = "";
    unsigned char byte;
    while (in.read(reinterpret_cast<char*>(&byte), 1)) {
        bitString += bitset<8>(byte).to_string();
    }
    in.close();

    
    string current = "";
    string decoded = "";

    for (char bit : bitString) {
        current += bit;
        if (reverseCodes.find(current) != reverseCodes.end()) {
            decoded += reverseCodes[current];
            current = "";
        }
    }

    cout << "---- DECOMPRESSED CONTENT ----\n";
    cout << decoded << endl;
    cout << "------------------------------\n";

    return 0;
}
