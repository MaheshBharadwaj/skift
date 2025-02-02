#include <libutils/Path.h>

#include <libsystem/eventloop/Timer.h>
#include <libsystem/system/System.h>

#include <libwidget/Application.h>
#include <libwidget/Widgets.h>
#include <libwidget/dialog/MessageBox.h>

#include "task-manager/CPUGraph.h"
#include "task-manager/RAMGraph.h"
#include "task-manager/TaskModel.h"

class TaskManagerWindow : public Window
{
private:
    RAMGraph *_ram_graph;
    CPUGraph *_cpu_graph;
    Table *_table;
    RefPtr<TaskModel> _table_model;
    OwnPtr<Timer> _table_timer;

public:
    TaskManagerWindow() : Window(WINDOW_RESIZABLE)
    {
        icon(Icon::get("memory"));
        title("Task Manager");
        size(Vec2i(700, 500));

        root()->layout(VFLOW(0));

        /// --- Toolbar --- ///
        auto toolbar = new Panel(root());

        toolbar->layout(HFLOW(4));
        toolbar->insets(Insetsi(4, 4));
        toolbar->max_height(38);
        toolbar->min_height(38);

        new Button(toolbar, BUTTON_FILLED, Icon::get("plus"), "New task");

        auto cancel_task_button = new Button(toolbar, BUTTON_TEXT, Icon::get("close"), "Cancel task");
        cancel_task_button->on(Event::ACTION, [&](auto) {
            auto result = MessageBox::create_and_show(
                "Cancel task",
                "Are you sure about that ?",
                Icon::get("close"),
                DialogButton::YES | DialogButton::NO);

            if (result == DialogResult::YES)
            {
                _table_model->kill_task(_table->selected());
            };
        });

        /// --- Table view --- //
        _table_model = make<TaskModel>();

        _table = new Table(root(), _table_model);
        _table->flags(Widget::FILL);

        _table_timer = own<Timer>(1000, [&]() {
            _table_model->update();
        });

        _table_timer->start();
        /// --- Graphs --- ///
        auto graphs_container = new Panel(root());
        graphs_container->layout(HFLOW(0));
        graphs_container->max_height(96);

        _cpu_graph = new CPUGraph(graphs_container, _table_model);

        new Separator(graphs_container);

        _ram_graph = new RAMGraph(graphs_container, _table_model);
    }
};

int main(int argc, char **argv)
{
    Application::initialize(argc, argv);

    auto window = new TaskManagerWindow();
    window->show();

    return Application::run();
}
