# CON31-C_POC
PoC of the rule CON31-C of SEI CERT C Coding Standard.

# Installation & Execution
1) You must have the "musl-gcc" compiler in order to compile "threads.h" library in UNIX systems: `sudo apt install musl-tools`
2) Clone the repo: `git clone https://github.com/Pitenager/CON31-C_POC.git`
3) Compile the source code: `musl-gcc concurrency.c -o concurrency -std=gnu11`
4) Execute the program: `./concurrency [0/1]` (0 = non compliant solution / 1 = compliant solution)
5) If you want to perform a quick analysis, you can run `perf stat ./concurrency [0/1]`
