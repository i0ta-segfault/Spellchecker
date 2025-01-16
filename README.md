# Spellchecker

I made this project for a data structures and algorithms course I took in college.

It's a simple spellchecker that uses Levenshtein distance to figure out suggestions for correct words. 

The program gives 2-3 word suggestions. Using the program is quite simple actually, but if you need help running the application, enter:

```bash
./main --help
```
This should bring up a "man" page for the application!

Ofcourse this goes without saying but, you should have ncurses installed on your system and make (preferably)

Once all dependencies are taken care of run :

```bash
make clean        #this is just to clean up
```

```bash
make              #create the binaries
```

```bash
./main            #run the application
```
