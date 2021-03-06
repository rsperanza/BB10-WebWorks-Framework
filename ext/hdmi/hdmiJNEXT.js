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

var hdmi,
    callbackMap = {};

///////////////////////////////////////////////////////////////////
// JavaScript wrapper for JNEXT plugin for connection
///////////////////////////////////////////////////////////////////

JNEXT.HDMI = function () {
    var self = this,
        hasInstance = false;
/*
    self.startSensor = function (sensor, callback) {
        callbackMap[sensor] = callback;
        JNEXT.invoke(self.m_id, "startSensor " + sensor);
    };
    
    self.stopSensor = function (sensor) {
        JNEXT.invoke(self.m_id, "stopSensor " + sensor);
        delete callbackMap[sensor];
    };

    self.setOptions = function (options) {
        JNEXT.invoke(self.m_id, "setOptions " + JSON.stringify(options));
    };

    self.supportedSensors = function () {
        return JNEXT.invoke(self.m_id, "supportedSensors");
    };

    self.onEvent = function (strData) {
        var arData = strData.split(" "),
            strEventDesc = arData[0],
            jsonData;

        jsonData = arData.slice(1, arData.length).join(" ");
        callbackMap[strEventDesc](JSON.parse(jsonData));
    };
*/

    self.getId = function () {
        return self.m_id;
    };

    self.getHDMIScreenSize = function () {
        return JNEXT.invoke(self.m_id, "GetHDMIScreenSize");
    };
    self.openWindow = function (args) {
        JNEXT.invoke(self.m_id, "OpenWindow " + JSON.stringify(args));
    };
    self.setBackgroundColor = function (args) {
        JNEXT.invoke(self.m_id, "SetBackgroundColor " + JSON.stringify(args));
    };

    self.init = function () {
        if (!JNEXT.require("libhdmi")) {
            return false;
        }

        self.m_id = JNEXT.createObject("libhdmi.HDMI");

        if (self.m_id === "") {
            return false;
        }

        JNEXT.registerEvents(self);
    };

    self.m_id = "";

    self.getInstance = function () {
        if (!hasInstance) {
            hasInstance = true;
            self.init();
        }
        return self;
    };

};

hdmi = new JNEXT.HDMI();

module.exports = {
    hdmi: hdmi
};
