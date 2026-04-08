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
constexpr auto ID_KEY = "ID";
constexpr auto NAME_KEY = "Name";
constexpr auto RANDOM_KEY = "Random";

const ColumnDefine ID_DEFINE = {ID_KEY, "ID", ColumnType::Int};
const ColumnDefine NAME_DEFINE = {NAME_KEY, "名前", ColumnType::String};
const ColumnDefine RANDOM_DEFINE = {RANDOM_KEY, "乱数", ColumnType::Float};

const QVector<QString> RequiredKey = {ID_KEY, "Name", "Random"};

// ランダムテーブル
class RandomTable{
    QVector<ColumnDefine> columnDefines;    // 列定義
    QSet<QString> columnKeySet;             // 列Key重複チェック
    QVector<QHash<QString, QVariant>> tableData;    // テーブルデータ
    int lastID = 0; // 最後に追加されたID
    QSet<int> usedID; // 使用済みID

    // ID自動採番
    int nextID(){
        while(usedID.contains(++lastID));
        return lastID;
    }

public:
    // コンストラクタ
    RandomTable(){
        columnDefines.append(ID_DEFINE);
        columnDefines.append(NAME_DEFINE);
        columnDefines.append(RANDOM_DEFINE);
    }

    // 列定義済みコンストラクタ(vector)
    RandomTable(const std::vector<ColumnDefine>& initColDefs)
        : RandomTable(QVector<ColumnDefine>(initColDefs.begin(), initColDefs.end()))
    {}

    // 列定義済みコンストラクタ
    RandomTable(const QVector<ColumnDefine>& initColDefs){
        // 1列ずつ
        for(const auto& colDef: initColDefs){
            // 重複確認
            if(columnKeySet.contains(colDef.key)){
                throw std::runtime_error("Duplicate column key: " + colDef.key.toStdString());
            }
            columnKeySet.insert(colDef.key);

            // 追加
            columnDefines.append(colDef);
        }

        // 必須キーがない場合エラー
        for (const auto& req : RequiredKey) {
            if (!columnKeySet.contains(req)) {
                throw std::runtime_error(
                    ("missing required key: " + req).toStdString()
                );
            }
        }
    }

    // 列追加
    void addColumnDef(const ColumnDefine& colDef){
        // 重複チェック
        if(columnKeySet.contains(colDef.key)){
            throw std::runtime_error("Duplicate column key: " + colDef.key.toStdString());
        }
        columnKeySet.insert(colDef.key);

        // 追加
        columnDefines.append(colDef);
    }

    // 行追加
    void addRowData(QHash<QString, QVariant> rowData){
        
        /* 必須列チェック */
        // Nameチェック
        if(!rowData.contains(NAME_KEY)){
            throw std::runtime_error(
                std::string("required key Not Found: ") + NAME_KEY
            );
        }        

        // IDチェック
        if(rowData.contains(ID_KEY)){
            if(usedID.contains(rowData[ID_KEY].toInt())){
                throw std::runtime_error(
                    "Duplicate ID: " + rowData[ID_KEY].toString().toStdString()
                );
            }
        }else{
            rowData[ID_KEY] = nextID();
        }
        lastID = rowData[ID_KEY].toInt();
        usedID.insert(lastID);

        // Randomチェック
        if(!rowData.contains(RANDOM_KEY)){
            rowData[RANDOM_KEY] = 0.0;
        }

        /* 型チェック */
        for(const auto& colDef: columnDefines){
            if(!rowData.contains(colDef.key)){
                continue;
            }

            bool isOk = false;
            const auto& checkCol = rowData[colDef.key];
            switch(colDef.Type){
                case ColumnType::Int:
                    checkCol.toInt(&isOk);
                    break;
                case ColumnType::Float:
                    checkCol.toDouble(&isOk);
                    break;
                case ColumnType::Select:
                    checkCol.toInt(&isOk);
                    break;
                case ColumnType::String:
                    isOk = true;
                    break;
            }

            if(!isOk){
                throw std::runtime_error(
                    "Column type is different: " + colDef.key.toStdString()
                );
            }
        }

        tableData.append(rowData);
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
        QList<QVariant> sequenceRowData;
        
        if(rowLen() <= row){
            throw std::out_of_range("index out of range");
        }

        const auto& rowData = tableData[row];
        for(auto& colDef: columnDefines){
            if(rowData.contains(colDef.key)){
                sequenceRowData.append(rowData[colDef.key]);
            }else{
                sequenceRowData.append(QVariant());
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
