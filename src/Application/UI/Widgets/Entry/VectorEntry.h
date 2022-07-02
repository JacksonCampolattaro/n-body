//
// Created by jackcamp on 2/21/22.
//

#ifndef N_BODY_VECTORENTRY_H
#define N_BODY_VECTORENTRY_H

#include <gtkmm/button.h>
#include <gtkmm/popover.h>
#include <gtkmm/grid.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/builder.h>

#include <spdlog/spdlog.h>

#include "Application/UI/Widgets/Entry/FloatEntry.h"
#include "Application/UI/Widgets/View/VectorView.h"

namespace UI {

    class VectorEntry : public Gtk::Grid {
    private:

        Gtk::Label _xLabel{"X"}, _yLabel{"Y"}, _zLabel{"Z"};
        FloatEntry _x, _y, _z;

    public: // Signals

        sigc::signal<void(float, float, float)> signal_changed;

    public: // Slots

        sigc::slot<void(float, float, float)> slot_changed;

    public:

        VectorEntry() {

            set_row_spacing(4);
            set_column_spacing(8);

            attach(_xLabel, 0, 0);
            attach(_x, 1, 0);
            attach(_yLabel, 0, 1);
            attach(_y, 1, 1);
            attach(_zLabel, 0, 2);
            attach(_z, 1, 2);

            slot_changed = sigc::mem_fun<void, VectorEntry, VectorEntry, float, float, float>(*this,
                                                                                              &VectorEntry::setValue);

            _x.signal_value_changed().connect(sigc::mem_fun(*this, &VectorEntry::on_edited));
            _y.signal_value_changed().connect(sigc::mem_fun(*this, &VectorEntry::on_edited));
            _z.signal_value_changed().connect(sigc::mem_fun(*this, &VectorEntry::on_edited));
        }

        void setValue(float x, float y, float z) {
            _x.set_value(x);
            _y.set_value(y);
            _z.set_value(z);
        }

        void setValue(const glm::vec3 &value) {
            setValue(value.x, value.y, value.z);
        }

    private:

        void on_edited() {
            spdlog::trace("Camera position manually edited");
            signal_changed.emit((float) _x.get_value(), (float) _y.get_value(), (float) _z.get_value());
        }

    };

    template<StringLiteral open, StringLiteral close>
    class CompactVectorEntry : public Gtk::Button {
    private:

        VectorView<open, close> _preview;

        Gtk::Popover _popover;
        VectorEntry _entry;

    public: // Signals

        sigc::signal<void(float, float, float)> &signal_changed;

    public: // Slots

        sigc::slot<void(float, float, float)> slot_changed;

    public:

        //        VectorEntry() : signal_changed(_entry.signal_changed) {
        //            //add_css_class("editable");
        //
        //            set_child(_preview);
        //
        //            _popover.set_child(_entry);
        //            _popover.set_parent(_preview);
        //
        //            signal_clicked().connect(sigc::mem_fun(_popover, &Gtk::Popover::popup));
        //
        //            slot_changed = sigc::mem_fun<void, VectorEntry, VectorEntry, float, float, float>(*this,
        //                                                                                                    &VectorEntry::setValue);
        //        }

        CompactVectorEntry(Gtk::Button::BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder)
                : Gtk::Button(cobject),
                  signal_changed(_entry.signal_changed) {

            set_valign(Gtk::Align::CENTER);

            set_child(_preview);

            _popover.set_child(_entry);
            _popover.set_parent(_preview);

            signal_clicked().connect(sigc::mem_fun(_popover, &Gtk::Popover::popup));

            slot_changed = sigc::mem_fun<void, CompactVectorEntry, CompactVectorEntry, float, float, float>(
                    *this, &CompactVectorEntry::setValue
            );
        }

        void setValue(float x, float y, float z) {
            _preview.setValue(x, y, z);
            _entry.setValue(x, y, z);
        }

        void setValue(const glm::vec3 &value) {
            setValue(value.x, value.y, value.z);
        };

    private:

    };

    typedef CompactVectorEntry<"(", ")"> CompactPositionEntry;
    typedef CompactVectorEntry<"⟨", "⟩"> CompactVelocityEntry;
    typedef CompactVectorEntry<"|", "|"> CompactDirectionEntry;
}

#endif //N_BODY_VECTORENTRY_H
