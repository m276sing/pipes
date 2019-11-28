#include<iostream>
#include<unistd.h>
#include<vector>
#include<signal.h>
#include<csignal>
#include <sys/wait.h>
using namespace std;

int A(int argc, char **argv){
  argv[0] = (char *)"a";
  int get_file = execv("a", argv);
  if (get_file == -1){
    std::cerr << "Error: not executed";
  }
  return 0;
}

int B(void){
  argv[2];
  argv[0] = (char *)"b";
  argv[1] = nullptr;
  int get_file = execv("b", argv);
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
  
  int aTob[2];
  pipe(aTob);
  pid_t c_pid = fork();
  if(c_pid == 0){
    dup2(mainToA[o],STDIN_FILENO);
    close(mainToA[0]);
    close(mainToA[1]);
    
    dup2(aTob[o],STDOUT_FILENO);
    close(aTob[0]);
    close(aTob[1]);
    
    return B();
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
