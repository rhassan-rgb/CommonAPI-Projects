#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <dlt/dlt.h>
#include <sstream>
#include <utility>

namespace mediaRemote::logger {

// Registers the application and context with the DLT daemon on first use
// and unregisters them on process exit.
struct DltRegistration {
    DltContext ctx;
    DltRegistration() {
        DLT_REGISTER_APP("MRMT", "Media Remote");
        DLT_REGISTER_CONTEXT(ctx, "MAIN", "Media Remote main context");
    }
    ~DltRegistration() {
        DLT_UNREGISTER_CONTEXT(ctx);
        DLT_UNREGISTER_APP();
    }
};

inline DltContext& context() {
    static DltRegistration reg;
    return reg.ctx;
}

template <typename... Args>
void log(Args&&... args) {
    std::ostringstream oss;
    (oss << ... << std::forward<Args>(args));
    DLT_LOG(context(), DLT_LOG_INFO, DLT_STRING(oss.str().c_str()));
}

} // namespace mediaRemote::logger

#define LOG(...) ::mediaRemote::logger::log(__VA_ARGS__)

#endif
