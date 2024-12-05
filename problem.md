While exploring, you encounter the legendary **123-maze** of dimensions **$N=100$** and **$M=100$**. 

The entrance to the maze is at its **top-left corner**, and the exit out of the maze is at its **bottom-right** corner.

However, **you do not know where any of the walls inside the maze are placed**.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
After kindly asking the maze warden for some help, they give you **$Q=100$** hint queries.

When asking the warden for a hint, one may select any cell in the maze and ***reveal*** it. 
For every *revealed* cell, the warden will say the information about its connections to adjacent cells. 

**The cells are revealed sequentially.**
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
You would like to get out of the maze **as quickly as you can**. 
To help make decisions, you have invented a property of the revealed cells called ***score***.

A ***good*** path is **any path which completes the maze**. A 123-maze will usually have multiple good paths. 
The *score* of a **good path** is defined as the number of *revealed* nodes in the path divided by the travel time:

$\mathrm{path\_score} = \frac{\mathrm{path\_revealed\_nodes}}{\mathrm{path\_travel\_time}}$

The ***total score*** of the maze is ***twice*** **the maximum** of all *scores* of good paths.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
The warden will tell your ***total score*** rounded to three decimal places after every hint they give.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
In addition to all this, you have **$W=100$** magical potions in your backpack which may be used anytime to go back to the time when you arrived at the maze and haven't used any hints. 

After drinking a potion, you retain all previously received information about the ***total scores*** which the cell sequences you guessed produced, **but forget all information about cell connections in the process**. 

When calculating your score, **only the hints asked in the most recent timeline are considered** since all other cells have not been *revealed* in the current timeline.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
**Conquer the 123-maze by maximising your** ***total score***!
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
* *Note.
 You may play around with the problem as you see fit and change values 
of the parameters Q and/or W. If you decide to attempt a modified 
version of this problem, include the modifications in one of your 
answers. In addition, the default version of the problem may also change
 as more responses to the survey are collected if the problem deems to 
be too unbalanced (either trivial or nearly impossible). We apologise in
 advance for any possible confusion caused by this.*
