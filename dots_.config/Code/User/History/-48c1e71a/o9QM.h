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
    std::vector<std::map<std::string, std::string>> fetch_anime_search_data_openani(const std::string& query);
    std::tuple<int, std::string, std::optional<int>, bool> fetch_anime_seasons_openani(const std::string& slug);
    std::vector<std::tuple<std::string, int, int>> fetch_anime_season_episodes(const std::string& slug);
    std::string fetch_anime_episode_watch_api_url(const std::string& slug, int episode, int season);
    std::string watch_url(const std::string& slug, const std::string& season_number, const std::string& video);
    nlohmann::json parseJsonResponse(const std::string& response);
    std::vector<std::map<std::string, std::string>> extractUrls(const nlohmann::json& jsonData);

private:
    std::string base_url;
    std::string player;
    std::map<std::string, std::string> headers;
    static size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* output);
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
};

#endif // FETCH_OPENANIME_H
