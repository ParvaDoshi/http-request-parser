#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <algorithm>
#include <stdexcept> // for exceptions
#include <cctype>    // for tolower
#include "httprequest.hpp"

// Custom exception for malformed requests
class MalformedRequestException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Malformed Request: Missing end of headers \\r\\n\\r\\n";
    }
};

// Constructor for HTTPRequest, takes the full request string and parses it
HTTPRequest::HTTPRequest(std::string request) {
    size_t end_of_headers = request.find("\r\n\r\n");

    // If we don't find the end of the headers, throw an exception
    if (end_of_headers == std::string::npos) {
        throw MalformedRequestException();
    }

    // Split the request into lines based on "\r\n"
    std::string headers_part = request.substr(0, end_of_headers);
    size_t pos = 0;
    size_t line_end;
    std::vector<std::string> lines;

    while ((line_end = headers_part.find("\r\n", pos)) != std::string::npos) {
        lines.push_back(headers_part.substr(pos, line_end - pos));
        pos = line_end + 2;  // Skip over \r\n
    }

    // Parse the command (first line)
    if (!lines.empty()) {
        parse_command(lines[0]);
        lines.erase(lines.begin());
    }

    // Parse each header line
    for (const auto& line : lines) {
        if (!line.empty()) {
            headers.push_back(HTTPHeader(line));
        }
    }

    // The part after the headers is the payload
    payload = request.substr(end_of_headers + 4);  // Skip over \r\n\r\n
}

// Parses the command (first line of the request)
void HTTPRequest::parse_command(std::string &command_string) {
    std::istringstream iss(command_string);
    std::string method, url, version;

    // Extract method, URL, and version from the command string
    iss >> method >> url >> version;

    // Check if the method is valid (GET, POST, HEAD)
    if (std::find(valid_commands.begin(), valid_commands.end(), method) == valid_commands.end()) {
        throw std::invalid_argument("Invalid HTTP method");
    }

    // Store method, url, and version in class variables
    this->method = method;
    this->url = url;
    this->version = version;
}

// Constructor for HTTPHeader, parses a single header line
HTTPHeader::HTTPHeader(std::string s) {
    size_t colon_pos = s.find(':');
    if (colon_pos == std::string::npos) {
        throw std::invalid_argument("Malformed Header: Missing ':'");
    }

    // Extract the name (before the ':')
    std::string name = s.substr(0, colon_pos);
    // Convert to lowercase and remove whitespace
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
    name.erase(std::remove_if(name.begin(), name.end(), ::isspace), name.end());

    // Extract the value (after the ':') and strip leading/trailing whitespace
    std::string value = s.substr(colon_pos + 1);
    value.erase(0, value.find_first_not_of(" \t"));  // Trim leading whitespace
    value.erase(value.find_last_not_of(" \t") + 1);  // Trim trailing whitespace

    // Store in class variables
    this->name = name;
    this->value = value;
}
