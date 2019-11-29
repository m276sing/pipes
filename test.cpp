#include <string>

#include <iostream>

#include <sys/wait.h>

#include <unistd.h>

#include <signal.h>

#include <csignal>

#include <vector>



using namespace std;





int apro(int argc, char **argv) { 

    argv[0] = (char*) "a";;

    int get_file = execv("a", argv);

    if(get_file == -1) {

        std::cerr << "Error: Unable to execute rgen file" << "\n";

    }

    return 0;

}


int bpro(void) {

    char* args2[2];

    args2[0] = (char*) "b";

    args2[1] = nullptr;

    int get_file = execv("b", args2);

    if(get_file == -1) {

        std::cerr << "Error: Unable to execute Assignment 2 file" << "\n";

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



int main(int argc, char **argv)

{

    std::vector<pid_t> children;

    int atob[2];

    pipe(atob); // pipe created from random number generator to the Assignemnt 1



    int btoa[2];

    pipe(btoa); // pipe created from Assignment 1 to Assignment 2



    pid_t c_pid;

    c_pid = fork(); //created a child process



    if (c_pid == 0) {

        dup2(atob[1], STDOUT_FILENO); //outputs the data from rgen to A1

        close(atob[0]);

        close(atob[1]);

        

        close(btoa[0]);

        close(btoa[1]); 

        return apro(argc, argv);



    }

    else if (c_pid < 0) {

        std::cerr << "Error: Fork request could not be completed." << std::endl;

        return 1;

    }



    children.push_back(c_pid); // pid is pushed into vector

    c_pid = fork(); // child is created for the next dup2



    if (c_pid == 0) {

        dup2(atob[0], STDIN_FILENO); // input from rgen to A1

        close(atob[0]);

        close(atob[1]);



       //dup2(a1Toa2[1], STDOUT_FILENO); // output from A1 to A2

        //close(a1Toa2[0]);

        //close(a1Toa2[1]);

        return bpro();



    }

    else if (c_pid < 0) {

        std::cerr << "Error: Fork request could not be completed." << std::endl;

        return 1;

    }



    children.push_back(c_pid); // pid is pushed into vector
/*
    c_pid = fork(); // child is created for the next dup2



    if (c_pid == 0) {

        dup2(a1Toa2[0], STDIN_FILENO); // input from A1 to A2

        close(a1Toa2[1]);

        close(a1Toa2[0]);

        return assignment_2();



    }

    else if (c_pid < 0) {

        std::cerr << "Error: Fork request could not be completed." << std::endl;

        return 1;

    }



    children.push_back(c_pid); // pid is pushed into vector
*/
    c_pid = 0; // marks the end of children processes



    //main function executes the final result instead of the child processes


    int output = get_input(); // to send s to a2



    for (pid_t i : children) {

        int status;

        kill (i, SIGTERM);

        waitpid(i, &status, 0);

    }



    return output;

}
