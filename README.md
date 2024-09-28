# book-search

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

### Datastructures

1. Document indexing (Inverted Index)
- **Efficent**: uses Hash Table
- **Time Complexity**: Average case \( O(1) \) for insert and lookup of keywords
- **Space Complexity**: \( O(n) \), n is the number of unique terms across all documents
- **Notes**: Scalability can be improved using compression techniques on the postings lists (e.g. encoding or delta encoding)

##  Resources
- **Hash Tables**  
    - [Open-sourcing F14 for faster, more memory-efficient hash tables](https://engineering.fb.com/2019/04/25/developer-tools/f14/)
