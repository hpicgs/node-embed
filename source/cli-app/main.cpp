#include <iostream>

#include "node.h"
#include "node_lib.h"

int main(int argc, char* argv[]) {
    std::cout << "Hello from C++" << std::endl;
    node::lib::Initialize(argc, argv);

    std::cout << "Running debug.js from C++" << std::endl;
    node::lib::Run("debug.js");
    while (node::lib::ProcessEvents()) { }

    std::cout << "Evaluating stuff from C++" << std::endl;
    node::lib::Evaluate("console.log('this is evaluated!');let christian = 'hmm';");
    while (node::lib::ProcessEvents()) { }

    std::cout << "Evaluating MORE stuff from C++" << std::endl;
    node::lib::Evaluate("console.log('this is EVEN MORE evaluated!');console.log(christian);");
    while (node::lib::ProcessEvents()) { }

    /*
    auto fs = node::lib::IncludeModule("fs");
    node::lib::Call(fs, "foo");
    while (node::lib::ProcessEvents()) { }
    */

    return 0;
}
