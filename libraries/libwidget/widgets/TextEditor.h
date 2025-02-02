#pragma once

#include <libwidget/Widget.h>
#include <libwidget/model/TextModel.h>
#include <libwidget/widgets/ScrollBar.h>

class TextEditor : public Widget
{
private:
    RefPtr<TextModel> _model;
    TextCursor _cursor{};

    ScrollBar *_vscrollbar;
    ScrollBar *_hscrollbar;

    int _vscroll_offset = 0;
    int _hscroll_offset = 0;

    bool _readonly = false;
    bool _overscroll = false;

    void update_scrollbar();

    void scroll_to_cursor();

    Recti minimum_view_bound()
    {
        return content_bound().cutoff_right(ScrollBar::SIZE).cutoff_botton(ScrollBar::SIZE);
    }

    Recti view_bound()
    {
        auto bound = content_bound();

        if (document_bound().height() > minimum_view_bound().height())
        {
            bound = bound.cutoff_right(ScrollBar::SIZE);
        }

        if (document_bound().width() > minimum_view_bound().width())
        {
            bound = bound.cutoff_botton(ScrollBar::SIZE);
        }

        return bound;
    }

    Recti document_bound()
    {
        return _model->bound(*font())
            .offset({-_hscroll_offset, -_vscroll_offset})
            .offset(content_bound().position());
    }

    Recti vscrollbar_bound()
    {
        auto bound = content_bound().take_right(ScrollBar::SIZE);

        if (document_bound().width() > minimum_view_bound().width())
        {
            bound = bound.cutoff_botton(ScrollBar::SIZE);
        }

        return bound;
    }

    Recti hscrollbar_bound()
    {
        auto bound = content_bound().take_bottom(ScrollBar::SIZE);

        if (document_bound().height() > minimum_view_bound().height())
        {
            bound = bound.cutoff_right(ScrollBar::SIZE);
        }

        return bound;
    }

public:
    void readonly(bool value)
    {
        _readonly = value;
        should_relayout();
        should_repaint();
    }

    void overscroll(bool value)
    {
        _overscroll = value;
        should_relayout();
        should_repaint();
    }

    TextEditor(Widget *parent, RefPtr<TextModel> model);

    ~TextEditor();

    void paint(Painter &painter, Recti rectangle);

    void event(Event *event);

    void do_layout();
};
