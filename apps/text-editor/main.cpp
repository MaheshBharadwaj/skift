#include <libwidget/Application.h>
#include <libwidget/widgets/Button.h>
#include <libwidget/widgets/Panel.h>
#include <libwidget/widgets/TextEditor.h>

int main(int argc, char **argv)
{
    Application::initialize(argc, argv);

    Window *window = new Window(WINDOW_RESIZABLE);

    window->icon(Icon::get("text-box"));

    if (argc == 2)
    {
        window->title(argv[1]);
    }
    else
    {
        window->title("Text Editor");
    }

    window->size(Vec2i(700, 500));

    window->root()->layout(VFLOW(0));

    auto toolbar = new Panel(window->root());

    toolbar->layout(HFLOW(4));
    toolbar->insets(Insetsi(4, 4));
    toolbar->max_height(38);
    toolbar->min_height(38);

    new Button(toolbar, BUTTON_TEXT, Icon::get("folder-open"));
    new Button(toolbar, BUTTON_TEXT, Icon::get("content-save"));
    new Button(toolbar, BUTTON_TEXT, Icon::get("file-plus"));

    auto model = TextModel::empty();

    if (argc == 2)
    {
        logger_info("Opening text document from '%s'", argv[1]);
        model = TextModel::from_file(argv[1]);
    }

    auto field = new TextEditor(window->root(), model);
    field->flags(Widget::FILL);
    field->overscroll(true);
    field->insets({4});
    field->focus();

    window->show();

    return Application::run();
}
