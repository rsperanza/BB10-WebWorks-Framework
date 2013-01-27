/*
* Copyright 2012 Research In Motion Limited.
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

#define SENSOR_MSG_PULSE 64
#define SENSOR_BASE_PULSE 128

#include <json/reader.h>
#include <json/writer.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <errno.h>
#include <stdio.h>
#include <algorithm>
#include <string>

#include "hdmi_ndk.hpp"
#include "hdmi_js.hpp"

#define MUTEX_LOCK() pthread_mutex_trylock(&m_lock)
#define MUTEX_UNLOCK() pthread_mutex_unlock(&m_lock)

namespace webworks {

pthread_mutex_t HDMINDK::m_lock = PTHREAD_MUTEX_INITIALIZER;
 
HDMINDK::HDMINDK(HDMI *parent) : m_parent(parent)
{
}

HDMINDK::~HDMINDK()
{
    stopThread();

    pthread_mutex_destroy(&m_lock);
}

void HDMINDK::SetHDMIOptions(const Json::Value& config)
{
    MUTEX_LOCK();

    if (config.isMember("delay")) {
        //sensor_set_delay(sensor, config["delay"].asInt());
    }

    if (config.isMember("queue")) {
        //sensor_set_queue(sensor, config["queue"].asInt());
    }

    if (config.isMember("batching")) {
        //sensor_set_batching(sensor, config["batching"].asInt());
    }

    if (config.isMember("background")) {
        //sensor_set_background(sensor, config["background"].asInt());
    }

    if (config.isMember("reducedReporting")) {
        //sensor_set_reduced_reporting(sensor, config["reducedReporting"].asInt());
    }

    MUTEX_UNLOCK();
}

std::string HDMINDK::GetHDMIScreenSize()
{
    Json::Value sizes;

    MUTEX_LOCK();

    int *size = getHDMIScreenSize();
    sizes["width"] = Json::Value(size[0]);
    sizes["height"] = Json::Value(size[1]);

    MUTEX_UNLOCK();

    return Json::FastWriter().write(sizes);
}

int* HDMINDK::getHDMIScreenSize()
{
    return OpenGLThread::getInstance()->getDisplaySize(DISPLAY_HDMI);
}

void HDMINDK::OpenWindow(const std::string& args)
{
    Json::Value obj;

    MUTEX_LOCK();

    bool parse = Json::Reader().parse(args, obj);

    if (parse) {
        int x = 0, y = 0, width = 0, height = 0;

        if (obj.isMember("x")) {
            x = obj["x"].asInt();
        }
        if (obj.isMember("y")) {
            y = obj["y"].asInt();
        }
        if (obj.isMember("width")) {
            width = obj["width"].asInt();
        }
        if (obj.isMember("height")) {
            height = obj["height"].asInt();
        }

        openWindow(x, y, width, height);
    }

    MUTEX_UNLOCK();
}

void HDMINDK::openWindow(const int x, const int y, const int width, const int height)
{
    m_fillView = new FillView(DISPLAY_HDMI);
    m_fillView->setPosition(x, y);
    m_fillView->setSize(width, height);
    m_fillView->add();
    m_fillView->setEnabled(true);
}


void HDMINDK::SetBackgroundColor(const std::string& args)
{
    Json::Value obj;

    MUTEX_LOCK();

    bool parse = Json::Reader().parse(args, obj);

    if (parse) {
        int red = 0, green = 0, blue = 0, alpha = 255;

        if (obj.isMember("red")) {
            red = obj["red"].asInt();
        }
        if (obj.isMember("green")) {
            green = obj["green"].asInt();
        }
        if (obj.isMember("blue")) {
            blue = obj["blue"].asInt();
        }
        if (obj.isMember("alpha")) {
            alpha = obj["alpha"].asInt();
        }

        setBackgroundColor(red, green, blue, alpha);
    }

    MUTEX_UNLOCK();
}
void HDMINDK::setBackgroundColor(const int red, const int green, const int blue, const int alpha)
{
    QVariantList* objectColor = new QVariantList();
    *objectColor << (float)red/255.0 << (float)green/255.0 << (float)blue/255.0 << (float)alpha/255.0;
    m_fillView->setObjectColor(*objectColor);
}


/*
void *HDMINDK::HDMIThread(void *args)
{
    HDMI *parent = reinterpret_cast<HDMI *>(args);
    struct _pulse pulse;

    // create channel for events
    m_sensorChannel = ChannelCreate(0);
    m_coid = ConnectAttach(ND_LOCAL_NODE, 0, m_sensorChannel, _NTO_SIDE_CHANNEL, 0);

    for (;;) {
        MUTEX_UNLOCK();
        if (EOK == MsgReceivePulse(m_sensorChannel, &pulse, sizeof(pulse), NULL)) {
        MUTEX_LOCK();
            if (pulse.code == SENSOR_MSG_PULSE) {
                ActiveSensorMap::iterator it;
                for (it = m_pActiveHDMI->begin(); it != m_pActiveHDMI->end(); ++it) {
                    stopActiveSensor(static_cast<sensor_type_e>((*it).first));
                }
                break;
            }
            Json::FastWriter writer;
            Json::Value root;

            sensor_event_t event;
            sensor_t *sensor = static_cast<sensor_t *>(pulse.value.sival_ptr);
            sensor_get_event(sensor, &event);

            std::string accuracy;
            std::string sensorEvent;

            switch (event.accuracy)
            {
                case SENSOR_ACCURACY_UNRELIABLE:
                    accuracy = "unreliable";
                    break;
                case SENSOR_ACCURACY_LOW:
                    accuracy = "low";
                    break;
                case SENSOR_ACCURACY_MEDIUM:
                    accuracy = "medium";
                    break;
                case SENSOR_ACCURACY_HIGH:
                    accuracy = "high";
                    break;
            }

            // additional sensor information
            root["accuracy"] = accuracy;
            root["timestamp"] = event.timestamp;

            sensor_info_t sensorInfo;
            sensor_get_info(sensor, &sensorInfo);

            // useful sensor information
            root["min"] = sensorInfo.range_min;
            root["max"] = sensorInfo.range_max;

            switch (event.type)
            {
                case SENSOR_TYPE_ACCELEROMETER:
                    sensorEvent = "deviceaccelerometer";
                    root["x"] = event.motion.dsp.x;
                    root["y"] = event.motion.dsp.y;
                    root["z"] = event.motion.dsp.z;
                    break;
                case SENSOR_TYPE_MAGNETOMETER:
                    sensorEvent = "devicemagnetometer";
                    root["x"] = event.motion.dsp.x;
                    root["y"] = event.motion.dsp.y;
                    root["z"] = event.motion.dsp.z;
                    break;
                case SENSOR_TYPE_GYROSCOPE:
                    sensorEvent = "devicegyroscope";
                    root["x"] = event.motion.dsp.x;
                    root["y"] = event.motion.dsp.y;
                    root["z"] = event.motion.dsp.z;
                    root["temperature"] = event.motion.gyro.temperature;
                    break;
                case SENSOR_TYPE_COMPASS:
                    sensorEvent = "devicecompass";
                    root["value"] = event.compass_s.azimuth;
                    root["isFaceDown"] = event.compass_s.is_face_down;
                    break;
                case SENSOR_TYPE_PROXIMITY:
                    sensorEvent = "deviceproximity";
                    root["value"] = event.proximity_s.distance;
                    root["normalized"] = event.proximity_s.normalized;
                    break;
                case SENSOR_TYPE_LIGHT:
                    sensorEvent = "devicelight";
                    root["value"] = event.light_s.illuminance;
                    break;
                case SENSOR_TYPE_GRAVITY:
                    sensorEvent = "devicegravity";
                    root["x"] = event.motion.dsp.x;
                    root["y"] = event.motion.dsp.y;
                    root["z"] = event.motion.dsp.z;
                    break;
                case SENSOR_TYPE_LINEAR_ACCEL:
                    sensorEvent = "devicelinearacceleration";
                    root["x"] = event.motion.dsp.x;
                    root["y"] = event.motion.dsp.y;
                    root["z"] = event.motion.dsp.z;
                    break;
                case SENSOR_TYPE_ROTATION_MATRIX:
                    sensorEvent = "devicerotationmatrix";
                    for (unsigned int i = 0; i < 9; i++) {
                        root["matrix"].append(event.rotation_matrix[i]);
                    }
                    break;
                case SENSOR_TYPE_ORIENTATION:
                    sensorEvent = "deviceorientation";
                    root["screen"] = event.orientation.screen;
                    root["face"] = event.orientation.face;
                    break;
                case SENSOR_TYPE_AZIMUTH_PITCH_ROLL:
                    sensorEvent = "deviceazimuthpitchroll";
                    root["azimuth"] = event.apr.azimuth;
                    root["pitch"] = event.apr.pitch;
                    root["roll"] = event.apr.roll;
                    break;
                case SENSOR_TYPE_HOLSTER:
                    sensorEvent = "deviceholster";
                    root["value"] = event.holster_s.holstered;
                    break;
                case SENSOR_TYPE_ROTATION_VECTOR:
                case SENSOR_TYPE_FACE_DETECT:
                case SENSOR_TYPE_ALTIMETER:
                case SENSOR_TYPE_TEMPERATURE:
                case SENSOR_TYPE_PRESSURE:
                    // not supported
                    break;
            }

            sensor_event_notify_rearm(sensor);
            parent->NotifyEvent(sensorEvent + " " +  writer.write(root));
        }
    }

    //clean up channels
    ConnectDetach(m_coid);
    ChannelDestroy(m_sensorChannel);
    return NULL;
}
*/

void HDMINDK::startThread()
{
/*
    if (!m_thread) {
        int error = pthread_create(&m_thread, NULL, HDMIThread, static_cast<void *>(m_pParent));

        if (error) {
            m_thread = 0;
        } else {
            MUTEX_LOCK();
        }
    }
*/
    OpenGLView::setRenderingAPI(GL_ES_1);
}

void HDMINDK::stopThread()
{
/*
    //MsgSendPulse(m_coid, SIGEV_PULSE_PRIO_INHERIT, SENSOR_MSG_PULSE, 0);
    if (m_thread) {
        pthread_join(m_thread, NULL);
        m_thread = 0;
        m_sensorsEnabled = false;
    }
*/
    OpenGLView::shutdown();
}

}
