# Processes & Threads

Playing with processes on Linux using the POSIX API: creating a child with
`fork`, connecting parent and child with a `pipe`, and redirecting the child's
standard output with `dup2`.

| Folder | Topic | Concepts |
| --- | --- | --- |
| [`process/`](process/) | Parent reads what the child prints | `fork`, `getpid`, `pipe`, `dup2`, `STDOUT_FILENO`, `read`/`write`, `waitpid` |

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

The project uses CMake:

```bash
cd process
cmake -S . -B build
cmake --build build
./build/main
```

See the [`process/` README](process/) for what the program does.
