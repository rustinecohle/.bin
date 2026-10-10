Open Terminal and check:


gcc --version


If GCC is installed, it prints a version. If you see gcc: command not found, install it with:


sudo apt update &&

sudo apt install build-essential


Enter your password if prompted. build-essential installs GCC and common tools for compiling C programs. After installation, check again with gcc --version.


-Wall and -Wextra are enabled for a compile when you pass them on the command line. To check, make a tiny C file that triggers a warning:
printf 'int main(void) { int unused; return 0; }\n' > check.c
gcc -Wall -Wextra -c check.c
If GCC prints a warning about unused, the flags were applied. Without those flags, GCC may print nothing. Clean up afterward with:
rm -f check.c check.o
