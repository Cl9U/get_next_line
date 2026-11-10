*This project has been created as part of the 42 curriculum by sfaraji.*

# get_next_line

## Description

**get_next_line** is a C function that returns **one line at a time** from a file descriptor, so that repeated calls read a whole file (or the standard input) line by line.

```c
char *get_next_line(int fd);
```

- It returns the line that was read, **including the terminating `\n`**, except for the last line of a file that does not end with a `\n`.
- It returns `NULL` when there is nothing left to read, or when an error occurs.
- It works on any file descriptor: regular files, standard input, pipes.
- The size of the chunks read from the file is set at compile time with `-D BUFFER_SIZE=n`.

The goal of the project is to learn how `read` really behaves, and to use **static variables** to keep a state between function calls, while managing the heap without leaks.

**Allowed functions:** `read`, `malloc`, `free`. `libft`, `lseek` and global variables are forbidden.

### Features

| Part | Feature |
|---|---|
| Mandatory | `get_next_line` for a single file descriptor, using one static pointer |
| Bonus | several file descriptors at the same time, with **one single static variable**, and no limit on the fd number |
| Both | the delimiter is a macro (`DELIM`, `'\n'` by default), so the function can split on another character |

### Files

| File | Content |
|---|---|
| `get_next_line.c`, `get_next_line.h`, `get_next_line_utils.c` | mandatory part |
| `get_next_line_bonus.c`, `get_next_line_bonus.h`, `get_next_line_utils_bonus.c` | bonus part |

## Instructions

### Compilation

The project must compile with and without `BUFFER_SIZE` (the default value is `42`).

```bash
# mandatory
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c

# without the flag (uses the default BUFFER_SIZE)
cc -Wall -Wextra -Werror get_next_line.c get_next_line_utils.c main.c

# bonus
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```

`main.c` is your own test program, it is not part of the project.

### Usage example

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int	main(int argc, char **argv)
{
	int		fd;
	char	*line;

	if (argc != 2)
		return (1);
	fd = open(argv[1], O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		printf("[%s]", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```

Reading from the standard input works the same way with `fd = 0`:



### Changing the separator

The character that ends a line is the `DELIM` macro in the header. To split on another character, change this line and recompile:

```c
# define DELIM '\n'
```

With `DELIM` set to `'e'`, the text `hello world` is returned as `he` and `llo world`.

### Testing

The project was checked with:

- several buffer sizes: `1`, a small size, `42`, a large size, and `10000000`;
- a buffer size equal to the line length, and one byte more or less;
- files with and without a final `\n`, empty files, files with empty lines, very long lines;
- standard input through a pipe;
- an invalid, a closed and a negative file descriptor;
- a memory checker (`valgrind` or `-fsanitize=address`) to look for leaks.

## Algorithm

### The problem

`read(fd, buffer, BUFFER_SIZE)` returns **up to** `BUFFER_SIZE` bytes. It does not stop at a `\n`, so one call can return the end of one line, several complete lines, and the beginning of the next one. The file offset of the descriptor only moves forward (and `lseek` is forbidden), so the bytes that were read **after** the line we return are gone from the file. They must be kept somewhere until the next call.

### The solution: a static "leftover" string

A `static char *left` keeps the text that was read but not yet returned. A static variable lives for the whole program and starts as `NULL`, which is exactly the memory the function needs between two calls, without a global variable.

Each call to `get_next_line` does three steps:

1. **`fill_left`**: while `left` contains no `DELIM` and `read` still returns data, read `BUFFER_SIZE` bytes into a temporary buffer, add a `'\0'`, and append the buffer to `left` with `ft_strjoin`. It stops as soon as a `DELIM` appears, so the function **reads as little as possible** and never loads the whole file.
2. **`get_line`**: copy from the start of `left` up to and including the first `DELIM` (or up to the end if there is none). `left` is not modified. If `left` is empty, it returns `NULL`, which is the end-of-file signal for the caller.
3. **`trim_left`**: build a new string with what comes after the line, free the old `left`, and return the new one. If nothing remains, it frees `left` and returns `NULL`, so the next call starts from a clean state.

### The states of `left`

| Value | Meaning |
|---|---|
| `NULL` | nothing stored (start of the file, or everything was returned) |
| `""` | end of file was reached and nothing remains |
| `"text..."` | text waiting to be returned |

After the last line is returned, `left` is `NULL` again, so any further call returns `NULL` safely.

### Return values of `read`

| `read` returns | Handling |
|---|---|
| `> 0` | data is appended to `left` |
| `0` | end of file: the loop stops, the last text (without `\n`) is still returned |
| `-1` | error: the buffer and `left` are freed and `NULL` is returned |

A short read is normal (pipes and terminals return what is available), so the loop never assumes that the buffer was filled.

### Memory management

- `ft_strjoin` **frees its first argument**, so replacing `left` never leaks the old string.
- `trim_left` always frees the old `left`, so `get_next_line` can overwrite it with the result.
- The returned line belongs to the caller, who must free it.
- Every error path (`malloc` failure, `read` error) frees everything allocated so far through the `release` helper, then returns `NULL`.
- `fd < 0` and `BUFFER_SIZE <= 0` are rejected at the start, because `BUFFER_SIZE` is chosen at compile time and the compiler accepts any value. The buffer size is converted to `size_t` before the `+ 1`, so a very large `BUFFER_SIZE` cannot overflow an `int`.

### Bonus: one list node per file descriptor

Each file descriptor has its own read position and its own leftover text, so the bonus stores **one leftover per fd**. It uses a linked list whose head is the **single static variable** of the function:

```c
typedef struct s_node
{
	int		fd;
	char		*left;
	struct s_node	*next;
}	t_node;

static t_node *head;
```

For each call:

1. `get_node` finds the node of this `fd`, or creates it with an empty `left`.
2. The same `fill_left` and `take_line` steps as the mandatory part work on **that node's** text. They do not know that several files exist.
3. When the text of a node becomes `NULL` (end of file or error), the node is removed from the list and freed.

`take_line` takes a `char **` because it has to return the line **and** replace the caller's `left` pointer. `get_node` takes a flag (`drop`) to find/create or to remove a node, which keeps the number of functions per file within the Norm.

**Why a list and not an array** such as `static char *left[1024]`: an array needs a fixed size, and a valid fd can be larger than any size chosen in advance (the limit of open files is a system setting). A list grows with the number of descriptors in use, has no limit, and still counts as one static variable. An invalid fd leaves no node behind, because the node is removed as soon as `read` fails.

## Resources

### References

- `man 2 read`, `man 2 open`, `man 2 dup`: behavior of `read`, file descriptors and file offsets (https://man7.org/linux/man-pages/)
- `man 3 malloc`, `man 3 free`: dynamic memory allocation
- Brian W. Kernighan and Dennis M. Ritchie, *The C Programming Language*: static variables, pointers, and storage duration
- ISO C standard (storage duration of objects, integer overflow): https://www.open-std.org/jtc1/sc22/wg14/
- Valgrind manual (leak checking): https://valgrind.org/docs/manual/
- AddressSanitizer documentation: https://clang.llvm.org/docs/AddressSanitizer.html
- gnlTester, used to check the project: https://github.com/Tripouille/gnlTester
- The 42 Norm and the project subject (get_next_line)

### Use of AI

An AI assistant was used as a **learning**.
| Understanding concepts | explanations of static variables, memory sections (`.bss`, heap, stack), file descriptors and file offsets, how `read` and `free` work, EOF, memory leaks, pointers to pointers |
