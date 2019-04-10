
CS 184: Computer Graphics and Imaging, Spring 2019
==================================================

Final Project Proposal: Position Based Fluids
=============================================

### Cheng Chen, Mingfang Zhang, Zhiming Xu

### Position Based Fluids: Real-time Water Simulation

In this project we will create an iterative density solver, integrated into the Position Based Dynamics framework (framework repository in resources). We hope to be able to create a method that allows for similar incompressibility and convergence to modern smoothed particle hydrodynamic solvers, but inherits the stability of the geometric, position based dynamics method, allowing large time steps suitable for real-time applications.

Members: Cheng Chen, Mingfang Zhang, Zhiming Xu 3034485754

Problem Description
-------------------

The main problem in creating a water simulation like this is being able to simulate the fluid to match its real physical properties, which often times is computationally expensive and has impractical time step efficiency. However, accurate stimulations of fluids can be applied in anything between animation cinema to microfluidic modelling, which is why we are driven to pursue this technical challenge. On top of that, we thought that since in most of the projects so far have been focusing on solid objects, creating something to do with fluids could be an interesting and challenging final project for us to tackle.

Goals and Deliverables
----------------------

Our baseline goal for this project is being able to create water/fluidic behavior between particles in a simulated environment. We expect to find some difficulties in correctly implementing the physics of the fluids, as well as making the particles have water like properties, and quantify the accuracy of our simulation by comparing its visual similarity to real life examples or real life observations. Ideally, we would be able to run a real time simulation, but that would be a bit more of a reach if we are ahead of schedule. Some other future goals we were thinking of are interactions between multiple bodies of water (dropping a large blob of water into a body of water) and simulating interactions between different Newtonian fluids.

Schedule
--------

Week 1: Read resources listed below to get an idea of what to do. Outline pseudocode if possible.

Week 2: Code - get basic physics working

Week 3: Code - water appearance. Consider adding one or more of these features: interface between fluids, water blob and real-time simulation.

Week 4: Debug - make already included features work together

Finish the write-up as we go over the steps mentioned above.

Resources
---------

*   [Position Based Fluids paper](http://mmacklin.com/pbf_sig_preprint.pdf) in conjunction with [PositionBasedDynamics framework](https://github.com/InteractiveComputerGraphics/PositionBasedDynamics).
*   We are very likely to use [OpenGL](http://www.opengl-tutorial.org/), so we will need to learn it.
*   Potentially import everything into [Mitsuba](http://www.mitsuba-renderer.org/) for easier and better rendering.
*   We may also need to use AWS instance if we need considerable amount of computation power.
