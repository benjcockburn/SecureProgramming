#include "file_Download.cpp"
#include "file_Upload.cpp"


int main() {
    std::string serverUrl = "http://yourserver.com/api/upload";
    std::string filePath = "path/to/your/file.txt";
    
    // Upload the file and get the file URL
    std::string fileUrl = uploadFile(serverUrl, filePath);
    if (!fileUrl.empty()) {
        std::cout << "File uploaded successfully. File URL: " << fileUrl << std::endl;

        // Download the file from the file URL
        std::string downloadDestination = "path/to/downloaded/file.txt";
        if (downloadFile(fileUrl, downloadDestination)) {
            std::cout << "File retrieved successfully." << std::endl;
        } else {
            std::cerr << "Failed to retrieve the file." << std::endl;
        }
    } else {
        std::cerr << "Failed to upload the file." << std::endl;
    }

    return 0;
}
