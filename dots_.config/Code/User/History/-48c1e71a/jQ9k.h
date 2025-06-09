#ifndef FETCH_OPENANIME_H
#define FETCH_OPENANIME_H

#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

class FetchData {
public:
    FetchData();
    std::string make_request(const std::string& url);
    nlohmann::json _get_json(const std::string& url);
    std::string getFinalUrl(const std::string& initial_url);
    std::vector<std::map<std::string, std::string>> fetch_anime_search_data(const std::string& query);
    std::vector<int> fetch_anime_seasons(const std::string& selected_id);
    std::vector<std::map<std::string, std::string>> fetch_anime_episodes(const std::string& selected_id);
    std::vector<std::map<std::string, std::string>> fetch_anime_watch_api_url(const std::string& url);
    std::string fetch_anime_watch_api_url_movie(const std::string& selected_id);
    nlohmann::json parseJsonResponse(const std::string& response);
    std::vector<std::map<std::string, std::string>> extractUrls(const nlohmann::json& jsonData);

private:
    std::string base_url;
    std::map<std::string, std::string> headers;
    std::vector<std::string> video_players;
    static size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* output);
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
};

#endif // FETCH_OPENANIME_H
