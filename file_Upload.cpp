#include <iostream>
#include <curl/curl.h>
#include <fstream>

// Callback to capture server response data
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    size_t totalSize = size * nmemb;
    response->append((char*)contents, totalSize);
    return totalSize;
}

std::string uploadFile(const std::string& serverUrl, const std::string& filePath) {
    CURL* curl;
    CURLcode res;
    curl_mime* form = NULL;
    curl_mimepart* field = NULL;
    std::string response;

    // Initialize CURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set URL for the file upload endpoint
        curl_easy_setopt(curl, CURLOPT_URL, serverUrl.c_str());

        // Create a form to upload the file
        form = curl_mime_init(curl);

        // Add the file field
        field = curl_mime_addpart(form);
        curl_mime_name(field, "file");
        curl_mime_filedata(field, filePath.c_str());

        // Attach the form to the request
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);

        // Set up callback to capture response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the file upload
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "File upload failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "File upload successful. Response: " << response << std::endl;
        }

        // Clean up
        curl_mime_free(form);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    // Parse and return the file_url from the response (assuming JSON format)
    size_t fileUrlPos = response.find("\"file_url\":\"");
    if (fileUrlPos != std::string::npos) {
        size_t startPos = fileUrlPos + 12;
        size_t endPos = response.find("\"", startPos);
        return response.substr(startPos, endPos - startPos);
    }

    return ""; // Return empty if no file_url found
}
