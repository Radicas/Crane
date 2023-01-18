#include <QAbstractTableModel>

class MyTableModel : public QAbstractTableModel {
    struct ModelItem {
        QString name;
        QString material;
        double  thickness;
        double  dk;
        double  df;
    };

public:
    explicit MyTableModel( QObject* parent = nullptr ) : QAbstractTableModel( parent ) {
        m_data = QList< ModelItem >{
            { "1", "GOLD", 1, 20, 90 },
            { "2", "COPPER", 1, 23, 91 },
            { "3", "SIV", 0, 21, 95 },
        };
    }

    // 获取行数
    int rowCount( const QModelIndex& parent = QModelIndex() ) const override {
        return m_data.count();
    }

    // 获取列数
    int columnCount( const QModelIndex& parent = QModelIndex() ) const override {
        return 5;
    }

    // 获取单元格数据
    QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override {
        if ( !index.isValid() )
            return {};
        if ( role == Qt::DisplayRole || role == Qt::EditRole ) {
            const int row = index.row();
            switch ( index.column() ) {
            case 0:
                return m_data.at( row ).name;
            case 1:
                return m_data.at( row ).material;
            case 2:
                return m_data.at( row ).thickness;
            case 3:
                return m_data.at( row ).dk;
            case 4:
                return m_data.at( row ).df;
            }
        }
        return {};
    }

    bool setData( const QModelIndex& index, const QVariant& value, int role ) override {
        if ( index.isValid() && role == Qt::EditRole ) {
            const int row = index.row();
            switch ( index.column() ) {
            case 0:
                m_data[ row ].name = value.toString();
                break;
            case 1:
                m_data[ row ].material = value.toString();
                break;
            case 2:
                m_data[ row ].thickness = value.toDouble();
                break;
            case 3:
                m_data[ row ].dk = value.toDouble();
                break;
            case 4:
                m_data[ row ].df = value.toDouble();
            }
            // 发送信号触发刷新
            emit dataChanged( index, index, QVector< int >() << role );
            return true;
        }
        return false;
    }

    Qt::ItemFlags flags( const QModelIndex& index ) const override {
        if ( !index.isValid() )
            return Qt::NoItemFlags;
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }

private:
    // 数据
    QList< ModelItem > m_data;
};
