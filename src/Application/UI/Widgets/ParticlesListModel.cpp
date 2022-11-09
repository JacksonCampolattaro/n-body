//
// Created by Jackson Campolattaro on 6/23/22.
//

#include "ParticlesListModel.h"

#include <span>

void UI::ParticlesListModel::add_particles(const std::vector<NBody::Entity> &particles) {

    // Break the particles list into groups of adjacent particles
    auto groupBegin = particles.begin();
    auto groupEnd = groupBegin + 1;
    while (groupBegin != particles.end()) {

        // Expand the group until the next particle after the end is not adjacent
        while (groupEnd != particles.end() && (ENTT_ID_TYPE) *groupEnd - (ENTT_ID_TYPE) *(groupEnd - 1) == 1)
            groupEnd++;

        // Determine where this group belongs in the live entity list
        auto iterator = std::upper_bound(_liveEntities.begin(), _liveEntities.end(), *groupBegin);
        auto index = iterator - _liveEntities.begin();
        auto count = groupEnd - groupBegin;
        _liveEntities.insert(iterator, groupBegin, groupEnd);
        items_changed(index, 0, count);

        // The next group will start after the end of this one
        groupBegin = groupEnd;
        groupEnd = groupBegin + 1;
    }

}
