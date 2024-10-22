#include "IndexManager.h"

#include <spdlog/spdlog.h>

#include <cereal/archives/binary.hpp>
#include <fstream>

IndexManager::IndexManager(const std::string& indexPath) : indexPath(indexPath) {}

void IndexManager::loadIndex(InvertedIndex& searchEngine, Trie& autoComplete) {
  std::ifstream is(indexPath, std::ios::binary);
  if (is.is_open()) {
    cereal::BinaryInputArchive iarchive(is);
    iarchive(searchEngine, autoComplete);
    spdlog::info("Index loaded from {}", indexPath);
  } else {
    spdlog::warn("No index found at {}", indexPath);
  }
}

void IndexManager::saveIndex(const InvertedIndex& searchEngine, const Trie& autoComplete) {
  std::ofstream os(indexPath, std::ios::binary);
  cereal::BinaryOutputArchive oarchive(os);
  oarchive(searchEngine, autoComplete);
  spdlog::info("Index saved to {}", indexPath);
}