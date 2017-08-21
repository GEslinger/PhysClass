# PhysClass
## Computational Physics
If you want to run my programs (I'm guessing you do), then you should run this command
```bash
echo 'runprg(){ mv main bkup; g++ $1 -std=c++11 -o main -I . -lboost_system -lboost_iostreams; ./main; };' >> ~/.bashrc
```
Then restart your terminal.
To run the programs type ```runprg PROGRAM``` and make sure you're in the same directory.
See the canvas upload for specific instructions!
