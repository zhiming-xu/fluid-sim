# <center>CS 184: Computer Graphics and Imaging</center>

## <center>Final Project: Smoothed Particle Hydrodynamics</center>

#### <center>Created by Zhiming Xu, Mingfang Zhang, [Cheng Chen](https://github.com/Julius-c)</center>

#### <center>SIDs: 3034485754, 3034485741, 3034488887<center>

### Abstract

We read the Smoothed Particle Hydronamics paper by $J. J. Monaghan$ and the Position Based Fluids paper by $Macklin$ and $Müller​$, and finally build a fluid simulator based on the simplified algorithm which is basically the SPH method. This technique combines pressure, viscosity and gravity to simulate water. We firstly implement the basic mathematical and physical simulation model. Afterwards, we polish our shader programs to create fantastic lighting effects.

### Technical approach and implementation

We can show either 2D or 3D simulation by commenting out or defining a c++​ macro. In 3D Particle Fluid Simulation, at the very beginning, we set up a ground plane and four virtual surrounding walls with which the particles collide. Our approach then can be divided into two main parts — collision detection and system update, force application and time step integration.

We've learned from class the approach of collision detection and correction and it's time we put it into good use.

Our calculation of total simulation force exerted on a single particle is a combination of three components which comes from pressure, viscosity and gravity.



Afterwards, we implement three different integration methods to update particle position.



**Problems encountered**

Throughout the course of our implementation journey, we encountered a couple of different technical problems.

To begin with, a bug in collision correction part leads to our particles' leaking out of the ground plane and bounding walls. (一个对应视频)

After navigating through according code block, we locate our calculation bug and successfully address this issue.

The next issue is about 

**Lessons learned**

### Results

Below is a demonstration of our results with xxx shading:

### References

- [Smoothed Particle Hydrodynamics](http://adsabs.harvard.edu/full/1992ARA%26A..30..543M)
- [Position Based Fluids[Macklin, Muller]](http://mmacklin.com/pbf_sig_preprint.pdf)
- 

### Contributions from each team member

- Zhiming Xu 
- Mingfang Zhang
- Cheng Chen