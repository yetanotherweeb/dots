cmake_minimum_required(VERSION 3.12)

project(anitr-cli VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -flto -funroll-loops -ftree-vectorize")

# Derlenecek dosyalar
set(SOURCES_ANIMECIX
    modules/FetchAnimeciX.cpp
    animecix-cli.cpp
)

set(SOURCES_OPENANIME
    modules/FetchOpenAnime.cpp
    openanime-cli.cpp
)

set(SOURCES_ANITR
    anitr-cli.cpp
)

# Animecix-cli için derleme
add_executable(animecix-cli ${SOURCES_ANIMECIX})

# Openanime-cli için derleme
add_executable(openanime-cli ${SOURCES_OPENANIME})

# Anitr-cli için derleme
add_executable(anitr-cli ${SOURCES_ANITR})

# Curl kütüphanesini linkle
target_link_libraries(animecix-cli PRIVATE curl)
target_link_libraries(openanime-cli PRIVATE curl)
target_link_libraries(anitr-cli PRIVATE curl)
