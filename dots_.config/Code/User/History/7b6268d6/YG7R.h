// FetchAnimeciX.h
#ifndef FETCH_ANIMECIX_H
#define FETCH_ANIMECIX_H

#include "IFetchData.h"

class FetchAnimeciX : public IFetchData {
public:
    FetchAnimeciX();

    std::string make_request(const std::string& url) override;
    std::vector<std::map<std::string, std::string>> fetch_anime_search_data(const std::string& query) override;
    std::vector<std::map<std::string, std::string>> fetch_anime_episodes(const std::string& selected_id) override;
    std::vector<int> fetch_anime_seasons(const std::string& selected_id) override;
    std::vector<std::map<std::string, std::string>> fetch_anime_watch_api_url(const std::string& url) override;
    std::string fetch_anime_watch_api_url_movie(const std::string& selected_id) override;
    std::string getFinalUrl(const std::string& initial_url) override;

private:
    json _get_json(const std::string& url);
    std::string base_url;
    std::vector<std::string> video_players;
    std::map<std::string, std::string> headers;

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
    static size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* out);
    json parseJsonResponse(const std::string& response);
    std::vector<std::map<std::string, std::string>> extractUrls(const json& jsonData);
};

#endif // FETCH_ANIMECIX_H
