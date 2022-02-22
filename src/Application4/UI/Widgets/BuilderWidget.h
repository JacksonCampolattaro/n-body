//
// Created by jackcamp on 2/22/22.
//

#ifndef N_BODY_BUILDERWIDGET_H
#define N_BODY_BUILDERWIDGET_H

#include <gtkmm/widget.h>
#include <gtkmm/builder.h>

#include "VectorView.h" // TODO move StringLiteral type

namespace UI {

    template<class W, StringLiteral ResPath>
    class BuilderWidget : public W {
    private:

        Glib::RefPtr<Gtk::Builder> _builder;

    protected:

        template<typename ...Args>
        BuilderWidget(typename W::ObjectBase *cobject, const Glib::RefPtr<Gtk::Builder> &builder, Args ...args);

    public:

        static Glib::RefPtr<BuilderWidget<W, ResPath>> create();

        Gtk::Builder &builder() { return *_builder; };

        virtual void setup() = 0;

    };

}

#endif //N_BODY_BUILDERWIDGET_H
