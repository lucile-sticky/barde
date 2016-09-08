cmake_minimum_required(VERSION 2.6)

set(PROJECT_NAME playlists)

project(${PROJECT_NAME})

add_definitions(-std=c++11)

find_library(LIB_BOOSTER booster)
find_library(LIB_CPPCMS cppcms)
find_library(LIB_CPPDB cppdb)
find_program(EXE_TMPL_CC cppcms_tmpl_cc)

find_path(INC_BOOSTER booster/config.h)
find_path(INC_CPPCMS cppcms/config.h)
find_path(INC_CPPDB cppdb/frontend.h)


set(SKIN_NAME generated_view)

set(PLAYLISTS_SRC
    app/main.cpp
    app/master.cpp
    app/login.cpp
    app/playlist.cpp
    app/playlistVote.cpp
    app/song.cpp
    app/songVote.cpp
    app/util/stringMethods.cpp
    app/validators/songValidator.cpp
    data/dbMapper.cpp
    data/artistMapper.cpp
    data/loginMapper.cpp
    data/playlistMapper.cpp
    data/playlistVoteMapper.cpp
    data/songMapper.cpp
    data/songVoteMapper.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/${SKIN_NAME}.cpp
)

include_directories(${INC_BOOSTER})
include_directories(${INC_CPPCMS})
include_directories(${INC_CPPDB})
include_directories(${CMAKE_SOURCE_DIR})


set(SKIN_TEMPLATES
    views/master.tmpl
    views/login.tmpl
    views/allPlaylists.tmpl
    views/playlist.tmpl
    views/playlistHtml5.tmpl
    views/playlistJwp.tmpl
    views/newPlaylist.tmpl
    views/editSong.tmpl
    views/proposedSongs.tmpl
)
add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${SKIN_NAME}.cpp"
	COMMAND ${EXE_TMPL_CC}
		${TMPL_CC_PARAMS}
        ${SKIN_TEMPLATES}
        -o "${CMAKE_CURRENT_BINARY_DIR}/${SKIN_NAME}.cpp"
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${SKIN_TEMPLATES})


add_executable(${PROJECT_NAME} ${PLAYLISTS_SRC})
target_link_libraries(${PROJECT_NAME}
    ${LIB_BOOSTER}
    ${LIB_CPPCMS}
    ${LIB_CPPDB}
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION sbin
)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config-fcgi.js
    DESTINATION /etc/${PROJECT_NAME}
)
