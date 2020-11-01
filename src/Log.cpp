#include "Log.h"


std::unique_ptr<Logger> Log::logger = nullptr;

Log::logLevel Log::global_log_level = Log::INFO;


void Log::setLoggingLevel(Log::logLevel lvl){
    global_log_level = lvl;
    if (!logger) 
        logger = std::unique_ptr<Logger>(new Logger());
}

Log::logLevel Log::getLoggingLevel(){
    if (logger)
        return global_log_level;
    else logger = std::unique_ptr<Logger>(new Logger());
    return Log::DEBUG; //¯\_(ツ)_/¯
}

Logger& Log::get(Log::logLevel lvl) {
    (*logger)(lvl);
    return *logger;
}

void Log::set_path(std::string path) {
    if (logger){
        logger->set_file_path(path);
    }
    else {
        logger = std::unique_ptr<Logger>(new Logger());
        logger->set_file_path(path);
    }
}



