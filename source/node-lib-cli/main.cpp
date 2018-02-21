#include <iostream>

#include "node.h"
#include "node_lib.h"

#include <cpplocate/cpplocate.h>

void cppFunctionExposedToJs(const v8::FunctionCallbackInfo<v8::Value>& args) {
  std::cout << "This is a C++ function called from JS." << std::endl;
}

int main(int argc, char* argv[]) {
  // Locate the JavaScript file we want to load:
  const std::string js_file = "data/node-lib-cli.js";
  const std::string data_path = cpplocate::locatePath(js_file);
  if (data_path.empty()) {
    std::cerr << "Could not find data path." << std::endl;
    return 1;
  }
  const std::string js_path = data_path + "/" + js_file;

  std::cout << "Initializing Node.js instance" << std::endl;
  std::string program_name = argc >= 1 ? std::string(argv[0]) : "cli_app";
  node::Initialize(program_name);

  std::cout << "Running node-lib-cli.js from C++" << std::endl;
  node::Run(js_path);
  std::cout << "Processing events in event queue" << std::endl;
  while (node::ProcessEvents()) { }

  std::cout << "Evaluating a JS string from C++" << std::endl;
  node::Evaluate("let text = 'This is text is stored in a global variable in JS.';");
  node::Evaluate("console.log(text);");

  std::cout << "Loading the 'fs' NPM module directly from C++" << std::endl;
  auto fs = node::IncludeModule("fs");
  std::cout << "Using 'fs' to check if file node-lib-cli.js exists" << std::endl;
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  auto result = node::Call(fs.ToLocalChecked(), "existsSync", {v8::String::NewFromUtf8(isolate, js_path.c_str())});
  auto file_exists = v8::Local<v8::Boolean>::Cast(result.ToLocalChecked())->BooleanValue();
  std::cout << (file_exists ? "node-lib-cli.js exists" : "node-lib-cli.js does not exist") << std::endl;

  std::cout << "Calling a global JS function from C++" << std::endl;
  auto root = node::GetRootObject().ToLocalChecked();
  node::Call(root, "eval", {v8::String::NewFromUtf8(isolate, "console.log('Called a global JS function.');")});

  node::RegisterModule("CliDemoModule", {
                         {"cppFunctionExposedToJs", cppFunctionExposedToJs},
                       });
  node::Evaluate(R"(
                 var cliDemoModule = process.binding('CliDemoModule');
                 cliDemoModule.cppFunctionExposedToJs();
                 )");

  node::Deinitialize();

  return 0;
}
