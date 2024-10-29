#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QInputDialog>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QPixmap>
#include <QFileDialog>
#include <QColorDialog>
#include <QGroupBox>
#include <QPushButton>
#include <QFile>
#include <QTextStream>

class TabloUygulamasi : public QMainWindow {
    Q_OBJECT

public:
    TabloUygulamasi(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Tablolayıcı");
        setMinimumSize(800, 600);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QVBoxLayout *anaDikeyYerlesim = new QVBoxLayout();

        tabloWidget = new QTableWidget(this);
        tabloWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        stilYukle();
        anaDikeyYerlesim->addWidget(tabloWidget);

        QGroupBox *ayarlarGrubu = new QGroupBox("Ayarlar", this);
        QVBoxLayout *ayarlarYerlesimi = new QVBoxLayout(ayarlarGrubu);

        QPushButton *satirBaslikRengiButton = new QPushButton("Satır Başlık Rengini Ayarla", this);
        QPushButton *sutunBaslikRengiButton = new QPushButton("Sütun Başlık Rengini Ayarla", this);
        QPushButton *yaziRengiButton = new QPushButton("Yazı Rengini Ayarla", this);
        QPushButton *hucresiniRenkAyarlamaButton = new QPushButton("Hücre Rengini Ayarla", this);

        connect(satirBaslikRengiButton, &QPushButton::clicked, this, &TabloUygulamasi::satirBaslikRenginiAyarla);
        connect(sutunBaslikRengiButton, &QPushButton::clicked, this, &TabloUygulamasi::sutunBaslikRenginiAyarla);
        connect(yaziRengiButton, &QPushButton::clicked, this, &TabloUygulamasi::yaziRenginiAyarla);
        connect(hucresiniRenkAyarlamaButton, &QPushButton::clicked, this, &TabloUygulamasi::hucresiniRenkAyarlama);

        ayarlarYerlesimi->addWidget(satirBaslikRengiButton);
        ayarlarYerlesimi->addWidget(sutunBaslikRengiButton);
        ayarlarYerlesimi->addWidget(yaziRengiButton);
        ayarlarYerlesimi->addWidget(hucresiniRenkAyarlamaButton);

        anaDikeyYerlesim->addWidget(ayarlarGrubu);

        tabloWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(tabloWidget, &QTableWidget::customContextMenuRequested, this, &TabloUygulamasi::sagTikMenuGoster);

        connect(tabloWidget, &QTableWidget::cellDoubleClicked, this, &TabloUygulamasi::hucresiniDuzenle);

        QWidget *merkezWidget = new QWidget(this);
        merkezWidget->setLayout(anaDikeyYerlesim);
        setCentralWidget(merkezWidget);
    }

private slots:
    void hucresiniDuzenle(int satir, int sutun) {
        tabloWidget->editItem(tabloWidget->item(satir, sutun));
    }

    void sagTikMenuGoster(const QPoint &pos) {
        QMenu sagTikMenu(tr("Menu"), this);
        QAction ekleSatir("Yeni Satır Ekle", this);
        QAction ekleSutun("Yeni Sütun Ekle", this);
        QAction silSatir("Satırı Sil", this);
        QAction silSutun("Sütunu Sil", this);
        QAction yenidenAdlandirSatir("Satırı Yeniden Adlandır", this);
        QAction yenidenAdlandirSutun("Sütunu Yeniden Adlandır", this);
        QAction hucresiniRenkAyarlama("Hücre Rengini Ayarla", this);
        QAction pngOlarakDisaAktar("PNG Olarak Dışa Aktar", this);

        connect(&ekleSatir, &QAction::triggered, this, [=]() {
            bool ok;
            QString satirBaslik = QInputDialog::getText(this, "Satır Başlığını Girin", "Başlık:", QLineEdit::Normal, "", &ok);
            if (ok && !satirBaslik.isEmpty()) {
                int satirSayisi = tabloWidget->rowCount();
                tabloWidget->insertRow(satirSayisi);
                tabloWidget->setVerticalHeaderItem(satirSayisi, new QTableWidgetItem(satirBaslik));
            } else {
                QMessageBox::warning(this, "Uyarı", "Başlık boş olamaz!");
            }
        });

        connect(&ekleSutun, &QAction::triggered, this, [=]() {
            bool ok;
            QString sutunBaslik = QInputDialog::getText(this, "Sütun Başlığını Girin", "Başlık:", QLineEdit::Normal, "", &ok);
            if (ok && !sutunBaslik.isEmpty()) {
                int sutunSayisi = tabloWidget->columnCount();
                tabloWidget->insertColumn(sutunSayisi);
                tabloWidget->setHorizontalHeaderItem(sutunSayisi, new QTableWidgetItem(sutunBaslik));
            } else {
                QMessageBox::warning(this, "Uyarı", "Başlık boş olamaz!");
            }
        });

        connect(&silSatir, &QAction::triggered, this, [=]() {
            int satir = tabloWidget->currentRow();
            if (satir >= 0) {
                tabloWidget->removeRow(satir);
            }
        });

        connect(&silSutun, &QAction::triggered, this, [=]() {
            int sutun = tabloWidget->currentColumn();
            if (sutun >= 0) {
                tabloWidget->removeColumn(sutun);
            }
        });

        connect(&yenidenAdlandirSatir, &QAction::triggered, this, [=]() {
            int satir = tabloWidget->currentRow();
            if (satir >= 0) {
                bool ok;
                QString yeniBaslik = QInputDialog::getText(this, "Yeni Satır Başlığını Girin", "Başlık:", QLineEdit::Normal, tabloWidget->verticalHeaderItem(satir)->text(), &ok);
                if (ok && !yeniBaslik.isEmpty()) {
                    tabloWidget->setVerticalHeaderItem(satir, new QTableWidgetItem(yeniBaslik));
                }
            }
        });

        connect(&yenidenAdlandirSutun, &QAction::triggered, this, [=]() {
            int sutun = tabloWidget->currentColumn();
            if (sutun >= 0) {
                bool ok;
                QString yeniBaslik = QInputDialog::getText(this, "Yeni Sütun Başlığını Girin", "Başlık:", QLineEdit::Normal, tabloWidget->horizontalHeaderItem(sutun)->text(), &ok);
                if (ok && !yeniBaslik.isEmpty()) {
                    tabloWidget->setHorizontalHeaderItem(sutun, new QTableWidgetItem(yeniBaslik));
                }
            }
        });

        connect(&hucresiniRenkAyarlama, &QAction::triggered, this, [=]() {
            int satir = tabloWidget->currentRow();
            int sutun = tabloWidget->currentColumn();
            if (satir >= 0 && sutun >= 0) {
                QColor renk = QColorDialog::getColor(Qt::white, this, "Hücre Rengini Seçin");
                if (renk.isValid()) {
                    QTableWidgetItem *item = tabloWidget->item(satir, sutun);
                    if (!item) {
                        item = new QTableWidgetItem();
                        tabloWidget->setItem(satir, sutun, item);
                    }
                    item->setBackground(renk);
                }
            }
        });

        connect(&pngOlarakDisaAktar, &QAction::triggered, this, &TabloUygulamasi::pngOlarakDisaAktar);

        sagTikMenu.addAction(&ekleSatir);
        sagTikMenu.addAction(&ekleSutun);
        sagTikMenu.addAction(&silSatir);
        sagTikMenu.addAction(&silSutun);
        sagTikMenu.addAction(&yenidenAdlandirSatir);
        sagTikMenu.addAction(&yenidenAdlandirSutun);
        sagTikMenu.addAction(&hucresiniRenkAyarlama);
        sagTikMenu.addAction(&pngOlarakDisaAktar);
        sagTikMenu.exec(tabloWidget->viewport()->mapToGlobal(pos));
    }

    void pngOlarakDisaAktar() {
        QPixmap pixmap(tabloWidget->size());
        tabloWidget->render(&pixmap);
        QString dosyaAdi = QFileDialog::getSaveFileName(this, "Dışa Aktar", "", "PNG Dosyası (*.png)");
        if (!dosyaAdi.isEmpty()) {
            pixmap.save(dosyaAdi, "PNG");
        }
    }

    void satirBaslikRenginiAyarla() {
        QColor renk = QColorDialog::getColor(Qt::white, this, "Satır Başlık Rengini Seçin");
        if (renk.isValid()) {
            tabloWidget->verticalHeader()->setStyleSheet(QString("QHeaderView::section { background-color: %1; }").arg(renk.name()));
        }
    }

    void sutunBaslikRenginiAyarla() {
        QColor renk = QColorDialog::getColor(Qt::white, this, "Sütun Başlık Rengini Seçin");
        if (renk.isValid()) {
            tabloWidget->horizontalHeader()->setStyleSheet(QString("QHeaderView::section { background-color: %1; }").arg(renk.name()));
        }
    }

    void yaziRenginiAyarla() {
        QColor renk = QColorDialog::getColor(Qt::white, this, "Yazı Rengini Seçin");
        if (renk.isValid()) {
            for (int satir = 0; satir < tabloWidget->rowCount(); ++satir) {
                for (int sutun = 0; sutun < tabloWidget->columnCount(); ++sutun) {
                    QTableWidgetItem *item = tabloWidget->item(satir, sutun);
                    if (!item) {
                        item = new QTableWidgetItem();
                        tabloWidget->setItem(satir, sutun, item);
                    }
                    item->setForeground(renk);
                    item->setTextAlignment(Qt::AlignCenter);
                }
            }
        }
    }

    void hucresiniRenkAyarlama() {
        int satir = tabloWidget->currentRow();
        int sutun = tabloWidget->currentColumn();
        if (satir >= 0 && sutun >= 0) {
            QColor renk = QColorDialog::getColor(Qt::white, this, "Hücre Rengini Seçin");
            if (renk.isValid()) {
                QTableWidgetItem *item = tabloWidget->item(satir, sutun);
                if (!item) {
                    item = new QTableWidgetItem();
                    tabloWidget->setItem(satir, sutun, item);
                }
                item->setBackground(renk);
            }
        }
    }

    void stilYukle() {
        QFile dosya("styles.css");
        if (dosya.open(QFile::ReadOnly)) {
            QTextStream stream(&dosya);
            QString stilSayfasi = stream.readAll();
            setStyleSheet(stilSayfasi);
            dosya.close();
        }
    }

private:
    QTableWidget *tabloWidget;
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    TabloUygulamasi w;
    w.show();
    return a.exec();
}
