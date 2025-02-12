# How to run
1. In terminal type 
```
make clean && make && make run-server
```
which will compile, link and run the game engine's code for the main master peer node.

2. Depending on the number of clients you have, you type this to join as a peer (increase the number for the next client)
```
make clean && make && make run-client1
```   
4. It is important to note that this is the engine's code only. Each of the three team memebers will submit their own main.cpp file which you will have to replace in order to test each individual game. 

5. To test the scaling bonus please press T
