#include <string>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <csignal>
#include <vector>

int main(){
  std::string s;
  std::getline(std::cin, s);
  std::cout << s << " ";
  std::cout << "b is executing\n";
  fflush(stdout);
  sleep(2);
  return 0;
}
