#include "Section.h"
#include "../Constants.h"
#include "VinexColours.h"
#include "VinexLookAndFeel.h"

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

    auto font = VinexLookAndFeel::getFont(VinexLookAndFeel::FontStyle::semiBold)
            .withPointHeight(constants::blockTitleSizePt);
    g.setFont(font);
    g.drawText(getName(), 7, 2, 71, 21, Justification::left);
}

void Section::resized()
{

}