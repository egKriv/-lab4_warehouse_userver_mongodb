cmake_minimum_required(VERSION 3.12...3.31)
project(inventory-api CXX)

# Ищем userver в системе
find_package(
    userver
    COMPONENTS core postgresql mongo
    QUIET
)
if(NOT userver_FOUND)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    include(DownloadUserver)
    download_userver(TRY_DIR third_party/userver)
    find_package(userver REQUIRED COMPONENTS core chaotic postgresql mongo)
endif()

userver_setup_environment()

# jwt-cpp через FetchContent (не требует CPM)
include(FetchContent)
find_package(jwt-cpp QUIET)
if (NOT jwt-cpp_FOUND)
  message(STATUS "Loading JWT-CPP via FetchContent")
  FetchContent_Declare(
    jwt-cpp
    GIT_REPOSITORY https://github.com/Thalhammer/jwt-cpp.git
    GIT_TAG v0.7.2
  )
  set(JWT_BUILD_EXAMPLES OFF CACHE BOOL "")
  set(JWT_BUILD_TESTS OFF CACHE BOOL "")
  set(JWT_CMAKE_FILES_INSTALL ON CACHE BOOL "")
  FetchContent_MakeAvailable(jwt-cpp)
endif()

include_directories(src)

file(GLOB DOMAIN_SOURCES src/domain/*.cpp)
file(GLOB INFRASTRUCTURE_SOURCES src/infrastructure/**/*.cpp)
file(GLOB LIB_SOURCES src/lib/**/*.cpp)
file(GLOB SERVICES_SOURCES src/services/**/*.cpp)
file(GLOB CONTROLLERS_SOURCES src/controllers/**/*.cpp)

add_library(
    ${PROJECT_NAME}_objs OBJECT
    ${DOMAIN_SOURCES}
    ${INFRASTRUCTURE_SOURCES}
    ${LIB_SOURCES}
    ${SERVICES_SOURCES}
    ${CONTROLLERS_SOURCES}
)
target_link_libraries(
    ${PROJECT_NAME}_objs
    PUBLIC userver::core
    PUBLIC userver::postgresql
    PUBLIC userver::mongo
    PUBLIC jwt-cpp::jwt-cpp
)
target_include_directories(${PROJECT_NAME}_objs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)

include(GNUInstallDirs)

if(DEFINED ENV{PREFIX})
    message(STATUS "Set install prefix: $ENV{PREFIX}")
    file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
    set(CMAKE_INSTALL_PREFIX "${PREFIX_PATH}")
endif()

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT ${PROJECT_NAME}
)
install(
    FILES ${CONFIGS_FILES}
    DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}
    COMPONENT ${PROJECT_NAME}
)