#include "uiutils.h"

#include <sstream>
#include <QString>

QString UIUtils::formatToScientific(double value) {
    std::ostringstream stream;
    stream << value;
    return QString::fromStdString(stream.str());
}
