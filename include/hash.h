#include <string>

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL
// Return 64-bit FNV-1a hash for key (NUL-terminated).
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
inline uint64_t fnv1a_hash(const std::string &key) {
  uint64_t hash = FNV_OFFSET;
  for (char c : key) {
    hash ^= static_cast<uint64_t>(static_cast<unsigned char>(c));
    hash *= FNV_PRIME;
  }
  return hash;
}
