#include<iostream>
#include<unistd.h>
#include<vector>
#include<signal.h>
#include<csignal>
using namespace std;

int A(int argc, char **argv){
  argv[0] = (char *)"a";
  int get_file = execv("a", argv);
  if (get_file == -1){
    std::cerr << "Error: not executed";
  }
  return 0;
}

int main(int argc, char **argv){
  std::cout << "main";
  std::vector <pid_t> children;
  int mainToA[2];
  pipe(mainToA);
  
  pid_t c_pid = fork();
  if(c_pid == 0){
    dup2(mainToA[1],STDOUT_FILENO);
    close(mainToA[0]);
    close(mainToA[1]);
    return A(argc, argv);
  }
  else {
    std::cout << "Error: error in executing fork!";
  }
  
  children.push_back(c_pid);
  
  for (pid_t i : children) {

        int status;

        kill (i, SIGTERM);

        waitpid(i, &status, 0);

    }
  return 0;
}
