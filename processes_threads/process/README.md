# Process

The parent process (the master) reads everything its child process (the slave)
writes to standard output.

A process can't `read` back from its own `STDOUT_FILENO`, because it is a
write-only file descriptor that goes straight to the terminal. So the program
creates a `pipe` before `fork`, and in the child uses `dup2` to point
`STDOUT_FILENO` at the write end of the pipe. Anything the child prints with
`printf` or `write` now goes into the pipe, and the parent reads it from the
read end and prints it on the real terminal.

| Step | Master (parent) | Slave (child) |
| --- | --- | --- |
| 1 | Prints its pid, `pipe`, `fork` | |
| 2 | Closes the write end `pipe_fd[1]` | Closes the read end `pipe_fd[0]` |
| 3 | | `dup2(pipe_fd[1], STDOUT_FILENO)`, then closes `pipe_fd[1]` |
| 4 | `read`s the pipe one byte at a time and `putchar`s it | `printf`s one progress line per second for each character of its message |
| 5 | | `fflush(stdout)`, `write`s "Child <pid>, is done sending" and closes `STDOUT_FILENO` |
| 6 | `read` returns 0 (end of file), `waitpid` for the child | Exits |

```mermaid
sequenceDiagram
    participant M as Master (parent)
    participant P as Pipe
    participant S as Slave (child)
    Note over M: pipe → fork
    Note over S: close pipe_fd[0], dup2(pipe_fd[1], STDOUT_FILENO)
    S->>P: printf / write to STDOUT_FILENO
    P->>M: read(pipe_fd[0])
    Note over S: close STDOUT_FILENO
    P-->>M: read returns 0 (end of file)
    Note over M: waitpid
```

## Build & run

```bash
cmake -S . -B build
cmake --build build
./build/main
```

The child sleeps one second per character of its message, so the program
takes about 25 seconds to finish. Expected output (the pids will differ):

```
Hi, I am the master 147793
I want to read from the slave 147793
Progress 0 on char H
Progress 1 on char i
Progress 2 on char ,
...
Progress 24 on char 4
Progress 25 on char
Child 147794, is done sending
Finished Receiving by Master 147793
```

The progress lines show up one per second because stdout was first used on the
terminal, before `fork`, so it is line buffered, and the child keeps that after
`dup2`. The child calls `fflush(stdout)` before its final `write`, so the lines
also arrive when the output is piped (`./build/main | cat`), just all at the
end.

Written and tested on Linux (WSL2). It uses `<sys/errno.h>`, which exists on
Linux and macOS; `<errno.h>` is the portable header.
