//
// Created by Jackson Campolattaro on 12/26/22.
//

#ifndef N_BODY_STACKVIEW_H
#define N_BODY_STACKVIEW_H

#include <gtkmm/stack.h>
#include <gtkmm/singleselection.h>
#include <gtkmm/listitemfactory.h>
#include <gtkmm/listitem.h>
#include <gtkmm/expression.h>
#include <gtkmm/box.h>
#include <gtkmm/builder.h>

namespace UI {

    class StackView : public Gtk::Stack {
    private:

        Glib::RefPtr<Gtk::SingleSelection> _model{};
        Glib::RefPtr<Gtk::Expression<Gtk::Widget *>> _factory{};

        std::map<Glib::RefPtr<Glib::ObjectBase>, Gtk::Widget *> _map;

    public:

        StackView(Gtk::Stack::BaseObjectType *cobject,
                  const Glib::RefPtr<Gtk::Builder> &builder);

        void set_model(const Glib::RefPtr<Gtk::SingleSelection> &model);

        void set_expression(const Glib::RefPtr<Gtk::Expression<Gtk::Widget *>> &factory);

    private:

        void update();

    };

}

#endif //N_BODY_STACKVIEW_H
