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


std::tuple<int, std::string, std::optional<int>> FetchData::fetch_anime_seasons_openani(const std::string& slug) {
    std::string url = base_url + "anime/" + slug;
    json data = _get_json(url);

    if (!data.is_null()) {
        int seasons_count = data.value("numberOfSeasons", 0);
        std::optional<int> malID;

        // Eğer malID varsa ve sayıysa al, yoksa boş bırak
        if (data.contains("malID") && data["malID"].is_number()) {
            malID = data["malID"].get<int>();
        }

        return {seasons_count, slug, malID};
    }

    return {0, slug, std::nullopt};
}

std::vector<std::tuple<std::string, int, int>> FetchData::fetch_anime_season_episodes(const std::string& slug) {
    int season_count;
    std::string actual_slug;
    std::optional<int> malID;

    std::tie(season_count, actual_slug, malID) = fetch_anime_seasons_data(slug);

    if (season_count == 0) {
        std::cout << "No seasons found." << std::endl;
        return {};
    }

    std::vector<std::tuple<std::string, int, int>> all_episodes;

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

    // Sırala: önce sezon numarasına, sonra bölüm numarasına göre
    std::sort(all_episodes.begin(), all_episodes.end(), [](const auto& a, const auto& b) {
        return std::tie(std::get<2>(a), std::get<1>(a)) < std::tie(std::get<2>(b), std::get<1>(b));
    });

    return all_episodes;
}


// url üzerinden media player'i ve oradan da api url'sini bul
// ve oradaki dataları çekerek nihai mp4 dosyasına ulaş
std::vector<std::map<std::string, std::string>> FetchData::fetch_anime_watch_api_url(const std::string& url) {
    std::string wtch_url = base_url + url;
    try {
        std::string finalResp = getFinalUrl(wtch_url);

        // 1. "embed/" sonrası embed_id'yi bul
        size_t embed_pos = finalResp.find("embed/");
        std::string embed_id;

        if (embed_pos != std::string::npos) {
            embed_pos += 6; 
            size_t next_slash = finalResp.find('/', embed_pos);
            if (next_slash != std::string::npos) {
                embed_id = finalResp.substr(embed_pos, next_slash - embed_pos);
            } else {
                embed_id = finalResp.substr(embed_pos); // Eğer '/' bulunmazsa sona kadar al
            }
        }

        // 2. "vid=" parametresini bul
        size_t vid_pos = finalResp.find("vid=");
        std::string vid_value;

        if (vid_pos != std::string::npos) {
            vid_pos += 4; // "vid=" uzunluğu 4 karakter
            size_t next_amp = finalResp.find('&', vid_pos); // '&' varsa, bir sonraki parametreyi sınırla
            if (next_amp != std::string::npos) {
                vid_value = finalResp.substr(vid_pos, next_amp - vid_pos);
            } else {
                vid_value = finalResp.substr(vid_pos); // '&' bulunmazsa sona kadar al
            }
        }

        std::string apiUrl = "https://tau-video.xyz/api/video/" + embed_id;
        std::vector<std::map<std::string, std::string>> urls;

        CURL* curl = curl_easy_init();
        if (curl) {
            //std::cout << "API URL: " << apiUrl << "\n";
            // HTTP GET isteği göndereceğiz
            curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());

            // Yanıtı bir string değişkenine kaydediyoruz
            std::string response_string;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

            // İstek gönderme ve yanıtı alma
            CURLcode res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
            } else {
                // JSON verisini işlemek için nlohmann/json kütüphanesini kullanıyoruz
                try {
                    nlohmann::json json_response = nlohmann::json::parse(response_string);

					std::string subtitle_url = "No Subtitle";  // Varsayılan değer
					
                    if (json_response.contains("subs")) {
                        for (const auto& subtitle : json_response["subs"]) {
                            // Her bir altyazı için 'url' değerini al
                            subtitle_url = subtitle.value("url", "No Subtitle URL");
                            // İhtiyaç duyarsanız başka işlemler de ekleyebilirsiniz
                        }
                    } else {
                        std::cerr << "'subs' dizisi boş veya eksik!" << std::endl;
                    }
                            
                    for (const auto& item : json_response["urls"]) {
                        std::map<std::string, std::string> urlData;
                        urlData["url"] = item.value("url", "No URL field");
                        urlData["quality"] = item.value("label", "Unknown");
                        urlData["subtitle"] = subtitle_url;  // Altyazı linkini ekle
                        
                        urls.push_back(urlData);
                    }
                } catch (const nlohmann::json::parse_error& e) {
                    std::cerr << "JSON parse error: " << e.what() << "\n";
                }
            }
        }

        return urls;
    } catch (const std::exception& e) {
        std::cerr << "Err" << e.what() << "\n";
    }
}

std::string FetchData::fetch_anime_watch_api_url_movie(const std::string& selected_id) {
    std::string url = "https://mangacix.net/secure/titles/" + selected_id;

	//std::cout << url << "\n";

    json data = _get_json(url);
    if (!data.empty()) {
        std::string video_url = data["title"]["videos"][0]["url"];
        if (!video_url.empty()) {
            try {
                std::string embed_id;
                size_t path_start = video_url.find("/embed/");
                if (path_start != std::string::npos) {
                    size_t id_start = path_start + 7; // "/embed/" uzunluğu 7 karakter
                    size_t id_end = video_url.find("/", id_start);
                    embed_id = video_url.substr(id_start, id_end - id_start);


                    //std::cout << embed_id << "\n";
                }

                std::string api_url = "https://" + video_players[0] + "/api/video/" + embed_id;
                //debug
                //std::cout << api_url << "\n";

                json video_data = _get_json(api_url);
                //std::cout << video_data["urls"] << "\n";

                std::vector<std::string> best_qualities = {"1080p", "720p", "480p"};
                for (const auto& quality : best_qualities) {
                    if (video_data.contains("urls")) {
                        for (const auto& url_info : video_data["urls"]) {
                            if (url_info.contains("label") && url_info["label"] == quality) {
                                return url_info["url"];
                            }
                        }
                    }
                }

            } catch (const std::exception& e) {
                std::cerr << "Error while fetching movie URL: " << e.what() << "\n";
                if (video_url.find(video_players[1]) != std::string::npos) {
                    return video_url + ".mp4";
                }
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
