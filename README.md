# MINI
### Modular Integrator for N-body Interaction

An n-body Gravity Simulator by Jackson Campolattaro.

![Galaxy](https://github.com/JacksonCampolattaro/n_body/blob/master/doc/galaxy.png)

NOTE: The branch gtkmm-magnum contains the most up-to-date code, but isn't currently functional.

## Concept

The project is an [N-Body newtonian physics simulation](https://en.wikipedia.org/wiki/N-body_simulation). I first adopted the idea because while it's conceptually simple, the emergent properties can be [beautiful](https://www.youtube.com/watch?v=DoLe1c-eokI). Originally built as an excersise to learn C++, it's grown to be something larger. I have a lot of plans for the future of this project, but only a limited amount of time. I look forward to incorporating more features and techniques as I learn what's possible!

## Progress

I've learned a lot while working on this project. It's been an opportunity to become familiar with project management tools, several powerful open-source libraries, and important techniques and paradigms.

### Algorithms

I originally built a functioning system using the naive algorithm (calculating physics for each pair of bodies, order O(n^2)) as a proof of concept. Being new to the language, it was mostly written inside my main.cpp, with only one "body" class to hold information about the orbital bodies. Once I had a working solution, I quicly split out functionality and began my optimization.

The O(n^2) performance of the naive algorithm very quickly became a limiting factor in the size and speed of my simulation, even after multithreading was added. The clear next step was to implement the tree-based Barnes-Hut algorithm, which reduces the problem to O(nlog(n)). A fantastic interactive explanation is available [here](https://jheer.github.io/barnes-hut/), and further information about the implementation can be found in [these curriculum materials](https://www.cs.princeton.edu/courses/archive/fall03/cs126/assignments/barnes-hut.html).

Adding another algorithm necessitated splitting off that part of the code. My current solution is for each algorithm to implement a virtual "Solver" class. This makes it very easy to switch between solvers, and makes it clear exactly the functionality any future solvers will need to have. 

I've made a few improvements to my particular implementation of the Barnes-Hut algorithm. The one I'm most proud of is re-centering the tree on each cycle. I noticed that the performance of the BH algorithm is at its best when the bodies are evenly distributed around the location of the tree's center. Profiling showed that when the bodies are offset to one side or very far from the center, the program spends a lot of time just building the tree. It needed to subdivide hundreds of times to give each body its own node. To prevent this, each cycle my algorithm finds the ideal location for the next cycle's tree. This comes with a small overhead, but it makes the program much faster than it was before. This solution almost certainly isn't unique but it's not something I saw in other people's implementations.

### Multithreading

Adding multithreading has been incredibly gratifying. After some experimentation with Posix threads and [Intel's TBB](https://github.com/intel/tbb), I settled on using [OpenMP](https://www.openmp.org/) to add concurrency. After adding a couple of `#pragma omp parallel for` complier directive in some key places, I could already see a massive improvement! For the naive algorithm, benchmarking showed that the performance was almost exactly multiplied by my CPU's number of threads, and the Barnes-Hut algorithm came close to the same improvement.

In the future I'm hoping to explore offloading some of the work to the GPU; this has become one of my major long-term goals for this project. Cuda and OpenCL are both pretty daunting; writing a kernel is on a much lower level than everything else I've done so far. Luckily, OpenMP provides [its own solution](https://www.ibm.com/support/knowledgecenter/en/SSXVZZ_16.1.0/com.ibm.xlcpp161.lelinux.doc/compiler_ref/prag_omp_target.html) for targeting an accelerator, which only requires a simple compiler directive! Unfortunately, as of 2019 this only supports language primitives and arrays. This means that in order to actually create my own GPU-based solver, I'll need to write it without any of the libraries I've come to rely on.

### Graphics

![Running Simulation](https://github.com/JacksonCampolattaro/n_body/blob/master/doc/running-simulation.png)

Throughout this project, graphics have consistently been a source of frustration. When I built my proof of concept, I used freeGLUT because there were so many tutorials and examples available for it. GLUT was a great graphics library for beginners... in 1999. 

GLUT quickly became limiting, especially because of the way it takes control of the main loop. I soon switched to GLFW, but I used the legacy settings, because I'm scared of Vertex Buffer Objects and the general lower level of modern OpenGL.

Eventually, even that became an issue, and the pre-2.0 OpenGL became a major performance bottleneck. I've since replaced it with [Raylib](https://www.raylib.com/). The main thing that led me to choose raylib over other options is their robust documentation, especially when it comes to cmake. I'm still not entirely happy with Raylib for several reasons, and I'm experimenting with other solutions, including [Magnum](https://magnum.graphics/), [Ogre](https://www.ogre3d.org/), and even [Qt 3d](https://doc.qt.io/qt-5/qt3d-index.html).

### Interface

![Physics panel](https://github.com/JacksonCampolattaro/n_body/blob/master/doc/interface/physics.png) 
![View panel](https://github.com/JacksonCampolattaro/n_body/blob/master/doc/interface/view.png) 
![Solver panel](https://github.com/JacksonCampolattaro/n_body/blob/master/doc/interface/solver.png)

I've built a simple interface for controlling the program using [Gtkmm](https://www.gtkmm.org/en/), the C++ interface for Gtk+. I've been very happy with Gtkmm because it's very lightweight and looks native on my operating system (Fedora linux). I also considered [Qt](https://www.qt.io/), which worked nicely but felt way too heavy for my purposes, and [WxWidgets](https://www.wxwidgets.org/), which is just a bit too messy for me.

The interface was a lot of fun to put together, and it was very satisfying being able to get the tangible results of actual buttons and dropdowns appearing. Right now, I can use the interface to choose between a couple of solvers, configure the rules my physics will follow, and customize how the results are displayed.

An interface isn't entirely necessary, and in the end the best way of using the program should probably be by the command line, but that's not what this is about. Integrating something like this into my project was a great learning experience, and helped me improve even seemingly unrelated parts. Adding each feature required making changes to how my project was structured. For example, in order to have easily customizable physics I split the physical rules into their own object, which is passed to the solver. This was a more robust paradigm that made future changes like logging and serialization significantly easier.

### Signals & Synchronicity

The biggest change Gtkmm brought with it was the switch to an event-driven flow. When I first added the interface, the entire program would freeze as soon as the simulation started, and didn't become responsive again until it finished. The solution was to run the simulation in its own thread, so that it wouldn't interfere with Gtkmm's own loop! 

Communication is done with the [libsigc++](https://github.com/libsigcplusplus/libsigcplusplus) signals library, which should eventually allow me to incorporate features like progress bars and status indicators. I really like libsigc's flexibility, and to me it feels even more intuitive than Qt's macro-based solution, despite being slightly lower level.

Eventually I even made the graphics system event based, too. The renderer is notified whenever buffers have been shifted, and updates the screen accordingly. This allows me to do all my rendering in a seperate thread, which came with massive performance improvements for reasons I won't get into here.

As things are now, the interface, solver, and renderer all run asynchronously on different threads. I normally keep the renderer locked to the solver's loop because it looks smoother, but I can choose not to synchronize it easily, and even render at a different framerate than I do my calculations!

### Serialization

I'm using [Cereal](https://uscilab.github.io/cereal/) for serialization. I'll admit that I originally chose it over Boost's solution because I think their branding is really clever, but it's ended up integrating very neatly into the project! For objects like individual Bodies and the Physics rule, it was as simple as adding a special function that contains a list of the variables to be serialized. In order to serialize the list of bodies, and the vectors containing colors, velocities, and positions, I subclassed the libraries I was using. This allowed me to give the values nice and readable names in the serialized structure.

Once all of these functions were present, it only took one line to save or load XML files! Because this is in flux, I still have a 'scenarios' object, which uses C++ to generate objects. This allows me to define complex simulations with large numbers of bodies using loops, without having to spend a lot of time editing the XML files directly. Eventually, I plan to create a better way to build large simulations with the interface, but for now, I genuinely believe that writing short script-like snippets was the easiest way.

## Potential Research

I believe that research is a natural next step for this project. Re-applying things taught in my classes continues to be a great opportunity to build a deeper understanding, and I think that an individual project would make this an even more powerful mechanism for learning. 

The nature of the topic makes it very open to research, at many levels of depth. The algorithms I use here have already been studied plenty, but their implementation remains an interesting topic. 

### Requirements / Prerequisites

Even in the most limited form, I believe that a research project centered around this work would be a fantastic learning opportunity. Doing research on the relative performance of different algorithms would require the following preparation:

- Creating a method for measuring the accuracy of different algorithms (e.g mean distance of each physical body from its position as calculated by the naive algorithm).
- Using this method to calibrate the accuracy for each algorithm/implementation to be tested (e.g finding a range for theta in the Barnes-Hut algorithm that gives an error < 4%)
- Assembling an automated testing framework that will measure the performance of each algorithm, among other things (perhaps using catch2's benchmarking functionality, or Python if that isn't adequate)Adding a headless mode to my program that would allow the testing framework to run it without the extra graphics overhead (this would also let me run it overnight on the rLogin cluster, if need be)
- Naturally, generating plenty of plots and writing an actual paper on the results. 

### Topics

There are so many easily variable and measurable aspects to this project that I have no shortage of topics I could choose to center my research around. Here's a collection of example 'central questions' that my research might explore, roughly in order of complexity:

- How does the performance of the naive algorithm and a Barnes-Hut algorithm scale with the number of bodies in the simulation? At what point does the Barnes-Hut algorithm become more more efficient?
- How does the performance of the Barnes-Hut algorithm scale as the approximation ratio, Theta, is varied?
- How does the accuracy of the Barnes-Hut algorithm change as Theta is varied? How does the accuracy of the naive algorithm change as the time granularity is decreased?
- How do the performances of different algorithms scale, when the accuracy is held to a specific value (relative to some base solution) by manual calibration of constants like Theta?
- How does the performance of the canonical Barnes-Hut algorithm scale for different arrangements of bodies (e.g close together/spread out, symmetric/all on one side)? How can the algorithm be improved to make it scale better in less-than-ideal scenarios, and how do these changes affect performance?
- How do different algorithms scale in performance as I increase or decrease the number of threads available?
- How does the naive algorithm run on the GPU compare to a more elaborate algorithm run on the CPU?
- How does the naive algorithm run on the GPU scale with the number of stream processors enabled.
- How does a partially accelerated Barnes-Hut algorithm scale with the number of bodies? (e.g tree is built on the CPU, and actual force application is done on the GPU)
- How can a tree-based (Barnes-Hut) algorithm be run entirely on the GPU? How do the memory acess limitations of the specialized hardware affect the performance of the more elaborate algorithm?


![Logo](https://github.com/JacksonCampolattaro/n_body/blob/master/doc/logo.svg)
