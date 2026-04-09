#pragma ones

#include <QVariant>
#include <QVector>
#include <QSet>

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

const QVector<QString> RequiredKey = {
    ID_KEY,
    NAME_KEY,
    RANDOM_KEY
};

// ランダムテーブル
class RandomTable{
    QVector<ColumnDefine> columnDefines;    // 列定義
    QSet<QString> columnKeySet;             // 列Key重複チェック
    QVector<QHash<QString, QVariant>> tableData;    // テーブルデータ
    int lastID = 0; // 最後に追加されたID
    QSet<int> usedID; // 使用済みID

    // ID自動採番
    int nextID();

public:
    // 列定義済みコンストラクタ(vector)
    RandomTable(const std::vector<ColumnDefine>&);

    // 列定義済みコンストラクタ
    RandomTable(const QVector<ColumnDefine>&);

    // 列追加
    void addColumnDef(const ColumnDefine&);

    // 行追加
    void addRowData(QHash<QString, QVariant>);

    // 列数
    size_t colLen() const;

    // 行数
    size_t rowLen() const;

    // 列名
    QStringList getFieldNames() const;

    // 列の型の取得
    QHash<QString, ColumnType> getColumnType() const;

    // 行データの順序取得
    QList<QVariant> getSequenceRowData(size_t row) const;

};
