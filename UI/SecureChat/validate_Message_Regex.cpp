//Bailey Atkinson (a1772413), Ben Cockburn (a1850196), Marcus Beckwith (a1852054)

#include <iostream>
#include <regex>
#include <string>
#include <json.hpp> // Include this for JSON parsing



// Function to validate the message using regex
bool isValidMessage(const std::string& message) {
    // Define a regex pattern for a valid message
    // Example: Allow alphanumeric characters, spaces, punctuation, and a maximum length of 256 characters
    std::regex pattern("^[a-zA-Z0-9\\s.,!?\"'()\\-]{1,256}$");

    // Check if the message matches the pattern
    return std::regex_match(message, pattern);
}
