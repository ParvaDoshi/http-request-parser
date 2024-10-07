
#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <algorithm>
#include <ranges>

#include "httprequest.hpp"

// This is the heart of the HTTP parsing.

// You should first take the request string and break it up
// into substrings wherever there is a '\r\n' pair.  The 
// easiest way to do this is to iterate through the entire input,
// keeping track of the current character, where in the string you are
// and the last character. When you get a '\n' when the previous was '\r',
// you have found the end of the substring (well, 2 past the end 
// the substring), allowing you to use std::string's substr function.
//
// But at the same time you shouldn't ever have a '\n' without a prior '\r', or 
// a '\r' without a subsequent '\n', however this is something we are not going to
// require you to check for.
// 
// (Note that substr takes the starting index and the length, so you will
// want to keep track of the current index, the last start point, and possibly
// the length as well).

// Finally, if you have '\r\n\r\n' in a row that is the end of the headers and you
// can use a break to exit the loop.  This ending must be present and if it isn't you
// should raise a MalformedRequestException.  

// Anything past the \r\n\r\n is the "payload".

// Now that it is split up, you can then parse both the command (the first part)
// and then the headers.


HTTPRequest::HTTPRequest(std::string request){
  throw "Need to implement!" + request;
}

auto valid_commands = {"GET", "POST", "HEAD"};

void HTTPRequest::parse_command(std::string &command_string){
  throw "Need to implement!" + command_string;
  
}

// This is the portion for a header.  The name should be the
// header name (the part before the :) in all lower case, and
// whitespace stripped out.  It technically should also not include
// any internal whitespace but we arent' checking for that.

// The value is ALL the data after the :, including any whitespace!
HTTPHeader::HTTPHeader(std::string s){
  throw "Need to implement" + s;
}