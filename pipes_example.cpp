#include <string>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <csignal>
#include <vector>

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
  char *argv[2];
  argv[0] = (char *)"b";
  argv[1] = nullptr;
  int get_file = execv("b", argv);
  if (get_file == -1){
    std::cerr << "Error: not executed";
  }
  return 0;
}

int get_input(void) {

    

    while (!std::cin.eof()) {

        string input;

        getline(std::cin, input);

        if (input.size() > 0) {

            std::cout << input << "\n";

        }

    }

    return 0;

}

int main(void)
{
  
  std::vector <pid_t> children;
  int mainToA[2];
  pipe(mainToA);
  int aTob[2];
  pipe(aTob);
  
  
  pid_t c;
  c = fork();
  
  std::cout << getpid <<"\n";
  if(c == 0){
    dup2(mainToA[1],STDOUT_FILENO);
    close(mainToA[0]);
    close(mainToA[1]);
    return A();
  }
  else if(c < 0){
    std::cout << "Error: error in executing fork!\n";
    return 1;
  }  
  children.push_back(c);
  
  
  c = fork();
  if(c == 0){
    dup2(mainToA[0],STDIN_FILENO);
    close(mainToA[0]);
    close(mainToA[1]);
    
    dup2(aTob[0],STDOUT_FILENO);
    close(aTob[0]);
    close(aTob[1]);
    
    return B();
  }
  else {
    std::cout << "Error: error in executing fork!\n";
    return 1;
  }
  children.push_back(c);
  c = 0;
 
  int output = get_input(); // to send s to a2



    for (pid_t i : children) {

        int status;

        kill (i, SIGTERM);

        waitpid(i, &status, 0);

    }



    return output;
}
