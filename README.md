# Iterative solver

This repository contains two main projects:

1. A C++ library which, given a matrix $A$, a vector $b$ and an error tolerance, finds $x$ such that $Ax=b$. Four iterative methods are implemented:
    
    1. Gradient
    2. Gauss-Seidel
    3. Conjugate Gradient
    4. Jacobi

    It makes use of [Eigen](https://eigen.tuxfamily.org).
    For each of these methods, there is a set of parameters which can be tuned in order to improve the convergence of the algorithm, which are:
    
    1. Distinct $\omega$ parameters for the relaxed versions of Jacobi and Gauss-Seidel.
    2. The type of norm that should be used for checking whether or not the algorithm has reached convergence. The three possibilities are the euclidian, manhattan and infinity norms.
    3. A parameter which can be used to skip the simmetry of and positive definition of $A$.

    There is also an estimation of the condition number of $A$ which can be used to warn the user in case of possible high errors due to the floating point precision. The estimation was inplemented on top of the method proposed by William Hager <cite>[1]</cite>.


2. A Qt Application which takes as input a matrix in `.mtx` format and compares all of the methods of the library in terms of relative errors and timings as tolerance increases. It shows the output in form of a table and graphs.

<img title="Output of the Qt application" alt="Output of the Qt application" src="img/QtOutput.svg" style="width:1000px;">
 
# References

[1] William W. Hager. ≪Condition Estimates≫. In: SIAM Journal on Scientific and Sta-
tistical Computing 5.2 (1984), pp. 311–316. doi: 10.1137/0905023. eprint: https://doi.org/10.1137/0905023. url: https://doi.org/10.1137/0905023.