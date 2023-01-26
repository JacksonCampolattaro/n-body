//
// Created by Jackson Campolattaro on 7/24/22.
//

#ifndef N_BODY_MULTIRENDERER_H
#define N_BODY_MULTIRENDERER_H

#include <spdlog/spdlog.h>

#include <gtkmm/singleselection.h>
#include <giomm/liststore.h>

#include <NBody/View/Renderer.h>

#include <NBody/View/Renderers/FlatRenderer.h>
#include <NBody/View/Renderers/InstancedFlatRenderer.h>
#include <NBody/View/Renderers/PhongRenderer.h>
#include <NBody/View/Renderers/InstancedPhongRenderer.h>
#include <NBody/View/Renderers/SpriteRenderer.h>
#include <NBody/View/Renderers/VelocityRenderer.h>

namespace NBody {

    class MultiRenderer : public SimulationRenderer {
    private:

        Glib::RefPtr<Gio::ListStore<SimulationRenderer>> _rendererList{Gio::ListStore<SimulationRenderer>::create()};
        Glib::RefPtr<Gtk::SingleSelection> _rendererSelection{Gtk::SingleSelection::create(_rendererList)};

        sigc::signal<void()> _signal_changed;

    public:

        explicit MultiRenderer(const Simulation &simulation) :
                SimulationRenderer(simulation) {

            // Add available renderers
            _rendererList->append(Glib::make_refptr_for_instance(new FlatRenderer(_simulation)));
            _rendererList->append(Glib::make_refptr_for_instance(new InstancedFlatRenderer(_simulation)));
            _rendererList->append(Glib::make_refptr_for_instance(new PhongRenderer(_simulation)));
            _rendererList->append(Glib::make_refptr_for_instance(new InstancedPhongRenderer(_simulation)));
            _rendererList->append(Glib::make_refptr_for_instance(new SpriteRenderer(_simulation)));
            _rendererList->append(Glib::make_refptr_for_instance(new VelocityRenderer(_simulation)));

            // One solver must always be selected
            _rendererSelection->set_can_unselect(false);

            _simulation.signal_changed.connect(_signal_changed.make_slot());
            _rendererSelection->signal_selection_changed().connect([&](guint, guint) {
                _signal_changed.emit();
            });
        }

        std::string id() override { return get().id(); };

        std::string name() override { return get().name(); };

        sigc::signal<void()> &signal_changed() override { return _signal_changed; };

        void draw(const Matrix4 &transformationMatrix,
                  const Matrix4 &projectionMatrix) override {
            get().draw(transformationMatrix, projectionMatrix);
        };

        void select(guint index) {
            _rendererSelection->select_item(index, true);
            spdlog::debug("Selected {} renderer", get().name());
        }

        void select(const std::string &id) {
            for (guint i = 0; i < _rendererList->get_n_items(); ++i) {
                if (id == _rendererList->get_item(i)->id()) {
                    select(i);
                    return;
                }
            }
            spdlog::error("Unrecognized renderer id \"{}\"", id);
        }

        SimulationRenderer &get() {
            return *std::dynamic_pointer_cast<SimulationRenderer>(_rendererSelection->get_selected_item());
        }

        const SimulationRenderer &get() const {
            return *std::dynamic_pointer_cast<SimulationRenderer>(_rendererSelection->get_selected_item());
        }

        template<typename SimulationRendererImplementation>
        SimulationRendererImplementation &get(const std::string &id) {
            for (int i = 0; i < _rendererList->get_n_items(); ++i) {
                if (id == _rendererList->get_item(i)->id()) {
                    return *std::dynamic_pointer_cast<SimulationRendererImplementation>(_rendererList->get_item(i));
                }
            }
            spdlog::error("Unrecognized solver id \"{}\"", id);
            return (SimulationRendererImplementation &) get(); // todo: use an exception here
        }

        Glib::RefPtr<Gio::ListStore<SimulationRenderer>> &listModel() { return _rendererList; }

        Glib::RefPtr<Gtk::SingleSelection> &selectionModel() { return _rendererSelection; }
    };

}

#endif //N_BODY_MULTIRENDERER_H
