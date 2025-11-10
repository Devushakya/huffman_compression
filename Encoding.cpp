#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
using namespace std;

struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f) : ch(c), freq(f), left(NULL), right(NULL) {}
};

struct compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

void buildCodes(Node* root, string str, unordered_map<char, string>& codes) {
    if (!root) return;

    if (!root->left && !root->right)
        codes[root->ch] = str;

    buildCodes(root->left, str + "0", codes);
    buildCodes(root->right, str + "1", codes);
}

Node* buildHuffmanTree(unordered_map<char, int>& freq) {
    priority_queue<Node*, vector<Node*>, compare> pq;

    for (auto pair : freq)
        pq.push(new Node(pair.first, pair.second));

    while (pq.size() > 1) {
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();

        Node *node = new Node('\0', left->freq + right->freq);
        node->left = left;
        node->right = right;
        pq.push(node);
    }

    return pq.top();
}

void compressFile(const string& inputFile, const string& outputFile, const string& codeFile) {
    ifstream in(inputFile, ios::binary);
    if (!in) {
        cout << "Error: Unable to open input file.\n";
        return;
    }

    unordered_map<char, int> freq;
    char ch;
    while (in.get(ch))
        freq[ch]++;

    Node* root = buildHuffmanTree(freq);

    unordered_map<char, string> codes;
    buildCodes(root, "", codes);

    ofstream codeOut(codeFile);
    for (auto pair : codes)
        codeOut << pair.first << " : " << pair.second << "\n";
    codeOut.close();

    in.clear();
    in.seekg(0, ios::beg);

    string encodedString = "";
    while (in.get(ch))
        encodedString += codes[ch];
    in.close();

    ofstream out(outputFile, ios::binary);
    unsigned char buffer = 0;
    int bitCount = 0;

    for (char bit : encodedString) {
        buffer = (buffer << 1) | (bit - '0');
        bitCount++;
        if (bitCount == 8) {
            out.put(buffer);
            buffer = 0;
            bitCount = 0;
        }
    }
    if (bitCount > 0) {
        buffer <<= (8 - bitCount);
        out.put(buffer);
    }

    out.close();

    cout << "Compression successful!\n";
    cout << "Compressed file: " << outputFile << "\n";
    cout << "Code map saved in: " << codeFile << "\n";
}

int main() {
    string inputFile = "input.txt";
    string outputFile = "compressed.bin";
    string codeFile = "codes.txt";

    compressFile(inputFile, outputFile, codeFile);

    return 0;
}
