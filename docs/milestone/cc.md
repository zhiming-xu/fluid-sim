## CS 184: Computer Graphics and Imaging, Spring 2019

# Milestone Status Report 

Members: Zhiming Xu 3034485754, Mingfang Zhang 3034485741, Cheng Chen 3034488887

##### [Video Link](<https://drive.google.com/open?id=1nC8FUH43q1RYpyE485neEkTzxLr1EyyS>)

##### [Slides Link](<https://drive.google.com/open?id=1hGhFIYccuz43eb13ja3d1o1CwRNDRuiS>)

## Summary

While the Navier-Stokes equations can describe the fluid movement accurately, it was too computationally expensive to implement it to simulate the fluid. 

> "It's a common misconception that visual effects are about simulating reality. They're not. Reality is boring. Visual effects are about simulating something dramatic."
>
> \- Jonathan Cohen, Rhythm & Hue 

As a result, we use Particle-Based simulations. In particular, we use a technique called Smoothed Particle Hydrodynamics (SPH).

In SPH approach, Fluid volume is discretized by particles and each particles represents a certain amount of fluid volume.

$$V_i = m_i/\rho_i​$$

Particles store attributes, and to evaluate an attribute, we take weighted average of particle values within a neighborhood. Smoothing kernel $W$ prescribes interpolation weights.

<div align="middle">
  <table style="width=100%">
        <img src="images/1.png" align="middle" width=400px/>
  </table>
</div>

Then we sum up contribution of neighboring particles $j​$:

$$A(x) = \sum_jm_j/\rho_j * A_jW(x-x_j,h)$$

$A(x)$ : Quantity $A$ at arbitrary position $x$; 
$\sum$ : Sum over all neighbor particles $j$ within $h$; 
$A_j$ : Quantity $A$ of particle $j$; 
$W$ : Smoothing kernel.

Up to now, we have basically implemented the algorithm described from the paper “Position-Based Fluids” by Macklin and Müller. Some key components are as follows.

**Particle Neighbor Finding**. With the help of a basic spatial hashmap.

<div align="middle">
  <table style="width=100%">
    	  <img src="images/neighbor.png" align="middle" width=400px/>
  </table>
</div>

**Density constraints**. In order to make the fluid resist compression, we calculate position correction $\bold{\Delta p_i}$ that satisfies the constraints $C(p+\Delta p)=0$, where $\begin{equation}C_i(p_1, \cdots, p_n) = \frac{\rho_i}{\rho_0} - 1\end{equation}$ and $\rho_i$ is the density at a particle evaluated by the *poly6 kernel*.

<div align="middle">
  <table style="width=100%">
    	  <img src="images/density.png" align="middle" width=400px/>
  </table>
</div>
**Surface tension**. To address the particle clustering and clumping problem in *SPH simulations*, an artificial pressure term $s_{corr}​$ is added in the particle position update. 

<div align="middle">
  <table style="width=100%">
    	  <img src="images/scorr.png" align="middle" width=400px/>
  </table>
</div>
**Collision handling**. Implement collosions with planes to keep the fluid inside the box.

**Vorticity confinement**. Vorticity confinement is implemented to counteract undesirable additional damping introduced by position based methods. The vorticity at a particle is calculated and afterwards a corrective force is applied.

**XSPH viscosity**. XSPH viscosity is implemented for coherent motion.

<div align="middle">
  <table style="width=100%">
    	  <img src="images/coherent.png" align="middle" width=400px/>
  </table>
</div>

However, the rendering procedure is far too slow and the fluid appears so coarse. We will set out to solve these problems in the remaining days.

## Schedule Update

We have 2 weeks left.

Week 1: Fine-grained water appearance. Consider adding one or more of these features. Preparing final report as well.

Week 2: Improve code structure and cleanliness. Create a satisfying result video for the final presentation. Update final report as well.



