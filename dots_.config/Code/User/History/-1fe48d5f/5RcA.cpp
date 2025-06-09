#include "FetchOpenAnime.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <regex>
#include <ctime>
#include <thread>
#include <optional>
#include <tuple>
	
using json = nlohmann::json;
	
FetchData::FetchData() {
    base_url = "https://api.openani.me/";
    player = "https://de2---vn-t9g4tsan-5qcl.yeshi.eu.org";
    headers["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/133.0.0.0 Safari/537.36";
    headers["origin"] = "https://openani.me";
    headers["Referer"] = "https://openani.me";
    headers["Accept"] = "application/json";
}

std::string FetchData::make_request(const std::string& url) {
    CURL *curl;
    CURLcode res;
    std::string read_buffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        struct curl_slist *chunk = NULL;
        for (const auto& header : headers) {
            std::string header_str = header.first + ": " + header.second;
            chunk = curl_slist_append(chunk, header_str.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "Error: " << curl_easy_strerror(res) << "\n";
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(chunk);
    }
    curl_global_cleanup();
    return read_buffer;
}

size_t FetchData::write_callback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append(static_cast<char*>(contents), total_size);
    return total_size;
}

// json çekmek için funksiyon
json FetchData::_get_json(const std::string& url) {
    std::string response = make_request(url);
    if (!response.empty()) {
        try {
            return json::parse(response);
        } catch (json::parse_error& e) {
            std::cerr << "Error parsing JSON: " << e.what() << "\n";
            std::cerr << "Response: " << response << "\n";  // Log the response for debugging
        }
    }
    return nullptr;
}

size_t FetchData::writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// yönlendirildiğimiz url'yi bulması için funksiyon.
std::string FetchData::getFinalUrl(const std::string& initial_url) {
    CURL* curl;
    CURLcode res;
    char* effective_url = nullptr;
    std::string response_data;

    curl = curl_easy_init(); // CURL başlat
    if (curl) {
        // CURL ayarları
        curl_easy_setopt(curl, CURLOPT_URL, initial_url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // otomatik yönlendirmeleri takip et
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        // HTTP isteğini gönder
        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
            if (effective_url) {
                std::string final_url(effective_url);
                curl_easy_cleanup(curl);
                return final_url;
            }
        } else {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << "\n";
        }
        curl_easy_cleanup(curl);
    }
    return "";
}

// anime ara ve sonucu çek
std::vector<std::map<std::string, std::string>> FetchData::fetch_anime_search_data_openani(const std::string& query) {
    std::string modified_query = query;
    std::replace(modified_query.begin(), modified_query.end(), ' ', '-');
    std::string search_url = base_url + "anime/search?q=" + modified_query;
    json data = _get_json(search_url);
    
    std::vector<std::map<std::string, std::string>> results;

    if (!data.is_null() && data.is_array()) {
        for (const auto& anime : data) {
            std::map<std::string, std::string> item;
            if (anime.contains("english") && anime["english"].is_string()) {
                item["name"] = anime["english"];
            } else {
                item["name"] = "Unknown";
            }

            if (anime.contains("slug") && anime["slug"].is_string()) {
                item["slug"] = anime["slug"];
            } else {
                item["slug"] = "unknown-slug";
            }

            results.push_back(item);
        }
    }

    return results;
}

std::tuple<int, std::string, std::optional<int>, bool> FetchData::fetch_anime_seasons_openani(const std::string& slug) {
    std::string url = base_url + "anime/" + slug;
    json data = _get_json(url);

    if (!data.is_null()) {
        bool is_movie = false;

        // Eğer numberOfSeasons alanı yoksa ya da null ise movie kabul edilir
        int seasons_count = 0;
        if (data.contains("numberOfSeasons") && !data["numberOfSeasons"].is_null()) {
            seasons_count = data["numberOfSeasons"].get<int>();
        } else {
            is_movie = true;
        }

        std::optional<int> malID;
        if (data.contains("malID") && data["malID"].is_number()) {
            malID = data["malID"].get<int>();
        }

        return {seasons_count, slug, malID, is_movie};
    }

    return {0, slug, std::nullopt, false};
}



std::vector<std::tuple<std::string, int, int>> FetchData::fetch_anime_season_episodes(const std::string& slug) {
    int season_count;
    std::string actual_slug;
    std::optional<int> malID;
    bool is_movie;

    std::tie(season_count, actual_slug, malID, is_movie) = fetch_anime_seasons_openani(slug);

    std::vector<std::tuple<std::string, int, int>> all_episodes;

    if (is_movie) {
        std::string url = base_url + "anime/" + actual_slug + "/season/1/episode/1";
        json data = _get_json(url);

        if (!data.empty() && data.contains("episodeData")) {
            std::string name = data["episodeData"].value("name", "No name");
            int episode_number = 1;
            int season_number = 1;
            all_episodes.emplace_back(name, episode_number, season_number);
        }

        return all_episodes;
    }

    if (season_count == 0) {
        std::cout << "No seasons found." << std::endl;
        return {};
    }

    for (int season = 1; season <= season_count; ++season) {
        std::string url = base_url + "anime/" + actual_slug + "/season/" + std::to_string(season);
        json data = _get_json(url);

        if (!data.empty() && data.contains("season") && data["season"].contains("episodes")) {
            const auto& episodes = data["season"]["episodes"];
            int season_number = data["season"].value("season_number", season);

            for (const auto& episode : episodes) {
                std::string name = episode.value("name", "No name");
                int episode_number = episode.value("episodeNumber", 0);

                all_episodes.emplace_back(name, episode_number, season_number);
            }
        }
    }

    std::sort(all_episodes.begin(), all_episodes.end(), [](const auto& a, const auto& b) {
        return std::tie(std::get<2>(a), std::get<1>(a)) < std::tie(std::get<2>(b), std::get<1>(b));
    });

    return all_episodes;
}

std::string FetchData::fetch_anime_episode_watch_api_url(const std::string& slug, int episode, int season) {
    if (slug.empty()) {
        std::cout << "No slug provided." << std::endl;
        return "";
    }

    std::string url = base_url + "anime/" + slug + "/season/" + std::to_string(season) + "/episode/" + std::to_string(episode);
    json data = _get_json(url);

    if (!data.empty() && data.contains("episodeData")) {
        // Iterate over the episode files to find the best one
        for (int index = 3; index >= 0; --index) {
            try {
                // Return the file URL if it exists
                return data["episodeData"]["files"][index]["file"];
            } catch (const std::exception& e) {
                // Continue if there's an issue with accessing the file at index
                continue;
            }
        }
    }

    return "";
}



json FetchData::parseJsonResponse(const std::string& response) {
    json jsonData;
    try {
        jsonData = json::parse(response);
    } catch (const json::parse_error& e) {
        throw std::runtime_error("Failed to parse JSON response: " + std::string(e.what()));
    }
    return jsonData;
}

std::vector<std::map<std::string, std::string>> FetchData::extractUrls(const json& jsonData) {
    std::vector<std::map<std::string, std::string>> urls;
    for (const auto& item : jsonData["urls"]) {
        std::map<std::string, std::string> urlData;
        urlData["url"] = item.value("url", "No URL field");
        urls.push_back(urlData);
    }
    return urls;
}
