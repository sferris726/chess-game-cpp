#include "InputHandler.h"

std::string InputHandler::getInput(const std::string &prompt) {
  std::string input;
  std::cout << prompt;
  std::getline(std::cin, input);
  std::cout << "\n";
  return input;
}

bool InputHandler::parseStr(const std::string &input, std::string &output1,
                            std::string &output2) {
  auto i2 = input;
  std::stringstream ss(i2);
  ss >> output1 >> output2;

  if (ss >> i2) {
    return false;
  }

  return !output1.empty();
}
