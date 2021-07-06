import os


def create_dir_if_not_exists(dir_name):
    if not os.path.exists(dir_name):
        os.mkdir(dir_name)


def yes_or_no(msg):
    while True:
        reply = str(input(f'{msg} [Y/N]: ')).lower().strip()
        if reply[:1] == 'y':
            return True
        if reply[:1] == 'n':
            return False
