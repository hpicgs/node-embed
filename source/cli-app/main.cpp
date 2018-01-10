#include <iostream>

#include "node.h"
#include "node_lib.h"

int main(int argc, char* argv[]) {
    std::cout << "Hello from C++" << std::endl;
    std::string program_name = argc >= 1 ? std::string(argv[0]) : "cli_app";
    node::lib::Initialize(program_name);

    std::cout << "Running cli-test.js from C++" << std::endl;
    node::lib::Run("cli-test.js");
    while (node::lib::ProcessEvents()) { }

    std::cout << "Evaluating stuff from C++" << std::endl;
    node::lib::Evaluate("console.log('this is evaluated!');let christian = 'hmm';");

    std::cout << "Evaluating MORE stuff from C++" << std::endl;
    node::lib::Evaluate("console.log('this is EVEN MORE evaluated!');console.log(christian);");
    v8::Isolate *isolate = v8::Isolate::GetCurrent();

    auto fs = node::lib::IncludeModule("fs");
    // Check if file cli-test.js exists in the current working directory.
    auto result = node::lib::Call(fs, "existsSync", {v8::String::NewFromUtf8(isolate, "cli-test.js")});
    auto file_exists = v8::Local<v8::Boolean>::Cast(result)->BooleanValue();
    std::cout << (file_exists ? "cli-test.js exists in cwd" : "cli-test.js does not exist in cwd") << std::endl;

    auto root = node::lib::GetRootObject();
    node::lib::Call(root, "eval", {v8::String::NewFromUtf8(isolate, "console.log('1234');")});

    // This would never return and run forever, even if Terminate is called.
    //node::lib::Evaluate("while (true) {}");

    node::lib::Terminate();

    return 0;
}
