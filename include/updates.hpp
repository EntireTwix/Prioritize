#pragma once

// contributed by STBoyden

#include "github_api/github_api_curl.hpp"
#include "github_api/request.hpp"
#include "nlohmann/json.hpp"
#include "file_io.hpp"
#include <iostream>
#include <utility>
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
    std::string created_at;
    std::string published_at;
    int id;
    GithubAuthor author;
};

static GithubRelease current_release;
static bool update_active = false;

void to_json(json &j, const GithubRelease &v)
{
    j = {
        {"html_url", v.html_url},
        {"tag_name", v.tag_name},
        {"created_at", v.created_at},
        {"published_at", v.published_at},
        {"id", v.id},
        {"author", {{"login", v.author.login}, {"url", v.author.url}}}};
}

void from_json(const json &j, GithubRelease &v)
{
    v = {j["html_url"],
         j["tag_name"],
         j["created_at"],
         j["published_at"],
         j["id"],
         GithubAuthor{j["author"]["login"], j["author"]["url"]}};
}

GithubRelease GetCurrent(const std::string &file_path)
{
    json data;
    if (!Load(file_path, data))
    {
        return GithubRelease{};
    }

    return data;
}

bool CheckForUpdates()
{
    GitHub::CurlBackend::Api github;
    GitHub::Request request(github.connect());

    GithubRelease fetched_release = json::parse(request.getReleases("EntireTwix", "PrioritizationGUI"))[0];
    auto stored = GetCurrent("current_version.json");

    if (fetched_release.id && (stored.id < fetched_release.id))
    {
        current_release = fetched_release;
    }

    if (!Save("current_version.json", fetched_release))
    {
        std::cerr << "Could not write to \"current_version.json\"" << std::endl;
    }
}
