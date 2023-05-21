#include "Button.h"
#include "core.h"

struct Button
{
    Texture* tex;

    unsigned int x;
    unsigned int y;
};

Button* newButton()
{
    Button* button = (Button*)malloc(sizeof(Button));
    button->tex = newTexture();
    button->x = 0;
    button->y = 0;

    return button;
}

void deleteButton(Button* button)
{
    if(button)
    {
        deleteTexture(button->tex);
        free(button);
    }
}
