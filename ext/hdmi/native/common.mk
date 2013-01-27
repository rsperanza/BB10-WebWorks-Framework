ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

NAME=hdmi
PLUGIN=yes
UTILS=yes

include ../../../../meta.mk

SRCS+=hdmi_ndk.cpp hdmi_js.cpp OpenGLThread.cpp OpenGLView.cpp FillView.cpp

ifeq ($(UNITTEST),yes)
#NAME=test
#SRCS+=test_main.cpp
USEFILE=
endif

include $(MKFILES_ROOT)/qtargets.mk

#LIBS+=hdmi
