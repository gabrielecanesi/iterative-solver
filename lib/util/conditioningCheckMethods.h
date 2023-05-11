#ifndef POWER_METHOD
#define POWER_METHOD

#include <Eigen/Sparse>
#include <iostream>

namespace conditioningCheck{

	template<typename T>
	bool arrestCriterion(Eigen::Matrix<T, Eigen::Dynamic, 1> q, Eigen::Matrix<T, Eigen::Dynamic, 1> q_old, double tol){

		return  (q - q_old).norm() < tol;
	}

	template<typename T>
	T powerMethod(Eigen::SparseMatrix<T> A, double tol, unsigned int maxIter){
		Eigen::Matrix<T, Eigen::Dynamic, 1> q_old, q, z;
		T v;
		q.setOnes();
		q.normalized();
		
		do {
			q_old = q;
			z = A * q_old;
			q = z / z.norm();
			v = q.transpose() * A * q;
			
			maxIter--;

			if (maxIter == 0){
				std::cerr << "Warning: the power method does not converge" << std::endl;
			}
		}while(arrestCriterion<T>(q, q_old, tol));
	
		return v;
	}


	template<typename T>
	T powerMethodInverse(Eigen::SparseMatrix<T> A, double tol, unsigned int maxIter){
		Eigen::Matrix<T, Eigen::Dynamic, 1> q_old, q, z;
		T v;
		q.setOnes();
		q.normalized();
	
		do {
			q_old = q;
			//A * z = q;
			q = z / z.norm();
			v = q.transpose() * A * q;
			
			maxIter--;

			if (maxIter == 0){
				std::cerr << "Warning: the power inverse method does not converge" << std::endl;
			}
		}while(arrestCriterion<T>(q, q_old, tol));	

		return v;
	}


	template<typename T> 
	T checkConditioning(Eigen::SparseMatrix<T> A, double tol, unsigned int maxIter){
		T maxA = powerMethod(A, tol, maxIter);
		T minA = powerMethodInverse(A, tol, maxIter);

		return abs(maxA)/abs(minA);
	}

	/*
	template<typename T>
	Eigen::Matrix<T, Eigen::Dynamic, 1> randomNormalizedVector(unsigned int n){

		Eigen::Matrix<T, Eigen::Dynamic, 1> v(n);
	    std::default_random_engine generator;
	    std::normal_distribution<T> distribution(0.0, 1.0);
	    for (int i = 0; i < n; i++) {
	        v(i) = distribution(generator);
	    }
	    return v.normalized();
	}*/

}




#endif