# Maze optimisation project

A research project I've done as part of a university academic writing assignment.

The project involves maze generation, heuristic algorithms, optimisation, and a really long and convoluted problem statement. 

## Background

To gather commonly-used heuristic strategies, a survey was conducted. Participants were presented with a challenging problem that they had to attempt to solve.

You can find a description of the problem in `problem.md` and the full survey in `survey.md`.

## Documentation

In the following discussion, we assume that the reader has already familiarised themselves with the problem statement and the definition of a `123-maze`.

`maze_gen` is for generating a `123-maze`. Change the dimensions of the maze in `main()`, run the program and it should generate a `123-maze`. Keep in mind that there is an upper bound of `250` for both maze dimensions.

`maze_run` runs different participant-submitted heuristics and optimisation algorithms. 
Since some algorithms can be quite slow, it is not recommended to run them all at once. 

To successfully run an algorithm, make sure it is uncommented in `main()`.

### `maze_run` implements $13$ different strategies to solve the presented problem:

- `random` - pick $100$ random cells per iteration until you run out of potions, take the best result
- `pso` - particle swarm optimisation using `random` as base candidate solutions. Uses $30$ particles and $70$ attempts to improve their global `total_score`. Encodes each candidate solution as a *particle* -- a vector in $\mathbb{R}^{200}$
- `multi-dfs` - starts multiple depth-first searches (choose random nodes to go next) starting from the top-left corner; chooses the one which gave max `total_score`
- `catalan` - constructs Catalan-esque (bottom or right moves only) paths from the top-left corner towards the bottom-right corner; skips every other cell (checkered path).
- `pattern` - chooses $35$ random cells in $0\le{x}<{\frac{N}{4}}, 0\le{y}<{\frac{M}{4}}$, $30$ random cells in ${\frac{N}{4}}\le{x}<{\frac{3N}{4}}, {\frac{M}{4}}\le{y}<{\frac{3M}{4}}$ and $35$ random cells in ${\frac{3N}{4}}\le{x}<N, {\frac{3M}{4}}\le{y}<M$ per iteration. Takes the best result; similar to `random`
- `corners` - same as `pattern`, except no cells are chosen in the middle.
- `single-dfs` - does one depth-first search across the entire `123-maze` (split over $100$ potions). For the last iteration, choose the segment which gave the best `total_score`
- `skip-dfs` - attempts to improve on `multi-dfs` by selecting every other cell and backtracking where necessary.
- `greedy` - attempts to improve on `multi-dfs` by prioritising lower edge weights.
- `improve` - perform a dfs while inspecting all of the neighbouring cells and choosing to go to the one which maximises your total_score next (random in case of a tie). Costs more nodes to construct a single full chain, hence lowering the amount of effective potions used to $\approx25$.
- `crossroad` - attempts to improve on `multi-dfs` by prioritising crossroads.
- `checker` - chooses $50$ random cells in a checkerboard pattern each in the top-left and bottom-right corners. Random cells are chosen in the interiors of rectangles of varied sizes. Take the best result
- `aco` - depth-first search based ant colony optimisation algorithm. Trails of pheromones are left as ants walk across the maze. Similar to `multi-dfs`, but prioritises stronger pheromone trails.
- `pso-improve` - combines `improve`, `checker`, and `aco` strategies to improve overall efficiency. Best strategy currently known to solve this problem as of yet.

### Explanation of classes and structs

- `invoke_srand`: stub class; calls `srand(time(0));` upon executing the program.
- `maze123`: 123-maze data structure.
    - `maze123()`: object constructor.
    - `see()`: prints maze in readable format.
    - `see_paths()`: prints the paths stored in the maze (which are used to calculate `total_score`).
    - `in_bounds(int x, int y)`: checks if the node $(x,y)$ is within the bounds of the maze.
    - `dfs(...)`: random dfs with allowed branching (replacing walls with edges)
    - `add_dfs(...)`: dfs used to generate more `good` paths and store them in the maze.
    - `regen()`: re-generates the maze. Called in `maze123()`.

### Explanation of other functions
- `encode(int n, int m)`: converts node $(n,m)$ to a single integer using Euclidean residue encoding.
- `decode(n)`: returns the unique pair of integers $(x,y)$ which are converted to $n$ via `encode`.
- `total_score(...)`: calculates the `total_score` of a given `unordered_set` of `encoded` cells.
- `total_score_vocative(...)`: same as `total_score()` but prints additional stuff.
- `convert_v_to_us(...)`: converts a `vector<int>` to an unordered set.
- `convert_vii_to_us(...)`: converts a `vector<array<int,2>>` to an unordered set.
- `rng(...)`: generates a random set of guesses.
- `rng_vec(...)`: converts the output of `rng()` into a guess sequence of nodes stored in the format `vector<array<int,2>>`.
- Other functions are called in previously-mentioned heuristic algorithms and are quite self-explanatory.
