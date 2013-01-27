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


#ifndef FILLVIEW_HPP
#define FILLVIEW_HPP

#include <assert.h>
#include <screen/screen.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <bps/navigator.h>
#include <bps/screen.h>
#include <bps/bps.h>
#include <bps/event.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <QtCore/QObject>
#include <QtCore/QString>

#include "OpenGLView.hpp"
#include "OpenGLThread.hpp"

class FillView : public OpenGLView {

Q_OBJECT

Q_PROPERTY(QVariantList objectColor READ objectColor WRITE setObjectColor) // object color

public:
	FillView(VIEW_DISPLAY display);

	virtual ~FillView();

	// property signals
	QVariantList& objectColor();


public Q_SLOTS:
	// property slots
	void setObjectColor(QVariantList objectColor);

	// action slots
	void reset(bool skipColour);

public:
	// overriden methods from OpenGLView
	int initialize();
	int regenerate();
	void cleanup();

	void handleScreenEvent(bps_event_t *event);

	void update();
	void render();

private:
        float obj_color[4];
};

#endif /* FILLVIEW_HPP */

