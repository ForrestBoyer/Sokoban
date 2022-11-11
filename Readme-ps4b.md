# PS4b: Sokoban

## Contact
Name: Forrest Boyer
Section: 201
Time to Complete: 5 hours

## Description
Explain what the project does.
Ther project is an implementation of a block pushing game. The objective is to push all the boxes onto the selected squares. 

### Features
Describe what your major decisions were and why you did things that way.
I chose to make everything directly use the 2d char array leveldata in my class so as to not have multiple areas the level is stored. This helped a lot in making adding new features easier. The draw, move, and all other functions directly use the data from the same place so there can't be a mess up. 

### Memory
Describe how you decided to store the level data including whether you used smart pointers.
I stored the memory of my level data using a 2d vector of type char. I did not use smart pointers. 

### Lambdas
Describe what <algorithm> functions you used and what lambda expressions you wrote.
I used the count_if algorithm function for my isWon function. I use count_if to count the number of boxes in the level compared to the number of storage areas. I used two different lambdas as I called the function twice. These lambdas check the equality of the char of my leveldata to the character symbols for boxes or storage areas. They then return true or false to the count_if function to know whether or not to count that square. 

### Issues
What did you have trouble with?  What did you learn?  What doesn't work?  Be honest.  You might be penalized if you claim something works and it doesn't.
I didn't have trouble with much and I actually very much enjoyed working on this project since it was a game. I have coded many games in the past but none this large in C++ so I learned a lot doing so. The only thing that "doesn't work" is an interaction between two of the extra credits that I did. When using the z key to undo the rotation of the player does not change to how it was in that gamestate. 

### Extra Credit
Anything special you did.  This is required to earn bonus points.
I made it so the player sprite updates depending on what direction you move.
I made it so that you can use the z key as many times as is possible to undo moves. 

## Acknowledgements
List all sources of help including the instructor or TAs, classmates, and web pages.
If you used images or other resources than the ones provided, list them here.
cplusplus.com
sfml-dev.org
geeksforgeeks.com
freefontspro.com - I downloaded an arial font format to use to display the time. The file is arial.ttf