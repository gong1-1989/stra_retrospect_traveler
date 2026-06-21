#ifndef INVENTORYPANEL_H
#define INVENTORYPANEL_H

#include <QWidget>

namespace Ui {
class InventoryPanel;
}

class InventoryPanel : public QWidget
{
    Q_OBJECT

public:
    explicit InventoryPanel(QWidget *parent = nullptr);
    ~InventoryPanel();

private:
    Ui::InventoryPanel *ui;
};

#endif // INVENTORYPANEL_H
