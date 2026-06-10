#!/bin/bash

# プロジェクトのルートディレクトリを取得
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

# プログラムへのパス
EXECUTABLE="${ROOT}/.build/a.out"
# 入力ファイルへのパス
INPUT_FILE="${ROOT}/.build/in.txt"
# 標準出力のリダイレクト先ファイル
OUTPUT_FILE="${ROOT}/.build/out.txt"
# 標準エラー出力のリダイレクト先ファイル
ERROR_FILE="${ROOT}/.build/err.txt"
# タイムリミット（s）
TIME_LIMIT="5s"

# .build ディレクトリが存在することを確認
mkdir -p "${ROOT}/.build"

# 実行ファイルが存在しない場合はエラーを表示
if [ ! -f "$EXECUTABLE" ]; then
    echo "エラー: 実行ファイルが見つかりません。"
    echo "  ターゲット: $EXECUTABLE"
    echo "  まずはプログラムをコンパイルして $EXECUTABLE に配置してください。"
    exit 1
fi

# 入力ファイルが存在しない場合は空ファイルを作成
if [ ! -f "$INPUT_FILE" ]; then
    touch "$INPUT_FILE"
    echo "警告: 入力ファイル $INPUT_FILE が見つからなかったため、空のファイルを作成しました。"
fi

# 既存の出力・エラーファイルをクリアする
> "$OUTPUT_FILE"
> "$ERROR_FILE"

echo "プログラムの実行を開始します..."
echo "  実行ファイル: $EXECUTABLE"
echo "  入力: $INPUT_FILE"
echo "  標準出力: $OUTPUT_FILE"
echo "  標準エラー出力: $ERROR_FILE"
echo "  タイムリミット: $TIME_LIMIT"
echo "--------------------------------------------------"

# timeout コマンドを使用して、指定された時間制限でプログラムを実行します。
timeout "$TIME_LIMIT" "$EXECUTABLE" < "$INPUT_FILE" > "$OUTPUT_FILE" 2> "$ERROR_FILE"

# timeoutコマンドの終了ステータスを取得します。
EXIT_STATUS=$?

if [ "$EXIT_STATUS" -eq 124 ]; then
    echo "TLE"
    echo "エラー: 実行時間が ${TIME_LIMIT} を超えたため、プログラムは強制終了されました。"
elif [ "$EXIT_STATUS" -ne 0 ]; then
    echo "エラー: プログラムの実行中に問題が発生しました。"
    echo "  終了コード: $EXIT_STATUS"
    echo "  詳細は '$ERROR_FILE' ファイルを確認してください。"
else
    echo "プログラムは正常に終了しました。"
    echo "  出力は '$OUTPUT_FILE' に保存されました。"
    echo "  エラーメッセージは '$ERROR_FILE' に保存されました (エラーがなければ空です)。"
fi
