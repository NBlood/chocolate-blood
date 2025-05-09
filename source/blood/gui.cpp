/*
 * Copyright (C) 2018, 2022 nukeykt
 *
 * This file is part of Blood-RE.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "typedefs.h"
#include "engine.h"
#include "debug4g.h"
#include "gfx.h"
#include "globals.h"
#include "gui.h"
#include "helix.h"
#include "key.h"
#include "misc.h"
#include "qmouse.h"
#include "resource.h"
#include "screen.h"

int blinkClock;

Resource gGuiRes;

QBITMAP *pMouseCursor;

QFONT *pFont;

void SetBlinkOn()
{
    blinkClock = 0;
}

void SetBlinkOff()
{
    blinkClock = 60;
}

QBOOL IsBlinkOn(void)
{
    return blinkClock < 60;
}

void UpdateBlinkClock(int n)
{
    blinkClock += n;
    while (blinkClock >= 100)
        blinkClock -= 100;
}

void CenterLabel(int a1, int a2, char *a3, int a4)
{
    if (pFont)
        a2 -= pFont->at13/2;
 
    gfxDrawLabel(a1 -  gfxGetLabelLen(a3, pFont) / 2, a2, a4, a3, pFont);
}

void DrawBevel(int x0, int y0, int x1, int y1, int c0, int c1)
{
    Video.SetColor(c0);
    gfxHLine(y0, x0, x1 - 2);
    gfxVLine(x0, y0 + 1, y1 - 2);
    Video.SetColor(c1);
    gfxHLine(y1 - 1, x0 + 1, x1 - 1);
    gfxVLine(x1 - 1, y0 + 1, y1 - 2);
}

void DrawRect(int x0, int y0, int x1, int y1, int c)
{
    Video.SetColor(c);
    gfxHLine(y0, x0, x1 - 1);
    gfxHLine(y1 - 1, x0, x1 - 1);
    gfxVLine(x0, y0 + 1, y1 - 2);
    gfxVLine(x1 - 1, y0 + 1, y1 - 2);
}

void DrawButtonFace(int x0, int y0, int x1, int y1, char a5)
{
    Video.SetColor(gStdColor[20]);
    gfxFillBox(x0, y0, x1, y1);

    if (a5)
    {
        Video.SetColor(gStdColor[26]);
        gfxHLine(y0, x0, x1 - 1);
        gfxVLine(x0, y0 + 1, y1 - 1);
        Video.SetColor(gStdColor[24]);
        gfxHLine(y0 + 1, x0 + 1, x1 - 1);
        gfxVLine(x0 + 1, y0 + 2, y1 - 1);
        DrawBevel(x0+2, y0+2, x1, y1, gStdColor[19], gStdColor[22]);
    }
    else
    {
        DrawBevel(x0, y0, x1, y1, gStdColor[16], gStdColor[24]);
        DrawBevel(x0+1, y0+1, x1-1, y1-1, gStdColor[18], gStdColor[22]);
    }
}

void DrawMouseCursor(int x, int y)
{
    dassert(pMouseCursor != NULL, 124);

    gfxDrawBitmap(pMouseCursor, x, y);
}

char GetHotKey(char *pzLabel)
{
    for (char* s = pzLabel; *s; s++)
    {
        if (*s == '&')
            return (char)toupper(*(s+1));
    }
    return 0;
}

Label::Label(int a1, int a2, char* a3) : Widget(a1, a2, 0, 0)
{
    at8 = strlen(a3) * 8;
    atc = 8;
    strcpy(at24, a3);
}

void Label::Paint(int x, int y, char)
{
    gfxDrawLabel(x, y, gStdColor[0], at24, pFont);
}

Container::Container(int a1, int a2, int a3, int a4) : Widget(a1, a2, a3, a4)
{
    at1e = 1;
    at24 = 0;
    at25 = MODAL_RESULT_0;

    at2e.at10 = at2e.at14 = &at2e;
    at26 = &at2e;
}

Container::~Container()
{
    for (Widget* pWidget = at2e.at14; pWidget != &at2e; pWidget = at2e.at14)
    {
        Remove(pWidget);
        delete pWidget;
    }
}

QBOOL Container::SetFocus(int a1)
{
    do
    {
        if (at26->at1e && ((Container*)at26)->SetFocus(a1))
            return 1;
        if (a1 > 0)
            at26 = at26->at14;
        else
            at26 = at26->at10;
        if (at26 == &at2e)
            return 0;
    } while (!at26->at1c);
    return 1;
}

void Container::Insert(Widget* widget)
{
    dassert(widget != NULL, 200);
    widget->at10 = at2e.at10;
    widget->at14 = &at2e;
    widget->at10->at14 = widget;
    widget->at14->at10 = widget;
    widget->at18 = this;
}

void Container::Remove(Widget* widget)
{
    dassert(widget != NULL, 211);
    widget->at10->at14 = widget->at14;
    widget->at14->at10 = widget->at10;
}

void Container::Paint(int x, int y, char a3)
{
    for (Widget* pWidget = at2e.at14; pWidget != &at2e; pWidget = pWidget->at14)
    {
        pWidget->Paint(x + pWidget->at0, y + pWidget->at4, at26 == pWidget);
    }
}

void Container::HandleEvent(GEVENT* event)
{
    if (event->at0 & GEVENT_TYPE_MOUSE_MASK)
    {
        event->at6.mouse.at8 -= at0;
        event->at6.mouse.atc -= at4;
        if (event->at0 == GEVENT_TYPE_MOUSE_1)
        {
            at2a = NULL;
            for (Widget* pWidget = at2e.at10; pWidget != &at2e; pWidget = pWidget->at10)
            {
                if (pWidget->Inside(event->at6.mouse.at8, event->at6.mouse.atc))
                {
                    at2a = pWidget;
                    if (at2a->at1c)
                        at26 = pWidget;
                    break;
                }
            }
        }
        if (at2a != NULL)
            at2a->HandleEvent(event);
    }
    else if (event->at0 == GEVENT_TYPE_KEYBOARD)
    {
        if (event->at6.keyboard.at2.bits.at2 && ScanToAsciiShifted[event->at6.keyboard.at1])
        {
            for (Widget* pWidget = at2e.at10; pWidget != &at2e; pWidget = pWidget->at10)
            {
                if (ScanToAsciiShifted[event->at6.keyboard.at1] == pWidget->at1f && pWidget->at1c)
                {
                    at26 = pWidget;
                    at26->HandleEvent(event);
                    return;
                }
            }
        }
        at26->HandleEvent(event);

        if (event->at0 != GEVENT_TYPE_NONE)
        {
            for (Widget* pWidget = at2e.at10; pWidget != &at2e; pWidget = pWidget->at10)
            {
                if (ScanToAsciiShifted[event->at6.keyboard.at1] == pWidget->at1f && pWidget->at1c)
                {
                    at26 = pWidget;
                    at26->HandleEvent(event);
                    return;
                }
            }
        }
    }
}

void Container::EndModal(MODAL_RESULT result)
{
    if (at24)
    {
        at24 = 0;
        at25 = result;
        return;
    }
    at18->EndModal(result);
}

void Panel::Paint(int x, int y, char a3)
{
    int i;
    int j = 0;
    Video.SetColor(gStdColor[20]);
    gfxFillBox(x, y, x + at8, y + atc);
    for (i = klabs(at52); i > 0; j++, i--)
    {
        if (at52 > 0)
        {
            DrawBevel(x+j,y+j,x+at8-j,y+atc-j, gStdColor[16], gStdColor[24]);
        }
        else
        {
            DrawBevel(x+j,y+j,x+at8-j,y+atc-j, gStdColor[24], gStdColor[16]);
        }
    }
    j += at56;
    for (i = klabs(at5a); i > 0; j++, i--)
    {
        if (at5a > 0)
        {
            DrawBevel(x+j,y+j,x+at8-j,y+atc-j, gStdColor[16], gStdColor[24]);
        }
        else
        {
            DrawBevel(x+j,y+j,x+at8-j,y+atc-j, gStdColor[24], gStdColor[16]);
        }
    }
    Container::Paint(x, y, a3);
}

TitleBar::TitleBar(int a1, int a2, int a3, int a4, const char *a5) : Widget(a1, a2, a3, a4)
{
    strcpy(at24, a5);
    at124 = strlen(at24);
}

void TitleBar::Paint(int x, int y, char a3)
{
    Video.SetColor(gStdColor[1]);
    gfxFillBox(x, y, x + at8, y + atc);
    DrawBevel(x, y, x + at8, y + atc, gStdColor[9], gStdColor[30]);
    CenterLabel(x + at8 / 2, y + atc / 2, at24, gStdColor[15]);
}

void TitleBar::HandleEvent(GEVENT *event)
{
    if (event->at0 & GEVENT_TYPE_MOUSE_MASK)
    {
        if (event->at6.mouse.at4 == 0)
        {
            switch (event->at0)
            {
            case GEVENT_TYPE_MOUSE_8:
                at18->at0 += event->at6.mouse.at10;
                at18->at4 += event->at6.mouse.at14;
                event->at0 = GEVENT_TYPE_NONE;
                break;
            case GEVENT_TYPE_MOUSE_2:
                break;
            }
        }
    }
}

Window::Window(int a1, int a2, int a3, int a4, const char* a5) : Panel(a1, a2, a3, a4, 1, 1, -1)
{
    at62 = new TitleBar(3, 3, a3-6, 12, a5);
    at5e = new Container(3, 15, a3-6, a4-18);
    Insert(at62);
    Insert(at5e);
    at2a = NULL;
}

void Button::Paint(int x, int y, char a3)
{
    Video.SetColor(gStdColor[0]);
    gfxHLine(y, x + 1, x + at8 - 2);
    gfxHLine(y + atc - 1, x + 1, x + at8 - 2);
    gfxVLine(x, y + 1, y + atc - 2);
    gfxVLine(x + at8 - 1, y + 1, y + atc - 2);
    DrawButtonFace(x + 1, y + 1, x + at8 - 1, y + atc - 1, at29);
}

void Button::HandleEvent(GEVENT* event)
{
    if (event->at0 == GEVENT_TYPE_KEYBOARD)
    {
        if (event->at6.keyboard.at0 == ' ' || ScanToAsciiShifted[event->at6.keyboard.at1] == at1f)
        {
            at29 = !at29;
            if (at25)
                at25(this);
            if (at24)
                EndModal(at24);
            event->at0 = GEVENT_TYPE_NONE;
        }
    }
    else if ((event->at0 & GEVENT_TYPE_MOUSE_MASK))
    {
        if (event->at6.mouse.at4 == 0)
            return;
        switch (event->at0)
        {
        case GEVENT_TYPE_MOUSE_1:
            at29 = 1;
            event->at0 = GEVENT_TYPE_NONE;
            break;
        case GEVENT_TYPE_MOUSE_8:
            at29 = Inside(event->at6.mouse.at8, event->at6.mouse.atc);
            event->at0 = GEVENT_TYPE_NONE;
            break;
        case GEVENT_TYPE_MOUSE_2:
            at29 = 0;
            if (Inside(event->at6.mouse.at8, event->at6.mouse.atc))
            {
                if (at25)
                    at25(this);
                if (at24)
                    EndModal(at24);
            }
            event->at0 = GEVENT_TYPE_NONE;
            break;
        }
    }
}

TextButton::TextButton(int a1, int a2, int a3, int a4, char *a5, MODAL_RESULT a6) : Button(a1, a2, a3, a4, a6), at2a(a5)
{
    at1f = GetHotKey(a5);
    at1c = 1;
}

void TextButton::Paint(int x, int y, char a3)
{
    Video.SetColor(gStdColor[0]);
    gfxHLine(y + 1, x + 2, x + at8 - 3);
    gfxHLine(y + atc - 2, x + 2, x + at8 - 3);
    gfxVLine(x + 1, y + 2, y + atc - 3);
    gfxVLine(x + at8 - 2, y + 2, y + atc - 3);
    if (a3)
    {
        Video.SetColor(gStdColor[15]);
        gfxHLine(y, x + 1, x + at8 - 2);
        gfxHLine(y + atc - 1, x + 1, x + at8 - 2);
        gfxVLine(x, y + 1, y + atc - 2);
        gfxVLine(x + at8 - 1, y + 1, y + atc - 2);
        gfxPixel(x + 1, y + 1);
        gfxPixel(x + at8 - 2, y + 1);
        gfxPixel(x + 1, y + atc - 2);
        gfxPixel(x + at8 - 2, y + atc - 2);
    }
    DrawButtonFace(x + 2, y + 2, x + at8 - 2, y + atc - 2, at29);
    if (at29)
        CenterLabel(x + at8 / 2 + 1, y + atc / 2 + 1, at2a, gStdColor[0]);
    else
        CenterLabel(x + at8 / 2, y + atc / 2, at2a, gStdColor[0]);
}

void TextButton::HandleEvent(GEVENT* event)
{
    if (event->at0 == GEVENT_TYPE_KEYBOARD && ScanToAsciiShifted[event->at6.keyboard.at1] == at1f)
    {
        at29 = !at29;
        if (at25)
            at25(this);
        if (at24)
            EndModal(at24);
        event->at0 = GEVENT_TYPE_NONE;
    }
    Button::HandleEvent(event);
}

void BitButton::Paint(int x, int y, char a3)
{
    Video.SetColor(gStdColor[0]);
    gfxHLine(y, x + 1, x + at8 - 2);
    gfxHLine(y + atc -1, x + 1, x + at8 - 2);
    gfxVLine(x, y + 1, y + atc - 2);
    gfxVLine(x + at8 - 1, y + 1, y + atc - 2);
    DrawButtonFace(x + 1, y + 1, x + at8 - 1, y + atc - 1, at29);

    int bx = x + at8 / 2;
    int by = y + atc / 2;
    QBITMAP* pQBM = (QBITMAP*)gGuiRes.Load(bitmap);

    if (at29)
        gfxDrawBitmap(pQBM, bx - pQBM->cols / 2 + 1, by - pQBM->rows / 2 + 1);
    else
        gfxDrawBitmap(pQBM, bx - pQBM->cols / 2, by - pQBM->rows / 2);
}

EditText::EditText(int a1, int a2, int a3, int a4, const char *a5) : Widget(a1,a2,a3,a4)
{
    at1c = 1;
    strcpy(at24, a5);
    at128 = at124 = strlen(at24);
    at12c = a3 / 8 - 1;
}

void EditText::Paint(int x, int y, char a3)
{
    DrawBevel(x, y, x + at8 - 1, y + atc - 1, gStdColor[24], gStdColor[16]);
    DrawRect(x + 1, y + 1, x + at8 - 2, y + atc - 2, gStdColor[0]);
    Video.SetColor(gStdColor[a3 ? 15 : 20]);
    gfxFillBox(x + 2, y + 2, x + at8 - 3, y + atc - 3);
    gfxDrawText(x + 3, y + atc / 2 - 4, gStdColor[0], at24, pFont);

    if (a3 && IsBlinkOn())
    {
        Video.SetColor(gStdColor[0]);
        y += atc / 2;
        gfxVLine(x + gfxGetTextNLen(at24, pFont, at128) + 3, y - 4, y + 3);
    }
}

void EditText::HandleEvent(GEVENT *event)
{
    if (event->at0 & GEVENT_TYPE_MOUSE_MASK)
    {
        if (event->at6.mouse.at4)
            return;
        switch (event->at0)
        {
        case GEVENT_TYPE_MOUSE_1:
        case GEVENT_TYPE_MOUSE_8:
            at128 = gfxFindTextPos(at24, pFont, event->at6.mouse.at8 - at0);
            SetBlinkOn();
            event->at0 = GEVENT_TYPE_NONE;
            break;
        }
    }
    else if (event->at0 == GEVENT_TYPE_KEYBOARD)
    {
        switch (event->at6.keyboard.at1)
        {
        case bsc_Backspace:
            if (at128 > 0)
            {
                memmove(at24+(at128-1), at24+at128, at124-at128);
                at128--;
                at124--;
                at24[at124] = 0;
            }
            event->at0 = GEVENT_TYPE_NONE;
            break;
        case bsc_Del:
            if (at128 < at124)
            {
                at124--;
                memmove(at24+at128, at24+(at128+1), at124-at128);
                at24[at124] = 0;
            }
            event->at0 = GEVENT_TYPE_NONE;
            break;
        case bsc_Left:
            if (at128 > 0)
                at128--;
            event->at0 = GEVENT_TYPE_NONE;
            break;
        case bsc_Right:
            if (at128 < at124)
                at128++;
            event->at0 = GEVENT_TYPE_NONE;
            break;
        case bsc_Home:
            at128 = 0;
            event->at0 = GEVENT_TYPE_NONE;
            break;
        case bsc_End:
            at128 = at124;
            event->at0 = GEVENT_TYPE_NONE;
            break;
        default:
            if (event->at6.keyboard.at0 != 0)
            {
                if (at124 < at12c)
                {
                    memmove(at24+(at128+1),at24+at128,at124-at128);
                    at24[at128++] = event->at6.keyboard.at0;
                    at24[++at124] = 0;
                }
                event->at0 = GEVENT_TYPE_NONE;
            }
            break;
        }
        SetBlinkOn();
    }
}

EditNumber::EditNumber(int a1, int a2, int a3, int a4, int a5) : EditText(a1, a2, a3, a4, ""), at130(a5)
{
    itoa(a5, at24, 10);
    at128 = at124 = strlen(at24);
}

void EditNumber::HandleEvent(GEVENT *event)
{
    if (event->at0 == GEVENT_TYPE_KEYBOARD)
    {
        switch (event->at6.keyboard.at1)
        {
        case bsc_Minus:
            if (at128 == 0 && at24[0] != '-' && at124 < at12c)
            {
                memmove(at24+1, at24, at124);
                at24[at128++] = '-';
                at24[++at124] = 0;
            }
            event->at0 = GEVENT_TYPE_NONE;
            break;
        case bsc_Backspace:
            break;
        case bsc_Del:
            break;
        case bsc_Left:
            break;
        case bsc_Right:
            break;
        case bsc_Home:
            break;
        case bsc_End:
            break;
        default:
            if (event->at6.keyboard.at0 != 0)
            {
                if (event->at6.keyboard.at0 >= '0' && event->at6.keyboard.at0 <= '9' && at124 < at12c)
                {
                    memmove(at24+(at128+1),at24+at128,at124-at128);
                    at24[at128++] = event->at6.keyboard.at0;
                    at24[++at124] = 0;
                }
                event->at0 = GEVENT_TYPE_NONE;
            }
            break;
        }
    }
    EditText::HandleEvent(event);
    at130 = atoi(at24);
}

void ThumbButton::HandleEvent(GEVENT* event)
{
    if (event->at0 & GEVENT_TYPE_MOUSE_MASK)
    {
        if (event->at6.mouse.at4)
            return;
        switch (event->at0)
        {
        case GEVENT_TYPE_MOUSE_8:
            at4 = ClipRange(event->at6.mouse.atc-atc/2, 10, at18->atc-10-atc);
            break;
        case GEVENT_TYPE_MOUSE_1:
            at29 = 1;
            break;
        case GEVENT_TYPE_MOUSE_2:
            at29 = 0;
            break;
        }
    }
}

void ScrollButton::HandleEvent(GEVENT* event)
{
    if (event->at0 & GEVENT_TYPE_MOUSE_MASK)
    {
        if (event->at6.mouse.at4)
            return;
        switch (event->at0)
        {
        case GEVENT_TYPE_MOUSE_1:
            at29 = 1;
            if (at25)
                at25(this);
            break;
        case GEVENT_TYPE_MOUSE_8:
            at29 = Inside(event->at6.mouse.at8, event->at6.mouse.atc);
            break;
        case GEVENT_TYPE_MOUSE_4:
            if (at29 && at25)
                at25(this);
            break;
        case GEVENT_TYPE_MOUSE_2:
            at29 = 0;
            break;
        }
    }
}

void ScrollLineUp(Widget* widget)
{
    ((ScrollBar*)widget->at18)->ScrollRelative(-1);
}

void ScrollLineDown(Widget* widget)
{
    ((ScrollBar*)widget->at18)->ScrollRelative(1);
}

ScrollBar::ScrollBar(int a1, int a2, int a3, int a4, int a5, int a6) : Container(a1, a2, 13, a3), at52(a4), at56(a5), at6e(a6)
{
    at5e = new ScrollButton(1, 1, 11, 10, gGuiRes.Lookup("UPARROW", "QBM"), ScrollLineUp);
    at62 = new ScrollButton(1, a3-11, 11, 10, gGuiRes.Lookup("DNARROW", "QBM"), ScrollLineDown);
    at6a = new ThumbButton(1, 10, 11, 20);

    Insert(at5e);
    Insert(at62);
    Insert(at6a);

    at5a = 0;
}

void ScrollBar::ScrollRelative(int a1)
{
    at6a->at4 = ClipRange(at6a->at4 + a1, 10, atc - 10 - at6a->atc);
}

void ScrollBar::Paint(int x, int y, char a1)
{
    DrawBevel(x, y, x + at8, y + atc, gStdColor[24], gStdColor[16]);
    DrawRect(x + 1, y + 1, x + at8 - 1, y + atc - 1, gStdColor[0]);
    Video.SetColor(gStdColor[24]);
    gfxFillBox(x + 2, y + 2, x + at8 - 2, y + atc - 2);

    Container::Paint(x, y, a1);
}

GEVENT_TYPE GetEvent(GEVENT* event)
{
    memset(event, 0, sizeof(GEVENT));

    byte key = keyGet();
    if (key != 0)
    {
        if (keystatus[bsc_Left])
            event->at6.keyboard.at2.bits.at3 = 1;
        if (keystatus[bsc_Right])
            event->at6.keyboard.at2.bits.at4 = 1;
        event->at6.keyboard.at2.bits.at0 = event->at6.keyboard.at2.bits.at3 | event->at6.keyboard.at2.bits.at4;
        if (keystatus[bsc_LCtrl])
            event->at6.keyboard.at2.bits.at5 = 1;
        if (keystatus[bsc_RShift])
            event->at6.keyboard.at2.bits.at6 = 1;
        event->at6.keyboard.at2.bits.at1 = event->at6.keyboard.at2.bits.at5 | event->at6.keyboard.at2.bits.at6;
        if (keystatus[bsc_LAlt])
            event->at6.keyboard.at2.bits.at7 = 1;
        if (keystatus[bsc_RAlt])
            event->at6.keyboard.at2.bits.at8 = 1;
        event->at6.keyboard.at2.bits.at2 = event->at6.keyboard.at2.bits.at7 | event->at6.keyboard.at2.bits.at8;
        if (event->at6.keyboard.at2.bits.at2)
            event->at6.keyboard.at0 = 0;
        else if (event->at6.keyboard.at2.bits.at1)
            event->at6.keyboard.at0 = 0;
        else if (event->at6.keyboard.at2.bits.at0)
            event->at6.keyboard.at0 = ScanToAsciiShifted[key];
        else
            event->at6.keyboard.at0 = ScanToAscii[key];
        event->at0 = GEVENT_TYPE_KEYBOARD;
        event->at6.keyboard.at1 = key;
        if (key == bsc_Esc)
            keystatus[bsc_Esc] = 0;
        return event->at0;
    }

    event->at6.mouse.at10 = Mouse::dX;
    event->at6.mouse.at14 = Mouse::dY;
    event->at6.mouse.at8 = Mouse::X;
    event->at6.mouse.atc = Mouse::Y;

    static int clickTime[3], downTime[3];
    static byte oldbuttons;

    byte delta = Mouse::buttons & ~oldbuttons;

    byte j = 1;

    for (int i = 0; i < 3; i++, j <<= 1)
    {
        event->at6.mouse.at4 = i;
        if (delta & j)
        {
            oldbuttons |= j;
            event->at0 = GEVENT_TYPE_MOUSE_1;
            if (clickTime[i]+60 > gFrameClock)
                event->at6.mouse.at18 = 1;
            clickTime[i] = gFrameClock;
            downTime[i] = 0;
            return event->at0;
        }
        else if (oldbuttons & j)
        {
            if (Mouse::buttons & j)
            {
                downTime[i] += gFrameTicks;
                if (event->at6.mouse.at10 || event->at6.mouse.at14)
                {
                    event->at0 = GEVENT_TYPE_MOUSE_8;
                    return event->at0;
                }
                else if (downTime[i] > 60)
                {
                    downTime[i] -= 6;
                    event->at0 = GEVENT_TYPE_MOUSE_4;
                    return event->at0;
                }
            }
            else
            {
                oldbuttons &= ~j;
                event->at0 = GEVENT_TYPE_MOUSE_2;
                return event->at0;
            }
        }
    }
    return GEVENT_TYPE_NONE;
}

MODAL_RESULT ShowModal(Container* container)
{
    GEVENT event;

    Container screen(0, 0, xdim, ydim);

    screen.Insert(container);

    container->at0 = (xdim - container->at8) / 2;
    container->at4 = (ydim - container->atc) / 2;

    int vc = xdim * ydim;

    while (!screen.SetFocus(1)) {}

    byte *pFrameBackup = (byte*)Resource::Alloc(vc);

    DICTNODE* hMouseCursor = gGuiRes.Lookup("MOUSE1", "QBM");
    dassert(hMouseCursor != NULL, 902);
    pMouseCursor = (QBITMAP*)gGuiRes.Lock(hMouseCursor);

    DICTNODE* hFont = gGuiRes.Lookup("FONT1", "QFN");
    dassert(hFont != NULL, 906);
    pFont = (QFONT*)gGuiRes.Lock(hFont);

    switch (vidoption)
    {
        case 0:
            break;
        case 1:
        case 2:
            memcpy(pFrameBackup, (void*)frameplace, vc);
            break;
    }

    container->at24 = 1;
    while (container->at24)
    {
        gFrameTicks = totalclock - gFrameClock;
        gFrameClock += gFrameTicks;
        UpdateBlinkClock((int)gFrameTicks);

        Mouse::Read(gFrameTicks);

        GetEvent(&event);
        if (event.at0 == GEVENT_TYPE_KEYBOARD)
        {
            switch (event.at6.keyboard.at0)
            {
            case 27:
                container->EndModal(MODAL_RESULT_2);
                continue;
            case 13:
                container->EndModal(MODAL_RESULT_1);
                continue;
            case 9:
                if (event.at6.keyboard.at2.bits.at0)
                {
                    while (!screen.SetFocus(-1)) {}
                }
                else
                {
                    while (!screen.SetFocus(1)) {}
                }
                continue;
            }
        }
        screen.HandleEvent(&event);
        memcpy((void*)frameplace, pFrameBackup, vc);
        screen.Paint(0, 0, 0);
        DrawMouseCursor(Mouse::X, Mouse::Y);
        if (vidoption == 2)
            WaitVBL();
        scrNextPage();
        memcpy((void*)frameplace, pFrameBackup, vc);
    }

    memcpy((void*)frameplace, pFrameBackup, vc);
    scrNextPage();

    gGuiRes.Unlock(hMouseCursor);
    pMouseCursor = NULL;

    gGuiRes.Unlock(hFont);
    pFont = NULL;

    Resource::Free(pFrameBackup);

    screen.Remove(container);

    return container->at25;
}

int GetStringBox(char *a1, char *a2)
{
    Window window(0, 0, 168, 40, a1);
    EditText* edittext = new EditText(4, 4, 154, 16, a2);

    window.at5e->Insert(edittext);

    ShowModal(&window);

    if (window.at25 != MODAL_RESULT_1)
        return 0;

    strcpy(a2, edittext->at24);

    return 1;
}

int GetNumberBox(const char *a1, int a2, int a3)
{
    Window window(0, 0, 168, 40, a1);
    EditNumber* editnumber = new EditNumber(4, 4, 154, 16, a2);

    window.at5e->Insert(editnumber);

    ShowModal(&window);

    if (window.at25 != MODAL_RESULT_1)
        return a3;

    return editnumber->at130;
}

#if 0

void Label::HandleEvent(GEVENT*)
{
}

void Widget::EndModal(MODAL_RESULT a1)
{
    at18->EndModal(a1);
}

Widget::~Widget()
{
}

Widget::Widget(int a1, int a2, int a3, int a4)
{
    at1c = 0;
    at1d = 0;
    at1e = 0;
    at1f = 0;

    at0 = a1;
    at4 = a2;
    at8 = a3;
    atc = a4;

    at10 = NULL;
    at14 = NULL;
    at18 = NULL;
}

#endif
