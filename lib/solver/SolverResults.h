#ifndef SOLVER_RESULTS_H
#define SOLVER_RESULTS_H

#include <Eigen/Dense>
using namespace Eigen;
template <typename Precision, typename MatrixType>
class SolverResults {
    protected:
    std::shared_ptr<Matrix<Precision, Eigen::Dynamic, 1>> M_solution;
    Precision M_conditionNumber;

    public:
    SolverResults(std::shared_ptr<Matrix<Precision, Eigen::Dynamic, 1>> solution, Precision conditionNumber) : M_solution(solution), M_conditionNumber(conditionNumber) {}
        
    
    std::shared_ptr<Matrix<Precision, Eigen::Dynamic, 1>> solution() const {
        return M_solution;
    }

    Precision conditionNumber() const {
        return M_conditionNumber;
    }
};

#endif