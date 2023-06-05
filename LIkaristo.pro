TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += "C:/Users/mateu/Desktop/infa2/SFML-2.5.1/include"

LIBS += -L"C:/Users/mateu/Desktop/infa2/SFML-2.5.1/lib"


CONFIG(debug, debug|release){
    LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-system-d -lsfml-window-d
} else {
    LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
}
SOURCES += \
        bloczek.cpp \
        main.cpp \
        potworki.cpp

HEADERS += \
    bloczek.h \
    potworki.h
