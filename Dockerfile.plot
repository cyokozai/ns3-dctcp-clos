# plotting/Dockerfile
FROM python:3.11-slim

WORKDIR /plotting

# 必要なライブラリをインストール
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# プロットスクリプトをコピー
COPY src/ .

# 実行コマンド
CMD ["python3", "plot-results.py"]
