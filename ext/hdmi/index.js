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

var hdmiext = require("./hdmiextJNEXT").hdmiext,
    _event = require("../../lib/event"),
    _utils = require("../../lib/utils"),
    _hdmiEvents = require("./hdmiextEvents")
    };

module.exports = {
/*
    registerEvents: function (success, fail, args, env) {
        try {
            var _eventExt = _utils.loadExtensionModule("event", "index");
            _eventExt.registerEvents(_actionMap);
            success();
        } catch (e) {
            fail(-1, e);
        }
    },
*/
    openWindow: function (success, fail, args) {
        if (args) {
            args = JSON.parse(decodeURIComponent(args));

            if (args.x && typeof(args.x) !== "number") {
                fail(-1, "x must be a number");
                return;
            }

            if (args.y && typeof(args.y) !== "number") {
                fail(-1, "y must be a number");
                return;
            }

            if (args.width && typeof(args.width) !== "number") {
                fail(-1, "width must be a number");
                return;
            }

            if (args.height && typeof(args.height) !== "number") {
                fail(-1, "height must be a number");
                return;
            }

            hdmiext.getInstance().openWindow(args);
            success();
        } else {
            fail(-1, "Need to specify arguments");
        }
    },
    setBackgroundColor: function (success, fail, args) {
        if (args) {
            args = JSON.parse(decodeURIComponent(args));

            if (args.red && typeof(args.red) !== "number") {
                fail(-1, "red must be a number");
                return;
            }

            if (args.green && typeof(args.green) !== "number") {
                fail(-1, "green must be a number");
                return;
            }

            if (args.blueidth && typeof(args.blue) !== "number") {
                fail(-1, "blue must be a number");
                return;
            }

            if (args.alpha && typeof(args.alpha) !== "number") {
                fail(-1, "alpha must be a number");
                return;
            }

            hdmiext.getInstance().openWindow(args);
            success();
        } else {
            fail(-1, "Need to specify arguments");
        }
    },

    getHDMIScreenSize: function (success, fail, args) {
        success(hdmiext.getInstance().GetHDMIScreenSize());
    }
};
