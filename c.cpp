#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <set>

using namespace std;

class Error {
protected:
    string message;
    int line;
public:
    virtual void detect(const vector<string>& code) = 0;
    virtual void display() = 0;
    virtual ~Error() {}
};


class MissingSemicolon : public Error {
public:
    void detect(const vector<string>& code) override {
        for (int i = 0; i < code.size(); i++) {
            string lineText = code[i];

            if (lineText.empty()) continue;

            if (lineText.find("int") != string::npos ||
                lineText.find("float") != string::npos ||
                lineText.find("cout") != string::npos) {

                if (lineText.back() != ';' &&
                    lineText.back() != '{' &&
                    lineText.back() != '}') {

                    cout << "\n[Error] Missing Semicolon";
                    cout << "\nLine: " << i + 1;
                    cout << "\nExplanation: Statement must end with ';'";
                    cout << "\nFix: Add ';' at the end\n";
                }
            }
        }
    }

    void display() override {}
};


class UnmatchedBrackets : public Error {
public:
    void detect(const vector<string>& code) override {
        stack<char> s;

        for (int i = 0; i < code.size(); i++) {
            for (char c : code[i]) {
                if (c == '{' || c == '(' || c == '[')
                    s.push(c);
                else if (c == '}' || c == ')' || c == ']') {
                    if (!s.empty())
                        s.pop();
                    else {
                        cout << "\n[Error] Unmatched Closing Bracket";
                        cout << "\nLine: " << i + 1;
                        cout << "\nFix: Remove extra closing bracket\n";
                    }
                }
            }
        }

        if (!s.empty()) {
            cout << "\n[Error] Unmatched Opening Bracket";
            cout << "\nFix: Add corresponding closing bracket\n";
        }
    }

    void display() override {}
};


class UndeclaredVariable : public Error {
public:
    void detect(const vector<string>& code) override {
        set<string> declared;

        for (int i = 0; i < code.size(); i++) {
            stringstream ss(code[i]);
            string word;

            while (ss >> word) {
                if (word == "int" || word == "float") {
                    string var;
                    ss >> var;
                    declared.insert(var);
                }
                else {
                    if (isalpha(word[0]) && declared.find(word) == declared.end()) {
                        if (word != "cout" && word != "endl") {
                            cout << "\n[Error] Undeclared Variable";
                            cout << "\nLine: " << i + 1;
                            cout << "\nVariable: " << word;
                            cout << "\nFix: Declare the variable before use\n";
                        }
                    }
                }
            }
        }
    }

    void display() override {}
};


class Analyzer {
private:
    vector<Error*> errors;

public:
    Analyzer() {
        errors.push_back(new MissingSemicolon());
        errors.push_back(new UnmatchedBrackets());
        errors.push_back(new UndeclaredVariable());
    }

    void analyze(const vector<string>& code) {
        for (auto e : errors) {
            e->detect(code);
        }
    }

    ~Analyzer() {
        for (auto e : errors)
            delete e;
    }
};


int main() {
    vector<string> code;
    string line;

    cout << "Enter your C++ code (type END to finish):\n";

    while (true) {
        getline(cin, line);
        if (line == "END") break;
        code.push_back(line);
    }

    Analyzer analyzer;
    analyzer.analyze(code);

    cout << "\n--- Analysis Complete ---\n";

    return 0;
}
