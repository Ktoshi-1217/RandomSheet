#include <QApplication>
#include <QTableWidget>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// ロードファイル名
constexpr auto LOAD_JSON_PATH = "../data.json";

// 列の型定義
enum class ColumnType{
    Int,    // 整数型
    Float,  // 小数型
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

const std::set<QString> RequiredKey = {"ID", "Name", "Random"};

// ランダムテーブル
class RandomTable{
    std::vector<ColumnDefine> columnDefines;
    std::set<QString> columnKeySet;
    std::vector<QHash<QString, QVariant>> tableData;

public:
    // コンストラクタ
    RandomTable(){
        columnDefines.push_back(ID_DEFINE);
        columnDefines.push_back(NAME_DEFINE);
        columnDefines.push_back(RANDOM_DEFINE);
    }

    // 列定義済みコンストラクタ
    RandomTable(const std::vector<ColumnDefine>& initColDefs){
        // 必須キーの数
        size_t isRequiredKey = 0;

        for(const auto& colDef: initColDefs){
            if(!columnKeySet.insert(colDef.key).second){
                throw std::runtime_error("Duplicate column key: " + colDef.key.toStdString());
            }

            if(RequiredKey.count(colDef.key)){
                isRequiredKey++;
            }

            columnDefines.push_back(colDef);
        }

        if(isRequiredKey != RequiredKey.size()){
            throw std::runtime_error("Not required key");
        }
    }

    // 列追加
    void addColumnDef(const ColumnDefine& colDef){
        if(!columnKeySet.count(colDef.key)){
            throw std::runtime_error("Key is defined");
        }

        columnDefines.push_back(colDef);
    }
    // 行追加
    void addRowData(const QHash<QString, QVariant>& rowData){
        tableData.push_back(rowData);
    }

    // 列数
    size_t colLen() const {
        return columnDefines.size();
    }

    // 行数
    size_t rowLen() const {
        return tableData.size();
    }

    // 列名
    auto getFieldNames() const {
        QStringList fieldNames;

        for(auto& colDef: columnDefines){
            fieldNames << colDef.name;
        }
        return fieldNames;
    }

    // 列の型の取得
    auto getColumnType() const {
        QHash<QString, ColumnType> columnType;
        for(auto& colDef: columnDefines){
            columnType[colDef.key] = colDef.Type;
        }
        return columnType;
    }

    // 行データの順序取得
    auto getSequenceRowData(size_t row) const {
        std::vector<QVariant> sequenceRowData;
        
        if(rowLen() <= row){
            throw std::out_of_range("index out of range");
        }

        const auto& rowData = tableData[row];
        for(auto& colDef: columnDefines){
            if(rowData.contains(colDef.key)){
                sequenceRowData.push_back(rowData[colDef.key]);
            }else{
                sequenceRowData.push_back(QVariant());
            }
        }

        return sequenceRowData;
    }


};

// ファイル全読み込み
auto load_all_Qfile(const char* file_name){
    QFile file(file_name);

    if(!file.open(QIODevice::ReadOnly)){
        throw std::runtime_error("Failed to open json file");
    }

    QByteArray data = file.readAll();
    file.close();

    return data;
}


int main(int argc, char* argv[]){
    // Qapp定義
    QApplication app(argc, argv);

    // ファイル読み込み
    auto data = load_all_Qfile(LOAD_JSON_PATH);

    // Json読み込み
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if(!doc.isObject()){
        throw std::runtime_error("json is not object");
    }

    QJsonObject root = doc.object();
    
    // 列定義
    QJsonArray fields = root["fields"].toArray();
    std::vector<ColumnDefine> columnDefines;

    for(const auto& f: fields){
        QJsonObject obj = f.toObject();
        
        ColumnDefine col;
        col.key = obj["key"].toString();
        col.name = obj["name"].toString();
        
        QString typeStr = obj["type"].toString();
        if (typeStr == "int") col.Type = ColumnType::Int;
        else if (typeStr == "float") col.Type = ColumnType::Float;
        else if (typeStr == "string") col.Type = ColumnType::String;
        else col.Type = ColumnType::String;
        
        columnDefines.push_back(col);
    }
    
    // テーブル定義
    RandomTable randomTable(columnDefines);

    // 行内容
    QJsonArray tableData = root["data"].toArray();
    for(const auto& d: tableData){
        QHash<QString, QVariant> rowData;

        QJsonObject d_obj = d.toObject();
        rowData["ID"] = d_obj["ID"].toInt();
        rowData["Name"] = d_obj["Name"].toString();
        rowData["Random"] = d_obj["Random"].toDouble();

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
