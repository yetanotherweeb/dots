#include <iostream>
#include "modules/FetchOpenAnime.h" // FetchOpenAnime başlık dosyasını ekliyoruz

int main() {
    FetchData fetchData;

    std::string query = "Talentless Nana";
    auto results = fetchData.fetch_anime_search_data_openani(query);

    if (results.empty()) {
        std::cout << "No results found for query: " << query << std::endl;
    } else {
        std::cout << "Search results for '" << query << "':" << std::endl;

        for (const auto& anime : results) {
            std::string slug = anime.at("slug");
            std::cout << "Name: " << anime.at("name") << ", Slug: " << slug << std::endl;

            auto [seasonCount, returnedSlug, malID] = fetchData.fetch_anime_seasons_openani(slug);
            std::cout << "--- Season Info ---" << std::endl;
            std::cout << "Slug: " << returnedSlug << std::endl;
            std::cout << "Number of Seasons: " << seasonCount << std::endl;
            if (malID.has_value()) {
                std::cout << "malID: " << malID.value() << std::endl;
            } else {
                std::cout << "malID: Not available" << std::endl;
            }
        }
    }

    return 0;
}
