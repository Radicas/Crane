#ifndef VISUAL_UTILS_H
#define VISUAL_UTILS_H

#include <QWidget>
#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>
namespace Utils {

/**
 * @brief 是窗口居中显示
 * 
 * @param window 
 */
void showCenteredWindow(QWidget* window) {
    // 获取屏幕的中心位置
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - window->width()) / 2;
    int y = (screenGeometry.height() - window->height()) / 2;

    // 将窗口移动到屏幕的中心位置
    window->move(x, y);

    window->show();
}

} // namespace Utils

#endif // VISUAL_UTILS_H