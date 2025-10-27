#include <string>
#include <iostream>

void printMessage()
{
  std::string message = "so this is the message";
  std::cout << message << std::endl;
};

int main()
{
  printMessage();
  return 0;
}