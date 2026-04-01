#include <QApplication>
#include <QTableWidget>
#include <ioStream>
// #include <QString>
// #include <QMap>
// #include <QVariant>
// #include <vector>
// #include <functional>

// 列の型定義
enum class ColumnType{
    Int,    // 整数型
    Float, // 少数型
    Select, // 選択型
    String, // 文字列型
};

// 列定義
struct ColumnDefine{
    QString key;    // キー
    QString name;   // 表示名
    ColumnType Type;   // 型
};

// 必須行の定義
const ColumnDefine ID_DEFINE = {"ID", "ID", ColumnType::Int};
const ColumnDefine NAME_DEFINE = {"Name", "名前", ColumnType::String};
const ColumnDefine RANDOM_DEFINE = {"Random", "乱数", ColumnType::Float};

// ランダムテーブル
class RandomTable{
    std::vector<ColumnDefine> columnDefines;
    std::vector<QHash<QString, QVariant>> tableData;
public:
    RandomTable(){
        columnDefines.push_back(ID_DEFINE);
        columnDefines.push_back(NAME_DEFINE);
        columnDefines.push_back(RANDOM_DEFINE);
    }

    void addColumnDef(const ColumnDefine& colDef){
        columnDefines.push_back(colDef);
    }
    void addRowData(QHash<QString, QVariant>& rowData){
        tableData.push_back(rowData);
    }

    auto getFieldNames(){
        std::vector<QString> fieldNames;
        for(auto& colDef: columnDefines){
            fieldNames.push_back(colDef.name);
        }
        return fieldNames;
    }
};

int main(int argc, char* argv[]){
    // Qapp定義
    QApplication app(argc, argv);

    // 行定義
    ColumnDefine colDef[] = {
        {"id", "ID", ColumnType::Int},
        {"name", "名前", ColumnType::String},
        {"random", "乱数", ColumnType::Float},
    };

    // 行内容
    QHash<QString, QVariant> data;
    data[""] = "";
    data["a"] = "b";

    QTableWidget table;

    // table.setRowCount(3);
    // table.setColumnCount(3);

    // QStringList headers = {"名称", "分類", "詳細"};
    // table.setHorizontalHeaderLabels(headers);

    // table.setItem(0, 0, new QTableWidgetItem("ベホイミ"));
    // table.setItem(0, 1, new QTableWidgetItem("呪文"));
    // table.setItem(0, 2, new QTableWidgetItem("回復"));

    table.show();

    return app.exec();
}
