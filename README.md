
CS 184: Computer Graphics and Imaging, Spring 2019
==================================================

Final Project Proposal: Position Based Fluids
=============================================

### Cheng Chen, Mingfang Zhang, Zhiming Xu

### Position Based Fluids: Real-time Water Simulation

In the lectures, we saw several demonstrations of fluids simulation and we all found it excited to discover the theory behind it and to implement it by ourselves. As we know, accurate stimulations of fluids can be applied in a really wide range of areas and the final effects are really cool. That is the reason why we are so driven to pursue this technical challenge.

Members: Cheng Chen 3034488887, Mingfang Zhang 3034485741, Zhiming Xu 3034485754

Problem Description
-------------------

Creating a fluid simulation means that we should simulate the fluid to match its real physical properties, which always leads to a high level of time complexity.  We aim to create an iterative density solver in this project, which will be integrated into the position based dynamics framework. We hope to be able to create a method that allows for similar incompressibility and convergence to modern smoothed particle hydrodynamic solvers, but inherits the stability of the geometric, position based dynamics method, allowing large time steps suitable for real-time applications.

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
