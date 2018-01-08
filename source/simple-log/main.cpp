#include <iostream>
//#include "node.h"
#include "node_lib.h"


int main(int argc, char* argv[]) {
    std::cout << "Hello from C++" << std::endl;
    node::lib::Initialize("argc, argv");

    std::cout << "Running debug.js from C++" << std::endl;
    node::lib::Run("log.js");
    while (node::lib::ProcessEvents()) { }

    std::cout << "Evaluating stuff from C++" << std::endl;
    node::lib::Evaluate("console.log('this is evaluated!');let myvar = 'my string';");
    while (node::lib::ProcessEvents()) { }

    std::cout << "Evaluating MORE stuff from C++" << std::endl;
    node::lib::Evaluate("console.log('this is EVEN MORE evaluated!');console.log(myvar);");
    while (node::lib::ProcessEvents()) { }
}
