# C-Shell

## Usage

1. make
2. ./C-Shell

You can exit the shell using the command `exit` or by sending the signal `Ctrl+D`

## Assumptions

- Hostname size is at max 1024 characters
- Path sizes are at max 2048 character
- History file should not be deleted during execution of the shell

## Features

### `;` separated commands

- You can run multiple shell commands in a single line by separating them with a `;`
- examples: `ls; echo "hello"; sleep 20`

### Input/Output redirection

- The shell supports redirection of input/output of a command to/from a file
- `<` is used to redirect input from a file
- `>` is used to redirect output to a file
- `>>` is used to append the output to a file (if it exists)
- example: `sort < inputFle > outputFile`

### Piping commands

- `|` can be used to redirect output of a command as input of another.
- The shell supports piping of multiple commands.
- example: `cat sample2.txt | head -7 | tail -2`

### Signals

- `Ctrl+Z` - sends the foreground process to the background and suspends it
- `Ctrl+C` - kills any running foreground process
- `Ctrl+D` - logs out of the shell

## Commands

The C-Shell supports the following built-in commands, in addition to all the basic linux commands.

### ls

- Lists the contents of the given directory.
- Add `-l` to display the details of each file.
- Add `-a` to display the hidden contents.
- ls accepts multiple flags and directories in any order.

### cd

- change the current working directory to the relative or the absolute path passed as argument.
- `~` or with no arguments to go back to the shell executable directory.
- `-` will change your directory to the previous directory you were in.

### pwd

- To see the present working directory `pwd`.

### pinfo

- Displays the process information.
    - Process ID
    - Process Status
    - Virtual memory
    - executable path.
- If the PID is not mentioned it will show the information about the shell process.

### history

- Displays the last 10 commands run on the shell, with most recent ones appearing at the bottom of the list.
- An optional argument can be used to specify the desired number of commands (max 20).

### Background process

- Append commands with '&' to run them in the background.

### jobs

- Displays all background processes running on the shell sorted in ascending order, along with their job number and
  state (running or stopped)
- Add flag `-r` to only display running processes
- Add flag `-s` to only display stopped processes
- Note that both flags cannot be used simultaneously.

### sig <jobNumber> <signal>

- Sends the signal corresponding to `signal` to the process with job number `jobNumber`

### bg/fg

- `bg <jobNumber>` sends a signal to continue the execution of a stopped background process (corresponding to job
  number `jobNumber`)
- `fg <jobNumber>` brings a background process (corresponding to job number `jobNumber`) to the foreground and continues
  its execution.

