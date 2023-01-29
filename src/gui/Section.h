#pragma once

#ifndef VINEX_SECTION_H
#define VINEX_SECTION_H

#include <JuceHeader.h>

class Section : public Component
{
public:
    Section(const String& name) : Component(name) {

    }

    void paint(Graphics& g) override;
    void resized() override;

private:
};


#endif //VINEX_SECTION_H
