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

#include <json/reader.h>
#include <string>
#include "hdmi_js.hpp"
#include "hdmi_ndk.hpp"

HDMI::HDMI(const std::string& id) : m_id(id)
{
    m_pHDMIController = new webworks::HDMINDK(this);
}

HDMI::~HDMI()
{
    if (m_pHDMIController)
        delete m_pHDMIController;
}

char* onGetObjList()
{
    static char name[] = "HDMI";
    return name;
}

JSExt* onCreateObject(const std::string& className, const std::string& id)
{
    if (className != "HDMI") {
        return NULL;
    }

    return new HDMI(id);
}

std::string HDMI::InvokeMethod(const std::string& command)
{
    int index = command.find_first_of(" ");
    std::string strCommand = command.substr(0, index);
    std::string arg = command.substr(index + 1, command.length());

    if (strCommand == "setOptions") {
/*
        Json::Value obj;

        bool parse = Json::Reader().parse(arg, obj);

        if (!parse)
            return "";

        m_pHDMIController->SetHDMIOptions(obj);
*/
    } else if (strCommand == "GetHDMIScreenSize") {
        return m_pHDMIController->GetHDMIScreenSize();
    } else if (strCommand == "OpenWindow") {
        m_pHDMIController->OpenWindow(arg);
    } else if (strCommand == "SetBackgroundColor") {
        m_pHDMIController->SetBackgroundColor(arg);
    }

    return "";
}

bool HDMI::CanDelete()
{
    return true;
}

// Notifies JavaScript of an event
void HDMI::NotifyEvent(const std::string& event)
{
    std::string eventString = m_id + " ";
    eventString.append(event);
    SendPluginEvent(eventString.c_str(), m_pContext);
}

