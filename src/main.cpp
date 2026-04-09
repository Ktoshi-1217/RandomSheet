#include <QApplication>
#include <QTableWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "RandomTable.h"
#include "Jsonio.h"

// ロードファイル名
constexpr auto LOAD_JSON_PATH = "../data/data.json";

int main(int argc, char* argv[]){
    // Qapp定義
    QApplication app(argc, argv);

    // ファイル読み込み
    auto data = load_QJson(LOAD_JSON_PATH);

    // Json読み込み
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if(!doc.isObject()){
        throw std::runtime_error("json is not object");
    }

    QJsonObject root = doc.object();
    
    // 列定義
    QJsonArray fields = root["fields"].toArray();
    QVector<ColumnDefine> columnDefines;

    for(const auto& f: fields){
        QJsonObject obj = f.toObject();
        
        ColumnDefine col;
        col.key = obj["key"].toString();
        col.name = obj["name"].toString();
        
        QString typeStr = obj["type"].toString();
        if (typeStr == "int") col.Type = ColumnType::Int;
        else if (typeStr == "float") col.Type = ColumnType::Float;
        else if (typeStr == "select") col.Type = ColumnType::Select;
        else if (typeStr == "string") col.Type = ColumnType::String;
        else col.Type = ColumnType::String;
        
        columnDefines.append(col);
    }
    
    // テーブル定義
    RandomTable randomTable(columnDefines);

    // 行内容
    QJsonArray tableData = root["data"].toArray();
    for(const auto& d: tableData){
        const auto& d_obj = d.toObject().toVariantMap();
        
        QHash<QString, QVariant> rowData;

        for(auto [key, val] : d_obj.asKeyValueRange()){
            rowData[key] = val;
        }

        randomTable.addRowData(rowData);
    }

    // テーブル
    QTableWidget table;

    // テーブルサイズ
    table.setRowCount(randomTable.rowLen());
    table.setColumnCount(randomTable.colLen());

    // タイトル行
    table.setHorizontalHeaderLabels(randomTable.getFieldNames());

    // テーブル内容
    size_t rowLen = randomTable.rowLen();
    for(size_t i=0;i<rowLen;i++){
        size_t j = 0;
        for(const auto& cellData: randomTable.getSequenceRowData(i)){
            auto* item = new QTableWidgetItem();
            item->setData(Qt::DisplayRole, cellData);

            table.setItem(i, j, item);
            j++;
        }

    }

    // ソート可能
    table.setSortingEnabled(true);

    // 表示
    table.show();

    return app.exec();
}
