#include <iostream>

#include "node.h"
#include "node_lib.h"
#include <cpplocate/cpplocate.h>

void calledFromJs(const v8::FunctionCallbackInfo<v8::Value>& args){
    std::cout << "I was called from JS!" << std::endl;
}

int main(int argc, char* argv[]) {
    const std::string cli_test_js_file = "data/node-lib-cli.js";
    const std::string data_path = cpplocate::locatePath(cli_test_js_file);
    if (data_path.empty()) {
        std::cerr << "Could not find data path." << std::endl;
        return 1;
    }
    const std::string cli_test_js_path = data_path + "/" + cli_test_js_file;

    std::cout << "Hello from C++" << std::endl;
    std::string program_name = argc >= 1 ? std::string(argv[0]) : "cli_app";
    node::lib::Initialize(program_name);

    std::cout << "Running cli-test.js from C++" << std::endl;
    node::lib::Run(cli_test_js_path);
    while (node::lib::ProcessEvents()) { }

    std::cout << "Evaluating stuff from C++" << std::endl;
    node::lib::Evaluate("console.log('this is evaluated!');let christian = 'hmm';");

    std::cout << "Evaluating MORE stuff from C++" << std::endl;
    node::lib::Evaluate("console.log('this is EVEN MORE evaluated!');console.log(christian);");
    v8::Isolate *isolate = v8::Isolate::GetCurrent();

    auto fs = node::lib::IncludeModule("fs");
    // Check if file cli-test.js exists.
    auto result = node::lib::Call(fs.ToLocalChecked(), "existsSync", {v8::String::NewFromUtf8(isolate, cli_test_js_path.c_str())});
    auto file_exists = v8::Local<v8::Boolean>::Cast(result.ToLocalChecked())->BooleanValue();
    std::cout << (file_exists ? "cli-test.js exists" : "cli-test.js does not exist") << std::endl;

    auto root = node::lib::GetRootObject().ToLocalChecked();
    node::lib::Call(root, "eval", {v8::String::NewFromUtf8(isolate, "console.log('1234');")});

    // This would never return and run forever, even if Terminate is called.
    //node::lib::Evaluate("while (true) {}");

    auto funcs = std::map<std::string, v8::FunctionCallback> {
            {"callFromJs", calledFromJs},
    };
    node::lib::RegisterModule("CliDemoModule", funcs);
    node::lib::Evaluate(R"(
        var cliDemoModule = process.binding('CliDemoModule');
        cliDemoModule.callFromJs();
)"
    );

    node::lib::StopEventLoop();

    return 0;
}
