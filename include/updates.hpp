#pragma once

// contributed by STBoyden

#include "github_api/github_api_curl.hpp"
#include "github_api/request.hpp"
#include "nlohmann/json.hpp"
#include <iostream>
#include <vector>

using namespace nlohmann;

struct GithubAuthor
{
    std::string login;
    std::string url;
};

struct GithubRelease
{
    std::string html_url;
    std::string tag_name;
    std::string body;
    std::string created_at;
    std::string published_at;
    GithubAuthor author;
};

GithubRelease GetCurrent(const char* file_path) {

}

bool CheckForUpdates()
{
    GitHub::CurlBackend::Api github;
    GitHub::Request request(github.connect());

    json response = json::parse(request.getReleases("EntireTwix", "PrioritizationGUI"))[0];

    GithubRelease release = {
        response["html_url"],
        response["tag_name"],
        response["body"],
        response["created_at"],
        response["published_at"],
        GithubAuthor{response["author"]["login"], response["author"]["url"]}};
}
