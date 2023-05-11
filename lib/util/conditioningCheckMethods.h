#ifndef POWER_METHOD
#define POWER_METHOD

#include <solver/IterativeSolver.h>
#include <updateStrategy/GradientUpdateStrategy.h>
#include <Eigen/Sparse>
#include <iostream>
#include <solver/NormType.h>

template<typename T, typename MatrixType>
class IterativeSolver;

namespace conditioningCheck{
	
	template<typename T, typename MatrixType> 
	T checkConditioning(MatrixType &A, double tol, unsigned int maxIter);

	template<typename T>
	bool arrestCriterion(Eigen::Matrix<T, Eigen::Dynamic, 1> q, Eigen::Matrix<T, Eigen::Dynamic, 1> q_old, double tol){

		return  (q - q_old).norm() < tol;
	}

	template<typename T, typename MatrixType>
	T powerMethod(MatrixType &A, double tol, unsigned int maxIter){
		Eigen::Matrix<T, Eigen::Dynamic, 1> q_old(A.rows(), 1), q(A.rows(), 1), z(A.rows(), 1);
		T v;
		q.setOnes();
		q.normalized();
		
		do {
			q_old = q;
			z = A * q_old;
			q = z / z.norm();
			
			maxIter--;

			if (maxIter == 0){
				std::cerr << "Warning: the power method does not converge" << std::endl;
			}
		}while(arrestCriterion<T>(q, q_old, tol));
	
		return q.transpose() * A * q;
	}


	template<typename T, typename MatrixType>
	T powerMethodInverse(MatrixType &A, double tol, unsigned int maxIter){
		Eigen::Matrix<T, Eigen::Dynamic, 1> q_old(A.rows(), 1), q(A.rows(), 1), z(A.rows(), 1);
		T v;
		q.setOnes();
		q.normalized();
		
		UpdateStrategy::GradientUpdateStrategy<T, MatrixType> strategy;
		IterativeSolver solver(100, &strategy, 1e-3, true, NormType::EUCLIDEAN, true);

		do {
			q_old = q;
			z = *solver.solve(A, q)->solution();
			q = z / z.norm();

			maxIter--;

			if (maxIter == 0){
				std::cerr << "Warning: the power inverse method does not converge" << std::endl;
			}
		} while(arrestCriterion<T>(q, q_old, tol));	

		Eigen::Matrix<T, Eigen::Dynamic, 1> lambdaV = *solver.solve(A, q)->solution();
		
		unsigned long i = 0;
		while (q[i++] == 0) {}

		return lambdaV(i) / q(i);
	}


	template<typename T, typename MatrixType> 
	T checkConditioning(MatrixType &A, double tol, unsigned int maxIter){
		T maxA = powerMethod<T, MatrixType>(A, tol, maxIter);
		T minA = powerMethodInverse<T, MatrixType>(A, tol, maxIter);

		return abs(maxA)/abs(minA);
	}

}




#endif