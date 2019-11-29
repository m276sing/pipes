#include <string>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <csignal>
#include <vector>

int main(){
  while (!std::cin.eof()) {
  std::string s;
  std::getline(std::cin, s);
  std::cout << s << " " << "a is executing\n";
  //fflush(stdout);
  //sleep(2);
  }
  return 0;
}
