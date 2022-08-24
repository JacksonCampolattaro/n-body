//
// Created by jackcamp on 2/22/22.
//

#ifndef N_BODY_BUILDERWIDGET_H
#define N_BODY_BUILDERWIDGET_H

#include <gtkmm/widget.h>
#include <gtkmm/box.h>
#include <gtkmm/builder.h>

#include "Application/UI/Widgets/View/VectorView.h" // TODO move StringLiteral type

namespace UI {

    template<StringLiteral ResPath>
    class BuilderWidget : public Gtk::Box {
    private:

        Glib::RefPtr<Gtk::Builder> _builder;

    protected:

        // todo: This widget could potentially be built by another builder
        //template<typename ...Args>
        //BuilderWidget(typename Gtk::Box::ObjectBase *cobject, const Glib::RefPtr<Gtk::Builder> &builder, Args ...args);

    public:

        BuilderWidget() : Gtk::Box(),
                          _builder(Gtk::Builder::create_from_resource(std::string("/ui/") + ResPath.value + ".xml")) {
            append(*_builder->get_widget<Gtk::Widget>("root"));
        }

        const Glib::RefPtr<Gtk::Builder> &builder() const { return _builder; };

        template<class W>
        W &getWidget(const Glib::ustring &name) {

            // Determine if the widget is a user-created type, based on its constructor
            constexpr bool isDerived = requires(typename W::BaseObjectType *baseObject,
                                                const Glib::RefPtr<Gtk::Builder> &b) {
                W(baseObject, b);
            };

            // Invoke the appropriate builder function, depending on whether the widget is a built-in
            if constexpr(isDerived)
                return *Gtk::Builder::get_widget_derived<W>(_builder, name);
            else
                return *_builder->get_widget<W>(name);

        };

    };

}

#endif //N_BODY_BUILDERWIDGET_H
