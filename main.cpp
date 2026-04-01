#include <QApplication>
#include <QTableWidget>
#include <QStringList>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv); // Qtの初期設定だろう、args送って何している？

    QTableWidget table; // Cppって型宣言時点でコンストラクタ呼ばれるんだっけ？

    table.setRowCount(3); // テーブルの定義を上でして、ここは行が3
    table.setColumnCount(3); // ここは列が3

    QStringList headers = {"名称", "分類", "詳細"}; // 型名的にQt用の文字列リスト？vectorとかとは何が違う？
    table.setHorizontalHeaderLabels(headers); // 英語ムズイ、が

    table.setItem(0, 0, new QTableWidgetItem("ベホイミ")); // 恐らく1行1列目に入れる内容
    table.setItem(0, 1, new QTableWidgetItem("呪文")); // こっちも1行2列目だろうね
    table.setItem(0, 2, new QTableWidgetItem("回復")); // で、個人的にはcss、json、yamlのどれかで行こうとは思ってる

    table.show(); // ここで表示すかね

    return app.exec(); // 実行してWindows閉じたら即終了
}
