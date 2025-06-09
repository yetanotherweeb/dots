#include <iostream>
#include "modules/IFetchData.h"
#include "modules/FetchOpenAnime.h"

int main() {
    // IFetchData arayüzünden bir işlevi test ediyoruz
    IFetchData* fetchData = new FetchOpenAnime(); // FetchOpenAnime kullanılıyor

    std::string query = "one piece";
    auto results = fetchData->fetch_anime_search_data(query);

    // Sonuçları yazdır
    for (const auto& anime : results) {
        std::cout << "Name: " << anime.at("name") << ", Slug: " << anime.at("slug") << std::endl;
    }

    delete fetchData; // Bellek temizliği
    return 0;
}
