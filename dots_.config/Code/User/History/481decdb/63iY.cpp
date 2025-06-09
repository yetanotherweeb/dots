#include <iostream>
#include "modules/FetchOpenAnime.h" // FetchOpenAnime başlık dosyasını ekliyoruz

int main() {
    // FetchData sınıfından bir nesne oluşturuyoruz
    FetchData fetchData;

    // Anime araması yapmak için bir sorgu yazıyoruz
    std::string query = "Talentless Nana";  // Arama yapmak istediğiniz anime ismi
    auto results = fetchData.fetch_anime_search_data_openani(query);

    // Sonuçları ekrana yazdırıyoruz
    if (results.empty()) {
        std::cout << "No results found for query: " << query << std::endl;
    } else {
        std::cout << "Search results for '" << query << "':" << std::endl;

        // Her bir animeyi ele alıyoruz
        for (const auto& anime : results) {
            std::string slug = anime.at("slug");
            std::cout << "Name: " << anime.at("name") << ", Slug: " << slug << std::endl;

            // Anime sezon bilgilerini alıyoruz
            auto [seasonCount, returnedSlug, malID] = fetchData.fetch_anime_seasons_openani(slug);
            std::cout << "--- Season Info ---" << std::endl;
            std::cout << "Slug: " << returnedSlug << std::endl;
            std::cout << "Number of Seasons: " << seasonCount << std::endl;
            if (malID.has_value()) {
                std::cout << "malID: " << malID.value() << std::endl;
            } else {
                std::cout << "malID: Not available" << std::endl;
            }

            // Eğer sezon varsa, bölümleri de alıyoruz
            if (seasonCount > 0) {
                auto episodes = fetchData.fetch_anime_season_episodes(returnedSlug);

                std::cout << "--- Episodes ---" << std::endl;
                for (const auto& [name, episodeNumber, seasonNumber] : episodes) {
                    std::cout << "Season " << seasonNumber << " - Episode " << episodeNumber << ": " << name << std::endl;
                }
            } else {
                std::cout << "No episodes found." << std::endl;
            }
        }
    }

    return 0;
}
