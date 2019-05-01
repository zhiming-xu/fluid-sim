# CS 184: Computer Graphics and Imaging, Spring 2019

# Milestone Status Report

Members: Zhiming Xu 3034485754, Mingfang Zhang 3034485741, Cheng Chen 3034488887

##### Video Link

##### Slides Link

## Summary

At the beginning, we are struggling to find a suitable starter code.

Up to now, we have basically implemented the algorithm described from the paper “Position-Based Fluids” by Macklin and Müller. Some key components are as follows.

- **Particle Neighbor Finding**. With the help of a basic spatial hashmap.
- **Density constraints**. In order to make the fluid resist compression, we calculate position correction $\bold{\Delta p_i}$ that satisfies the constraints $C(p+\Delta p)=0$, where $\begin{equation}C_i(p_1, \cdots, p_n) = \frac{\rho_i}{\rho_0} - 1\end{equation}$ and $\rho_i​$ is the density at a particle evaluated by the *poly6 kernel*.
- **Surface tension**. To address the particle clustering and clumping problem in *SPH simulations*, an artificial pressure term $s_{corr}​$ is added in the particle position update. 
- **Collision handling**. Implement collosions with planes to keep the fluid inside the box.
- **Vorticity confinement**. Vorticity confinement is implemented to counteract undesirable additional damping introduced by position based methods. The vorticity at a particle is calculated and afterwards a corrective force is applied.
- **XSPH viscosity**. XSPH viscosity is implemented for coherent motion.

However, the rendering procedure is far too slow and the fluid appears so coarse. We will set out to solve these problems in the remaining days.

## Schedule Update

We have 2 weeks left.

Week 1: Fine-grained water appearance. Consider adding one or more of these features. Preparing final report as well.

Week 2: Improve code structure and cleanliness. Create a satisfying result video for the final presentation. Update final report as well.