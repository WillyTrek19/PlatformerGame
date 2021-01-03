#include "GuiCheckBox.h"

#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Fonts.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
    this->bounds = bounds;
    this->text = text;
    this->offsetText = this->text.Length() * 24 + (24 * 3);

    normal = { 217,0,54, 54 };
    focused = { 217,109,54, 54 };
    pressed = { 217,218,54, 54 };
    disabled = { 217,327,54, 54 };

    normalChecked = { 217,54,54, 54 };
    focusedChecked = { 217,163, 54, 54 };
    pressedChecked = { 217,272, 54, 54 };
    disabledChecked = { 217,381, 54, 54 };
}

GuiCheckBox::~GuiCheckBox()
{
}

bool GuiCheckBox::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        app->input->GetMousePosition(mouseX, mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                checked = !checked;
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiCheckBox::Draw(int cPosX, int cPosY)
{
    // Draw the right button depending on state
    app->fonts->DrawText(cPosX + bounds.x - offsetText, cPosY + bounds.y, titleFont, text.GetString());

    switch (state)
    {
    case GuiControlState::DISABLED:
    {
        if (!checked)
        {
            app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &disabled);
        }
        else
        {
            app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &disabledChecked);
        }
        break;
    }
    case GuiControlState::NORMAL:
    {
        if (!checked)
        {
            app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &normal);
        }
        else
        {
            app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &normalChecked);
        }
        break;
    }
    case GuiControlState::FOCUSED:
    {
        if (!checked)
        {
            app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &focused);
        }
        else
        {
            app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &focusedChecked);
        }
        break;
    }
    case GuiControlState::PRESSED:
    {
        if (!checked)
        {
            app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &pressed);
        }
        else
        {
            app->render->DrawTexture(texture, cPosX + bounds.x, cPosY + bounds.y, false, &pressedChecked);
        }
        break;
    }
    default:
    {
        break;
    }
    }

    return false;
}
