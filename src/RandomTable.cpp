#include "RandomTable.h"

// ID自動採番
int RandomTable::nextID(){
    while(usedID.contains(++lastID));
    return lastID;
}

// 列定義済みコンストラクタ(vector)
RandomTable::RandomTable(const std::vector<ColumnDefine>& initColDefs)
    : RandomTable(QVector<ColumnDefine>(initColDefs.begin(), initColDefs.end()))
{}

// 列定義済みコンストラクタ
RandomTable::RandomTable(const QVector<ColumnDefine>& initColDefs){
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
void RandomTable::addColumnDef(const ColumnDefine& colDef){
    // 重複チェック
    if(columnKeySet.contains(colDef.key)){
        throw std::runtime_error("Duplicate column key: " + colDef.key.toStdString());
    }
    columnKeySet.insert(colDef.key);

    // 追加
    columnDefines.append(colDef);
}

// 行追加
void RandomTable::addRowData(QHash<QString, QVariant> rowData){
    
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
size_t RandomTable::colLen() const {
    return columnDefines.size();
}

// 行数
size_t RandomTable::rowLen() const {
    return tableData.size();
}

// 列名
QStringList RandomTable::getFieldNames() const {
    QStringList fieldNames;

    for(auto& colDef: columnDefines){
        fieldNames << colDef.name;
    }
    return fieldNames;
}

// 列の型の取得
QHash<QString, ColumnType> RandomTable::getColumnType() const {
    QHash<QString, ColumnType> columnType;
    for(auto& colDef: columnDefines){
        columnType[colDef.key] = colDef.Type;
    }
    return columnType;
}

// 行データの順序取得
QList<QVariant> RandomTable::getSequenceRowData(size_t row) const {
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
