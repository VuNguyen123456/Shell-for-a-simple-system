#include "fork.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SIZE 10001

int call_exe(Command *);

//Free 1 node
void freeNode(Command *node){
    int i; 
    for(i = 0; i < node->arguments_length; ++i){
        free(node->arguments[i]);
    }
    free(node->arguments);
    free(node->command);
    free(node);

}

void freeCommand(Command* cmd){
    Command* curr = cmd;
    Command* prev = NULL;
    //Free All Node that's in the link list
    while(curr!=NULL){
        prev = curr;
        curr = curr->next;
        freeNode(prev);
	    prev = NULL;
    }

}
//Main function
int main()
{
    char inBuf[SIZE];
    char inBUfCopy[SIZE];
    int n = 0;//n is the index
    //char *historyList[100];
    Command *cmd = NULL, *prev;
    prev = NULL;

    //Indefinte while loop 
    while(1){
        do{
        printf("262$");
        fflush(stdout);
        //If a EOF error appear
        if(fgets(inBuf,SIZE,stdin) == NULL){
            if (feof(stdin)) {
                //Free everything then exit
                freeCommand(cmd);
                return 0;
            }
        }
        //if user enter "enter"
        }while(inBuf[0] == '\n');

        strncpy(inBUfCopy, inBuf, SIZE);

        //Removing \n with \0
        int i = 0;
        for(i = 0; i < SIZE; ++i){
            if(inBuf[i] == '\n'){
                inBuf[i] = '\0';
                break;
            }
        }
        
        //Checking on if the list length is 100 if it is then shift 1 up
        if(n > 100){ 
            Command *tmp = cmd;
            cmd = cmd->next;
            Command *head = cmd;
            int w;
            for(w=0; w < tmp->arguments_length; ++w){
                free(tmp->arguments[w]);
            }
            free(tmp->arguments);
            free(tmp->command);
            free(tmp);
            int updateIndex = 0;
            while(head != NULL){
                head->index = updateIndex;
                updateIndex += 1;
                head = head->next;
            }
            n -= 1;
        }


//Building the command
        //Malloc the node, the command,  the arguments
        Command *curr = (Command*)malloc(sizeof(Command));
        if(curr == NULL){
            printf("Cannot allocate\n");
            exit(-1);
        }
        curr->command = (char*)malloc(sizeof(char)* SIZE);//Handle
        if(curr->command == NULL){
            printf("Cannot allocate\n");
            exit(-1);
        }
        curr->index = 0;
        curr->arguments_length = 0;
        curr->arguments = (char**)malloc(sizeof(char*) * 129);
        if(curr->arguments == NULL){
            printf("Cannot allocate\n");
            exit(-1);
        }
        curr->next = NULL;




//Reading input from user
        char *token = strtok(inBuf, " \n");
        // int track = 0;
        while(token != NULL){
            if(curr->arguments_length >= 128){
                fprintf(stderr, "error: %s\n", "too many arguments");
                free(curr->arguments);
                free(curr->command);
                free(curr);
                break; //exiting with error
            }
            //Assigning the thing in inBuf to arguments, also updates argument length
            curr->arguments[(curr->arguments_length)] = (char*)malloc(sizeof(char)* SIZE);
            strncpy(curr->arguments[(curr->arguments_length)++], token, SIZE);
            token = strtok(NULL, " \n");
        }
        //Make sure the last argument is null and assign index node
        curr->arguments[curr->arguments_length] = NULL;
            strncpy(curr->command, curr->arguments[0],SIZE);
            curr->index = n;

        int pass = 0;
        //if It's not a path
        if(inBuf[0] != '/'){
//ADD TO LIST IF VALID
        //If command is not history or blank then add it to link list 
        if(strcmp(curr->arguments[0], "history") != 0 && strcmp(curr->arguments[0], "\n") != 0){
            if (prev == NULL) {
                cmd = curr;
            } else {
                prev->next = curr;
            }
            prev = curr;
            n+=1;
        }

//Check the history of values to execute them Here: Only for the histtory[#] part
        //If command is history
        // printf("curr->argument[0]: %s\n",curr->arguments[0] );
        if(strcmp(curr->arguments[0], "history") == 0){
            // printf("HELLO\n");
            //If no argument
            if(curr->arguments_length == 1){
                //Will need to iterate through linkList then print out the command
                int w = 0, p;
                Command* head = cmd;
                while(head != NULL){
                    printf("%d:",w);
                    for(p = 0; p < head->arguments_length; ++p){
                        printf(" %s",head->arguments[p]);
                    }
                    printf("\n");
                    head = head->next;
                    w += 1;
                }
                //Free the node after done because don't want it in linkList
                // freeNode(curr);
            }

            //clear history
            else if(curr->arguments_length == 2){
                //Reset everything
                if(strcmp(curr->arguments[1],"-c") == 0){
                    freeCommand(cmd); 
                    n = 0;
                    cmd = NULL;
                    prev = NULL;
                    // freeNode(curr);
                }
            
            //Re-execute history
                //Check the index
                else if(atoi(curr->arguments[1]) >= 0 && atoi(curr->arguments[1]) < n){
                        int j;
                        Command* head = cmd;
                        while(head != NULL){
                            //If found the right index then change the size of the argument of both founded node and current node accordingly
                            if(atoi(curr->arguments[1]) == head->index){
                                strncpy(curr->command, head->command, SIZE);
                                if(head->arguments_length < curr->arguments_length){
				                    free(curr->arguments[1]);
                                    curr->arguments[1] = NULL;
                                    curr->arguments_length -= 1;
                                }
                                //if curr < Node
                                else if(head->arguments_length > curr->arguments_length){
                                    
                                    for(j = 0; j < head->arguments_length; ++j){
                                        if(j >= 2){
                                            curr->arguments[j] = (char*)malloc(sizeof(char)* SIZE);
                                        }
                                    }
                                    curr->arguments[j] = NULL;
                                    curr->arguments_length = j;
                                }

                                //Assigning the value of founded node to current node
                                for(j = 0; j < head->arguments_length;++j){ 
                                    strncpy(curr->arguments[j], head->arguments[j], SIZE);
                                }
                                //Re-executing is a path then don't add to linkList because it will be add in later
                                if(curr->command[0] != '/'){
                                    if (prev == NULL) {
                                        cmd = curr;
                                    } else {
                                        prev->next = curr;
                                    }
                                    prev = curr;
                                    n+=1;
                                }
                                //A pass specifically for re-executed path 
                                if(curr->command[0] == '/' ){
                                    pass = 1;
                                }
                                // if(strcmp(curr->arguments[0],"cd") == 0){
                                //     passCd = 1;
                                // }
                                break;
                            }
                            head = head->next;
                        }
                }
                //If enter invalid index
                else{
                        fprintf(stderr, "error: %s\n", "Index in history list does not exist");
                        // freeNode(curr);
                    }
            }
            //if the arguments was wrong
            // else{
            //     freeNode(curr);
            // }

        }
//Process input
        //Exiting
        else if(strcmp(curr->arguments[0], "exit") == 0 && curr->arguments_length == 1){
            break;
        }
        //Changing directory
        // printf("Hello2\n");
        // printf("PassCD: %d", passCd);
        if((strcmp(curr->arguments[0],"cd") == 0)){
            //chdir is finding if the directory exit or not
            if(curr->arguments_length < 2 || chdir(curr->arguments[1]) != 0){
                fprintf(stderr, "error: %s\n", strerror(errno));
            }
        }
        }
        //Call external command
        //Let it pass if it have the pass or it's a path
        if(pass == 1 || inBuf[0] == '/'){
            //Add value to linked list
            if (prev == NULL) {
                cmd = curr;
            } else {
                prev->next = curr;
            }
            prev = curr;
            n+=1;
            int status = call_exe(curr);
            int exit_status = WEXITSTATUS(status);
            //0 is successful
            if(exit_status != 0){
                fprintf(stderr, "error: %s\n", strerror(exit_status));
            }
        }

        if(strcmp(curr->arguments[0], "history") == 0){
            freeNode(curr);
        }

//LOOP BAck
    }
    //free everything
    freeCommand(cmd);
    return EXIT_SUCCESS;// return 0;
}
