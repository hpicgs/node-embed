#include <iostream>

#include "node.h"
#include "node_lib.h"

int main(int argc, char* argv[]) {
    std::cout << "Hello from C++" << std::endl;
    std::string program_name = argc >= 1 ? std::string(argv[0]) : "cli_app";
    node::lib::Initialize(program_name);

    // Example #1: Execute JS code on the fly
    node::lib::Evaluate("const uuid = require('uuid/v4'); console.log(uuid());");

    // Example #2: Exchange values between C++ host and JS
{
    auto isolate = v8::Isolate::GetCurrent();
    auto fs = node::lib::IncludeModule("fs");
    auto result = node::lib::Call(fs, "existsSync", {v8::String::NewFromUtf8(isolate, "cli-test.js")});
    auto file_exists = v8::Local<v8::Boolean>::Cast(result)->BooleanValue();
    std::cout << (file_exists ? "cli-test.js exists in cwd" : "cli-test.js does not exist in cwd") << std::endl;
}

    node::lib::Terminate();

    return 0;
}
