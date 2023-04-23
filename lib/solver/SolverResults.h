#ifndef SOLVER_RESULTS_H
#define SOLVER_RESULTS_H

#include <Eigen/Dense>

template <typename Precision, typename MatrixType>
class SolverResults {
    protected:
    const Eigen::Matrix<Precision, Eigen::Dynamic, 1> *M_solution;
    Precision M_conditionNumber;

    public:
    SolverResults(const Eigen::Matrix<Precision, Eigen::Dynamic, 1> *solution, Precision conditionNumber) : M_solution(solution), M_conditionNumber(conditionNumber) {}
        
    
    const Eigen::Matrix<Precision, Eigen::Dynamic, 1> *solution() const {
        return M_solution;
    }

    Precision conditionNumber() const {
        return M_conditionNumber;
    }
};

#endif