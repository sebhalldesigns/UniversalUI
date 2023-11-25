//
// Created by sebam on 11/1/2023.
//

#include <utility>

#include "../include/uWindow.h"

#include "../include/uWindowManager.h"

uWindow::~uWindow() = default;

uWindow::uWindow(double initWidth, double initHeight, std::string title) {
    width = initWidth;
    height = initHeight;
    uWindowManager::CreateNewWindow(this, width, height, std::move(title));


    rootView.layoutNode = &layoutTree.rootNode;
    layoutTree.rootNode.frame.x = 0.0;
    layoutTree.rootNode.frame.y = 0.0;
    layoutTree.rootNode.frame.width = width;
    layoutTree.rootNode.frame.height = height;

    rootView.backgroundColor = { 1.0f, 0.5f, 0.5f, 1.0f};
}

void uWindow::SetFullscreen(bool fullscreen) {

}

void uWindow::SetVisibility(uWindowVisibility visibility) {
    uWindowManager::SetWindowVisibility(this, visibility);
}

