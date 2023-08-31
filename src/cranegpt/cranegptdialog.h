/**
 * @file cranegptdialog.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-04-11
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef CRANE_CRANEGPTDIALOG_H
#define CRANE_CRANEGPTDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QTextEdit;
QT_END_NAMESPACE
class MessageHandler;
class CraneGPTDialog : public QDialog {
public:
    explicit CraneGPTDialog(QWidget* parent = nullptr);

    ~CraneGPTDialog() override;

private:
    void setUp();

    void initUi();

    void initConnections();

private slots:
    void commitClicked();

private:
    std::unique_ptr<QTextEdit> m_answer_edit;
    std::unique_ptr<QLineEdit> m_commit_edit;
    std::unique_ptr<QPushButton> m_commit_btn;
    std::shared_ptr<MessageHandler> m_message_handler;
};

#endif // CRANE_CRANEGPTDIALOG_H
