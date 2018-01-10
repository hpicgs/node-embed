#include <iostream>

#include "node.h"
#include "node_lib.h"

int main(int argc, char* argv[]) {
    std::cout << "Hello from C++" << std::endl;
    node::lib::Initialize("here_is_the_program_name");

    std::cout << "Running cli-test.js from C++" << std::endl;
    node::lib::Run("cli-test.js");
    while (node::lib::ProcessEvents()) { }

    std::cout << "Evaluating stuff from C++" << std::endl;
    node::lib::Evaluate("console.log('this is evaluated!');let christian = 'hmm';");
    while (node::lib::ProcessEvents()) { }

    std::cout << "Evaluating MORE stuff from C++" << std::endl;
    node::lib::Evaluate("console.log('this is EVEN MORE evaluated!');console.log(christian);");
    while (node::lib::ProcessEvents()) { }

    auto fs = node::lib::IncludeModule("fs");
    node::lib::Call(fs, "foo");
    while (node::lib::ProcessEvents()) { }

    return 0;
}
