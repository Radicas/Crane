#include "cranegptdialog.h"
#include "messagehandler.h"

#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

CraneGPTDialog::CraneGPTDialog(QWidget* parent)
    : QDialog(parent)
    , m_answer_edit(new QTextEdit())
    , m_commit_edit(new QLineEdit())
    , m_commit_btn(new QPushButton(tr("commit")))
    , m_message_handler(new MessageHandler()) {
    setUp();
}

CraneGPTDialog::~CraneGPTDialog() = default;

void CraneGPTDialog::setUp() {
    initUi();
    initConnections();
}

void CraneGPTDialog::initUi() {
    auto* global_layout = new QVBoxLayout();

    m_answer_edit->setFixedSize(500, 300);
    m_answer_edit->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    global_layout->addWidget(m_answer_edit.get());
    global_layout->addWidget(m_commit_edit.get());
    global_layout->addWidget(m_commit_btn.get());
    setLayout(global_layout);
}

void CraneGPTDialog::initConnections() {
    connect(m_commit_btn.get(), &QPushButton::clicked, this, &CraneGPTDialog::commitClicked);
}

void CraneGPTDialog::commitClicked() {
    m_answer_edit->append(m_commit_edit->text());
}
