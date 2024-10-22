#pragma once

#include <booksearch/InvertedIndex.h>
#include <spdlog/spdlog.h>

#include <string>

constexpr char SEPARATOR[] = "--------------------------------------------------";
constexpr char BULLET[] = "•";

void paginate_results(const Vector<SearchResult>& results, const size_t results_per_page);
