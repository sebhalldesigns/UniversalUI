//
// Created by sebam on 11/1/2023.
//

#include <utility>

#include "../include/uWindow.h"

#include "../include/uWindowManager.h"

uWindow::~uWindow() = default;

uWindow::uWindow(double width, double height, std::string title) {

    uWindowManager::CreateNewWindow(this, width, height, std::move(title));

}

void uWindow::SetFullscreen(bool fullscreen) {

}

void uWindow::SetVisibility(uWindowVisibility visibility) {
    uWindowManager::SetWindowVisibility(this, visibility);
}

