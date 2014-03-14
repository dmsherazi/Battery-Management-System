/********************************************************************************
** Form generated from reading UI file 'power-management-record.ui'
**
** Created: Fri Feb 21 12:29:45 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POWER_2D_MANAGEMENT_2D_RECORD_H
#define UI_POWER_2D_MANAGEMENT_2D_RECORD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PowerManagementRecordDialog
{
public:
    QLabel *title;
    QLabel *errorLabel;
    QSpinBox *recordingFrequencySpinBox;
    QLabel *recordFreqencyLabel;
    QLabel *recordingIntervalLabel;
    QPushButton *stopButton;
    QListView *fileListView;
    QLineEdit *recordFileName;
    QPushButton *closeButton;
    QLabel *diskSpaceAvailable;
    QLabel *spaceAvailableLabel;
    QLabel *recordFilenameLabel;
    QPushButton *deleteButton;
    QCheckBox *deleteCheckBox;
    QPushButton *registerButton;
    QLineEdit *readFileName;
    QLineEdit *localFileName;
    QProgressBar *progressBar;
    QPushButton *cancelDownloadButton;
    QLabel *readFilenameLabel;
    QLabel *localFilenameLabel;
    QPushButton *pauseDownloadButton;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *recordFileButton;
    QPushButton *closeFileButton;
    QPushButton *startButton;
    QPushButton *readFileButton;
    QPushButton *localFileButton;
    QPushButton *downloadButton;

    void setupUi(QDialog *PowerManagementRecordDialog)
    {
        if (PowerManagementRecordDialog->objectName().isEmpty())
            PowerManagementRecordDialog->setObjectName(QString::fromUtf8("PowerManagementRecordDialog"));
        PowerManagementRecordDialog->resize(682, 422);
        title = new QLabel(PowerManagementRecordDialog);
        title->setObjectName(QString::fromUtf8("title"));
        title->setGeometry(QRect(86, 12, 525, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Andale Mono"));
        font.setPointSize(18);
        title->setFont(font);
        errorLabel = new QLabel(PowerManagementRecordDialog);
        errorLabel->setObjectName(QString::fromUtf8("errorLabel"));
        errorLabel->setGeometry(QRect(20, 390, 511, 20));
        recordingFrequencySpinBox = new QSpinBox(PowerManagementRecordDialog);
        recordingFrequencySpinBox->setObjectName(QString::fromUtf8("recordingFrequencySpinBox"));
        recordingFrequencySpinBox->setGeometry(QRect(37, 356, 61, 23));
        recordFreqencyLabel = new QLabel(PowerManagementRecordDialog);
        recordFreqencyLabel->setObjectName(QString::fromUtf8("recordFreqencyLabel"));
        recordFreqencyLabel->setGeometry(QRect(24, 330, 147, 20));
        recordingIntervalLabel = new QLabel(PowerManagementRecordDialog);
        recordingIntervalLabel->setObjectName(QString::fromUtf8("recordingIntervalLabel"));
        recordingIntervalLabel->setGeometry(QRect(107, 360, 64, 17));
        stopButton = new QPushButton(PowerManagementRecordDialog);
        stopButton->setObjectName(QString::fromUtf8("stopButton"));
        stopButton->setGeometry(QRect(138, 150, 93, 27));
        fileListView = new QListView(PowerManagementRecordDialog);
        fileListView->setObjectName(QString::fromUtf8("fileListView"));
        fileListView->setGeometry(QRect(384, 61, 256, 286));
        recordFileName = new QLineEdit(PowerManagementRecordDialog);
        recordFileName->setObjectName(QString::fromUtf8("recordFileName"));
        recordFileName->setGeometry(QRect(139, 66, 164, 23));
        closeButton = new QPushButton(PowerManagementRecordDialog);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setGeometry(QRect(548, 355, 91, 27));
        diskSpaceAvailable = new QLabel(PowerManagementRecordDialog);
        diskSpaceAvailable->setObjectName(QString::fromUtf8("diskSpaceAvailable"));
        diskSpaceAvailable->setGeometry(QRect(306, 115, 70, 20));
        spaceAvailableLabel = new QLabel(PowerManagementRecordDialog);
        spaceAvailableLabel->setObjectName(QString::fromUtf8("spaceAvailableLabel"));
        spaceAvailableLabel->setGeometry(QRect(158, 115, 143, 20));
        recordFilenameLabel = new QLabel(PowerManagementRecordDialog);
        recordFilenameLabel->setObjectName(QString::fromUtf8("recordFilenameLabel"));
        recordFilenameLabel->setGeometry(QRect(158, 90, 125, 20));
        deleteButton = new QPushButton(PowerManagementRecordDialog);
        deleteButton->setObjectName(QString::fromUtf8("deleteButton"));
        deleteButton->setGeometry(QRect(339, 64, 34, 27));
        deleteCheckBox = new QCheckBox(PowerManagementRecordDialog);
        deleteCheckBox->setObjectName(QString::fromUtf8("deleteCheckBox"));
        deleteCheckBox->setGeometry(QRect(311, 67, 22, 23));
        registerButton = new QPushButton(PowerManagementRecordDialog);
        registerButton->setObjectName(QString::fromUtf8("registerButton"));
        registerButton->setEnabled(true);
        registerButton->setGeometry(QRect(386, 356, 91, 27));
        readFileName = new QLineEdit(PowerManagementRecordDialog);
        readFileName->setObjectName(QString::fromUtf8("readFileName"));
        readFileName->setGeometry(QRect(138, 195, 164, 23));
        localFileName = new QLineEdit(PowerManagementRecordDialog);
        localFileName->setObjectName(QString::fromUtf8("localFileName"));
        localFileName->setGeometry(QRect(138, 240, 164, 23));
        progressBar = new QProgressBar(PowerManagementRecordDialog);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setEnabled(false);
        progressBar->setGeometry(QRect(138, 285, 103, 23));
        progressBar->setValue(0);
        progressBar->setInvertedAppearance(false);
        cancelDownloadButton = new QPushButton(PowerManagementRecordDialog);
        cancelDownloadButton->setObjectName(QString::fromUtf8("cancelDownloadButton"));
        cancelDownloadButton->setGeometry(QRect(316, 285, 57, 27));
        readFilenameLabel = new QLabel(PowerManagementRecordDialog);
        readFilenameLabel->setObjectName(QString::fromUtf8("readFilenameLabel"));
        readFilenameLabel->setGeometry(QRect(160, 220, 125, 20));
        localFilenameLabel = new QLabel(PowerManagementRecordDialog);
        localFilenameLabel->setObjectName(QString::fromUtf8("localFilenameLabel"));
        localFilenameLabel->setGeometry(QRect(160, 265, 125, 20));
        pauseDownloadButton = new QPushButton(PowerManagementRecordDialog);
        pauseDownloadButton->setObjectName(QString::fromUtf8("pauseDownloadButton"));
        pauseDownloadButton->setGeometry(QRect(250, 285, 57, 27));
        layoutWidget = new QWidget(PowerManagementRecordDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(16, 50, 99, 271));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        recordFileButton = new QPushButton(layoutWidget);
        recordFileButton->setObjectName(QString::fromUtf8("recordFileButton"));

        verticalLayout->addWidget(recordFileButton);

        closeFileButton = new QPushButton(layoutWidget);
        closeFileButton->setObjectName(QString::fromUtf8("closeFileButton"));

        verticalLayout->addWidget(closeFileButton);

        startButton = new QPushButton(layoutWidget);
        startButton->setObjectName(QString::fromUtf8("startButton"));

        verticalLayout->addWidget(startButton);

        readFileButton = new QPushButton(layoutWidget);
        readFileButton->setObjectName(QString::fromUtf8("readFileButton"));
        readFileButton->setEnabled(false);

        verticalLayout->addWidget(readFileButton);

        localFileButton = new QPushButton(layoutWidget);
        localFileButton->setObjectName(QString::fromUtf8("localFileButton"));
        localFileButton->setEnabled(false);

        verticalLayout->addWidget(localFileButton);

        downloadButton = new QPushButton(layoutWidget);
        downloadButton->setObjectName(QString::fromUtf8("downloadButton"));
        downloadButton->setEnabled(false);

        verticalLayout->addWidget(downloadButton);


        retranslateUi(PowerManagementRecordDialog);

        QMetaObject::connectSlotsByName(PowerManagementRecordDialog);
    } // setupUi

    void retranslateUi(QDialog *PowerManagementRecordDialog)
    {
        PowerManagementRecordDialog->setWindowTitle(QApplication::translate("PowerManagementRecordDialog", "Solar Power BMS Recording", 0, QApplication::UnicodeUTF8));
        title->setText(QApplication::translate("PowerManagementRecordDialog", "Solar Power Remote Recording Control", 0, QApplication::UnicodeUTF8));
        errorLabel->setText(QString());
        recordFreqencyLabel->setText(QApplication::translate("PowerManagementRecordDialog", "Recording Frequency", 0, QApplication::UnicodeUTF8));
        recordingIntervalLabel->setText(QApplication::translate("PowerManagementRecordDialog", "0.1 sec", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        stopButton->setToolTip(QApplication::translate("PowerManagementRecordDialog", "Pause recording but do not close the file.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        stopButton->setText(QApplication::translate("PowerManagementRecordDialog", "Stop Record", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fileListView->setToolTip(QApplication::translate("PowerManagementRecordDialog", "Click on an item to place it in the text boxes left.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        recordFileName->setToolTip(QApplication::translate("PowerManagementRecordDialog", "Name of remote file to open (or create if it doesn't exists)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        closeButton->setText(QApplication::translate("PowerManagementRecordDialog", "Close", 0, QApplication::UnicodeUTF8));
        diskSpaceAvailable->setText(QString());
        spaceAvailableLabel->setText(QApplication::translate("PowerManagementRecordDialog", "Space Available (MB)", 0, QApplication::UnicodeUTF8));
        recordFilenameLabel->setText(QApplication::translate("PowerManagementRecordDialog", "Record File Name", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        deleteButton->setToolTip(QApplication::translate("PowerManagementRecordDialog", "Delete the file selected. Select the check box first.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        deleteButton->setText(QApplication::translate("PowerManagementRecordDialog", "Del", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        deleteCheckBox->setToolTip(QApplication::translate("PowerManagementRecordDialog", "Delete protection. Set to enable file deletion.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        deleteCheckBox->setText(QString());
#ifndef QT_NO_TOOLTIP
        registerButton->setToolTip(QApplication::translate("PowerManagementRecordDialog", "Attempt to re-register the SD card if there are problems accessing it.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        registerButton->setText(QApplication::translate("PowerManagementRecordDialog", "Remount", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        readFileName->setToolTip(QApplication::translate("PowerManagementRecordDialog", "Name of existing remote file to be read.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        localFileName->setToolTip(QApplication::translate("PowerManagementRecordDialog", "Name of local file to receive downloaded records.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        cancelDownloadButton->setToolTip(QApplication::translate("PowerManagementRecordDialog", "Abort download", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        cancelDownloadButton->setText(QApplication::translate("PowerManagementRecordDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        readFilenameLabel->setText(QApplication::translate("PowerManagementRecordDialog", "Remote File Name", 0, QApplication::UnicodeUTF8));
        localFilenameLabel->setText(QApplication::translate("PowerManagementRecordDialog", "Local File Name", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        pauseDownloadButton->setToolTip(QApplication::translate("PowerManagementRecordDialog", "Pause download but maintain position in file.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        pauseDownloadButton->setText(QApplication::translate("PowerManagementRecordDialog", "Pause", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        recordFileButton->setToolTip(QApplication::translate("PowerManagementRecordDialog", "Open the specified remote file for recording.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        recordFileButton->setText(QApplication::translate("PowerManagementRecordDialog", "Record File", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        closeFileButton->setToolTip(QApplication::translate("PowerManagementRecordDialog", "Close the remote recording file. Recording is also stopped.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        closeFileButton->setText(QApplication::translate("PowerManagementRecordDialog", "Close File", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        startButton->setToolTip(QApplication::translate("PowerManagementRecordDialog", "Start recording on an open file (from end of file if it exists).", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        startButton->setText(QApplication::translate("PowerManagementRecordDialog", "Start Record", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        readFileButton->setToolTip(QApplication::translate("PowerManagementRecordDialog", "Open a remote file for reading.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        readFileButton->setText(QApplication::translate("PowerManagementRecordDialog", " Remote File", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        localFileButton->setToolTip(QApplication::translate("PowerManagementRecordDialog", "Open a local file for storage of downloaded records.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        localFileButton->setText(QApplication::translate("PowerManagementRecordDialog", " Local File", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        downloadButton->setToolTip(QApplication::translate("PowerManagementRecordDialog", "Start download of remote records.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        downloadButton->setText(QApplication::translate("PowerManagementRecordDialog", "Download", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PowerManagementRecordDialog: public Ui_PowerManagementRecordDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POWER_2D_MANAGEMENT_2D_RECORD_H
