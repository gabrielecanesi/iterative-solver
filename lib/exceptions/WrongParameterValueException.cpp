#include "WrongParameterValueException.h"

#include <stdexcept>

WrongParameterValueException::WrongParameterValueException(double minValue, double maxValue,
                                                            bool minIncluded, bool maxIncluded) : std::exception(),
                                                                                                M_minValue(minValue),
                                                                                                M_maxValue(maxValue),
                                                                                                M_minIncluded(minIncluded),
                                                                                                M_maxIncluded(maxIncluded) {}

double WrongParameterValueException::minValue() const {
    return M_minValue;
}

double WrongParameterValueException::maxValue() const {
    return M_maxValue;
}

bool WrongParameterValueException::minIncluded() const {
    return M_minIncluded;
}

bool WrongParameterValueException::maxIncluded() const {
    return M_maxIncluded;
}