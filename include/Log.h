#ifndef BALLESTRINQUE_LOG_H
#define BALLESTRINQUE_LOG_H

//usar estos macros

//EJ: LOG(Log::ERROR) << "se encontraron " << 3 << " errores" <<std::endl;
#define LOG(level) \
if (level > Log::getLoggingLevel()) ; \
else Log::get(level)

#define SET_LOGGING_LEVEL(level) \
Log::setLoggingLevel(level);

#define SET_LOG_FILE(path) \
Log::set_path(path);



#include <string>
#include <memory>

#include "Logger.h"

class Log {

    public:

        enum logLevel {ERROR, INFO, DEBUG};

    private:

        static logLevel global_log_level;

        static std::unique_ptr<Logger> logger;

    public:

        static void setLoggingLevel(logLevel lvl);

        static logLevel getLoggingLevel(); //devuelve el global

        static Logger& get(logLevel lvl); //devuelve la instancia

        static void set_path(std::string path);


        Log() = delete;

        Log(const Log&) = delete;
        
        Log& operator=(const Log&) = delete;

};

#endif