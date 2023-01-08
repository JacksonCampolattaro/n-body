//
// Created by jackcamp on 2/22/22.
//

#ifndef N_BODY_BUILDERWIDGET_H
#define N_BODY_BUILDERWIDGET_H

#include <gtkmm/widget.h>
#include <gtkmm/box.h>
#include <gtkmm/builder.h>

#include <spdlog/spdlog.h>

namespace UI {

    template<class OwnerWidget>
    class BuilderWidget : public OwnerWidget {
    private:

        Glib::RefPtr<Gtk::Builder> _builder;

        void setup() {

            // Determine if this is a widget with multiple children (e.g. Box)
            constexpr bool multiChild = requires(OwnerWidget &w, Gtk::Widget &child) {
                w.append(child);
            };

            // Use the appropriate function to add the root widget
            if constexpr(multiChild)
                OwnerWidget::append(getWidget<Gtk::Widget>("root"));
            else
                OwnerWidget::set_child(getWidget<Gtk::Widget>("root"));
        }

    public:

        BuilderWidget(typename OwnerWidget::BaseObjectType *cobject,
                      const Glib::RefPtr<Gtk::Builder> &builder) :
                OwnerWidget(cobject),
                _builder(builder) {

            // We don't need to invoke setup() here,
            // because this widget and all of its children are part of the same file
        }

        BuilderWidget(typename OwnerWidget::BaseObjectType *cobject,
                      const Glib::RefPtr<Gtk::Builder> &builder,
                      const Glib::ustring &path) :
                OwnerWidget(cobject),
                _builder(Gtk::Builder::create_from_resource(path)) {
            setup();
        }

        explicit BuilderWidget(const Glib::ustring &path) :
                OwnerWidget(),
                _builder(Gtk::Builder::create_from_resource(path)) {
            setup();
        }

        [[nodiscard]] const Glib::RefPtr<Gtk::Builder> &builder() const { return _builder; };

        template<class W, typename... Args>
        W &getWidget(const Glib::ustring &name, Args &&... args) {

            // Determine if the widget is a user-created type, based on its constructor
            constexpr bool isDerived = requires(typename W::BaseObjectType *baseObject,
                                                const Glib::RefPtr<Gtk::Builder> &b,
                                                Args &&... a) {
                W(baseObject, b, std::forward<Args>(a)...);
            };

            // Invoke the appropriate builder function, depending on whether the widget is a built-in
            W *widget;
            if constexpr(isDerived)
                widget = Gtk::Builder::get_widget_derived<W>(_builder, name, std::forward<Args>(args)...);
            else
                widget = _builder->get_widget<W>(name);

            // todo: in the future, it might be better to throw an error
            if (!widget) spdlog::error("Failed to retrieve widget with name \"{}\"", name.raw());
            return *widget;
        };

    };

}

#endif //N_BODY_BUILDERWIDGET_H
