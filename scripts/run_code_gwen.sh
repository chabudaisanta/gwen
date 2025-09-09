#!/bin/bash

# プログラムへのパス
EXECUTABLE="/home/chabudaisanta/lib/gwen/debug/a.exe"
# 入力ファイルへのパス
INPUT_FILE="/home/chabudaisanta/lib/gwen/debug/in.txt"
# 標準出力のリダイレクト先ファイル
OUTPUT_FILE="/home/chabudaisanta/lib/gwen/debug/out.txt"
# 標準エラー出力のリダイレクト先ファイル
ERROR_FILE="/home/chabudaisanta/lib/gwen/debug/err.txt"
# タイムリミット（s）
TIME_LIMIT="5s"

# 既存の出力・エラーファイルをクリアする（新しい実行結果を確実に記録するため）
# ファイルが存在しない場合は作成されます。
> "$OUTPUT_FILE"
> "$ERROR_FILE"

echo "プログラムの実行を開始します..."
echo "  実行ファイル: $EXECUTABLE"
echo "  入力: $INPUT_FILE"
echo "  標準出力: $OUTPUT_FILE"
echo "  標準エラー出力: $ERROR_FILE"
echo "  タイムリミット: $TIME_LIMIT"
echo "--------------------------------------------------"

# 'timeout' コマンドを使用して、指定された時間制限でプログラムを実行します。
# プログラムの標準入力をINPUT_FILEから読み込み、標準出力をOUTPUT_FILEに、標準エラー出力をERROR_FILEにリダイレクトします。
timeout "$TIME_LIMIT" "$EXECUTABLE" < "$INPUT_FILE" > "$OUTPUT_FILE" 2> "$ERROR_FILE"

# timeoutコマンドの終了ステータスを取得します。
# timeoutが時間切れでコマンドを終了させた場合、終了ステータスは124になります。
# timeoutが内部エラーで終了した場合も124以外のステータスが返されることがあります。
# 実行されたコマンドが正常終了した場合、そのコマンドの終了ステータスが返されます。
EXIT_STATUS=$?

# echo "--------------------------------------------------"

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

echo "完了。"