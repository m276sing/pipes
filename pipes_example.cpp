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


int main(int argc, char **argv)
{
  /*
  std::vector <pid_t> children;
  int mainToA[2];
  pipe(mainToA);
  int aTob[2];
  pipe(aTob);
  
  
  pid_t c_pid;
  c_pid = fork();
  
  std::cout << getpid <<"\n";
  if(c_pid == 0){
    dup2(mainToA[1],STDOUT_FILENO);
    close(mainToA[0]);
    close(mainToA[1]);
    return A(argc, argv);
  }
  else if(c_pid < 0){
    std::cout << "Error: error in executing fork!\n";
  }  
  children.push_back(c_pid);
  
  
  c_pid = fork();
  if(c_pid == 0){
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
  }
  children.push_back(c_pid);
  c_pid = 0;
  
  */
  
  std::vector<pid_t> children;

    int rgentoA1[2];

    pipe(rgentoA1); // pipe created from random number generator to the Assignemnt 1



    int a1Toa2[2];

    pipe(a1Toa2); // pipe created from Assignment 1 to Assignment 2



    pid_t c_pid;
    sleep(1);
    std::cout <<"before";
    c_pid = fork(); //created a child process
  sleep(1);
    std::cout <<"after" << c_pid;


    if (c_pid == 0) {

        dup2(rgentoA1[1], STDOUT_FILENO); //outputs the data from rgen to A1

        close(rgentoA1[0]);

        close(rgentoA1[1]);

        
        std::cout << "here";
        close(a1Toa2[0]);

        close(a1Toa2[1]); 

        return A(argc, argv);



    }

    else if (c_pid > 0) {

        std::cerr << "Error: Fork request could not be completed." << std::endl;

        return 1;

    }



    children.push_back(c_pid);
  std::cout <<"last";
  c_pid = 0;
  
  dup2(a1Toa2[1], STDOUT_FILENO); //outputs the result to the standard output

    close(a1Toa2[0]);

    close(a1Toa2[1]);

    

    close(a1Toa2[0]);

    close(a1Toa2[1]); 

    int output = get_input(); // to send s to a2
  
  for (pid_t i : children) {

        int status;

        kill (i, SIGTERM);

        waitpid(i, &status, 0);

    }
  return 0;
}
