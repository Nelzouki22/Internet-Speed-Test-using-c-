#include <iostream>
#include <curl/curl.h>
#include <chrono>
#include <thread>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    (void)contents;
    (void)userp;
    return size * nmemb;
}

double measure_speed(const std::string& url) {
    CURL* curl;
    CURLcode res;
    double speed_download = 0.0;
    double speed_upload = 0.0;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        auto start = std::chrono::high_resolution_clock::now();

        res = curl_easy_perform(curl);
        auto end = std::chrono::high_resolution_clock::now();

        if (res == CURLE_OK) {
            curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &speed_download);
            curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
        }

        curl_easy_cleanup(curl);
    }

    return speed_download;
}

int main() {
    const std::string url = "http://speedtest.tele2.net/1MB.zip";

    std::cout << "إجراء اختبار سرعة التحميل...\n";
    double download_speed = measure_speed(url);

    std::cout << "سرعة التحميل: " << download_speed / 1024 / 1024 << " ميغابت في الثانية\n";

    return 0;
}
