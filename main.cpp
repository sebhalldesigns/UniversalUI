//
// Created by sebam on 11/1/2023.
//

#include <stdio.h>

#include "Core/UniversalUI.h"

#include <random>

/*double lower_bound = 0;
double upper_bound = 1000;
std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
std::default_random_engine re;

class MyView : public uView {
public:
    void Draw(uCanvas& canvas) override {

        /*for (int i = 0; i < 10000; i++) {
            Dot* point = canvas.NewPoint(unif(re), unif(re));
            point->color = PAINFUL_GREEN;
            point->radius = 10.0;
        }
        
    }

};*/

class MyApp : public uApplication {

    void FinishedLaunching() override {

        printf("started launching!!\n");

        /*uWindow* window1 = new uWindow(1000.0, 750.0, "window 1");
        window1->SetVisibility(uWindowVisibility::VISIBLE);
        window1->rootView.backgroundColor = { 0.0, 0.0, 0.0, 1.0};

        MyView* view = new MyView();
        view->backgroundColor = { 1.0, 1.0, 1.0, 1.0};
        
        uLayoutNode node;
        node.frame = { 10.0, 10.0, 250.0, 250.0 };

        window1->rootView.layoutNode->childNodes.push_back(node);
        view->layoutNode = &window1->rootView.layoutNode->childNodes[0];
        window1->rootView.subviews.push_back(view);
        view->layoutNode->AddConstraint(window1->rootView.layoutNode, uLayoutAnchor::TOP, uLayoutAnchor::TOP, 0.0);
        view->layoutNode->AddConstraint(window1->rootView.layoutNode, uLayoutAnchor::BOTTOM, uLayoutAnchor::BOTTOM, 0.0);
        view->layoutNode->AddConstraint(window1->rootView.layoutNode, uLayoutAnchor::LEFT, uLayoutAnchor::LEFT, 0.0);
        view->layoutNode->AddConstraint(window1->rootView.layoutNode, uLayoutAnchor::WIDTH, uLayoutAnchor::WIDTH, 0.0, 1.0);*/
        
        printf("finished launching!\n");
    }
};



int main() {

    printf("main!\n");
    return UniversalUI(new MyApp());
}