#ifndef ANITR_FETCH_H
#define ANITR_FETCH_H

#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class FetchData {
public:
    FetchData();
    std::string make_request(const std::string& url);
    std::vector<std::map<std::string, std::string>> fetch_anime_search_data_anmcx(const std::string& query);
    std::vector<std::map<std::string, std::string>> fetch_anime_episodes_anmcx(const std::string& selected_id);
    std::vector<int> fetch_anime_seasons_anmcx(const std::string& selected_id);
    std::vector<std::map<std::string, std::string>> fetch_anime_watch_api_url(const std::string& base_url, const std::string& url);
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
    static size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* out);
    json parseJsonResponse(const std::string& response);
    std::vector<std::map<std::string, std::string>> extractUrls(const json& jsonData);
    std::string fetch_anime_watch_api_url_movie(const std::string& selected_id);
    std::string getFinalUrl(const std::string& initial_url);

private:
    json _get_json(const std::string& url);
    std::string anmcx_base_url;
    std::vector<std::string> video_players;
    std::map<std::string, std::string> headers;
};

#endif // ANITR_FETCH_H
