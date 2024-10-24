#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <limits>

class BinOps {
private:
    float operand1;
    float operand2;
    char op;

public:
    BinOps() : operand1(0), operand2(0), op('+') {}

    void setOperand1(float opnd1) { operand1 = opnd1; }
    void setOperand2(float opnd2) { operand2 = opnd2; }
    void setOperator(char oper) {
        if (oper == '+' || oper == '-' || oper == '*' || oper == '/') {
            op = oper;
        } else {
            throw std::invalid_argument("Invalid operator");
        }
    }

    float getOperand1() const { return operand1; }
    float getOperand2() const { return operand2; }
    char getOperator() const { return op; }

    float evaluate() const {
        switch (op) {
            case '+': return operand1 + operand2;
            case '-': return operand1 - operand2;
            case '*': return operand1 * operand2;
            case '/': 
                if (operand2 == 0) throw std::runtime_error("Division by zero");
                return operand1 / operand2;
            default:
                throw std::runtime_error("Unknown operator");
        }
    }

    std::string getExpression() const {
        std::ostringstream oss;
        oss << operand1 << " " << op << " " << operand2;
        return oss.str();
    }
};

void readUsingStreamExtraction(std::vector<BinOps>& expressions) {
    float opnd1, opnd2;
    char oper;
    while (true) {
        std::cout << "Enter expression (operand1 operator operand2) or 'q' to quit: ";
        if (!(std::cin >> opnd1 >> oper >> opnd2)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            char check;
            std::cin >> check;
            if (check == 'q') break;
            std::cerr << "Invalid input. Please try again.\n";
            continue;
        }

        try {
            BinOps binop;
            binop.setOperand1(opnd1);
            binop.setOperand2(opnd2);
            binop.setOperator(oper);
            expressions.push_back(binop);

            float result = binop.evaluate();
            std::cout << binop.getExpression() << " = " << result << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Error evaluating expression: " << e.what() << "\n";
        }
    }
}

void readUsingGetline(std::vector<BinOps>& expressions) {
    std::string input;
    while (true) {
        std::cout << "Enter expression (operand1 operator operand2) or 'q' to quit: ";
        std::getline(std::cin, input);
        if (input == "q") break;

        std::istringstream iss(input);
        float opnd1, opnd2;
        char oper;

        if (iss >> opnd1 >> oper >> opnd2) {
            try {
                BinOps binop;
                binop.setOperand1(opnd1);
                binop.setOperand2(opnd2);
                binop.setOperator(oper);
                expressions.push_back(binop);

                float result = binop.evaluate();
                std::cout << binop.getExpression() << " = " << result << "\n";
            } catch (const std::exception& e) {
                std::cerr << "Error evaluating expression: " << e.what() << "\n";
            }
        } else {
            std::cerr << "Invalid input format. Please try again.\n";
        }
    }
}

void writeResultsToFile(const std::vector<BinOps>& expressions, const std::string& filename) {
    std::ofstream outFile(filename);

    if (!outFile) {
        std::cerr << "Error opening file for writing.\n";
        return;
    }

    for (const auto& expr : expressions) {
        try {
            float result = expr.evaluate();
            outFile << expr.getExpression() << " = " << result << "\n";
        } catch (const std::exception& e) {
            outFile << expr.getExpression() << " = Error: " << e.what() << "\n";
        }
    }

    std::cout << "Results written to " << filename << "\n";
}

int main() {
    std::vector<BinOps> expressions;
    int methodChoice;

    std::cout << "Choose input method:\n";
    std::cout << "1. Using stream extraction (>>)\n";
    std::cout << "2. Using getline and istringstream\n";
    std::cout << "Enter your choice: ";
    std::cin >> methodChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (methodChoice == 1) {
        readUsingStreamExtraction(expressions);
    } else if (methodChoice == 2) {
        readUsingGetline(expressions);
    } else {
        std::cerr << "Invalid choice.\n";
        return 1;
    }

    writeResultsToFile(expressions, "results.txt");

    return 0;
}