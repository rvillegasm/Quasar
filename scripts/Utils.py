import os
import sys
import time

import requests
from fake_useragent import UserAgent


def download_file(url, filepath):
    with open(filepath, "wb") as file:
        user_agent = UserAgent()
        headers = {"User-Agent": user_agent.chrome}
        response = requests.get(url, headers=headers, stream=True)
        total = response.headers.get('content-length')

        if total is None:
            file.write(response.content)
        else:
            downloaded = 0
            total = int(total)
            start_time = time.time()
            for data in response.iter_content(chunk_size=max(total//1000, 1024*1024)):
                downloaded += len(data)
                file.write(data)
                done = int(50 * downloaded / total)
                percentage = (downloaded / total) * 100
                elapsed_time = time.time() - start_time
                average_kb_per_second = (downloaded / 1024) / elapsed_time
                average_speed_string = f"{average_kb_per_second:.2f} KB/s"
                if average_kb_per_second > 1024:
                    average_mb_per_second = average_kb_per_second / 1024
                    average_speed_string = f"{average_mb_per_second:.2f} MB/s"

                sys.stdout.write(f"\r[{'█' * done}{'.' * (50 - done)}] {percentage:.2f}% ({average_speed_string})     ")
                sys.stdout.flush()

    sys.stdout.write("\n")


def create_dir_if_not_exists(dir_name):
    if not os.path.exists(dir_name):
        os.mkdir(dir_name)


def yes_or_no():
    while True:
        reply = str(input('[Y/N]: ')).lower().strip()
        if reply[:1] == 'y':
            return True
        if reply[:1] == 'n':
            return False
