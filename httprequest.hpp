#ifndef _HTTP_REQUEST_H
#define _HTTP_REQUEST_H

#include <map>
#include <memory>
#include <exception>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

// Exception raised for malformed HTTP requests
class MalformedRequestException : public std::exception {
public:
    MalformedRequestException(const std::string& msg) {
        message = "Malformed HTTP Request: " + msg;
    }

    MalformedRequestException() {
        message = "Malformed HTTP Request";
    }

    virtual const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};

// Class representing a single HTTP header
class HTTPHeader {
public:
    HTTPHeader(const std::string& header_line) {
        // Split header line into name and value
        size_t separator_pos = header_line.find(':');
        if (separator_pos == std::string::npos) {
            throw MalformedRequestException("Invalid header format");
        }

        name = header_line.substr(0, separator_pos);
        value = header_line.substr(separator_pos + 1);

        // Clean up name
        name.erase(std::remove_if(name.begin(), name.end(), ::isspace), name.end());
        std::transform(name.begin(), name.end(), name.begin(), ::tolower); // Convert to lowercase

        // Clean up value
        value.erase(0, value.find_first_not_of(" \t")); // Trim leading whitespace
        value.erase(value.find_last_not_of(" \t") + 1); // Trim trailing whitespace
    }

    const std::string& get_name() const { return name; }
    const std::string& get_value() const { return value; }

private:
    std::string name;
    std::string value;
};

// Class representing an HTTP request
class HTTPRequest {
public:
    HTTPRequest(const std::string& request_string) {
        // Split request string into lines
        std::istringstream request_stream(request_string);
        std::string line;

        // Read the request command
        if (std::getline(request_stream, line)) {
            parse_command(line);
        } else {
            throw MalformedRequestException("Empty request");
        }

        // Read headers
        while (std::getline(request_stream, line) && !line.empty()) {
            headers.emplace(line, std::make_shared<HTTPHeader>(line));
        }

        // Handle the payload
        if (std::getline(request_stream, line)) {
            payload = line; // Assuming single line payload for simplicity
        }
    }

    const std::string& get_command() const { return command; }
    const std::string& get_resource() const { return resource; }
    const std::string& get_payload() const { return payload; }
    std::map<std::string, std::shared_ptr<HTTPHeader>>& get_headers() { return headers; }

private:
    std::map<std::string, std::shared_ptr<HTTPHeader>> headers;
    std::string command;
    std::string resource;
    std::string payload;

    void parse_command(const std::string& command_string) {
        size_t space_pos = command_string.find(' ');
        if (space_pos == std::string::npos) {
            throw MalformedRequestException("Invalid command format");
        }

        command = command_string.substr(0, space_pos);
        resource = command_string.substr(space_pos + 1);

        // Validate the command
        if (command != "GET" && command != "POST" && command != "HEAD") {
            throw MalformedRequestException("Unsupported command: " + command);
        }
    }
};

#endif
