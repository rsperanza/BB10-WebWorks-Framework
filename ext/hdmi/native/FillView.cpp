/*
 * Copyright (c) 2011-2012 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "FillView.hpp"
//#include "bbutil.h"
#include <math.h>

#include <QDebug>

using namespace bb::cascades;

FillView::FillView(VIEW_DISPLAY display)
{
    setDisplay(display);
}

FillView::~FillView() {
    // TODO Auto-generated destructor stub
}

int FillView::initialize() {

    int returnCode = OpenGLView::initGL();
    if (returnCode == EXIT_SUCCESS) {
        reset(false);

        // reset stale flag for initialization only
        setStale(false);

        //Common gl setup
        glShadeModel(GL_SMOOTH);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    qDebug()  << "FillView::initialize ";

    return returnCode;
}

int FillView::regenerate()
{
    int returnCode = OpenGLView::regenerate();

    reset(true);

    return returnCode;
}

void FillView::cleanup() {
    OpenGLView::cleanup();
}

void FillView::render() {
    if (m_display == DISPLAY_HDMI) {
        //Typical render pass
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    // after rendering this view is no longer stale
    setStale(false);
}

// properties and slots
QVariantList& FillView::objectColor() {

    m_viewMutex.lock();

    QVariantList* objectColor = new QVariantList();
    *objectColor << obj_color[0] << obj_color[1] << obj_color[2] << obj_color[3];

    m_viewMutex.unlock();

    return *objectColor;
}

void FillView::setObjectColor(QVariantList color) {

    m_viewMutex.lock();

    obj_color[0] = color[0].value<float>();
    obj_color[1] = color[1].value<float>();
    obj_color[2] = color[2].value<float>();
    obj_color[3] = color[3].value<float>();

    m_viewMutex.unlock();

    setStale(true);
}

void FillView::reset(bool skipColour) {
    if (!skipColour) {
        obj_color[0] = 1.0f;
        obj_color[1] = 0.0f;
        obj_color[2] = 0.0f;
        obj_color[3] = 1.0f;
    }

    setStale(true);
}

void FillView::update() {
}

void FillView::handleScreenEvent(bps_event_t *event) {
    int screenEvent;
    int buttons;
    int position[2];

    screen_event_t screen_event = screen_event_get_event(event);

    //Query type of screen event and its location on the screen
    screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE,
            &screenEvent);
    screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_SOURCE_POSITION,
            position);

    switch (screenEvent) {
        case SCREEN_EVENT_MTOUCH_TOUCH:
            break;

        case SCREEN_EVENT_MTOUCH_MOVE:
            break;

        case SCREEN_EVENT_MTOUCH_RELEASE:
            break;

        case SCREEN_EVENT_POINTER:
            screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_BUTTONS,
                &buttons);

            switch (buttons) {
                case SCREEN_LEFT_MOUSE_BUTTON:
                case SCREEN_RIGHT_MOUSE_BUTTON:
                    //handleClick(position[0], position[1]);
                    break;
            }

            break;
    }
}
