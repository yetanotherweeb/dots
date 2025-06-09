#ifndef IFETCHDATA_H
#define IFETCHDATA_H

#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class IFetchData {
public:
    virtual ~IFetchData() = default;

    virtual std::string make_request(const std::string& url) = 0;
    virtual std::vector<std::map<std::string, std::string>> fetch_anime_search_data(const std::string& query) = 0;
    virtual std::vector<std::map<std::string, std::string>> fetch_anime_episodes(const std::string& selected_id) = 0;
    virtual std::vector<int> fetch_anime_seasons(const std::string& selected_id) = 0;
    virtual std::vector<std::map<std::string, std::string>> fetch_anime_watch_api_url(const std::string& url) = 0;
    virtual std::string fetch_anime_watch_api_url_movie(const std::string& selected_id) = 0;
    virtual std::string getFinalUrl(const std::string& initial_url) = 0;
};

#endif // IFETCHDATA_H
