//
// Created by Jackson Campolattaro on 2/7/23.
//

#ifndef N_BODY_NODEBASE_H
#define N_BODY_NODEBASE_H

#include <span>

#include <NBody/Simulation/Simulation.h>
#include <NBody/Physics/Summaries/SummaryType.h>

namespace NBody {

    using namespace Physics;

    template<class NodeImplementation, SummaryType S>
    class NodeBase {
    public:

        using Summary = S;

    private:

        std::span<Entity> _contents;

        Summary _summary;

    public:

        NodeBase() : _contents() {}

        NodeBase(std::span<Entity> contents) : _contents(contents) {}

        // todo: Nodes shouldn't be copy-constructible. This has caused me too many problems
        //NodeBase(const NodeBase<NodeImplementation, S> &_) = delete;

        inline Summary &summary() { return _summary; }

        inline const Summary &summary() const { return _summary; }

        template<typename Context>
        void summarize(const Context &context) {
            if (isLeaf()) _summary.summarize(implementation().contents(), context);
            else _summary.summarize(implementation().children());

            // sanity check: a node should always enclose all of its children
            for (auto &entity: contents()) {
                auto bbox = implementation().boundingBox();
                auto p = context.template get<const Position>(entity);
                auto e = ENTT_ID_TYPE(entity);

                assert(implementation().boundingBox().contains(context.template get<const Position>(entity)));
            }
        }

        template<typename... Context>
        void refine(std::size_t maxDepth,
                    std::function<bool(const NodeImplementation &)> splitCriterion,
                    Context &&...context) {

            if (_contents.empty()) return;

            if (maxDepth > 0 && splitCriterion((const NodeImplementation &) *this)) {
                implementation().split(std::forward<Context>(context)...);

                for (auto &child: implementation().children())
                    child.refine(maxDepth - 1, splitCriterion, std::forward<Context>(context)...);

            } else {
                implementation().merge();
            }

            summarize(context...);
        }

        [[nodiscard]] const std::span<Entity> &contents() const { return _contents; };

        [[nodiscard]] std::span<Entity> &contents() { return _contents; };

        [[nodiscard]] std::size_t depth() const {
            if (isLeaf()) return 0;
            std::size_t maxDepth = 0;
            for (const NodeImplementation &child: implementation().children())
                maxDepth = std::max(maxDepth, child.depth());
            return maxDepth + 1;
        }

        [[nodiscard]] bool isLeaf() const { return implementation().children().empty(); }


        [[nodiscard]] std::string toString(const std::string &offset = "") const {
            if (contents().empty()) return "";
            auto &c = (std::span<ENTT_ID_TYPE> &) contents();
            std::stringstream ss;
            ss << offset << fmt::format("[{}] ", fmt::join(c, ", ")) << summary() << "\n";
            for (auto &child: implementation().children())
                ss << child.toString(offset + "\t");
            return ss.str();
        }

        friend std::ostream &operator<<(std::ostream &out, const NodeBase<NodeImplementation, S> &n) {
            return out << n.toString();
        }

    private:

        const NodeImplementation &implementation() const { return static_cast<const NodeImplementation &>(*this); }

        NodeImplementation &implementation() { return static_cast<NodeImplementation &>(*this); }

    };
}

#endif //N_BODY_NODEBASE_H
