#include <iostream>
#include <sys/wait.h>

int main(){
  std::string s;
  std::getline(std::cin, s);
  std::cout << s << " " << "a is executing\n";
  fflush(stdout);
  sleep(2);
  return 0;
}
