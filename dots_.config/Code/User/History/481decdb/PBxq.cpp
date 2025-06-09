#include <curl/curl.h>
#include <iostream>
#include <string>

size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed initialization\n";
        return 1;
    }

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openani.me/anime");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // 10 saniye timeout
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
    else
        std::cout << "Response: " << response.substr(0, 200) << "\n";

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
}

