import requests
import time
import os
from bs4 import BeautifulSoup
from urllib.parse import urljoin

url = 'http://192.168.1.85/'

def parse_images_from_page(page_url):
    response = requests.get(page_url)
    if response.status_code == 200:
        soup = BeautifulSoup(response.content, 'html.parser')
        image_tags = soup.find_all('img')  # Подставьте тег, содержащий изображения на вашем сайте
        for img_tag in image_tags:
            img_url = urljoin(page_url, img_tag['src'])  # Получаем ссылку на изображение
            save_image(img_url)

def save_image(img_url):
    response = requests.get(img_url)
    if response.status_code == 200:
        img_data = response.content
        filename = "src/images/camera.bmp"
        f = open(filename, 'wb')
        f.write(img_data)
        print(f"Сохранено изображение: {filename}")

def monitor_pages():
    i = 1
    while True:
        page_url = f'http://192.168.1.85/camera?{i}'
        parse_images_from_page(page_url)
        i += 1

        time.sleep(5)

if __name__ == "__main__":
    time.sleep(10)
    monitor_pages()
