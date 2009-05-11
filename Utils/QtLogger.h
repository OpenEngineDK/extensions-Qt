// Qt logger.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
// --------------------------------------------------------------------

#ifndef _QT_LOGGER_H_
#define _QT_LOGGER_H_

#include <Logging/ILogger.h>
#include <QtGui>
#include <string>

namespace OpenEngine {
namespace Utils {

/**
 * Qt logger.
 *
 * @class QtLogger QtLogger.h Logging/QtLogger.h
 */
class QtLogger : public Logging::ILogger, public QWidget {
private:
    QTextEdit* text;

public:
    QtLogger();
    virtual ~QtLogger();
    void Write(Logging::LoggerType, std::string);
};

} //NS Utils
} //NS OpenEngine

#endif // _QT_LOGGER_H_
