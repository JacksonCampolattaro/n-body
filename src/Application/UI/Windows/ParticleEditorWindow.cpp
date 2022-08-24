//
// Created by Jackson Campolattaro on 8/24/22.
//

#include "ParticleEditorWindow.h"

UI::ParticleEditorWindow::ParticleEditorWindow() {

    set_child(_particleEntry);

    slot_open = [&](auto &particle) {
        _particleEntry.bind(particle);
        set_title("Particle #" + std::to_string((ENTT_ID_TYPE) particle->entity()));
        show();
        present();
    };

    slot_close = [&]() { hide(); };

    signal_close_request().connect(
            [&] {
                hide();
                return true;
            },
            false
    );
}
