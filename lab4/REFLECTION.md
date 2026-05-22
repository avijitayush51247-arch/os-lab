# LAB 4 REFLECTION

The hardest concept in this lab was understanding inter-process communication using pipes and how file descriptors are shared between processes. At first, it was confusing how data flows between parent and child processes, but after implementing pipe-based programs, it became clearer.

What clicked unexpectedly for me was how fork, exec, and wait work together to form the basis of a shell. I realized that the Linux terminal is not magic but just a program using these system calls.

For future projects, I would like to try building a more advanced shell with features like multiple pipelines, job control, and process scheduling simulation. I am also interested in exploring how operating systems handle deadlocks and memory management internally.
