#ifndef ACHACK_OVERLAY_H
#define ACHACK_OVERLAY_H

namespace overlay {
    void GetInput();
    void Overlay();
    void ModMenu();
    int Setup();
    void Esp();
    enum menu {
        main,
        aim,
        visual,
    };
}

#endif //ACHACK_OVERLAY_H
