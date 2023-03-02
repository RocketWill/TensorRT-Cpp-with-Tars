#pragma once

#include <dirent.h>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <sys/stat.h>
#include <assert.h>

#include "json/json.h"

static inline int read_files_in_dir(const char* p_dir_name, std::vector<std::string>& file_names) {
    DIR *p_dir = opendir(p_dir_name);
    if (p_dir == nullptr) {
        return -1;
    }

    struct dirent* p_file = nullptr;
    while ((p_file = readdir(p_dir)) != nullptr) {
        if (strcmp(p_file->d_name, ".") != 0 &&
            strcmp(p_file->d_name, "..") != 0) {
            //std::string cur_file_name(p_dir_name);
            //cur_file_name += "/";
            //cur_file_name += p_file->d_name;
            std::string cur_file_name(p_file->d_name);
            file_names.push_back(cur_file_name);
        }
    }

    closedir(p_dir);
    return 0;
}

// Function to trim leading and trailing whitespace from a string
static inline std::string trim_leading_whitespace(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// Src: https://stackoverflow.com/questions/16605967
static inline std::string to_string_with_precision(const float a_value, const int n = 2) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

static inline int read_labels(const std::string labels_filename, std::unordered_map<int, std::string>& labels_map) {

    std::ifstream file(labels_filename);
    // Read each line of the file
    std::string line;
    int index = 0;
    while (std::getline(file, line)) {
        // Strip the line of any leading or trailing whitespace
        line = trim_leading_whitespace(line);

        // Add the stripped line to the labels_map, using the loop index as the key
        labels_map[index] = line;
        index++;
    }
    // Close the file
    file.close();

    return 0;
}

static inline bool file_exist (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

static inline Json::Value parse_json_file (const std::string& file) {
  Json::Value root;
  std::ifstream ifs;
  ifs.open(file);
  Json::CharReaderBuilder builder;
  builder["collectComments"] = true;
  JSONCPP_STRING errs;
  if (!parseFromStream(builder, ifs, &root, &errs)) {
    assert(false);
  }
  
  return root;
}

static inline std::string jsonify (Json::Value &root) {
    Json::StreamWriterBuilder builder;
    const std::string json_str = Json::writeString(builder, root);
    return json_str;
}