#include "pyenvdialog.h"
#include "ui_pyenvdialog.h"

#include <stdlib.h>
#include <stdio.h>
// #include <cstdlib>
#include <iostream>
#include <QTextCodec>
#include <windows.h>

PyEnvDialog::PyEnvDialog(QWidget *parent) : QDialog(parent), ui(new Ui::PyEnvDlg)
{
    ui->setupUi(this);
    setUp();
}

PyEnvDialog::~PyEnvDialog()
{
}

void PyEnvDialog::setUp()
{
    std::cout << "setup" << std::endl;
    ui->installBtn->setEnabled(false);
    ui->textEdit->setReadOnly(true);
    connect(ui->checkBtn, QPushButton::clicked, this, PyEnvDialog::checked);
    connect(ui->installBtn, QPushButton::clicked, this, []() {});
}

void PyEnvDialog::checked()
{
    const int LINE_LENGTH = 300;
    std::cout << "/* message */" << std::endl;
    AllocConsole();                          // 为进程分配新的控制台
    ShowWindow(GetConsoleWindow(), SW_HIDE); // 隐藏自己创建的控制台
    // char *valPy = std::getenv("python");   // stdlib
    // char *valPyEnv = std::getenv("pyenv"); // stdlib
    // auto f = popen("echo test", "w");      // stdio
    // ui->installBtn->setEnabled(valPy && valPyEnv);
    FILE *f = popen("ls", "w");
    char line[LINE_LENGTH];
    QString str;
    if (NULL != f)
    {
        while (fgets(line, sizeof(line), f) != NULL)
        {
            printf("%s", line);
        }
    }
    // QTextCodec *gbk = QTextCodec::codecForName("GBK");
    // QByteArray arr = gbk->fromUnicode(str);
    ui->textEdit->setText(QString(str));
    pclose(f);
}