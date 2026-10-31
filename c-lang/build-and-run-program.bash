# ./build-program.bash bounce.c # *.c file or files ( adapt it )
gcc $@ -lm $( pkgconf SDL2 --cflags --libs ) -o program && ./program
