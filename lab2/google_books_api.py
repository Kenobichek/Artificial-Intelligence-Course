import requests

from config import GB_API_KEY

def get_books(q: str):
    url = f'https://www.googleapis.com/books/v1/volumes?q={q}"&key={GB_API_KEY}'

    response = requests.get(url)
    data = response.json()

    book_titles = []

    for item in data.get('items', []):
        title = item['volumeInfo']['title']
        authors = item['volumeInfo'].get('authors', [])
        description = item['volumeInfo'].get('description', '')
        published_date = item['volumeInfo'].get('publishedDate', '')
        image_link = item['volumeInfo']['imageLinks']['thumbnail'] if 'imageLinks' in item['volumeInfo'] else 'No image available'

        book_titles.append({
            'title': title,
            'authors': authors,
            'description': description,
            'published_date': published_date,
            'image_link': image_link
        })

        print(f'Title: {title}')
        print(f'Authors: {", ".join(authors)}')
        print(f'Description: {description}')
        print(f'Published Date: {published_date}')
        print(f'Image Link: {image_link}\n')


    return book_titles
