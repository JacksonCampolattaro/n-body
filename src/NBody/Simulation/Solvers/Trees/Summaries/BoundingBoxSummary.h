//
// Created by Jackson Campolattaro on 2/8/23.
//

#ifndef N_BODY_BOUNDINGBOXSUMMARY_H
#define N_BODY_BOUNDINGBOXSUMMARY_H

#include <NBody/Simulation/Solvers/Trees/SummaryType.h>

namespace NBody {

    using namespace Physics;

    template<SummaryType SummaryBase>
    class BoundingBoxSummary : public SummaryBase {
    private:

        BoundingBox _boundingBox{};

    public:

        // We should be able to use the same context type as the base summary,
        // as long as it can provide entity positions
        using typename SummaryBase::Context;
        using SummaryBase::context;

    public:

        BoundingBoxSummary() = default;

        template<typename C>
        void summarize(const std::span<Entity> &entities, const C &context) {
            SummaryBase::summarize(entities, context);

            _boundingBox = {};
            for (const auto &entity: entities) {
                auto entityPosition = context.template get<const Position>(entity);
                _boundingBox.min() = glm::min((glm::vec3) entityPosition, _boundingBox.min());
                _boundingBox.max() = glm::max((glm::vec3) entityPosition, _boundingBox.max());
            }

            // If the base summary provides a center of mass, ensure it's contained within the bounding box
            // todo: not all SummaryBase types have a center of mass!
            SummaryBase::centerOfMass() =
                    glm::min(glm::max((glm::vec3) _boundingBox.min(), SummaryBase::centerOfMass()), _boundingBox.max());
        }

        template<typename NodeList>
        void summarize(const NodeList &childNodes) {
            SummaryBase::summarize(childNodes);

            _boundingBox = {};
            for (const auto &child: childNodes) {
                _boundingBox.min() = glm::min((glm::vec3) child.boundingBox().min(), _boundingBox.min());
                _boundingBox.max() = glm::max((glm::vec3) child.boundingBox().max(), _boundingBox.max());
            }
        }

        [[nodiscard]] const BoundingBox &boundingBox() const { return _boundingBox; }

        BoundingBox &boundingBox() { return _boundingBox; }

    };
}

#endif //N_BODY_BOUNDINGBOXSUMMARY_H
