# Shell-for-a-simple-system
This program implements a shell that processes user commands, maintains a command history, and executes commands using external programs. It begins by defining constants, including SIZE for buffer size, and a function prototype call_exe for executing commands. Additionally, helper functions like freeNode and freeCommand are provided to manage memory for command structures and linked lists, ensuring proper cleanup when commands are removed or the program exits.

The main function initializes a command history system with a linked list to store commands. It reads user input in an indefinite loop, displaying a shell prompt (262$) and processing commands until an exit condition is met. Input handling includes removing newline characters, validating arguments, and storing commands in dynamically allocated structures. Commands are stored in a linked list, maintaining an index for retrieval from history.

If the history exceeds 100 entries, the oldest command is removed, and indices are updated. Special handling is provided for the history command, allowing users to view previous commands, clear the history, or re-execute commands by index. Invalid indices generate error messages, and memory is dynamically adjusted to match the size of historical commands.

The shell supports built-in commands like cd for changing directories, validating paths, and printing errors if the operation fails. It also implements the exit command to terminate execution, ensuring all memory associated with commands is freed before exiting.

External commands are handled by passing arguments to call_exe, which executes the process and captures its exit status. Errors are displayed if the process fails. Commands containing absolute paths (/) are treated as external commands without being added to the history, while re-executed history commands are tracked.

The program concludes by freeing all allocated memory and exiting successfully. Error handling, dynamic memory allocation, and command parsing make this shell a robust tool for command execution with history tracking.
