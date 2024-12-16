//
// Created by Josh Wright on 3/27/24.
//

#include "BwLogger.h"
#include <ctime>
#include <iomanip>
#include <execinfo.h>
#include <print>
#include <unistd.h>


void BwLogger::setupLogger() {
    juce::File appDataDir = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory);
    juce::File blackwatchDir = appDataDir.getChildFile("blackwatch");
    if (!blackwatchDir.exists()) {
      bool dirCreated = blackwatchDir.createDirectory();
      if (!dirCreated) {
        DBG("Failed to create the blackwatch directory.");
        return;
      }
    }
    auto logsDir = blackwatchDir.getChildFile("logs");
    if (!logsDir.exists()) {
        bool dirCreated = logsDir.createDirectory();
        if (!dirCreated) {
            DBG("Failed to create the logs directory.");
        }
    }

    std::ostringstream ss;
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    ss << std::put_time(now, "%m%d%Y");
    auto filename = ss.str() + ".txt";

    logFile = logsDir.getChildFile(filename);
    auto l = new juce::FileLogger(logFile, "Blackwatch Studios Logs for " + ss.str());
    juce::Logger::setCurrentLogger(l);
}

void signalHandler(int signal) {
    BwLogger::log("BAD SIGNAL RECEIVED: " + std::to_string(signal));
    //BwLogger::shared().submit();
    _exit(1);
}

BwLogger::BwLogger()
{
    setupLogger();

    std::set_terminate([]() {
        // BwLogger::shared().submit();
        std::abort(); // Ensure termination and prevent further execution.
    });

    signal(SIGSEGV, signalHandler);
}



//void BwLogger::submit() {
//    auto logs = readLogs();
//    juce::URL pastebinURL("https://pastebin.com/api/api_post.php");
//
//    juce::String postData = "api_dev_key=SSqMZKMlFARSBN4LMAx9l6q_oUDA3cIH&api_paste_private=1&api_paste_expire_date=1M&api_option=paste&api_paste_name=logs&api_paste_code=" + juce::URL::addEscapeChars(logs, true);
//
//    // Use MemoryBlock if needed, or directly pass the string
//    juce::MemoryBlock postDataBlock(postData.toRawUTF8(), postData.getNumBytesAsUTF8());
//
//    auto response = pastebinURL.withPOSTData(postData).readEntireTextStream();
//
//    //BwLogger::log(response);
//}

juce::String BwLogger::readLogs() {
    if (!logFile.existsAsFile()) return "Log file does not exist.";

    juce::FileInputStream inputStream(logFile);
    if (!inputStream.openedOk()) return "Failed to open log file.";

    return inputStream.readEntireStreamAsString();
}

std::string BwLogger::getStackTrace(){
    // todo: this class is not good, should be removed
    return {};
    try {
        void* addrlist[64];
        int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

        char** symbollist = backtrace_symbols(addrlist, addrlen);
        std::ostringstream oss;
        for (int i = 0; i < addrlen; i++) {
            oss << symbollist[i] << "\n";
        }
        free(symbollist);
        return oss.str();
    }catch(const std::exception& e){
        return "Failed to get stack trace.";
    }
}
