// Qt logger.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
// --------------------------------------------------------------------

#include <Utils/QtLogger.h>
#include <Meta/Time.h>

namespace OpenEngine {
namespace Utils {

using namespace Logging;
using namespace std;

// Helper to create per-type styling
QString TypeStyle(LoggerType type){
    QString s;
    if (type == Error) {
        s.append("background-color: #ffa6af;");
    }
    if (type == Warning) {
        s.append("color: #990000;");
    }
    if (type == Info) {
        
    }
    return s;
}

/**
 * Create a logger widget.
 *
 */
QtLogger::QtLogger() {
    text = new QTextEdit();
    text->setReadOnly(true);
    QVBoxLayout* lay = new QVBoxLayout();
    lay->addWidget(text);
    setLayout(lay);
}

/**
 * Destruct the Qt logger.
 */
QtLogger::~QtLogger(){
    delete text;
}

/**
 * Write a log message.
 *
 * @param type Log message type.
 * @param msg Message to log.
 */
void QtLogger::Write(LoggerType type, string msg) {
    time_t t; time(&t); // get the current time
    char buf[20]; memset (buf, '0', 20); // this terminates the string
	strftime (buf, sizeof(buf), /*"%Y/%m/%d %H:*/"%M:%S", localtime(&t));
    text->append("<div style=\""+ TypeStyle(type) +"\">" +
                 QString(buf) + " - " +
                 QString(msg.c_str()) + "</div>");
}

} //NS Logging
} //NS OpenEngine
