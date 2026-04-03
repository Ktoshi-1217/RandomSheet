#include <QApplication>
#include <QTableWidget>

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

// ランダムテーブル
class RandomTable{
    std::vector<ColumnDefine> columnDefines;
    std::vector<QHash<QString, QVariant>> tableData;
public:
    // コンストラクタ
    RandomTable(){
        columnDefines.push_back(ID_DEFINE);
        columnDefines.push_back(NAME_DEFINE);
        columnDefines.push_back(RANDOM_DEFINE);
    }

    // 列追加
    void addColumnDef(const ColumnDefine& colDef){
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

int main(int argc, char* argv[]){
    // Qapp定義
    QApplication app(argc, argv);

    // テーブル定義
    RandomTable randomTable;

    // 行内容
    QHash<QString, QVariant> rowData;
    rowData[ID_DEFINE.key] = 1;
    rowData[NAME_DEFINE.key] = "ベホイミ";
    rowData[RANDOM_DEFINE.key] = 0.876;
    randomTable.addRowData(rowData);

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
        for(auto& cellData: randomTable.getSequenceRowData(i)){
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
