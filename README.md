# Book Search Engine
## Group Name: 

## Table of Contents
- [Project Assignment](#project-assignment)
- [Design](#design)
    - [Document indexing](#document-indexing-inverted-index)
    - [Keyword autocomplete](#keyword-autocomplete-trie)
- [Usage](#usage)

## Project Assignment

You’ve acquired a large number of college textbooks in electronic form. Naturally, all of these
documents were legally purchased and were most certainly not the result of piracy (perish
the thought). Regardless of their somewhat dubious provenance, textbooks aren’t much use
if you don’t know which ones contain relevant information!

Your task is to build a mini search engine for your repository of documents. First, you’ll need
to index the contents of the documents. This index should be stored for later use; rebuilding
it on the fly won’t cut it! Next, you’ll need to build a keyword search engine that uses the
index to return the most relevant results for each query. Ideally, you shouldn’t need access to
the original documents to perform a search.

As this is a data structures and algorithms project, you should design eﬃcient and scalable
solutions to each problem that this project poses! You’ll be required to justify the choices
that you make by making reference to Big O notation and asymptotic analysis in general.
Citing third-party analysis and results is acceptable, but you must be able to adequately
explain everything you include if you want to be credited for it!

### Design

#### Document indexing (Inverted Index)
- **Underlying Datastructures**: Hash Table
- **Time Complexity**: Average case \( O(n) \), n is the number of words.
- **Space Complexity**: \( O(v + e) \), v is vocabulary size, and e is the number of postings (term:document pairs).
- **Notes**: Scalability can be improved using compression techniques on the postings lists (e.g. encoding or delta encoding).

#### Keyword autocomplete (Trie)
- **Underlying Datastructures**: Prefix Tree
- **Time Complexity**: Average case \( O(n) \) for insert and search.
- **Space Complexity**: \( O(n) \), n is the number of keys.
- **Notes**: Space can be optimized using a "Radix tree" also known as a compressed trie.
#### Relevence Ranking (TF-IDF with Cosine Similarity)
- Relevance to the search query.
- Filter out [**Stop Words**](https://github.com/stopwords-iso/stopwords-en)

## Usage
```
./bin/main [options]
Options:
-h | --help        Print this help
-v | --verbose     Enable verbose logging
-a | --add         Add a document to the index
-s | --search      Search through the index
```

##  Resources
- **Hash Tables**  
    - [Open-sourcing F14 for faster, more memory-efficient hash tables](https://engineering.fb.com/2019/04/25/developer-tools/f14/)
- **Inverted Index**
    - [Inverted Index: a simple yet powerful data structure](https://evanxg852000.github.io/tutorial/rust/data/structure/2020/04/09/inverted-index-simple-yet-powerful-ds.html)
- **Relevence Ranking**
    - [TF-IDF Vectorization with Cosine Similarity](https://medium.com/@anurag-jain/tf-idf-vectorization-with-cosine-similarity-eca3386d4423)
        - Note: ingore the nlp stuff, I'm not implememting neural networks in c++...