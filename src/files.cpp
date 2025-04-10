#include "files.hpp"

#include "http/request.hpp"
#include "utils/console.hpp"

void files(Request *request) {
    log("full" + request->full_path());
    request->respond(503, "In development");
}