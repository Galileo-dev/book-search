import argparse
import csv
import logging
import threading
from pathlib import Path
import re
import requests

# Adapted from https://skeptric.com/gutenberg/index.html

GUTENBERG_CATALOG_CSV_URL = "https://www.gutenberg.org/cache/epub/feeds/pg_catalog.csv.gz"
GUTENBERG_TEXT_URLS = [
    "https://www.gutenberg.org/files/{id}/{id}.txt",
    "https://www.gutenberg.org/files/{id}/{id}-0.txt",
    "https://www.gutenberg.org/files/{id}/{id}-8.txt",
]
GUTENBERG_START_TEXT_MARKER = "*** START OF THE PROJECT GUTENBERG EBOOK"
GUTENBERG_END_TEXT_MARKER = "*** END OF THE PROJECT GUTENBERG EBOOK"

def get_catalog():
    logging.info("Downloading catalog...")
    response = requests.get(GUTENBERG_CATALOG_CSV_URL)
    response.raise_for_status()
    csv_text = response.content.decode("utf-8")
    reader = csv.DictReader(csv_text.splitlines())
    return list(reader)

def filter_books(catalog, author=None):
    logging.info("Filtering books...")
    return [
        row for row in catalog
        if (row["Language"] == "en")
        and row["Type"] == "Text"
    ][0:100]

def strip_headers(text):
    start_marker = re.escape(GUTENBERG_START_TEXT_MARKER)
    end_marker = re.escape(GUTENBERG_END_TEXT_MARKER)
    pattern = f"{start_marker}.*?{end_marker}"
    match = re.search(pattern, text, re.DOTALL)
    return match.group(0) if match else text

def download_book(book, data_path, session):
    book_id = book["Text#"]
    title = book['Title']

    for url_template in GUTENBERG_TEXT_URLS:
        url = url_template.format(id=book_id)
        logging.info(f"Downloading {title} from {url}")
        try:
            response = session.get(url)
            if response.status_code == 200:
                text = response.text
                clean_text = strip_headers(text)
                file_path = data_path / f"{book_id} ({title}).txt"
                with open(file_path, "w", encoding="utf-8") as f:
                    f.write(clean_text)
                logging.info(f"Saved {title} to {file_path}")
                return
            else:
                logging.debug(f"Received status code {response.status_code} for URL {url}")
        except requests.HTTPError as e:
            logging.error(f"Failed to download {url}: {e}")
        except Exception as e:
            logging.error(f"An error occurred while downloading {url}: {e}")
    logging.error(f"Failed to download {title} (ID: {book_id}). No valid URLs found.")

def download_books(books, data_path, num_threads):
    logging.info(f"Starting download of {len(books)} books...")
    threads = []
    session = requests.Session()
    for book in books:
        thread = threading.Thread(target=download_book, args=(book, data_path, session))
        threads.append(thread)
        thread.start()
        if len(threads) >= num_threads:
            for t in threads:
                t.join()
            threads = []
    for t in threads:
        t.join()
    logging.info("All downloads completed.")


def main():
    parser = argparse.ArgumentParser(description="Download books from Project Gutenberg.")
    parser.add_argument("--author", type=str, default="Wodehouse, P. G.", help="Author to filter by.")
    parser.add_argument("--output-dir", type=str, default="datasets/books/", help="Directory to save books.")
    parser.add_argument("--num-threads", type=int, default=5, help="Number of threads for downloading.")
    args = parser.parse_args()

    logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s] %(message)s")

    data_path = Path(args.output_dir)
    data_path.mkdir(parents=True, exist_ok=True)

    catalog = get_catalog()

    books = filter_books(catalog, author=args.author)
    logging.info(f"Found {len(books)} books by {args.author}")

    download_books(books, data_path, args.num_threads)
if __name__ == "__main__":
    main()