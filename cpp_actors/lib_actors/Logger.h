/*
 * Copyright (c) 2023, Henrik Larsen
 * https://github.com/henrik7264/CPP_Actors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef CPP_ACTORS_LOGGER_H
#define CPP_ACTORS_LOGGER_H
#include <ctime>
#include <chrono>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>


namespace Loggers
{
    class Logger: public std::ostringstream
    {
    private:
        enum LogLevel {DEBUG, INFO, WARNING, ERROR, FATAL} logLevel;
        std::string loggerName;

        explicit Logger(LogLevel level, const std::string& loggerName): logLevel(level), loggerName(loggerName) {};

        static const char* loglevel2Txt(LogLevel level) {
            static const char* Levels[] = {"DEBUG", "INFO", "WARNING", "ERROR", "FATAL"};
            return Levels[level];
        }

    public:
        ~Logger() override {
            auto now = std::chrono::system_clock::now();
            auto nowMilli = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
            auto currTime = std::chrono::system_clock::to_time_t(now);
            auto localTime = std::localtime(&currTime);

            std::ostringstream logStream;
            logStream << std::put_time(localTime, "%F %T") << "," << std::setw(3) << std::setfill('0') << nowMilli.count() % 1000  << " ";
            logStream << loggerName << " ";
            logStream << loglevel2Txt(logLevel) << " ";
            logStream << str();

            std::clog << logStream.str() << std::endl;
        }

        inline static Logger debug(const std::string& loggerName) {return Logger(DEBUG, loggerName);}
        inline static Logger info(const std::string& loggerName) {return Logger(INFO, loggerName);}
        inline static Logger warning(const std::string& loggerName) {return Logger(WARNING, loggerName);}
        inline static Logger error(const std::string& loggerName) {return Logger(ERROR, loggerName);}
        inline static Logger fatal(const std::string& loggerName) {return Logger(FATAL, loggerName);}
    }; // Logger
} // Loggers

#endif //CPP_ACTORS_LOGGER_H
