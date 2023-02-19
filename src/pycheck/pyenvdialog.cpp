#include "pyenvdialog.h"
#include "ui_pyenvdialog.h"
#include "utils/utils.hpp"
#ifdef _WIN32
#include <windows.h>
#endif

PyEnvDialog::PyEnvDialog( QWidget* parent ) : QDialog( parent ), ui( new Ui::PyEnvDlg ) {
    ui->setupUi( this );
    setUp();
}
PyEnvDialog::~PyEnvDialog() {}
void PyEnvDialog::setUp() {
    ui->installBtn->setEnabled( false );
    ui->textEdit->setReadOnly( true );
    connect( ui->checkBtn, SIGNAL( clicked() ), this, SLOT( checked() ) );
    connect( ui->clearBtn, SIGNAL( clicked() ), this, SLOT( clearText() ) );
}
void PyEnvDialog::checked() {
    const char* PY_CHECK    = "python --version";
    const char* PY3_CHECK   = "python3 --version";
    const char* PYENV_CHECK = "pyenv --version";
    const char* IO_MODE     = "r";
    std::string text;
#ifdef _WIN32
    AllocConsole();                             // 为进程分配新的控制台
    ShowWindow( GetConsoleWindow(), SW_HIDE );  // 隐藏自己创建的控制台
#endif
    try {
        text.append( SUtil::execCmd( PY3_CHECK, IO_MODE ) );
        text.append( SUtil::execCmd( PYENV_CHECK, IO_MODE ) );
    }
    catch ( const std::exception& e ) {
        std::cerr << e.what() << '\n';
    }
    ui->textEdit->setText( QString::fromStdString( text ) );
}

void PyEnvDialog::clearText() {
    ui->textEdit->clear();
}