//
// Created by Josh Wright on 3/27/24.
//

#ifndef BLACKWATCH_BWLOGGER_H
#define BLACKWATCH_BWLOGGER_H

#include <juce_core/juce_core.h>

class BwLogger {
public:

    static std::string getStackTrace();

    // Static method that uses the current() singleton
    static void log(const juce::String& message) {
      // todo: this class is not good, should be removed
      return;
      shared();
      juce::Logger::writeToLog(message);
    }

private:
    // Private constructor for singleton pattern.
    BwLogger();
    void setupLogger();
    juce::File logFile;
    juce::String readLogs();
        static BwLogger& shared() {
        static BwLogger instance;
        return instance;
    }
};


#endif //BLACKWATCH_BWLOGGER_H
