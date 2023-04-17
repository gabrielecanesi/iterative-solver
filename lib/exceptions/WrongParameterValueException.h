#ifndef WRONG_PARAMETER_EXCEPTION_H
#define WRONG_PARAMETER_EXCEPTION_H

#include <stdexcept>

class WrongParameterValueException : std::exception {
    private:
    double M_minValue;
    double M_maxValue;
    bool M_minIncluded;
    bool M_maxIncluded;

    public:
        WrongParameterValueException(double minValue, double maxValue, bool minIncluded, bool maxIncluded);
        double minValue() const;
        double maxValue() const;
        bool minIncluded() const;
        bool maxIncluded() const;
};

#endif