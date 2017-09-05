# PhysClass
## Computational Physics
If you want to run my first two projects (Logistic Map or Drag), then you should run this command
```bash
echo 'runprg(){ mv main bkup; g++ $1 -std=c++11 -o main -I . -lboost_system -lboost_iostreams; ./main; };' >> ~/.bashrc
```
Then restart your terminal.
To run the programs type ```runprg PROGRAM``` and make sure you're in the same directory.

For my later projects, I use a Makefile and suggest running commands directly.
CD into the project directory, run ```make``` and then ```./main``` or whichever program name makes sense.
See the canvas upload for specific instructions!
