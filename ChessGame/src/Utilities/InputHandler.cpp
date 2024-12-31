#include "InputHandler.h"
#include <iostream>

std::string InputHandler::getInput(const std::string &prompt) {
  std::string input;
  std::cout << prompt;
  std::getline(std::cin, input);
  std::cout << "\n";
  return input;
}

bool InputHandler::parseStr(const std::string &input, std::string &output1,
                            std::string &output2) {
  std::stringstream ss(input);
  ss >> output1 >> output2;
  return !ss.fail();
}
