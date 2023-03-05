# Readme

This is the source code for the Wails website.

## Development

To run the website locally, you will need to install python + pip. Follow the guide on the [mkdocs](https://www.mkdocs.org/user-guide/installation/) website.

Once you have installed mkdocs, run the following commands:

```bash
pip install -r requirements.txt
```

To run the website locally, run the following command:

```bash
mkdocs serve
```

## Windows

1. Install python from https://www.python.org/downloads/release/python-31010/
2. Ensure PATH is set up correctly:
   - Add `C:\Users\[username]\AppData\Local\Programs\Python\Python310` to PATH
   - Add `C:\Users\[username]\AppData\Local\Programs\Python\Python310\scripts` to PATH
3. Make sure to remove any previous python installation and remove any references to them in PATH
4. Reopen any terminal windows to pick up the new path. If you use goland, you need to restart the IDE.
5. In the `websitev3` directory, run `pip install -r requirements.txt`
6. Run `mkdocs serve` to start the server

---

Windows PowerShell
Copyright (C) Microsoft Corporation. All rights reserved.

Install the latest PowerShell for new features and improvements! https://aka.ms/PSWindows

PS C:\wails> cd .\websitev3\
PS C:\wails\websitev3> pip install -r requirements.txt
Collecting mkdocs-material==9.1.0
Using cached mkdocs_material-9.1.0-py3-none-any.whl (7.7 MB)
Collecting mkdocs-material-extensions==1.1.1
Using cached mkdocs_material_extensions-1.1.1-py3-none-any.whl (7.9 kB)
Collecting markdown>=3.2
Using cached Markdown-3.4.1-py3-none-any.whl (93 kB)
Collecting colorama>=0.4
Using cached colorama-0.4.6-py2.py3-none-any.whl (25 kB)
Collecting pymdown-extensions>=9.9.1
Using cached pymdown_extensions-9.9.2-py3-none-any.whl (219 kB)
Collecting mkdocs>=1.4.2
Using cached mkdocs-1.4.2-py3-none-any.whl (3.7 MB)
Collecting jinja2>=3.0
Using cached Jinja2-3.1.2-py3-none-any.whl (133 kB)
Collecting requests>=2.26
Downloading requests-2.28.2-py3-none-any.whl (62 kB)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 62.8/62.8 kB 3.3 MB/s eta 0:00:00
Collecting regex>=2022.4.24
Downloading regex-2022.10.31-cp310-cp310-win_amd64.whl (267 kB)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 267.7/267.7 kB 5.5 MB/s eta 0:00:00
Collecting pygments>=2.14
Using cached Pygments-2.14.0-py3-none-any.whl (1.1 MB)
Collecting MarkupSafe>=2.0
Downloading MarkupSafe-2.1.2-cp310-cp310-win_amd64.whl (16 kB)
Collecting pyyaml>=5.1
Downloading PyYAML-6.0-cp310-cp310-win_amd64.whl (151 kB)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 151.7/151.7 kB 8.8 MB/s eta 0:00:00
Collecting mergedeep>=1.3.4
Using cached mergedeep-1.3.4-py3-none-any.whl (6.4 kB)
Collecting pyyaml-env-tag>=0.1
Using cached pyyaml_env_tag-0.1-py3-none-any.whl (3.9 kB)
Collecting watchdog>=2.0
Using cached watchdog-2.3.1-py3-none-win_amd64.whl (80 kB)
Collecting packaging>=20.5
Using cached packaging-23.0-py3-none-any.whl (42 kB)
Collecting markdown>=3.2
Using cached Markdown-3.3.7-py3-none-any.whl (97 kB)
Collecting click>=7.0
Using cached click-8.1.3-py3-none-any.whl (96 kB)
Collecting ghp-import>=1.0
Using cached ghp_import-2.1.0-py3-none-any.whl (11 kB)
Collecting certifi>=2017.4.17
Downloading certifi-2022.12.7-py3-none-any.whl (155 kB)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 155.3/155.3 kB 4.5 MB/s eta 0:00:00
Collecting idna<4,>=2.5
Using cached idna-3.4-py3-none-any.whl (61 kB)
Collecting charset-normalizer<4,>=2
Downloading charset_normalizer-3.0.1-cp310-cp310-win_amd64.whl (96 kB)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 96.5/96.5 kB 2.8 MB/s eta 0:00:00
Collecting urllib3<1.27,>=1.21.1
Downloading urllib3-1.26.14-py2.py3-none-any.whl (140 kB)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 140.6/140.6 kB 4.2 MB/s eta 0:00:00
Collecting python-dateutil>=2.8.1
Using cached python_dateutil-2.8.2-py2.py3-none-any.whl (247 kB)
Collecting six>=1.5
Using cached six-1.16.0-py2.py3-none-any.whl (11 kB)
Installing collected packages: charset-normalizer, watchdog, urllib3, six, regex, pyyaml, pygments, packaging, mkdocs-material-extensions, mergedeep, MarkupSafe, markdown, idna, colorama, certifi, requests, pyyaml-env-tag, python-dateutil, pymdown-extensions, jinja2, click, ghp-import, mkdocs, mkdocs-material
Successfully installed MarkupSafe-2.1.2 certifi-2022.12.7 charset-normalizer-3.0.1 click-8.1.3 colorama-0.4.6 ghp-import-2.1.0 idna-3.4 jinja2-3.1.2 markdown-3.3.7 mergedeep-1.3.4 mkdocs-1.4.2 mkdocs-mat
erial-9.1.0 mkdocs-material-extensions-1.1.1 packaging-23.0 pygments-2.14.0 pymdown-extensions-9.9.2 python-dateutil-2.8.2 pyyaml-6.0 pyyaml-env-tag-0.1 regex-2022.10.31 requests-2.28.2 six-1.16.0 urllib3-1.26.14 watchdog-2.3.1

[notice] A new release of pip available: 22.3.1 -> 23.0.1
[notice] To update, run: python.exe -m pip install --upgrade pip
PS C:\wails\websitev3> mkdocs serve
INFO - Building documentation...
INFO - Cleaning site directory
INFO - Documentation built in 0.95 seconds
INFO - [15:10:30] Watching paths for changes: 'docs', 'mkdocs.yml'
INFO - [15:10:30] Serving on http://127.0.0.1:8000/

## Docker

```bash
docker run --rm -it -p 8000:8000 -v ${PWD}:/docs squidfunk/mkdocs-material
```

If it is Windows bash:

```bash
MSYS_NO_PATHCONV=1 docker run --rm -it -p 8000:8000 -v ${PWD}:/docs squidfunk/mkdocs-material
```
