//
// Created by Jackson Campolattaro on 2/9/23.
//

#ifndef N_BODY_NODE_H
#define N_BODY_NODE_H

#include "NodeBase.h"

namespace NBody {

    // To satisfy the NodeType concept, a class must extend the appropriate CRTP base class & provide a few methods
    template<typename T>
    concept NodeType = std::is_base_of_v<NodeBase<T, typename T::Summary>, T> &&
                       requires(T &t, ContextType<typename T::Summary> &context) {
                           t.isLeaf();
                           t.children();
                           t.boundingBox();
                           t.split(context);
                           t.merge();

                           // I would prefer to enforce this as part of the SummaryType concept,
                           // but that would actually require a cyclic dependency!
                           // Tragically, concepts can't provide existential quantifiers for functions.
                           t.summary().summarize(t.children());
                       };

}

#endif //N_BODY_NODE_H
