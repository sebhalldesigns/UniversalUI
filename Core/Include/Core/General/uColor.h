#ifndef UCOLOR_H
#define UCOLOR_H

struct uColor {
    float r;
    float g;
    float b;
    float a;
};

const uColor BrightRed { 1.0f, 0.0f, 0.0f, 1.0f };
const uColor BrightGreen { 0.0f, 1.0f, 0.0f, 1.0f };
const uColor BrightBlue { 0.0f, 0.0f, 1.0f, 1.0f };

const uColor White { 1.0f, 1.0f, 1.0f, 1.0f };
const uColor Black { 0.0f, 0.0f, 0.0f, 0.0f };

#endif // UCOLOR_H