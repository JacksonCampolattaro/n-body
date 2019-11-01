# MINI
### Modular Integrator for N-body Interaction

An n-body Gravity Simulator by Jackson Campolattaro.

## Concept

The project is an [N-Body newtonian physics simulation](https://en.wikipedia.org/wiki/N-body_simulation). I originally adopted the idea because while it's conceptually simple, the emergent properties can be [beautiful](https://www.youtube.com/watch?v=DoLe1c-eokI). Originally built as an excersise to learn C++, it's grown to be something larger. I have a lot of plans for the future of this project, but only a limited amount of time. I look forward to incorporating more features and techniques as I learn what's possible!

## Progress

I've learned a lot while working on this project. It's been an opportunity to become familiar with project management toosls, several powerful open-source libraries, and important techniques and paradigms.

### Algorithms

I originally built a functioning system using the naive algorithm (calculating physics for each pair of bodies, order O(n^2)) as a proof of concept. Being new to the language, it was mostly written inside my main.cpp, with only one "body" class to hold information about the orbital bodies. Once I had a working solution, I quicly split out functionality and began my optimization.

The O(n^2) performance of the naive algorithm very quickly became a limiting factor in the size and speed of my simulation, even after multithreading was added. The clear next step was to implement the tree-based Barnes-Hut algorithm, which reduces the problem to O(nlog(n)). A fantastic interactive explanation is available [here](https://jheer.github.io/barnes-hut/), and further information about the implementation can be found in [these curriculum materials](https://www.cs.princeton.edu/courses/archive/fall03/cs126/assignments/barnes-hut.html).

Adding another algorithm necessitated splitting off that part of the code. My current solution is for each algorithm to implement a virtual "Solver" class. This makes it very easy to switch between solvers, and makes it clear exactly the functionality any future solvers will need to have. 

I've made a few improvements to my particular implementation of the Barnes-Hut algorithm. The one I'm most proud of is re-centering the tree on each cycle. I noticed that the performance of the BH algorithm is at its best when the bodies are evenly distributed around the location of the tree's center. Profiling showed that when the bodies are offset to one side or very far from the center, the program spends a lot of time just building the tree.

### Multithreading

### Graphics

### Interface design

### Signals & Synchronicity

### Serialization


## Potential Research
TODO
