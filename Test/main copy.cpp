#include "nApplication.h"
#include "uApplication.h"

class MyApp : public uApplication {

    void FinishedLaunching() override {

        printf("started launching!!\n");

        printf("finished launching!");
    }
};



int main() {

    return nApplication::Run(new MyApp());
}