#include <iostream>
#include <sstream>
#include <string>

#pragma once

class InputHandler {
public:
  static std::string getInput(const std::string &prompt);
  static bool parseStr(const std::string &input, std::string &output1,
                       std::string &output2);
};
