
/********************************************************
   Name: Deyonta Robinson
   Date: 11/04/2018
   rpc1.c - Rock, Paper, Scissors game with IPC
   Programming Assigment 03
 *********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#define  CHOICE_SIZE 10
char* choice1 = "Paper";
char* choice2 = "Scissors";
char* choice3 = "Rock";

int main(int argc, char const *argv[]) {

        //convert cmd line character into integer
        int rounds = atoi(argv[1]);

        int pid1, pid2;
        int child1_win = 0;
        int child2_win = 0;
        int signal; //1 if need to throw or 0 if the game is over
        int firstThrow, secondThrow;
        int i, readMessage, choice, status;
        char readPipe[100];
        char chose[3];

        //4 pipes
        int fd1[2]; //parent to read to child1 & child1 to write from parent
        int fd2[2]; //child1 to read to parent & parent to write from child1
        int fd3[2]; //parent to read to child2 & child2 to write from parent
        int fd4[2]; //child2 to read to parent & parent to write from child2

        //create the pipes
        if ( pipe(fd1) == -1 ) {
                perror("Pipe 1 cant be created!!!");
                exit(1);
        }
        if ( pipe(fd2) == -1 ) {
                perror("Pipe 2 cant be created!!!");
                exit(1);
        }
        if ( pipe(fd3) == -1 ) {
                perror("Pipe 3 cant be created!!!");
                exit(1);
        }
        if ( pipe(fd4) == -1 ) {
                perror("Pipe 4 cant be created!!!");
                exit(1);
        }



        //set these up for random numbers
        time_t t;
        srand(time(0));

        if ( pid1 = fork() ) { //first child process
          //write to parent
          write(fd1[1], "Child 1 Ready", strlen("Child 1 Ready")+1 );

          for (i = 0; i < rounds; i++) {
            readMessage = read( fd2[0], readPipe, 100); //read from the parent process

             //choose the choice
             srand(time(0));
             choice = (rand() % 3) + 1;

             //change into a string
             sprintf(chose, "%d", choice);

             //write the choice back to the parent through the pipe
             write(fd1[1], chose, strlen(chose) + 1);
          }


        }

        else if (pid2 = fork() ) { //second child process
          //write to parent
          write(fd3[1], "Child 2 Ready", strlen("Child 2 Ready")+1 );

          for (i = 0; i < rounds; i++) {
            readMessage = read( fd4[0], readPipe, 100); //read from the parent process
            sleep(1);
             //choose the choice
             srand(time(0));
             choice = (rand() % 3) + 1;

             //change into a string
             sprintf(chose, "%d", choice);

             //write the choice back to the parent through the pipe
             write(fd3[1], chose, strlen(chose) + 1);
        }

      }

      else { //parent process

/***
          pid1 = getpid();
          pid2 = getpid();
          printf("Child 1 PID: %d\n", pid1 );
          printf("Child 2 PID: %d\n", pid2 );
  ***/
          readMessage = read(fd1[0], readPipe, 100);
          printf(readPipe);
          printf("\n");
          readMessage = read(fd3[0], readPipe, 100);
          printf(readPipe);
          printf("\n");
          printf("Beginning %d Rounds.....\n", rounds);
          printf("Fight!\n");
          printf("-----------------------------------------\n");

          for (i = 0; i < rounds; i++) {
            printf("Round %d: \n",i );
            //signal to the two processes
            write(fd2[1], "Go", strlen("Go") + 1);
            write(fd4[1], "Go", strlen("Go") + 1);

            //read pipes from the child1
            readMessage = read( fd1[0], readPipe, 100);
            if ( strcmp(readPipe,"1" ) == 0 ) { printf( "Child 1 throws Rock!\n" ); }
            if ( strcmp(readPipe,"2" ) == 0 ) { printf( "Child 1 throws Paper!\n" ); }
            if ( strcmp(readPipe,"3" ) == 0 ) { printf( "Child 1 throws Scissors!\n" ); }

            /*** child2 convert it into a number ***/
            firstThrow = atoi(readPipe);

            //read pipes from child2
            readMessage = read( fd3[0], readPipe, 100);
            if ( strcmp(readPipe,"1" ) == 0 ) { printf( "Child 2 throws Rock!\n" ); }
            if ( strcmp(readPipe,"2" ) == 0 ) { printf( "Child 2 throws Paper!\n" ); }
            if ( strcmp(readPipe,"3" ) == 0 ) { printf( "Child 2 throws Scissors!\n" ); }

            /*** child1 convert it into a number**/
            secondThrow = atoi(readPipe);

            if ( firstThrow == 1 &&  secondThrow ==2 ) {printf("Child 2 Won!\n"); child2_win++; }
            if ( firstThrow == 1 &&  secondThrow ==3 ) {printf("Child 1 Won!\n"); child1_win++; }
            if ( firstThrow == 2 &&  secondThrow ==1 ) {printf("Child 2 Won!\n"); child2_win++; }
            if ( firstThrow == 2 &&  secondThrow ==3 ) {printf("Child 2 Won!\n"); child2_win++; }
            if ( firstThrow == 3 &&  secondThrow ==1 ) {printf("Child 1 Won!\n"); child1_win++; }
            if ( firstThrow == 3 &&  secondThrow ==2 ) {printf("Child 1 Won!\n"); child1_win++; }
            if ( firstThrow == secondThrow) {printf("Tie!!!!...So Noboody Wins or Looses!\n"); }
            printf("-----------------------------------------\n");

        }
            //close the pipes
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[0]);
            close(fd2[1]);
            close(fd3[0]);
            close(fd3[1]);
            close(fd4[0]);
            close(fd4[1]);

            //compare for final results
            printf("-----------------------------------------\n");
            if ( child1_win > child2_win) {
              printf("Results:\n");
              printf("Child 1: %d\n", child1_win);
              printf("Child 2: %d\n", child2_win);
              printf("Child 1 Wins Game!!!\n");
            } else {
              printf("Results:\n");
              printf("Child 1: %d\n", child1_win);
              printf("Child 2: %d\n", child2_win);
              printf("Child 2 Wins Game!!!\n");
            }
      }

    return 0;
}
