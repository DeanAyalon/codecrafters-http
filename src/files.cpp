#include "files.hpp"

#include <filesystem>
#include <fstream>

#include "http/request.hpp"
#include "utils/console.hpp"
#include "utils/utils.hpp"

using std::string;
using std::vector;

void files(Request *request, string files_dir) {
    // File path
    vector<string> path = request->get_path();
    path.erase(path.begin());
    const string file_path = vec::join(path, "/");
    if (path.empty()) return request->respond(400, "No file requested");

    const string full_path = files_dir + "/" + file_path;
    if (std::filesystem::is_directory(full_path))
        return request->respond(404, file_path + " is not a file");

    log("Fetching file: " + full_path);
    std::ifstream file(full_path, std::ios::binary);

    if (!file) return request->respond(404, file_path + " not found");
    request->respond(200, &file);
}