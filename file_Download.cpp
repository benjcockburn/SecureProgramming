#include <iostream>
#include <curl/curl.h>
#include <fstream>

// Callback to write the file data to a local file
static size_t WriteFileCallback(void* contents, size_t size, size_t nmemb, std::ofstream* file) {
    size_t totalSize = size * nmemb;
    file->write((char*)contents, totalSize);
    return totalSize;
}

bool downloadFile(const std::string& fileUrl, const std::string& destinationPath) {
    CURL* curl;
    CURLcode res;
    std::ofstream file;

    // Open the destination file
    file.open(destinationPath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open destination file." << std::endl;
        return false;
    }

    // Initialize CURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set URL for the file download
        curl_easy_setopt(curl, CURLOPT_URL, fileUrl.c_str());

        // Set up callback to write file data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFileCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);

        // Perform the file download
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            std::cerr << "File download failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "File downloaded successfully to: " << destinationPath << std::endl;
        }

        // Clean up
        curl_easy_cleanup(curl);
    }

    file.close();
    curl_global_cleanup();

    return res == CURLE_OK;
}
