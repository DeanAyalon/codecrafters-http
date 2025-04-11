#include "files.hpp"

#include <filesystem>
#include <fstream>

#include "http/request.hpp"
#include "utils/console.hpp"
#include "utils/utils.hpp"

using std::string;
using std::vector;

void files(Request *request, string files_dir) {
    const string method = request->get_method();

    // File path
    vector<string> path = request->get_path();
    path.erase(path.begin());
    const string file_path = vec::join(path, "/");
    if (path.empty()) return request->respond(400, "No file path provided");
    const string full_path = files_dir + "/" + file_path;
    if (std::filesystem::is_directory(full_path))
        return request->respond(400, file_path + " is a directory");

    if (method.compare("GET") == 0) {
        log("Fetching file: " + full_path);
        std::ifstream file(full_path, std::ios::binary);

        if (!file) return request->respond(404, file_path + " not found");
        request->respond(200, &file);

    } else if (method.compare("POST") == 0) {
        // Check data
        std::string body = request->get_body();
        if (body.empty()) request->respond(400, "Empty file received");
        
        // Ensure parent directory exists
        std::filesystem::path parent_dir = std::filesystem::path(full_path).parent_path();
        if (!std::filesystem::exists(parent_dir)) std::filesystem::create_directories(parent_dir);

        // Write to file
        //                            bit-wise OR (combines the options)
        // 0xnum = hexadecimal   Process all bytes (0x4)  Clear file before writing to it (0x20=32)
        std::ofstream file(full_path, std::ios::binary | std::ios::trunc); // Overwrite if exists
        if (!file) return request->respond(500, "Failed to write file");
        file.write(body.c_str(), body.size());
        file.close();
        request->respond(201, file_path + " file was created");
    } else
        request->respond(400, method + " requests are not supported");
}