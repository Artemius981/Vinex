#include "Section.h"
#include "../Constants.h"
#include "VinexColours.h"

void Section::paint(Graphics &g)
{
    auto availableArea = getLocalBounds();

    g.setColour(vinex_colours::blockBackground);
    g.fillRoundedRectangle(availableArea.toFloat(), constants::blockCornersRadius);

    g.setColour(vinex_colours::blockHeader);
    g.fillRoundedRectangle(availableArea.removeFromTop(constants::blockHeaderHeight).toFloat()
                           , constants::blockCornersRadius);

    jassert(getName().isNotEmpty());
    g.setColour(vinex_colours::textFirst);
    g.setFont(16);
    g.drawText(getName(), 5, 1, 60, 16, Justification::left);
}

void Section::resized()
{

}