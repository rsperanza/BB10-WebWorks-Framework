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

#ifndef HDMI_NDK_HPP_
#define HDMI_NDK_HPP_

#include <json/value.h>
#include <pthread.h>
//#include <map>
#include <string>
#include <utility>

#include "FillView.hpp"

class HDMI;

namespace webworks {

class HDMINDK {
public:
    explicit HDMINDK(HDMI *parent = NULL);
    ~HDMINDK();
    void SetHDMIOptions(const Json::Value& config);

    std::string GetHDMIScreenSize();
    void OpenWindow(const std::string& args);
    void SetBackgroundColor(const std::string& args);

    int* getHDMIScreenSize();
    void openWindow(const int x, const int y, const int width, const int height);
    void setBackgroundColor(const int red, const int green, const int blue, const int alpha);

/*
    void StartSensor(const std::string& sensor);
    void StopSensor(const std::string& sensor);
    std::string SupportedSensors();
    static void* SensorThread(void *args);
*/
private:
    void startThread();
    void stopThread();
/*
    SensorTypeMap _sensorTypeMap;
    static ActiveSensorMap *m_pActiveSensors;
    struct sigevent m_sigEvent;
    static int m_sensorChannel;
    static int m_coid;
    static void stopActiveSensor(const sensor_type_e sensorType);
    void startEvents();
    void stopEvents();
    void applySensorOptions(sensor_t *sensor, const Json::Value& config);
    void createSensorMap();
*/

    //static bool m_threadRunning;
    //static pthread_t m_thread;

    static pthread_mutex_t m_lock;

    FillView *m_fillView;
    HDMI *m_parent;
};

} // namespace webworks

#endif /* HDMI_NDK_HPP_ */
