# C Implementation of Bash Shell

Welcome to the C Implementation of Bash Shell project! This project aims to recreate the basic functionalities of a bash shell using C. The primary goal is to gain a deeper understanding of how shell operations work and to handle various edge cases that arise in command-line interfaces.

## Key Features

### 1. Display Information

- **Current User and Hostname**: Displays the current user and hostname of the system.

### 2. Input Handling

- **Robust Input Processing**: Efficiently handles user inputs while taking care of edge cases.

### 3. Warp (cd Command)

- **Directory Change**: Changes the current directory and prints the full path after changing.
- **Handles Non-Existent Directories**: Provides error messages for invalid directories.
- **Flags Supported**:
    - `.` (current directory)
    - `..` (parent directory)
    - `~` (home directory)
    - `` (previous directory)
- **Sequential Execution**: Processes multiple arguments sequentially.
- **Default Directory**: Defaults to the home directory if no arguments are provided.

### 4. Peek (ls Command)

- **List Files and Directories**: Lists all files and directories in lexicographic order.
- **Flags Supported**:
    - `a`: Show all files, including hidden files.
    - `l`: Show detailed information about files.

### 5. Past Events (history Command)

- **Command History**: Manages and displays command history similar to bash.
    - `pastevents purge`: Clears command history.
    - `pastevents execute <index>`: Executes a specific command from history.
    - **Default Behavior**: Displays the last 15 commands if no arguments are provided.

### 6. System Commands

- **Execute System Commands**: Capable of executing other bash system commands like `emacs`, `gedit`, etc.
- **Background Processing**: Supports background processing with the `&` symbol.

### 7. Proclore (ps Command)

- **Process Information**: Retrieves and displays information about processes.
    - **Default Behavior**: Displays information about the shell process if no arguments are provided.

### 8. Seek (find Command)

- **File and Directory Search**: Searches for files/directories in the specified directory or current directory.
- **Flags Supported**:
    - `d`: Search only directories.
    - `f`: Search only files.
    - `e`: Executes appropriate actions if exactly one result is found, with necessary permissions.
## Running the Code

To run the code, follow these steps:

### Compile the Code

```bash
bashCopy code
make

```

### Execute the Shell

```bash
bashCopy code
./a.out

```

### Command Examples

- **Change Directory**:
    
    ```bash
    bashCopy code
    warp /path/to/directory
    
    ```
    
- **List Files**:
    
    ```bash
    bashCopy code
    peek -a -l
    
    ```
    
- **View Command History**:
    
    ```bash
    bashCopy code
    pastevents
    
    ```
    
- **Execute System Command in Background**:
    
    ```bash
    bashCopy code
    emacs &
    
    ```
    
- **Retrieve Process Information**:
    
    ```bash
    bashCopy code
    proclore
    
    ```
    
- **Search for Files/Directories**:
    
    ```bash
    bashCopy code
    seek -f -e directory filename
    
    ```
    

