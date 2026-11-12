# Processes & Threads

Playing with processes on Linux using the POSIX API: creating a child with
`fork`, connecting parent and child with a `pipe`, redirecting the child's
standard output with `dup2`, and running a command in the child with `execlp`
while the parent reads its progress without blocking.

| Folder | Topic | Concepts |
| --- | --- | --- |
| [`process/`](process/) | Parent reads what the child prints | `fork`, `getpid`, `pipe`, `dup2`, `STDOUT_FILENO`, `read`/`write`, `waitpid` |
| [`process_com_without_wait/`](process_com_without_wait/) | Parent reads a command's progress without blocking | `execlp`, `fcntl`, `O_NONBLOCK`, `waitpid` with `WNOHANG` |

```mermaid
graph LR
    subgraph Child [child / slave]
        A["printf / write(STDOUT_FILENO)"]
    end
    subgraph Parent [parent / master]
        C["read(pipe_fd[0])"] --> D[putchar to terminal]
    end
    A -- "fd 1 = pipe_fd[1] (dup2)" --> B[(pipe)] --> C
```

## Build & run

Both projects use CMake:

```bash
cd process                       # or process_com_without_wait
cmake -S . -B build
cmake --build build
./build/main                     # ./build/PROCESS_COM_NO_WAIT in process_com_without_wait
```

See each project's README for what the program does.
