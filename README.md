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

I've made a few improvements to my particular implementation of the Barnes-Hut algorithm. The one I'm most proud of is re-centering the tree on each cycle. I noticed that the performance of the BH algorithm is at its best when the bodies are evenly distributed around the location of the tree's center. Profiling showed that when the bodies are offset to one side or very far from the center, the program spends a lot of time just building the tree. It needed to subdivide hundreds of times to give each body its own node. To prevent this, each cycle my algorithm finds the ideal location for the next cycle's tree. This comes with a small overhead, but it makes the program much faster than it was before. This solution almost certainly isn't unique but it's not something I saw in other people's implementations.

### Multithreading

Adding multithreading has been incredibly gratifying. After some experimentation with Posix threads and [Intel's TBB](https://github.com/intel/tbb), I settled on using [OpenMP](https://www.openmp.org/) to add concurrency. After adding a couple of `#pragma omp parallel for` complier directive in some key places, I could already see a massive improvement! For the naive algorithm, benchmarking showed that the performance was almost exactly multiplied by my CPU's number of threads, and the Barnes-Hut algorithm came close to the same improvement.

In the future I'm hoping to explore offloading some of the work to the GPU; this has become one of my major long-term goals for this project. Cuda and OpenCL are both pretty daunting; writing a kernel is on a much lower level than everything else I've done so far. Luckily, OpenMP provides [its own solution](https://www.ibm.com/support/knowledgecenter/en/SSXVZZ_16.1.0/com.ibm.xlcpp161.lelinux.doc/compiler_ref/prag_omp_target.html) for targeting an accelerator, which only requires a simple compiler directive! Unfortunately, as of 2019 this only supports language primitives and arrays. This means that in order to actually create my own GPU-based solver, I'll need to write it without any of the libraries I've come to rely on.

### Graphics

Throughout this project, graphics have consistently been a source of frustration. When I built my proof of concept, I used freeGLUT because there were so many tutorials and examples available for it. GLUT was a great graphics library for beginners... in 1999. 

GLUT quickly became limiting, especially because of the way it takes control of the main loop. I soon switched to GLFW, but I used the legacy settings, because I wasn't prepared to write my own shaders.

Eventually, even that became an issue, and the pre-2.0 OpenGL became a major performance bottleneck. I've since replaced it with [Raylib](https://www.raylib.com/). The main thing that led me to choose raylib over other options is their robust documentation, especially when it comes to cmake. I'm still not entirely happy with Raylib for several reasons, and I'm experimenting with other solutions, including [Magnum](https://magnum.graphics/), [Ogre](https://www.ogre3d.org/), and even [Qt 3d](https://doc.qt.io/qt-5/qt3d-index.html).

### Interface design

I've built a simple interface for controlling the program using [Gtkmm](https://www.gtkmm.org/en/), the C++ interface for Gtk+. I've been very happy with Gtkmm because it's very lightweight and looks native on my operating system (Fedora linux). I also considered Qt, which worked nicely but felt way too heavy for my purposes, and WxWidgets, which is just a bit too messy for me.

The interface was a lot of fun to put together, and it was very satisfying being able to get the tangible results of actual buttons and dropdowns appearing. Right now, I can use the interface to choose between a couple of solvers, configure the rules my physics will follow, and customize how the results are displayed.

An interface isn't entirely necessary, and in the end the best way of using the program should probably be by the command line, but that's not what this is about. Integrating something like this into my project was a great learning experience, and helped me improve even seemingly unrelated parts. Adding each feature required making changes to how my project was structured. For example, in order to have easily customizable physics I split the physical rules into their own object, which is passed to the solver. This was a more robust paradigm that made future changes like logging and serialization significantly easier.

### Signals & Synchronicity


### Serialization

## Next Steps

## Potential Research
TODO
