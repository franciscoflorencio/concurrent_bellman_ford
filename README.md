# concurrent_bellman_ford

This project implements a parallel Bellman-Ford algorithm using pthreads.

Build and run using the provided Makefile.

Recommended build (use 2 cores here as an example):

```bash
make -j2
```

Run the program via Make:

```bash
make run
```

Cleanup (remove object files and binary):

```bash
make clean
```

Note for pull/merge requests
---------------------------
Before creating a merge request, please run:

```bash
make clean
```

This ensures the repo does not contain built artifacts and avoids merge conflicts with generated binaries.