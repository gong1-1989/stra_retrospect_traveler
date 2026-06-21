#include "inventorypanel.h"
#include "ui_inventorypanel.h"

InventoryPanel::InventoryPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InventoryPanel)
{
    ui->setupUi(this);
}

InventoryPanel::~InventoryPanel()
{
    delete ui;
}
